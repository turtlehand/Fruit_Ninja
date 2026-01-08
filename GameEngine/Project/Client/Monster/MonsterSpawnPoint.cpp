#include "pch.h"
#include "Object.h"
#include "MonsterSpawnPoint.h"
#include "Monster/Monster.h"
#include "World/World.h"
#include "Component/SceneComponent.h"

CMonsterSpawnPoint::CMonsterSpawnPoint() :
	CGameObject(),
	m_SpawnClass(0),
	m_Time(0.0),
	m_SpawnTime(1.0),
	m_SpawnMonster()
{
	SetClassType<CMonsterSpawnPoint>();
}

CMonsterSpawnPoint::CMonsterSpawnPoint(const CMonsterSpawnPoint& _Ref) :
	CGameObject(_Ref),
	m_SpawnClass(_Ref.m_SpawnClass),
	m_Time(_Ref.m_Time),
	m_SpawnTime(_Ref.m_SpawnTime),
	m_SpawnMonster()
{

}

CMonsterSpawnPoint::CMonsterSpawnPoint(CMonsterSpawnPoint&& _Ref) noexcept :
	CGameObject(std::move(_Ref)),
	m_SpawnClass(std::move(_Ref.m_SpawnClass)),
	m_Time(std::move(_Ref.m_Time)),
	m_SpawnTime(std::move(_Ref.m_SpawnTime)),
	m_SpawnMonster(std::move(_Ref.m_SpawnMonster))
{

}

CMonsterSpawnPoint::~CMonsterSpawnPoint()
{
}

bool CMonsterSpawnPoint::Init()
{
	CGameObject::Init();

	CreateComponent<CSceneComponent>("Root");

	return true;
}

void CMonsterSpawnPoint::Update(double _DeltaTime)
{
	Spawn(_DeltaTime);

	CGameObject::Update(_DeltaTime);

}

void CMonsterSpawnPoint::Spawn(double _DeltaTime)
{
	// 만약 생성한 몬스터가 제거 되었거나 처음 시작한다면
	// 몬스터를 생성한다.
	if (m_SpawnMonster.expired())
	{
		m_Time += _DeltaTime;

		// 시간이 지나면 생성
		if (m_Time >= m_SpawnTime)
		{
			m_Time = 0.f;

			std::weak_ptr<CObject> Origin = CObject::FindCDO<CObject>(m_SpawnClass);

			std::shared_ptr<CMonster> OriginMonster = std::dynamic_pointer_cast<CMonster>(Origin.lock());
			std::shared_ptr<CWorld> World = m_World.lock();

			std::weak_ptr<CMonster> SpawnMonster = World->CreateCloneGameObject<CMonster>("Monster", OriginMonster);

			std::shared_ptr<CMonster> Monster = std::dynamic_pointer_cast<CMonster>(SpawnMonster.lock());

			Monster->SetRelativePos(GetWorldPos());
			//Monster->SetRelativeRotation(GetWorldRot());

			m_SpawnMonster = Monster;


		}
	}
}
