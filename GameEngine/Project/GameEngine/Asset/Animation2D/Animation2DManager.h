#pragma once
#include "Animation2D.h"

class CAnimation2DManager
{
    friend class CAssetManager;
    // 소멸자는 공개를 해야지만 shared_ptr에 정상적으로 넣을 수 있다.
    // shared_ptr에서 소멸자를 사용해서 그런것 같다.
private:
    CAnimation2DManager();
public:
    ~CAnimation2DManager();
private:
    std::unordered_map<std::string, std::shared_ptr<class CAnimation2D>> m_Animation2DMap;

public:
    bool Init();

public:
    bool CreateAnimation(const std::string& _Name);
    std::weak_ptr<class CAnimation2D> FindAnimation(const std::string& _Name);
    bool SetAnimation2DTextureType(const std::string& _Name, EAnimation2DTextureType _Type);

    /*
    bool SetTexture(const std::string& _AnimationName, const std::weak_ptr<class CTexture>& _Texture);
    bool SetTexture(const std::string& _AnimationName, const std::string& _Name);
    bool SetTexture(const std::string& _AnimationName, const std::string& _Name, const wchar_t* _FileName, const std::wstring& _PathName = L"Texture");
    bool SetTextureFullPath(const std::string& _AnimationName, const std::string& _Name, const wchar_t* _FullPath);
    bool SetTexture(const std::string& _AnimationName, const std::string& _Name, const std::vector<const wchar_t*>& _FileName, const std::wstring& _PathName = L"Texture");
    bool SetTextureFullPath(const std::string& _AnimationName, const std::string& _Name, const std::vector<const wchar_t*>& _FullPath);
    */
    bool AddFrame(const std::string& _AnimationName, const FVector2& _Start, const FVector2& _Size);
    bool AddFrame(const std::string& _AnimationName, float _StartX, float _StartY, float _SizeX, float _SizeY);
    bool AddFrame(const std::string& _AnimationName, int _Count, const FVector2& _Start, const FVector2& _Size);
    bool AddFrame(const std::string& _AnimationName, int _Count, float _StartX, float _StartY, float _SizeX, float _SizeY);
    void ReleaseAsset(const std::string& _Name);
};

