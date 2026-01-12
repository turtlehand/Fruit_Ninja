#include "pch.h"
#include "WorldCollision.h"
#include "../Component/Collider.h"

CWorldCollision::CWorldCollision() :
	m_ColliderList(),
	m_Interval(0.f),
	m_IntervalTime(0.f)
{

}

CWorldCollision::~CWorldCollision()
{
}

void CWorldCollision::AddCollider(const std::weak_ptr<CCollider>& _Collider)
{
	m_ColliderList.push_back(_Collider);
}

bool CWorldCollision::Init()
{
	return true;
}

void CWorldCollision::Update(double _DeltaTime)
{
	if (m_Interval > 0.f)
	{
		m_IntervalTime += (float)_DeltaTime;

		if (m_IntervalTime < m_Interval)
			return;

		m_IntervalTime -= m_Interval;
	}

	if (m_ColliderList.empty())
		return;

	auto iter = m_ColliderList.begin();
	auto iterLast = m_ColliderList.end();
	--iterLast;

	auto iterEnd = m_ColliderList.end();

	// SrcCol 0 ~ (N - 1)
	for (; iter != iterLast;)
	{
#pragma region SrcCollider
		// 유효하지 않음
		if ((*iter).expired())
		{
			iter = m_ColliderList.erase(iter);
			iterLast = m_ColliderList.end();
			--iterLast;
			iterEnd = m_ColliderList.end();
			continue;
		}

		auto SrcCollider = (*iter).lock();

		// 죽었음
		if (!SrcCollider->GetAlive())
		{
			iter = m_ColliderList.erase(iter);
			iterLast = m_ColliderList.end();
			--iterLast;
			iterEnd = m_ColliderList.end();
			continue;
		}

		// 비활성화 상태
		else if (!SrcCollider->GetEnable() || !SrcCollider->GetGameObjectEnable())
		{
			++iter;
			continue;
		}

		FCollisionProfile* SrcProfile = SrcCollider->GetCollisionProfile();

		if (!SrcProfile->Enable)
		{
			++iter;
			continue;
		}
#pragma endregion

#pragma region DestCollider
		// 다른 콜라이더와 충돌 중인지 체크한다.
		auto iter1 = iter;
		++iter1;
		// Dest Col 1 ~ N

		for (; iter1 != iterEnd;)
		{
			// 유효하지 않음
			if ((*iter1).expired())
			{
				iter1 = m_ColliderList.erase(iter1);
				iterLast = m_ColliderList.end();
				--iterLast;
				iterEnd = m_ColliderList.end();
				continue;
			}

			auto DestCollider = (*iter1).lock();

			// 죽었음
			if (!DestCollider->GetAlive())
			{
				iter1 = m_ColliderList.erase(iter1);
				iterLast = m_ColliderList.end();
				--iterLast;
				iterEnd = m_ColliderList.end();
				continue;
			}

			// 비활성화 상태
			else if (!DestCollider->GetEnable() ||!DestCollider->GetGameObjectEnable())
			{
				++iter1;
				continue;
			}

			FCollisionProfile* DestProfile = DestCollider->GetCollisionProfile();

			if (!DestProfile->Enable)
			{
				++iter1;
				continue;
			}
#pragma endregion


			// 상대방 채널에 대해 충돌이 무시일 경우 충돌처리를 건너뛴다.
			else if (SrcProfile->Interaction[DestProfile->Channel->Channel] == ECollisionInteraction::Ignore ||
				DestProfile->Interaction[SrcProfile->Channel->Channel] == ECollisionInteraction::Ignore)
			{
				++iter1;
				continue;
			}

			// 실제 충돌처리를 진행한다.
			std::vector<FVector3> HitPoint;
			
			// Collision 함수의 반환값이 true일 경우 충돌 되었다는 의미이다.
			if (SrcCollider->Collision(HitPoint, DestCollider))
			{
				// 두 물체가 이전 프레임에 충돌이 되어 서로 충돌 오브젝트로 가지고 있는지 판단한다.
				if (!SrcCollider->CheckCollisionObject(DestCollider.get()))
				{
					SrcCollider->CallCollisionBegin(HitPoint, *iter1);
					DestCollider->CallCollisionBegin(HitPoint, *iter);
				}
				else 
				{
					SrcCollider->CallCollision(HitPoint, *iter1);
					DestCollider->CallCollision(HitPoint, *iter);
				}
			}
			// 두 물체가 현재 프레임에는 부딪히지 않았지만 이전 프레임까지는 부딪히고 있었을 경우는 이제 막 떨어지는 경우이다.
			else if (SrcCollider->CheckCollisionObject(DestCollider.get()))
			{
				SrcCollider->CallCollisionEnd(*iter1);
				DestCollider->CallCollisionEnd(*iter);
			}
			++iter1;
		}
		++iter;
	}
}