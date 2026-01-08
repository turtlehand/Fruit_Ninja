#pragma once
#include "../Asset/AssetManager.h"
#include "../Asset/Asset.h"

#include "../Asset/Mesh/MeshManager.h"
#include "../Asset/Mesh/Mesh.h"

// Shader는 거의 항상 필요하므로 World가 관리하지 않는다.

#include "../Asset/Material/MaterialManager.h"
#include "../Asset/Material/Material.h"

#include "../Asset/Texture/TextureManager.h"
#include "../Asset/Texture/Texture.h"

#include "../Asset/Animation2D/Animation2DManager.h"
#include "../Asset/Animation2D/Animation2D.h"

class CWorldAssetManager
{
	friend class CWorld;

private:
	CWorldAssetManager();

public:
	~CWorldAssetManager();

private:
	// AssetType_Name
	std::unordered_map<std::string, std::shared_ptr<CAsset>> m_AssetMap;

public:
	bool Init();
	void Update(double _DeltaTime);

#pragma region Mesh
public:
	bool CreateMesh(const std::string& _Name,
		void* _VertexData,
		int _VertexSize, int _VertexCount,
		D3D11_USAGE _VertexUsage, D3D11_PRIMITIVE_TOPOLOGY _Primitive,
		void* _IndexData = nullptr, int _IndexSize = 0, int _IndexCount = 0,
		DXGI_FORMAT _Fmt = DXGI_FORMAT_UNKNOWN, D3D11_USAGE _IndexUsage = D3D11_USAGE_DEFAULT);
	std::weak_ptr<CMesh> FindMesh(const std::string& _Name);
#pragma endregion

#pragma region Material
#pragma endregion Material

#pragma region Texture

public:
	bool LoadTexture(const std::string& _Name,
		const wchar_t* _FileName,
		const std::wstring& _PathName = L"Texture");
	bool LoadTextureFullPath(const std::string& _Name,
		const wchar_t* _FullPath);
	bool LoadTexture(const std::string& _Name,
		const std::vector<const wchar_t*>& _FileName,
		const std::wstring& _PathName = L"Texture");
	bool LoadTextureFullPath(const std::string& _Name,
		const std::vector<const wchar_t*> _FullPath);
	std::weak_ptr<CTexture> FindTexture(
		const std::string& _Name);

#pragma endregion Texture

#pragma region Animation2D

public:
	bool CreateAnimation(const std::string& _Name);
	
	std::weak_ptr<CAnimation2D> FindAnimation(const std::string& _Name);

	bool SetAnimation2DTextureType(const std::string& _Name, EAnimation2DTextureType _Type);

	bool SetTexture(const std::string& _AnimationName, const std::weak_ptr<class CTexture>& _Texture);

	bool SetTexture(const std::string& _AnimationName, const std::string& _Name);
	
	bool SetTexture(const std::string& _AnimationName, const std::string& _Name, const wchar_t* _FileName, const std::wstring& _PathName = L"Texture");
	
	bool SetTextureFullPath(const std::string& _AnimationName, const std::string& _Name, const wchar_t* _FullPath);
	
	bool SetTexture(const std::string& _AnimationName, const std::string& _Name, const std::vector<const wchar_t*>& _FileName, const std::wstring& _PathName = L"Texture");
	
	bool SetTextureFullPath(const std::string& _AnimationName, const std::string& _Name, const std::vector<const wchar_t*>& _FullPath);

	bool AddFrame(const std::string& _AnimationName, const FVector2& _Start, const FVector2& _Size);

	bool AddFrame(const std::string& _AnimationName, float _StartX, float _StartY, float _SizeX, float _SizeY);

	bool AddFrame(const std::string& _AnimationName, int _Count, const FVector2& _Start, const FVector2& _Size);

	bool AddFrame(const std::string& _AnimationName, int _Count, float _StartX, float _StartY, float _SizeX, float _SizeY);

#pragma endregion Animation2D

};

