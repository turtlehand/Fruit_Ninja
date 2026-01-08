#include "pch.h"
#include "Animation2DManager.h"

CAnimation2DManager::CAnimation2DManager()
{

}

CAnimation2DManager::~CAnimation2DManager()
{
}

bool CAnimation2DManager::Init()
{
	return true;
}

bool CAnimation2DManager::CreateAnimation(const std::string& _Name)
{
	if (!FindAnimation(_Name).expired())
		return true;

	std::shared_ptr<CAnimation2D> Animation(new CAnimation2D);

	Animation->SetName(_Name);

	m_Animation2DMap.insert(std::make_pair(_Name, Animation));

	return true;
}

std::weak_ptr<class CAnimation2D> CAnimation2DManager::FindAnimation(const std::string& _Name)
{
	auto iter = m_Animation2DMap.find(_Name);

	if (iter == m_Animation2DMap.end())
		return std::weak_ptr<CAnimation2D>();

	return iter->second;
}

bool CAnimation2DManager::SetAnimation2DTextureType(const std::string& _Name, EAnimation2DTextureType _Type)
{
	auto iter = m_Animation2DMap.find(_Name);

	if (iter == m_Animation2DMap.end())
		return false;

	iter->second->SetAnimation2DTextureType(_Type);

	return true;
}
/*
bool CAnimation2DManager::SetTexture(const std::string& _AnimationName, const std::weak_ptr<CTexture>& _Texture)
{
	auto	iter = m_Animation2DMap.find(_AnimationName);

	if (iter == m_Animation2DMap.end())
		return false;

	iter->second->SetTexture(_Texture);

	return true;
}

bool CAnimation2DManager::SetTexture(const std::string& _AnimationName, const std::string& _Name)
{
	auto	iter = m_Animation2DMap.find(_AnimationName);

	if (iter == m_Animation2DMap.end())
		return false;

	iter->second->SetTexture(_Name);

	return true;
}

bool CAnimation2DManager::SetTexture(const std::string& _AnimationName, const std::string& _Name, const wchar_t* _FileName, const std::wstring& _PathName)
{
	auto	iter = m_Animation2DMap.find(_AnimationName);

	if (iter == m_Animation2DMap.end())
		return false;

	iter->second->SetTexture(_Name, _FileName, _PathName);

	return true;
}

bool CAnimation2DManager::SetTextureFullPath(const std::string& _AnimationName, const std::string& _Name, const wchar_t* _FullPath)
{
	auto	iter = m_Animation2DMap.find(_AnimationName);

	if (iter == m_Animation2DMap.end())
		return false;

	iter->second->SetTextureFullPath(_Name, _FullPath);

	return true;
}

bool CAnimation2DManager::SetTexture(const std::string& _AnimationName, const std::string& _Name, const std::vector<const wchar_t*>& _FileName, const std::wstring& _PathName)
{
	auto	iter = m_Animation2DMap.find(_AnimationName);

	if (iter == m_Animation2DMap.end())
		return false;

	iter->second->SetTexture(_Name, _FileName, _PathName);

	return true;
}

bool CAnimation2DManager::SetTextureFullPath(const std::string& _AnimationName, const std::string& _Name, const std::vector<const wchar_t*>& _FullPath)
{
	auto	iter = m_Animation2DMap.find(_AnimationName);

	if (iter == m_Animation2DMap.end())
		return false;

	iter->second->SetTextureFullPath(_Name, _FullPath);

	return true;
}
*/
bool CAnimation2DManager::AddFrame(const std::string& _AnimationName, const FVector2& _Start, const FVector2& _Size)
{
	auto	iter = m_Animation2DMap.find(_AnimationName);

	if (iter == m_Animation2DMap.end())
		return false;

	iter->second->AddFrame(_Start, _Size);

	return true;
}

bool CAnimation2DManager::AddFrame(const std::string& _AnimationName, float _StartX, float _StartY, float _SizeX, float _SizeY)
{
	auto	iter = m_Animation2DMap.find(_AnimationName);

	if (iter == m_Animation2DMap.end())
		return false;

	iter->second->AddFrame(_StartX, _StartY, _SizeX, _SizeY);

	return true;
}

bool CAnimation2DManager::AddFrame(const std::string& _AnimationName, int _Count, const FVector2& _Start, const FVector2& _Size)
{
	auto	iter = m_Animation2DMap.find(_AnimationName);

	if (iter == m_Animation2DMap.end())
		return false;

	iter->second->AddFrame(_Count, _Start, _Size);

	return true;
}

bool CAnimation2DManager::AddFrame(const std::string& _AnimationName, int _Count, float _StartX, float _StartY, float _SizeX, float _SizeY)
{
	auto	iter = m_Animation2DMap.find(_AnimationName);

	if (iter == m_Animation2DMap.end())
		return false;

	iter->second->AddFrame(_Count, _StartX, _StartY, _SizeX, _SizeY);

	return true;
}


void CAnimation2DManager::ReleaseAsset(const std::string& _Name)
{
	auto iter = m_Animation2DMap.find(_Name);

	if (iter != m_Animation2DMap.end())
	{
		// 다른 월드에서 더 이상 들고 있지 않을 경우
		if (iter->second.use_count() == 1)
		{
			m_Animation2DMap.erase(iter);
		}
	}
}
