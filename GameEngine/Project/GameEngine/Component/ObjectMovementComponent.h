#pragma once
#include "MovementComponent.h"

class CObjectMovementComponent :
	public CMovementComponent
{
	friend class CGameObject;

protected:
	CObjectMovementComponent();
	CObjectMovementComponent(const CObjectMovementComponent& ref);
	CObjectMovementComponent(CObjectMovementComponent&& ref)	noexcept;

public:
	virtual ~CObjectMovementComponent();

protected:
	FVector3		m_MoveDir = FVector3::Zero;
	float			m_Speed = 200.f;
	FVector3		m_Velocity;

public:
	const FVector3& GetVelocity()	const
	{
		return m_Velocity;
	}

	const FVector3& GetMoveDir()	const
	{
		return m_MoveDir;
	}

	float GetSpeed()	const
	{
		return m_Speed;
	}

	float GetVelocityFrame()	const
	{
		if (m_MoveDir.IsZero())
			return 0.f;

		return m_Speed * (float)CTimer::GetDeltaTime();
	}

public:
	void SetSpeed(float _Speed)
	{
		m_Speed = _Speed;
	}

	void AddMove(const FVector3& _Dir)
	{
		m_MoveDir += _Dir;
	}

public:
	virtual bool Init();
	virtual void Update(double _DeltaTime);
	virtual void PostUpdate(double _DeltaTime);
	virtual void Destroy();

protected:
	virtual CObjectMovementComponent* Clone()	const;
};

