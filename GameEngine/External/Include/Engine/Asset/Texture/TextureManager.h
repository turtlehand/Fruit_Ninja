#pragma once
#include "../Shader/CBufferContainer.h"
#include "../Texture/Texture.h"


class CTextureManager
{
    friend class CAssetManager;
    // 소멸자는 공개를 해야지만 shared_ptr에 정상적으로 넣을 수 있다.
    // shared_ptr에서 소멸자를 사용해서 그런것 같다.
private:
    CTextureManager();
public:
    ~CTextureManager();

private:
    std::unordered_map<std::string, std::shared_ptr<class CTexture>> m_TextureMap;
    std::unordered_map<std::string, ComPtr<ID3D11SamplerState>> m_SamplerMap;

public:
    bool Init();

public:
    bool LoadTexture(const std::string& _Name, const wchar_t* _FileName, const std::wstring& _PathName = L"Texture");
    bool LoadTextureFullPath(const std::string& _Name, const wchar_t* _FullPath);
    bool LoadTexture(const std::string& _Name, const std::vector<const wchar_t*> _FileName, const std::wstring& _PathName = L"Texture");
    bool LoadTextureFullPath(const std::string& _Name, const std::vector<const wchar_t*> _FullPath);
    std::weak_ptr<CTexture> FindTexture(const std::string& _Name);

    bool CreateSampler(const std::string& _Name, D3D11_FILTER _Filter,
        D3D11_TEXTURE_ADDRESS_MODE _u = D3D11_TEXTURE_ADDRESS_WRAP,
        D3D11_TEXTURE_ADDRESS_MODE _v = D3D11_TEXTURE_ADDRESS_WRAP, 
        D3D11_TEXTURE_ADDRESS_MODE _w = D3D11_TEXTURE_ADDRESS_WRAP,
        UINT _MaxAnisotropy = 0,
        D3D11_COMPARISON_FUNC _Func = D3D11_COMPARISON_NEVER);
    ComPtr<ID3D11SamplerState> FindSampler(const std::string& _Name);
    void SetSampler(const std::string& _Name, int _Register, int _ShaderBufferType = EShaderBufferType::Pixel);
    void ReleaseAsset(const std::string& _Name);

};

