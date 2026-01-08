#pragma once
#include "../Asset.h"
#include "DirectXtex.h"

struct FTextureInfo
{
    // Texture 의 Pixel 정보를 저장하기 위한 객체이다.
    DirectX::ScratchImage* Image = nullptr;

    //위의 Pixel 정보를 이용하여 화면에 출력하기 위해 shader에 texture 정보를 바탕으로 보내주기 위한 객체이다ㅏ.
    ComPtr<ID3D11ShaderResourceView> SRV;

    unsigned int Width;
    unsigned int Height;
};

class CTexture :
    public CAsset
{
    friend class CTextureManager;

protected:
    CTexture();

public:
    virtual ~CTexture();

protected:
    // 이미지 여러장을 하나의 Texture로 구성 가능하도록 제작.
    std::vector<FTextureInfo*> m_TextureList;

public:
    const FTextureInfo* GetTexture(int _Index = 0) const
    {
        return m_TextureList[_Index];
    }

    int GetTextureCount() const
    {
        return (int)m_TextureList.size();
    }

public:
    bool LoadTexture(const wchar_t* _FileName, const std::wstring& _PathName);
    bool LoadTexture(const wchar_t* _FullPath);
    bool LoadTexture(const std::vector<const wchar_t*> _FileName, const std::wstring& _PathName);
    bool LoadTexture(const std::vector<const wchar_t*> _FullPath);
    void SetShader(int _Register, int _ShaderBufferType, int _TextureIndex);
private:
    bool CreateResourceView(int _Index = 0);
};

