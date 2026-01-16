#pragma once
#include "Engine\Object\GameObject.h"

class CFruitManager :
	public CGameObject
{
	friend class CWorld;
	friend class CObject;

protected:
	CFruitManager();
	CFruitManager(const CFruitManager& _Ref);
	CFruitManager(CFruitManager&& _Ref) noexcept;

public:
	virtual ~CFruitManager();

private:
	//std::weak_ptr<class CCameraComponent>               m_CameraC;
	std::list<class CFruit>	m_Fruits;

	float				m_FruitScale;

public:
	virtual bool Init() override;
	virtual void Update(double _DeltaTime) override;

private:
	void Spawn();

	void SpawnApple();
	void SpawnBanana();
	void SpawnStar();

	void Random_Transform(std::weak_ptr<class CSceneComponent> _SC);

};

