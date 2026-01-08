#include "pch.h"
#include "Mesh.h"
#include "Device.h"
#include "../Material/Material.h"

CMesh::CMesh() :
	CAsset(EAssetType::Mesh),
	m_VB(),
	m_MeshSlot(),
	m_Primitive(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{
}

CMesh::~CMesh()
{

}

// GPU에게 메쉬의 정보(Vertex, Index 등)을 바인딩하고 
// 그린다.
void CMesh::Render()
{
	UINT Stride = m_VB.Size;
	UINT Offset = 0;

	//출력할 도형 타입을 지정한다.
	CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(m_Primitive);

	// 출력할 버텍스 버퍼를 지정한다.
	CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 1, m_VB.Buffer.GetAddressOf(), &Stride, &Offset);
	if (m_MeshSlot.empty())
	{
		CDevice::GetInst()->GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
		CDevice::GetInst()->GetContext()->Draw(m_VB.Count, 0);
	}
	else 
	{
		size_t Size = m_MeshSlot.size();

		for (size_t i = 0; i < Size; ++i)
		{
			// Material 정보를 Shader 상수버퍼로 남겨준다.
			if (m_MeshSlot[i]->Material)
				m_MeshSlot[i]->Material->UpdateConstantBuffer();

			// 출력에 사용할 인덱스 버퍼를 지정한다.
			CDevice::GetInst()->GetContext()->IASetIndexBuffer(m_MeshSlot[i]->IndexBuffer.Buffer.Get(),
				m_MeshSlot[i]->IndexBuffer.Fmt, 0);

			// 인덱스 버퍼의 인덱스를 참고하여 
			CDevice::GetInst()->GetContext()->DrawIndexed(m_MeshSlot[i]->IndexBuffer.Count, 0, 0);
		}
	}
}

void CMesh::Render(int _SlotIndex)
{
	UINT Stride = m_VB.Size;
	UINT Offset = 0;

	//출력할 도형 타입을 지정한다.
	CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(m_Primitive);

	// 출력할 버텍스 버퍼를 지정한다.
	CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 1, m_VB.Buffer.GetAddressOf(), &Stride, &Offset);
	if (m_MeshSlot.empty())
	{
		CDevice::GetInst()->GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
		CDevice::GetInst()->GetContext()->Draw(m_VB.Count, 0);
	}
	else
	{
		// 출력에 사용할 인덱스 버퍼를 지정한다.
		CDevice::GetInst()->GetContext()->IASetIndexBuffer(m_MeshSlot[_SlotIndex]->IndexBuffer.Buffer.Get(), m_MeshSlot[_SlotIndex]->IndexBuffer.Fmt, 0);

		// 인덱스 버퍼의 인덱스를 참고하여 
		CDevice::GetInst()->GetContext()->DrawIndexed(m_MeshSlot[_SlotIndex]->IndexBuffer.Count, 0, 0);

	}
}

// 메쉬를 생성하는 함수
// 기본은 정점만 넣으면 된다.
// 원한다면 인덱스도 넣으면 된다.
/*
	D3D_USAGE : 버퍼나 텍스처 같은 리소스를 어떤 용도로 사용하는지 정의하는 타입이다.
	D3D11_USAGE_DEFAULT		= 0 : GPU 전용(읽기/쓰기 가능). CPU는 읽기/쓰기 모두 불가능하다.
	D3D11_USAGE_IMMUTABLE	= 1 : GPU 읽기 가능, CPU 쓰기 불가능. 한번 넣고 정보를 안바꿀 경우 사용한다. 속도가 가장 빠르다
	D3D11_USAGE_DYNAMIC		= 2 : GPU 읽기 가능, CPU 쓰기 가능, 정보를 자주 업데이트 해야 하는 곳에 주로 사용한다.
	D3D11_USAGE_STAGING		= 3 : CPU 읽기/쓰기 모두 가능. CPU는 사용 안한다. 메모리를 확보하고 데이터를 저장해두는 용도로 주로 사용한다.
*/
bool CMesh::CreateMesh(void* _VertexData, int _VertexSize, int _VertexCount, D3D11_USAGE _VertexUsage, D3D11_PRIMITIVE_TOPOLOGY _Primitive, void* _IndexData, int _IndexSize, int _IndexCount, DXGI_FORMAT _Fmt, D3D11_USAGE _IndexUsage)
{
	m_VB.Size = _VertexSize;
	m_VB.Count = _VertexCount;
	m_Primitive = _Primitive;

	if (!CreateBuffer(&m_VB.Buffer, D3D11_BIND_VERTEX_BUFFER, _VertexData, _VertexSize, _VertexCount, _VertexUsage))
		return false;

	// 각 정점의 위치값을 구하여 Mesh의 Min, Max 크기를 구한다
	// 예를 들어 반지름이 0.5인 원이라고 할 때 원의 각 정점을 비교하여 최대, 최소를 구한다.
	char* Vertices = (char*)_VertexData;

	for (int i = 0; i < _VertexCount; ++i)
	{
		FVector3 Pos = *((FVector3*)Vertices);

		if (m_Min.x > Pos.x)
			m_Min.x = Pos.x;

		if (m_Min.y > Pos.y)
			m_Min.y = Pos.y;

		if (m_Min.z > Pos.z)
			m_Min.z = Pos.z;

		if (m_Max.x < Pos.x)
			m_Max.x = Pos.x;

		if (m_Max.y < Pos.y)
			m_Max.y = Pos.y;

		if (m_Max.z < Pos.z)
			m_Max.z = Pos.z;
		Vertices += _VertexSize;
	}

	m_MeshSize = m_Max - m_Min;

	// 인덱스 데이터가 있다면 인덱스 버퍼도 만들어준다.
	if (_IndexData)
	{
		std::shared_ptr<FMeshSlot> Slot(new FMeshSlot);
		Slot->IndexBuffer.Size = _IndexSize;
		Slot->IndexBuffer.Count = _IndexCount;
		Slot->IndexBuffer.Fmt = _Fmt;

		m_MeshSlot.push_back(Slot);

		SetMaterial(0);

		if (!CreateBuffer(&Slot->IndexBuffer.Buffer, D3D11_BIND_INDEX_BUFFER, _IndexData, _IndexSize, _IndexCount, _IndexUsage))
			return false;
	}

	return true;
}

void CMesh::SetMaterial(int _SlotIndex)
{
	if (!m_MeshSlot[_SlotIndex]->Material)
	{
		m_MeshSlot[_SlotIndex]->Material.reset(new CMaterial);

		m_MeshSlot[_SlotIndex]->Material->Init();
	}
}

void CMesh::SetMaterialBaseColor(int _SlotIndex, float _r, float _g, float _b, float _a)
{
	if (!m_MeshSlot[_SlotIndex]->Material)
	{
		m_MeshSlot[_SlotIndex]->Material.reset(new CMaterial);

		m_MeshSlot[_SlotIndex]->Material->Init();
	}

	m_MeshSlot[_SlotIndex]->Material->SetBaseColor(_r, _g, _b, _a);
}

void CMesh::SetMaterialBaseColor(int _SlotIndex, unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a)
{
	if (!m_MeshSlot[_SlotIndex]->Material)
	{
		m_MeshSlot[_SlotIndex]->Material.reset(new CMaterial);

		m_MeshSlot[_SlotIndex]->Material->Init();
	}

	m_MeshSlot[_SlotIndex]->Material->SetBaseColor(_r, _g, _b, _a);
}

void CMesh::SetMaterialBaseColor(int _SlotIndex, const FVector4& _Color)
{
	if (!m_MeshSlot[_SlotIndex]->Material)
	{
		m_MeshSlot[_SlotIndex]->Material.reset(new CMaterial);

		m_MeshSlot[_SlotIndex]->Material->Init();
	}

	m_MeshSlot[_SlotIndex]->Material->SetBaseColor(_Color);
}

void CMesh::SetMaterialOpacity(int _SlotIndex, float _Opacity)
{
	if (!m_MeshSlot[_SlotIndex]->Material)
	{
		m_MeshSlot[_SlotIndex]->Material.reset(new CMaterial);

		m_MeshSlot[_SlotIndex]->Material->Init();
	}

	m_MeshSlot[_SlotIndex]->Material->SetOpacity(_Opacity);
}


// 버퍼를 생성하는 함수
bool CMesh::CreateBuffer(ComPtr<ID3D11Buffer>* _Buffer, D3D11_BIND_FLAG _Flag, void* _Data, int _Size, int _Count, D3D11_USAGE _Usage)
{
	// 버퍼를 생성하기 위한 구조체
	D3D11_BUFFER_DESC BufferDesc = {};
	
	// 버퍼의 전체 메모리 크기
	BufferDesc.ByteWidth = _Size * _Count;

	// 버퍼의 쓰임새 Vertex 라던가, Index 라던가
	BufferDesc.BindFlags = _Flag;

	// 버퍼의 사용 환경
	BufferDesc.Usage = _Usage;

	//GPU 읽기 가능, CPU 쓰기 가능, 정보를 자주 업데이트 해야 하는 곳에 주로 사용한다.
	if (_Usage == D3D11_USAGE_DYNAMIC)
		BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	// CPU 읽기 / 쓰기 모두 가능.CPU는 사용 안한다.메모리를 확보하고 데이터를 저장해두는 용도로 주로 사용한다.
	else if (_Usage == D3D11_USAGE_STAGING)
		BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;

	// 데이터
	D3D11_SUBRESOURCE_DATA BufferData = {};
	BufferData.pSysMem = _Data;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&BufferDesc, &BufferData, _Buffer->GetAddressOf())))
		return false;

	return true;
}

