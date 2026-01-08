#include "pch.h"
#include "WorldAssetManager.h"

CWorldAssetManager::CWorldAssetManager()
{

}

CWorldAssetManager::~CWorldAssetManager()
{
	auto iter = m_AssetMap.begin();

	for (; iter != m_AssetMap.end();)
	{
		EAssetType Type = iter->second->GetAssetType();

		std::string Name = iter->second->GetName();

		// 카우트 1 감소.
		iter = m_AssetMap.erase(iter);

		CAssetManager::GetInst()->ReleaseAsset(Name, Type);
	}
}

bool CWorldAssetManager::Init()
{
	FVertexColor	CenterRectColor[4] =
	{
		FVertexColor(-0.5f, 0.5f, 0.f, 1.f, 0.f, 0.f, 1.f),
		FVertexColor(0.5f, 0.5f, 0.f, 0.f, 1.f, 0.f, 1.f),
		FVertexColor(-0.5f, -0.5f, 0.f, 0.f, 0.f, 1.f, 1.f),
		FVertexColor(0.5f, -0.5f, 0.f, 1.f, 1.f, 0.f, 1.f)
	};

	unsigned short	CenterRectColorIdx[6] = { 0, 1, 3, 0, 3, 2 };

	if (!CreateMesh("CenterRectColor", CenterRectColor, sizeof(FVertexColor),
		4, D3D11_USAGE_IMMUTABLE, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		CenterRectColorIdx, 2, 6, DXGI_FORMAT_R16_UINT,
		D3D11_USAGE_IMMUTABLE))
		return false;
	return true;
}

void CWorldAssetManager::Update(double _DeltaTime)
{

}

#pragma region Mesh

bool CWorldAssetManager::CreateMesh(const std::string& _Name,
	void* _VertexData,
	int _VertexSize, int _VertexCount,
	D3D11_USAGE _VertexUsage, D3D11_PRIMITIVE_TOPOLOGY _Primitive,
	void* _IndexData, int _IndexSize, int _IndexCount,
	DXGI_FORMAT _Fmt, D3D11_USAGE _IndexUsage)
{
	auto MeshMgr = CAssetManager::GetInst()->GetMeshManager().lock();

	if (!MeshMgr)
		return false;

	std::string Key = "Mesh_" + _Name;

	// 새로운 메시를 만들지 못했다면 실패
	if (!MeshMgr->CreateMesh(Key,
		_VertexData, _VertexSize, _VertexCount, _VertexUsage, _Primitive,
		_IndexData, _IndexSize, _IndexCount, _Fmt, _IndexUsage))
	{
		return false;
	}

	auto iter = m_AssetMap.find(Key);

	// 현재 월드의 AssetMap에 추가가 안되어 있을 경우
	if (iter == m_AssetMap.end())
	{
		// MeshManager와 m_AssetMap에 들어가 있는 Mesh는 공용이다.
		// 그러므로 서로 shared_ptr이므로 같은 객체를 공유하며 카운트가 둘다 1씩 증가하여 2가 되어 있을 것이다.
		m_AssetMap.insert(std::make_pair(Key, MeshMgr->FindMesh(Key)));
	}
	return true;
}

std::weak_ptr<CMesh> CWorldAssetManager::FindMesh(const std::string& _Name)
{
	std::string Key = "Mesh_" + _Name;

	auto iter = m_AssetMap.find(Key);

	// 현재 월드의 AssetMap에 추가가 안되어 있을 경우
	if (iter == m_AssetMap.end())
	{
		auto MeshMgr = CAssetManager::GetInst()->GetMeshManager().lock();
		auto Mesh = MeshMgr->FindMesh(Key);

		// 유효하다면 월드의 AssetMap에 추가
		if (!Mesh.expired())
		{
			m_AssetMap.insert(std::make_pair(Key, Mesh));
		}

		// 다시 찾음
		iter = m_AssetMap.find(Key);
	}

	// 존재하는 것을 넣었다면 옳게 반환됨
	return std::dynamic_pointer_cast<CMesh>(iter->second);
}

#pragma endregion Mesh

#pragma region Material
#pragma endregion Material

#pragma region Texture

bool CWorldAssetManager::LoadTexture(const std::string& _Name, const wchar_t* _FileName, const std::wstring& _PathName)
{
	auto TextureMgr = CAssetManager::GetInst()->GetTextureManager().lock();

	if (TextureMgr)
	{
		std::string	Key = "Texture_" + _Name;

		if (!TextureMgr->LoadTexture(Key, _FileName, _PathName))
			return false;

		auto	iter = m_AssetMap.find(Key);

		// 현재 월드의 AssetMap에 추가가 안되어 있을 경우
		if (iter == m_AssetMap.end())
		{
			// MeshManager와 mAssetMap에 들어가 있는 Mesh는
			// 공용이다. 그러므로 서로 shared_ptr이므로 같은
			// 객체를 공유하며 카운트가 둘다 1씩 증가하여
			// 2가 되어 있을 것이다.
			m_AssetMap.insert(std::make_pair(Key,
				TextureMgr->FindTexture(Key)));
		}

		return true;
	}

	return false;
}

bool CWorldAssetManager::LoadTextureFullPath(const std::string& _Name, const TCHAR* _FullPath)
{
	auto TextureMgr = CAssetManager::GetInst()->GetTextureManager().lock();

	if (TextureMgr)
	{
		std::string	Key = "Texture_" + _Name;

		if (!TextureMgr->LoadTextureFullPath(Key, _FullPath))
			return false;

		auto	iter = m_AssetMap.find(Key);

		// 현재 월드의 AssetMap에 추가가 안되어 있을 경우
		if (iter == m_AssetMap.end())
		{
			// MeshManager와 mAssetMap에 들어가 있는 Mesh는
			// 공용이다. 그러므로 서로 shared_ptr이므로 같은
			// 객체를 공유하며 카운트가 둘다 1씩 증가하여
			// 2가 되어 있을 것이다.
			m_AssetMap.insert(std::make_pair(Key, TextureMgr->FindTexture(Key)));
		}

		return true;
	}

	return false;
}

bool CWorldAssetManager::LoadTexture(const std::string& _Name, const std::vector<const wchar_t*>& _FileName, const std::wstring& _PathName)
{
	auto TextureMgr =
		CAssetManager::GetInst()->GetTextureManager().lock();

	if (TextureMgr)
	{
		std::string	Key = "Texture_" + _Name;

		if (!TextureMgr->LoadTexture(Key, _FileName, _PathName))
			return false;

		auto	iter = m_AssetMap.find(Key);

		// 현재 월드의 AssetMap에 추가가 안되어 있을 경우
		if (iter == m_AssetMap.end())
		{
			// MeshManager와 mAssetMap에 들어가 있는 Mesh는
			// 공용이다. 그러므로 서로 shared_ptr이므로 같은
			// 객체를 공유하며 카운트가 둘다 1씩 증가하여
			// 2가 되어 있을 것이다.
			m_AssetMap.insert(std::make_pair(Key,
				TextureMgr->FindTexture(Key)));
		}

		return true;
	}

	return false;
}

bool CWorldAssetManager::LoadTextureFullPath(const std::string& _Name, const std::vector<const wchar_t*> _FullPath)
{
	auto TextureMgr =
		CAssetManager::GetInst()->GetTextureManager().lock();

	if (TextureMgr)
	{
		std::string	Key = "Texture_" + _Name;

		if (!TextureMgr->LoadTextureFullPath(Key, _FullPath))
			return false;

		auto	iter = m_AssetMap.find(Key);

		// 현재 월드의 AssetMap에 추가가 안되어 있을 경우
		if (iter == m_AssetMap.end())
		{
			// MeshManager와 mAssetMap에 들어가 있는 Mesh는
			// 공용이다. 그러므로 서로 shared_ptr이므로 같은
			// 객체를 공유하며 카운트가 둘다 1씩 증가하여
			// 2가 되어 있을 것이다.
			m_AssetMap.insert(std::make_pair(Key, TextureMgr->FindTexture(Key)));
		}

		return true;
	}

	return false;
}

std::weak_ptr<CTexture> CWorldAssetManager::FindTexture(const std::string& _Name)
{
	std::string Key = "Texture_" + _Name;

	auto	iter = m_AssetMap.find(Key);

	// 현재 월드의 AssetMap에 추가가 안되어 있을 경우
	if (iter == m_AssetMap.end())
	{
		auto TextureMgr =
			CAssetManager::GetInst()->GetTextureManager().lock();

		auto	Texture = TextureMgr->FindTexture(Key);

		if (!Texture.expired())
		{
			m_AssetMap.insert(std::make_pair(Key, Texture));
		}

		iter = m_AssetMap.find(Key);
	}

	return std::dynamic_pointer_cast<CTexture>(iter->second);
}


#pragma endregion Texture

#pragma region Animation2D

bool CWorldAssetManager::CreateAnimation(
	const std::string& _Name)
{
	auto Animation2DMgr =
		CAssetManager::GetInst()->GetAnimation2DManager().lock();

	if (Animation2DMgr)
	{
		std::string	Key = "Animation2D_" + _Name;

		if (!Animation2DMgr->CreateAnimation(Key))
			return false;

		auto	iter = m_AssetMap.find(Key);

		// 현재 월드의 AssetMap에 추가가 안되어 있을 경우
		if (iter == m_AssetMap.end())
		{
			// MeshManager와 mAssetMap에 들어가 있는 Mesh는
			// 공용이다. 그러므로 서로 shared_ptr이므로 같은
			// 객체를 공유하며 카운트가 둘다 1씩 증가하여
			// 2가 되어 있을 것이다.
			m_AssetMap.insert(std::make_pair(Key,
				Animation2DMgr->FindAnimation(Key)));
		}

		return true;
	}

	return false;
}

std::weak_ptr<CAnimation2D> CWorldAssetManager::FindAnimation( const std::string& _Name)
{
	std::string	Key = "Animation2D_" + _Name;

	auto	iter = m_AssetMap.find(Key);

	// 현재 월드의 AssetMap에 추가가 안되어 있을 경우
	if (iter == m_AssetMap.end())
	{
		auto Animation2DMgr =
			CAssetManager::GetInst()->GetAnimation2DManager().lock();

		auto	Animation2D = Animation2DMgr->FindAnimation(Key);

		if (!Animation2D.expired())
		{
			m_AssetMap.insert(std::make_pair(Key, Animation2D));
		}

		iter = m_AssetMap.find(Key);
	}

	return std::dynamic_pointer_cast<CAnimation2D>(iter->second);
}

bool CWorldAssetManager::SetAnimation2DTextureType(const std::string& _Name, EAnimation2DTextureType _Type)
{
	std::string	Key = "Animation2D_" + _Name;

	auto	iter = m_AssetMap.find(Key);

	if (iter == m_AssetMap.end())
		return false;

	std::dynamic_pointer_cast<CAnimation2D>(iter->second)->SetAnimation2DTextureType(_Type);

	return true;
}

bool CWorldAssetManager::SetTexture(const std::string& _AnimationName, const std::weak_ptr<CTexture>& _Texture)
{
	std::string	Key = "Animation2D_" + _AnimationName;

	auto	iter = m_AssetMap.find(Key);

	if (iter == m_AssetMap.end())
		return false;

	std::dynamic_pointer_cast<CAnimation2D>(iter->second)->SetTexture(_Texture);

	return true;
}

bool CWorldAssetManager::SetTexture(const std::string& _AnimationName, const std::string& _Name)
{
	std::string	Key = "Animation2D_" + _AnimationName;

	auto	iter = m_AssetMap.find(Key);

	if (iter == m_AssetMap.end())
		return false;

	std::weak_ptr<CTexture> Texture = FindTexture(_Name);

	if (Texture.expired())
		return false;

	std::dynamic_pointer_cast<CAnimation2D>(iter->second)->SetTexture(Texture);

	return true;
}

bool CWorldAssetManager::SetTexture(const std::string& _AnimationName, const std::string& _Name, const wchar_t* _FileName,const std::wstring& _PathName)
{
	std::string	Key = "Animation2D_" + _AnimationName;

	auto	iter = m_AssetMap.find(Key);

	if (iter == m_AssetMap.end())
		return false;

	if (!LoadTexture(_Name, _FileName, _PathName))
		return false;

	std::weak_ptr<CTexture> Texture = FindTexture(_Name);

	if (Texture.expired())
		return false;

	std::dynamic_pointer_cast<CAnimation2D>(iter->second)->SetTexture(Texture);

	return true;
}

bool CWorldAssetManager::SetTextureFullPath(const std::string& _AnimationName,const std::string& _Name,const wchar_t* _FullPath)
{
	std::string	Key = "Animation2D_" + _AnimationName;

	auto	iter = m_AssetMap.find(Key);

	if (iter == m_AssetMap.end())
		return false;

	if (!LoadTextureFullPath(_Name, _FullPath))
		return false;

	std::weak_ptr<CTexture> Texture = FindTexture(_Name);

	if (Texture.expired())
		return false;

	std::dynamic_pointer_cast<CAnimation2D>(iter->second)->SetTexture(Texture);

	return true;
}

bool CWorldAssetManager::SetTexture(const std::string& _AnimationName, const std::string& _Name, const std::vector<const wchar_t*>& _FileName, const std::wstring& _PathName)
{
	std::string	Key = "Animation2D_" + _AnimationName;

	auto	iter = m_AssetMap.find(Key);

	if (iter == m_AssetMap.end())
		return false;

	if (!LoadTexture(_Name, _FileName, _PathName))
		return false;

	std::weak_ptr<CTexture> Texture = FindTexture(_Name);

	if (Texture.expired())
		return false;

	std::dynamic_pointer_cast<CAnimation2D>(iter->second)->SetTexture(Texture);

	return true;
}

bool CWorldAssetManager::SetTextureFullPath(
	const std::string& _AnimationName,
	const std::string& _Name,
	const std::vector<const TCHAR*>& _FullPath)
{
	std::string	Key = "Animation2D_" + _AnimationName;

	auto	iter = m_AssetMap.find(Key);

	if (iter == m_AssetMap.end())
		return false;

	if (!LoadTextureFullPath(_Name, _FullPath))
		return false;

	std::weak_ptr<CTexture> Texture = FindTexture(_Name);

	if (Texture.expired())
		return false;

	std::dynamic_pointer_cast<CAnimation2D>(iter->second)->SetTexture(Texture);

	return true;
}

bool CWorldAssetManager::AddFrame(
	const std::string& _AnimationName,
	const FVector2& _Start, const FVector2& _Size)
{
	std::string	Key = "Animation2D_" + _AnimationName;

	auto	iter = m_AssetMap.find(Key);

	if (iter == m_AssetMap.end())
		return false;

	std::dynamic_pointer_cast<CAnimation2D>(iter->second)->AddFrame(_Start, _Size);

	return true;
}

bool CWorldAssetManager::AddFrame(
	const std::string& _AnimationName,
	float _StartX, float _StartY, float _SizeX,
	float _SizeY)
{
	std::string	Key = "Animation2D_" + _AnimationName;

	auto	iter = m_AssetMap.find(Key);

	if (iter == m_AssetMap.end())
		return false;

	std::dynamic_pointer_cast<CAnimation2D>(iter->second)->AddFrame(_StartX, _StartY, _SizeX, _SizeY);

	return true;
}

bool CWorldAssetManager::AddFrame(
	const std::string& _AnimationName,
	int _Count, const FVector2& _Start, const FVector2& _Size)
{
	std::string	Key = "Animation2D_" + _AnimationName;

	auto	iter = m_AssetMap.find(Key);

	if (iter == m_AssetMap.end())
		return false;

	std::dynamic_pointer_cast<CAnimation2D>(iter->second)->AddFrame(_Count, _Start, _Size);

	return true;
}

bool CWorldAssetManager::AddFrame(
	const std::string& _AnimationName,
	int _Count, float _StartX, float _StartY, float _SizeX,
	float _SizeY)
{
	std::string	Key = "Animation2D_" + _AnimationName;

	auto	iter = m_AssetMap.find(Key);

	if (iter == m_AssetMap.end())
		return false;

	std::dynamic_pointer_cast<CAnimation2D>(iter->second)->AddFrame(_Count, _StartX, _StartY, _SizeX, _SizeY);

	return true;
}


#pragma endregion Animation2D