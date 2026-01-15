#pragma once

class CWorldCollision
{
	friend class CWorld;

private:
	CWorldCollision();

public:
	~CWorldCollision();

private:
	std::vector<std::weak_ptr<class CCollider>>	m_NextColliderList;
	std::vector<std::weak_ptr<class CCollider>>	m_DeleteColliderList;
	std::list<std::weak_ptr<class CCollider>>	m_ColliderList;
	float										m_Interval;
	float										m_IntervalTime;

public:
	void SetInterval(float _Interval) { m_Interval = _Interval; }
	void AddCollider(const std::weak_ptr<class CCollider>& _Collider);
public:
	bool Init();
	void Update(double _DeltaTime);
};

