#include "pch.h"
#include "Animation2D.h"
#include "../Texture/Texture.h"
#include "../Texture/TextureManager.h"
#include "../AssetManager.h"

CAnimation2D::CAnimation2D() :
	CAsset(EAssetType::Animation2D),
	m_TextureType(EAnimation2DTextureType::None)
{
}

CAnimation2D::~CAnimation2D()
{
}

void CAnimation2D::SetTexture(const std::weak_ptr<CTexture>& _Texture)
{
	m_Texture = _Texture;
}

void CAnimation2D::SetTexture(const std::string& _Name)
{
	auto	TextureManager = CAssetManager::GetInst()->GetTextureManager().lock();

	m_Texture = TextureManager->FindTexture(_Name);
}

void CAnimation2D::SetTexture(const std::string& _Name, const wchar_t* _FileName, const std::wstring& _PathName)
{
	auto	TextureManager = CAssetManager::GetInst()->GetTextureManager().lock();

	TextureManager->LoadTexture(_Name, _FileName, _PathName);

	m_Texture = TextureManager->FindTexture(_Name);
}

void CAnimation2D::SetTextureFullPath(const std::string& _Name, const wchar_t* _FullPath)
{
	auto	TextureManager = CAssetManager::GetInst()->GetTextureManager().lock();

	TextureManager->LoadTextureFullPath(_Name, _FullPath);

	m_Texture = TextureManager->FindTexture(_Name);
}

void CAnimation2D::SetTexture(const std::string& _Name, const std::vector<const wchar_t*>& _FileName, const std::wstring& _PathName)
{
	auto	TextureManager = CAssetManager::GetInst()->GetTextureManager().lock();

	TextureManager->LoadTexture(_Name, _FileName, _PathName);

	m_Texture = TextureManager->FindTexture(_Name);
}

void CAnimation2D::SetTextureFullPath(const std::string& _Name, const std::vector<const wchar_t*>& _FullPath)
{
	auto	TextureManager = CAssetManager::GetInst()->GetTextureManager().lock();

	TextureManager->LoadTextureFullPath(_Name, _FullPath);

	m_Texture = TextureManager->FindTexture(_Name);
}

void CAnimation2D::AddFrame(const FVector2& _Start, const FVector2& _Size)
{
	FTextureFrame	Frame;

	Frame.Start = _Start;
	Frame.Size = _Size;

	m_FrameArray.push_back(Frame);
}

void CAnimation2D::AddFrame(float _StartX, float _StartY, float _SizeX, float _SizeY)
{
	FTextureFrame	Frame;

	Frame.Start = FVector2(_StartX, _StartY);
	Frame.Size = FVector2(_SizeX, _SizeY);

	m_FrameArray.push_back(Frame);
}

void CAnimation2D::AddFrame(int _Count, const FVector2& _Start, const FVector2& _Size)
{
	for (int i = 0; i < _Count; ++i)
	{
		FTextureFrame	Frame;

		Frame.Start = _Start;
		Frame.Size = _Size;

		m_FrameArray.push_back(Frame);
	}
}

void CAnimation2D::AddFrame(int _Count, float _StartX, float _StartY, float _SizeX, float _SizeY)
{
	for (int i = 0; i < _Count; ++i)
	{
		FTextureFrame	Frame;

		Frame.Start = FVector2(_StartX, _StartY);
		Frame.Size = FVector2(_SizeX, _SizeY);

		m_FrameArray.push_back(Frame);
	}
}
