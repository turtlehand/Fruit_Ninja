#pragma once
#include "Engine\Object\GameObject.h"

class CSlashManager :
	public CGameObject
{
	friend class CWorld;
	friend class CObject;

protected:
	CSlashManager();
	CSlashManager(const CSlashManager& _Ref);
	CSlashManager(CSlashManager&& _Ref) noexcept;

public:
	virtual ~CSlashManager();

private:
	//std::weak_ptr<class CCameraComponent>               m_CameraC;
	std::weak_ptr<class CMeshComponent>                 m_SlashSimul;
	FLine2DInfo                                         m_Line2DInfo;
	std::weak_ptr<class CSlash>                         m_Slash;
	bool                                                m_SlashTick;

	std::list<std::pair<float, FVector2>>				m_PrePos;

	FVector2											m_PrevPos;
	bool												m_IsSlash;

public:
	virtual bool Init() override;
	virtual void Update(double _DeltaTime) override;

private:
	void MousePress();
	void MouseHold();
	void MouseRelease();

};

