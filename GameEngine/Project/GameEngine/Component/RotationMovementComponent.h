#pragma once
#include "MovementComponent.h"

class CRotationMovementComponent :
    public CMovementComponent
{
	friend class CGameObject;

protected:
	CRotationMovementComponent();
	CRotationMovementComponent(const CRotationMovementComponent& _Ref);
	CRotationMovementComponent(CRotationMovementComponent&& _Ref)	noexcept;

public:
	virtual ~CRotationMovementComponent();

protected:
	FVector3		m_MoveDir = FVector3::Zero;
	float			m_Speed = 200.f;
	FVector3		m_Velocity;

	FVector3		m_RotAxis;		// 회전축
	float			m_RotSpeed;		// 회전속도

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

	const FVector3&	GetRotAxis() { return m_RotAxis; }
	float GetRotSpeed() { return m_RotSpeed; }

public:
	void SetSpeed(float _Speed)
	{
		m_Speed = _Speed;
	}

	void AddMove(const FVector3& _Dir)
	{
		m_MoveDir += _Dir;
	}

	void SetRotAxis(const FVector3& _RotAxis) { m_RotAxis = _RotAxis; }
	void SetRotSpeed(float _RotSpeed) { m_RotSpeed = _RotSpeed; }

public:
	virtual bool Init();
	virtual void Update(double _DeltaTime);
	virtual void PostUpdate(double _DeltaTime);
	virtual void Destroy();

protected:
	virtual CRotationMovementComponent* Clone()	const;

private:
	void Move(double _DeltaTime);
	void Rotation(double _DeltaTime);
};

