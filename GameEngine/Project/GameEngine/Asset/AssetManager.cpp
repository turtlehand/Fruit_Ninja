#include "pch.h"
#include "AssetManager.h"
#include "Asset.h"
#include "Mesh/MeshManager.h"
#include "Mesh/Mesh.h"
#include "Shader/ShaderManager.h"
#include "Texture/TextureManager.h"
#include "Animation2D/Animation2DManager.h"

CAssetManager::CAssetManager()
{

}

CAssetManager::~CAssetManager()
{
}

int CAssetManager::Init()
{
	m_ShaderManager.reset(new CShaderManager);

	if (!m_ShaderManager->Init())
		return E_FAIL;

	m_MeshManager.reset(new CMeshManager);

	if (!m_MeshManager->Init())
		return E_FAIL;

	m_TextureManager.reset(new CTextureManager);

	if (!m_TextureManager->Init())
		return E_FAIL;

	m_Animation2DManager.reset(new CAnimation2DManager);

	if (!m_Animation2DManager->Init())
		return E_FAIL;

	return S_OK;
}

void CAssetManager::ReleaseAsset(const std::string& _Name, EAssetType _Type)
{
	switch (_Type)
	{
	case EAssetType::Mesh:
		m_MeshManager->ReleaseAsset(_Name);
		break;
	case EAssetType::Shader:
		m_ShaderManager->ReleaseShader(_Name);
		break;
	case EAssetType::ConstantBuffer:
		m_ShaderManager->ReleaseCBuffer(_Name);
		break;
	case EAssetType::Material:
		break;
	case EAssetType::Texture:
		m_TextureManager->ReleaseAsset(_Name);
		break;
	case EAssetType::Animation2D:
		m_Animation2DManager->ReleaseAsset(_Name);
		break;
	default:
		break;
	}
}

