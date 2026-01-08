#pragma once
#include "Object\GameObject.h"

class CMonsterSpawnPoint :
	public CGameObject
{
	friend class CWorld;
	friend class CObject;

protected:
	CMonsterSpawnPoint();
	CMonsterSpawnPoint(const CMonsterSpawnPoint& _Ref);
	CMonsterSpawnPoint(CMonsterSpawnPoint&& _Ref) noexcept;

public:
	virtual ~CMonsterSpawnPoint();

private:
	size_t							m_SpawnClass = 0;
	double							m_Time = 0.0;
	double							m_SpawnTime = 1.0;
	std::weak_ptr<class CMonster>	m_SpawnMonster;	// 원본 이 대상을 복제하여 사용할 것임

public:
	void SetSpawnTime(float _SpawnTime)
	{
		m_SpawnTime = _SpawnTime;
	}

	template<typename T>
	void SetSpawnClass()
	{
		m_SpawnClass = typeid(T).hash_code();
	}

public:
	virtual bool Init();
	virtual void Update(double _DeltaTime);

private:
	void Spawn(double _DeltaTime);
};

