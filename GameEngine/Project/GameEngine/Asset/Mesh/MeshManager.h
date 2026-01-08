#pragma once

class CMeshManager
{
    friend class CAssetManager;
    // 소멸자는 공개를 해야지만 shared_ptr에 정상적으로 넣을 수 있다.
    // shared_ptr에서 소멸자를 사용해서 그런것 같다.
private:
    CMeshManager();
public:
    ~CMeshManager();

private:
    std::unordered_map<std::string, std::shared_ptr<class CMesh>> m_MeshMap;
public:
    bool Init();
    bool CreateMesh(const std::string& _Name, void* _VertexData, int _VertexSize, int _VertexCount,
        D3D11_USAGE _VertexUsage, D3D11_PRIMITIVE_TOPOLOGY _Primitive,
        void* _IndexData = nullptr, int _IndexSize = 0, int _IndexCount = 0,
        DXGI_FORMAT _Fmt = DXGI_FORMAT_UNKNOWN, D3D11_USAGE _IndexUsage = D3D11_USAGE_DEFAULT);
    std::weak_ptr<class CMesh> FindMesh(const std::string& _Name);

public:
    void ReleaseAsset(const std::string& _Name);




};

