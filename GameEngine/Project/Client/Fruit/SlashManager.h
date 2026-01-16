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
	//std::weak_ptr<class CCameraComponent>             m_CameraC;
	std::weak_ptr<class CMeshComponent>                 m_SlashSimul;
	FLine2DInfo                                         m_Line2DInfo;
	std::weak_ptr<class CSlash>                         m_SlashObject;

	FVector2											m_PrevPos;			// 이전 마우스 위치

	// 마우스 속력
	float												m_CriteriaSpeed;	// 기준 속력
	bool												m_IsBeyond;		// 기준 속력을 넘어섰는지 여부

public:
	virtual bool Init() override;
	virtual void Update(double _DeltaTime) override;

private:
	void Start_Simul();
	void Update_Simul();
	void Active_Slash();

	void Slash_Mouse_Speed(double _DeltaTime);

};

