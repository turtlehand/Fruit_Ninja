#pragma once
#include "MovementComponent.h"

class CProjectileMovementComponent :
    public CMovementComponent
{
    friend class CGameObject;
protected:
	CProjectileMovementComponent();
	CProjectileMovementComponent(const CProjectileMovementComponent& _Ref);
	CProjectileMovementComponent(CProjectileMovementComponent&& _Ref)	noexcept;

public:
	virtual ~CProjectileMovementComponent();

protected:
	FVector3								m_MoveDir;			// 방향
	float									m_Speed;			// 속력
	FVector3								m_Velocity;			// 속도
	float									m_Range;

	// 사정거리 끝까지 날아간 뒤에 호출될 함수를 등록
	std::function<void()>	m_RangeFunction;

public:
	const FVector3& GetVelocity()	const { return m_Velocity; }

	const FVector3& GetMoveDir()	const { return m_MoveDir; }

	float GetSpeed()	const { return m_Speed; }

	float GetVelocityFrame()	const
	{
		if (m_MoveDir.IsZero())
			return 0.f;

		return m_Speed * (float)CTimer::GetDeltaTime();
	}

	float GetRange() const { return m_Range; }

public:
	void SetSpeed(float _Speed) { m_Speed = _Speed;}
	
	void SetRange(float _Range) { m_Range = _Range; }

	void AddMove(const FVector3& _Dir) { m_MoveDir += _Dir;}

public:
	virtual bool Init();
	virtual void Update(double _DeltaTime);
	virtual void PostUpdate(double _DeltaTime);
	virtual void Destroy();

protected:
	virtual CProjectileMovementComponent* Clone()	const;

public:
	template <typename T>
	void SetRangeFunction(T* Obj, void(T::* Func)())
	{
		m_RangeFunction = std::bind(Func, Obj);
	}
};

