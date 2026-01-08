#pragma once
#include "../Asset.h"

// 이 클래스가 하나의 모션이다.
// 걷기, 달리기, 대기, 공격 4가지 모션을 가지고 있는 애니메이션이 있다면 이 클래스가 4개 생성이 되어야 한다.
// 모션 하나당 하나의 객체
class CAnimation2D :
	public CAsset
{
	friend class CAnimation2DManager;

protected:
	CAnimation2D();

public:
	virtual ~CAnimation2D();

protected:
	std::weak_ptr<class CTexture>   m_Texture;		// 이 모션이 사용하는 Texture
	EAnimation2DTextureType         m_TextureType;
	std::vector<FTextureFrame>      m_FrameArray;	// 이 모션이 가지고 있는 장면(스프라이트)들

public:
	EAnimation2DTextureType GetAnimationTextureType() const { return m_TextureType; }
	const std::weak_ptr<class CTexture>& GetTexture() const { return m_Texture; }
	const FTextureFrame& GetFrame(int _Index) { return m_FrameArray[_Index]; }
	int GetFrameCount() { return (int)m_FrameArray.size(); }

	void SetAnimation2DTextureType(EAnimation2DTextureType _Type) { m_TextureType = _Type; }

	void SetTexture(const std::weak_ptr<class CTexture>& _Texture);
	void SetTexture(const std::string& _Name);
	void SetTexture(const std::string& _Name, const wchar_t* _FileName, const std::wstring& _PathName = L"Texture");
	void SetTextureFullPath(const std::string& _Name, const wchar_t* _FullPath);
	void SetTexture(const std::string& _Name, const std::vector<const wchar_t*>& _FileName, const std::wstring& _PathName = L"Texture");
	void SetTextureFullPath(const std::string& _Name, const std::vector<const wchar_t*>& _FullPath);

	void SetName(const std::string& _Name) { m_Name = _Name; }

	void AddFrame(const FVector2& _Start, const FVector2& _Size);
	void AddFrame(float _StartX, float _StartY, float _SizeX, float _SizeY);
	void AddFrame(int _Count, const FVector2& _Start, const FVector2& _Size);
	void AddFrame(int _Count, float _StartX, float _StartY, float _SizeX, float _SizeY);
	

};

