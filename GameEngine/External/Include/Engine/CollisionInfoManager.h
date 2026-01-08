#pragma once
class CCollisionInfoManager : public CSingleton<CCollisionInfoManager>
{
	friend class CSingleton<CCollisionInfoManager>;
private:
	CCollisionInfoManager();
	~CCollisionInfoManager();

private:
	std::unordered_map<std::string, FCollisionChannel*> m_ChannelMap;
	ECollisionChannel::Type m_CreateChannel = ECollisionChannel::Static;

	std::unordered_map<std::string, FCollisionProfile*> m_ProfileMap;

public:
	bool Init();
	void CreateChannel(const std::string& _Name);
	bool CreateProfile(const std::string& _Name,
		const std::string& _ChannelName, bool _Enable,
		ECollisionInteraction::Type _DefaultInteraction =
		ECollisionInteraction::Collision);
	bool CreateProfile(const std::string& _Name,
		ECollisionChannel::Type _Channel, bool _Enable,
		ECollisionInteraction::Type _DefaultInteraction =
		ECollisionInteraction::Collision);
	bool SetProfileInteraction(const std::string& _Name,
		const std::string& _ChannelName,
		ECollisionInteraction::Type _Interaction);
	bool SetProfileInteraction(const std::string& _Name,
		ECollisionChannel::Type _Channel,
		ECollisionInteraction::Type _Interaction);
	bool SetProfileEnable(const std::string& _Name, bool _Enable);

	FCollisionProfile* FindProfile(const std::string& _Name);

private:
	FCollisionChannel* FindChannel(const std::string& _Name);
};

