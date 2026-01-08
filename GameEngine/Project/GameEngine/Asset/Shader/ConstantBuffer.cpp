#include "pch.h"
#include "ConstantBuffer.h"
#include "Device.h"

CConstantBuffer::CConstantBuffer() :
	CAsset(EAssetType::ConstantBuffer),
	m_Size(),
	m_Register(),
	m_ShaderBuffer()
{

}


CConstantBuffer::~CConstantBuffer()
{
}

// 버퍼의 크기와 번호 등을 세팅한다.
bool CConstantBuffer::Init(int _Size, int _Register, int _ShaderBuffer)
{
	m_Size = _Size;
	m_Register = _Register;
	m_ShaderBuffer = _ShaderBuffer;

	D3D11_BUFFER_DESC Desc = {};

	// 상수버퍼는 데이터의 갱신이 빈번하게 일어나기 때문에 Dynamic으로 생성한다.
	Desc.Usage = D3D11_USAGE_DYNAMIC;
	Desc.ByteWidth = m_Size;
	Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&Desc,
		nullptr, m_Buffer.GetAddressOf())))
		return false;

	return true;
}

// GPU에게 데이터를 넘겨준다.
void CConstantBuffer::Update(void* _Data)
{
	// 인자로 들어온 데이터로 버퍼를 갱신한다.
	D3D11_MAPPED_SUBRESOURCE Map = {};
	
	CDevice::GetInst()->GetContext()->Map(m_Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &Map);

	memcpy(Map.pData, _Data, m_Size);

	CDevice::GetInst()->GetContext()->Unmap(m_Buffer.Get(), 0);

	// Shader의 상수버퍼를 보내준다.
	if (m_ShaderBuffer & EShaderBufferType::Vertex)
	{
		CDevice::GetInst()->GetContext()->VSSetConstantBuffers(m_Register, 1, m_Buffer.GetAddressOf());
	}
	if (m_ShaderBuffer & EShaderBufferType::Pixel)
	{
		CDevice::GetInst()->GetContext()->PSSetConstantBuffers(m_Register, 1, m_Buffer.GetAddressOf());
	}
	if (m_ShaderBuffer & EShaderBufferType::Domain)
	{
		CDevice::GetInst()->GetContext()->DSSetConstantBuffers(m_Register, 1, m_Buffer.GetAddressOf());
	}
	if (m_ShaderBuffer & EShaderBufferType::Hull)
	{
		CDevice::GetInst()->GetContext()->HSSetConstantBuffers(m_Register, 1, m_Buffer.GetAddressOf());
	}
	if (m_ShaderBuffer & EShaderBufferType::Geometry)
	{
		CDevice::GetInst()->GetContext()->GSSetConstantBuffers(m_Register, 1, m_Buffer.GetAddressOf());
	}
	if (m_ShaderBuffer & EShaderBufferType::Compute)
	{
		CDevice::GetInst()->GetContext()->CSSetConstantBuffers(m_Register, 1, m_Buffer.GetAddressOf());
	}
}
