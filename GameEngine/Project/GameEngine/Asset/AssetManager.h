#pragma once
#include "Asset.h"

class CMeshManager;
class CShaderManager;
class CTextureManager;
class CAnimation2DManager;

class CAssetManager : public CSingleton<CAssetManager>
{
private:
    std::shared_ptr<CMeshManager>       m_MeshManager;
    std::shared_ptr<CShaderManager>     m_ShaderManager;
    std::shared_ptr<CTextureManager>    m_TextureManager;
    std::shared_ptr<CAnimation2DManager> m_Animation2DManager;

public:
    std::weak_ptr<CMeshManager> GetMeshManager() { return m_MeshManager; }
    std::weak_ptr<CShaderManager> GetShaderManager() { return m_ShaderManager; }
    std::weak_ptr<CTextureManager> GetTextureManager() { return m_TextureManager; }
    std::weak_ptr<CAnimation2DManager> GetAnimation2DManager() { return m_Animation2DManager; }

public:
    int Init();
    void ReleaseAsset(const std::string& _Name, EAssetType _Type);

private:
    friend class CSingleton<CAssetManager>;
    CAssetManager();
    ~CAssetManager();
};

