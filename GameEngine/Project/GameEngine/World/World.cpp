#include "pch.h"
#include "World.h"
#include "Object/GameObject.h"
#include "World/CameraManager.h"
#include "WorldCollision.h"

CWorld::CWorld()
{
}

CWorld::~CWorld()
{
}

bool CWorld::Init()
{
	m_CameraManager.reset(new CCameraManager);

	if (!m_CameraManager->Init())
		return false;

	m_WorldAssetManager.reset(new CWorldAssetManager);

	if (!m_WorldAssetManager->Init())
		return false;

	m_Input.reset(new CInput);

	if (!m_Input->Init())
		return false;

	m_Collision.reset(new CWorldCollision);

	if (!m_Collision->Init())
		return false;


	m_ObjectList.reserve(10000);

	m_StartObjList.reserve(100);

	return true;
}

void CWorld::Update(double _DeltaTime)
{
	Begin();

	// 입력은 오브젝트 업데이트 전에 해야 한다.
	m_Input->Update(_DeltaTime);

	auto iter = m_ObjectList.begin();
	auto iterEnd = m_ObjectList.end();

	for (; iter != iterEnd;)
	{
		// 파괴되었거나 더 이상 참조하지 않을 때는 지운다.
		if (!iter->second->GetAlive() || iter->second.use_count() == 0)
		{
			iter = m_ObjectList.erase(iter);
			iterEnd = m_ObjectList.end();
			continue;
		}
		// 비활성화 상태라면
		else if (!iter->second->GetEnble())
		{
			++iter;
			continue;
		}
		iter->second->Update(_DeltaTime);
		++iter;
	}

	m_CameraManager->Update(_DeltaTime);
}

void CWorld::PostUpdate(double _DeltaTime)
{
	auto iter = m_ObjectList.begin();
	auto iterEnd = m_ObjectList.end();

	for (; iter != iterEnd;)
	{
		// 파괴되었거나 더 이상 참조하지 않을 때는 지운다.
		if (!iter->second->GetAlive() || iter->second.use_count() == 0)
		{
			iter = m_ObjectList.erase(iter);
			iterEnd = m_ObjectList.end();
			continue;
		}
		// 비활성화 상태라면
		else if (!iter->second->GetEnble())
		{
			++iter;
			continue;
		}
		iter->second->PostUpdate(_DeltaTime);
		++iter;
	}

	// 모든 데이터가 업데이트가 완료된 후에 충돌을 진행한다.
	m_Collision->Update(_DeltaTime);
}


//void CWorld::Render()
//{
//	auto iter = m_ObjectList.begin();
//	auto iterEnd = m_ObjectList.end();
//
//	for (; iter != iterEnd;)
//	{
//		// 파괴되었거나 더 이상 참조하지 않을 때는 지운다.
//		if (!iter->second->GetAlive() || iter->second.use_count() == 0)
//		{
//			iter = m_ObjectList.erase(iter);
//			iterEnd = m_ObjectList.end();
//			continue;
//		}
//		// 비활성화 상태라면
//		else if (!iter->second->GetEnble())
//		{
//			++iter;
//			continue;
//		}
//		iter->second->Render();
//		++iter;
//	}
//}

void CWorld::Begin()
{
	if (!m_StartObjList.empty())
	{
		size_t Size = m_StartObjList.size();

		for (size_t i = 0; i < Size; ++i)
		{
			auto Obj = m_StartObjList[i].lock();
			Obj->Begin();
		}

		m_StartObjList.clear();
	}
}