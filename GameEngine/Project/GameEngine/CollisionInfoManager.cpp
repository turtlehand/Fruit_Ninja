#include "pch.h"
#include "CollisionInfoManager.h"

CCollisionInfoManager::CCollisionInfoManager()
{

}

CCollisionInfoManager::~CCollisionInfoManager()
{
	auto	iter = m_ProfileMap.begin();
	auto	iterEnd = m_ProfileMap.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}

	auto	iter1 = m_ChannelMap.begin();
	auto	iter1End = m_ChannelMap.end();

	for (; iter1 != iter1End; ++iter1)
	{
		SAFE_DELETE(iter1->second);
	}
}

bool CCollisionInfoManager::Init()
{
	CreateChannel("Static");
	CreateChannel("Player");
	CreateChannel("Monster");

	CreateProfile("Static", "Static", true);
	CreateProfile("Player", "Player", true);
	CreateProfile("Monster", "Monster", true);

	return true;
}

void CCollisionInfoManager::CreateChannel(const std::string& _Name)
{
	if (m_CreateChannel == ECollisionChannel::End)
		return;

	FCollisionChannel* Channel = FindChannel(_Name);

	if (Channel)
		return;

	Channel = new FCollisionChannel;

	Channel->Name = _Name;
	Channel->Channel = m_CreateChannel;

	m_CreateChannel = (ECollisionChannel::Type)(m_CreateChannel + 1);

	m_ChannelMap.insert(std::make_pair(_Name, Channel));
}

FCollisionChannel* CCollisionInfoManager::FindChannel(const std::string& _Name)
{
	std::unordered_map<std::string, FCollisionChannel*>::iterator iter = m_ChannelMap.find(_Name);

	if (iter == m_ChannelMap.end())
		return nullptr;

	return iter->second;
}

bool CCollisionInfoManager::CreateProfile(const std::string& _Name, const std::string& _ChannelName, bool _Enable, ECollisionInteraction::Type _DefaultInteraction)
{
	FCollisionProfile* Profile = FindProfile(_Name);

	if (Profile)
		return true;

	FCollisionChannel* Channel = FindChannel(_ChannelName);

	if (!Channel)
		return false;

	Profile = new FCollisionProfile;

	Profile->Name = _Name; 
	Profile->Channel = Channel;
	Profile->Enable = _Enable;

	for (int i = 0; i < ECollisionChannel::End; ++i)
	{
		Profile->Interaction[i] = _DefaultInteraction;
	}

	m_ProfileMap.insert(std::make_pair(_Name, Profile));

	return true;
}

bool CCollisionInfoManager::CreateProfile(const std::string& _Name, ECollisionChannel::Type _Channel, bool _Enable, ECollisionInteraction::Type _DefaultInteraction)
{
	FCollisionProfile* Profile = FindProfile(_Name);

	if (Profile)
		return true;

	FCollisionChannel* Channel = nullptr;

	std::unordered_map<std::string, FCollisionChannel*>::iterator iter = m_ChannelMap.begin();
	std::unordered_map<std::string, FCollisionChannel*>::iterator iterEnd = m_ChannelMap.end();

	for (; iter != iterEnd; ++iter)
	{
		if (iter->second->Channel == _Channel)
		{
			Channel = iter->second;
			break;
		}
	}

	if (!Channel)
		return false;

	Profile = new FCollisionProfile;

	Profile->Name = _Name;
	Profile->Channel = Channel;
	Profile->Enable = _Enable;

	for (int i = 0; i < ECollisionChannel::End; ++i)
	{
		Profile->Interaction[i] = _DefaultInteraction;
	}

	m_ProfileMap.insert(std::make_pair(_Name, Profile));

	return true;
}

bool CCollisionInfoManager::SetProfileInteraction(const std::string& _Name, const std::string& _ChannelName, ECollisionInteraction::Type _Interaction)
{
	FCollisionProfile* Profile = FindProfile(_Name);

	if (!Profile)
		return false;

	FCollisionChannel* Channel = FindChannel(_ChannelName);

	if (!Channel)
		return false;

	Profile->Interaction[Channel->Channel] = _Interaction;

	return true;
}

bool CCollisionInfoManager::SetProfileInteraction(const std::string& _Name, ECollisionChannel::Type _Channel, ECollisionInteraction::Type _Interaction)
{
	FCollisionProfile* Profile = FindProfile(_Name);

	if (!Profile)
		return false;

	Profile->Interaction[_Channel] = _Interaction;

	return true;
}

bool CCollisionInfoManager::SetProfileEnable(const std::string& _Name, bool _Enable)
{
	FCollisionProfile* Profile = FindProfile(_Name);

	if (!Profile)
		return false;

	Profile->Enable = _Enable;

	return true;
}

FCollisionProfile* CCollisionInfoManager::FindProfile(const std::string& _Name)
{
	auto	iter = m_ProfileMap.find(_Name);

	if (iter == m_ProfileMap.end())
		return nullptr;

	return iter->second;
}
