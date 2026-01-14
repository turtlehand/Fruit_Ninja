#pragma once
#include "../Asset.h"

struct FVertexBuffer
{
	// 버텍스를 저장하기 위한 버퍼
	ComPtr<ID3D11Buffer> Buffer = nullptr;
	// 버텍스 1개의 메모리 크기
	int Size = 0;
	// 버텍스의 개수
	int Count = 0;
};

struct FIndexBuffer
{
	// 인덱스를 저장하기 위한 버퍼
	ComPtr<ID3D11Buffer> Buffer = nullptr;
	// 인덱스 1개의 메모리 크기
	int Size = 0;
	// 인덱스의 개수
	int Count = 0;

	DXGI_FORMAT Fmt = DXGI_FORMAT_UNKNOWN;
};

// 나중에 부위별로 재질을 다르게 할 때 사용하는 구조체
struct FMeshSlot
{
	FIndexBuffer	IndexBuffer;
	std::shared_ptr<class CMaterial> Material;
};

class CMesh : 
	public CAsset
{
protected:
	friend class CMeshManager;
	friend class CColliderPolygon2D;
	friend class CDynamicMeshComponent;
	CMesh();
public:
	virtual ~CMesh() override;

protected:
	std::string								m_Name;

	FVertexBuffer							m_VB;			// Vertex

	std::vector<std::shared_ptr<FMeshSlot>>	m_MeshSlot;		// Index

	D3D11_PRIMITIVE_TOPOLOGY				m_Primitive;	// 모양

	FVector3 m_Min = FVector3(FLT_MAX, FLT_MAX, FLT_MAX);
	FVector3 m_Max = FVector3(FLT_MIN, FLT_MIN, FLT_MIN);
	FVector3 m_MeshSize;

public:
	const FVector3& GetMin()	const
	{
		return m_Min;
	}

	const FVector3& GetMax()	const
	{
		return m_Max;
	}

	const FVector3& GetMeshSize()	const
	{
		return m_MeshSize;
	}
	size_t GetMeshSlotCount() const { return m_MeshSlot.size(); }
	std::shared_ptr<FMeshSlot> GetMeshSlot(int _Index) const { return m_MeshSlot[_Index]; }

	void SetMaterial(int _SlotIndex);
	void SetMaterialBaseColor(int _SlotIndex, float _r, float _g, float _b, float _a);
	void SetMaterialBaseColor(int _SlotIndex, unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a);
	void SetMaterialBaseColor(int _SlotIndex, const FVector4& _Color);
	void SetMaterialOpacity(int _SlotIndex, float _Opacity);

public:
	void Render();
	void Render(int _SlotIndex);
	bool CreateMesh(void* _VertexData, int _VertexSize, int _VertexCount,
		D3D11_USAGE _VertexUsage, D3D11_PRIMITIVE_TOPOLOGY _Primitive,
		void* _IndexData = nullptr, int _IndexSize = 0, int _IndexCount = 0,
		DXGI_FORMAT _Fmt = DXGI_FORMAT_UNKNOWN, D3D11_USAGE _IndexUsage = D3D11_USAGE_DEFAULT);

protected:
	// _Buffer : 정보를 담은 최종 그릇
	// _Flag : 어떤 정보를 담을 것인지
	// _Data : 정보를 담은 임시 그릇
	bool CreateBuffer(ComPtr<ID3D11Buffer>* _Buffer, D3D11_BIND_FLAG _Flag, void* _Data, int _Size, int _Count, D3D11_USAGE _Usage);

	bool ChangeVertexBuffer(void* _Data, int _Size, int _Count);
	bool ChangeIndexBuffer(int _SlotIndex, void* _Data, int _Size, int _Count);


};

