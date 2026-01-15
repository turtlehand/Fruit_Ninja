#include "pch.h"
#include "SlashManager.h"

#include "Slash.h"

#include "World/World.h"
#include "Component/MeshComponent.h"
#include "World/Input.h"
#include "Asset/Shader/Shader.h"
#include "Asset/Shader/ShaderManager.h"
#include "Component/ColliderLine2D.h"

CSlashManager::CSlashManager()
{
}

CSlashManager::CSlashManager(const CSlashManager& _Ref)
{
}

CSlashManager::CSlashManager(CSlashManager&& _Ref) noexcept
{
}

CSlashManager::~CSlashManager()
{
}

bool CSlashManager::Init()
{
	auto	World = m_World.lock();

	m_SlashSimul = CreateComponent<CMeshComponent>("SlashSimul");
	m_Slash = World->CreateGameObject<CSlash>("Slash");


	auto MeshC = m_SlashSimul.lock();
	
	if (MeshC)
	{
		MeshC->SetMesh("LineUP2D_Material");
		MeshC->SetShader("MaterialColor2D");
		MeshC->SetMaterialBaseColor(0, FVector4::Green);
		MeshC->SetInheritScale(false);
		MeshC->SetEnable(false);
	}

	auto SSlash = m_Slash.lock();

	if (SSlash)
	{
		SSlash->SetEnble(false);
	}

	// 플레이어에서 사용할 키를 등록한다.

	auto	Input = World->GetInput().lock();

	//Input->AddBindFunction<CSlashManager>("MousePress", VK_LBUTTON, EInputType::Press, this, &CSlashManager::MousePress);
	//Input->AddBindFunction<CSlashManager>("MouseHold", VK_LBUTTON, EInputType::Hold, this, &CSlashManager::MouseHold);
	//Input->AddBindFunction<CSlashManager>("MouseRelease", VK_LBUTTON, EInputType::Release, this, &CSlashManager::MouseRelease);


	return true;
}

void CSlashManager::Update(double _DeltaTime)
{
	auto	World = m_World.lock();
	auto	Input = World->GetInput().lock();


	if (m_Slash.lock()->GetEnble())
	{
		if (m_SlashTick)
		{
			m_Slash.lock()->SetEnble(false);
			m_SlashTick = false;
		}
		else
		{
			m_SlashTick = true;
		}
	}

	// 0.1초 동안 움직인 거리가 100이라면 베기 판정
	FVector2 DisPos = Input->GetMouseGamePos();


	if (!m_PrePos.empty())
	{
		std::list <std::pair<float, FVector2>>::iterator iter = m_PrePos.begin();
		std::list <std::pair<float, FVector2>>::iterator iterEnd = m_PrePos.end();

		for (iter; iterEnd != iter;)
		{
			FVector2 PrevPos = iter->second;
			float Dis = (iter->second - Input->GetMouseGamePos()).Length();
			float Time = iter->first + _DeltaTime;
			iter->first = Time;

			if (0.05f < Time)
			{
				iter = m_PrePos.erase(iter);
				iterEnd = m_PrePos.end();

				if (300.0f < Dis)
				{
					m_Line2DInfo.Start = FVector3(PrevPos.x, PrevPos.y, 0.f);
					m_Line2DInfo.End = FVector3(DisPos.x, DisPos.y, 0.f);
					MouseRelease();
					break;
				}

			}
			else
				++iter;
		}
	}

	m_PrePos.push_back(std::make_pair(0.f, DisPos));



}

void CSlashManager::MousePress()
{
	auto MeshC = m_SlashSimul.lock();
	auto World = m_World.lock();
	auto Input = World->GetInput().lock();

	FVector2 MousePos = Input->GetMouseGamePos();

	m_Line2DInfo.Start = FVector3(MousePos.x, MousePos.y, 0.f);
	m_Line2DInfo.End = FVector3(MousePos.x, MousePos.y, 0.f);

	MeshC->SetEnable(true);
	MeshC->SetRelativePos(m_Line2DInfo.Start);
	MeshC->SetRelativeScale(1.f, (m_Line2DInfo.End - m_Line2DInfo.Start).Length(), 1.f);
}

void CSlashManager::MouseHold()
{
	auto MeshC = m_SlashSimul.lock();
	auto World = m_World.lock();
	auto Input = World->GetInput().lock();

	FVector2 MousePos = Input->GetMouseGamePos();

	//m_Line2DInfo.Start = FVector3(MousePos.x, MousePos.y, 0.f);
	m_Line2DInfo.End = FVector3(MousePos.x, MousePos.y, 0.f);

	FVector3 Dir = m_Line2DInfo.End - m_Line2DInfo.Start;
	float Dis = Dir.Length();
	MeshC->SetRelativeScale(1.f, Dis, 1.f);
	Dir.Normalize();
	MeshC->SetRelativeRotationZ(FVector3::GetAngle2D( FVector3::Axis[EAxis::Y], Dir));
}

void CSlashManager::MouseRelease()
{
	auto MeshC = m_SlashSimul.lock();
	auto World = m_World.lock();
	auto SSlash = m_Slash.lock();


	MeshC->SetEnable(false);
	FVector3 Scale = MeshC->GetWorldScale();

	
	SSlash->SetEnble(true);
	std::weak_ptr<CColliderLine2D> WSlashCollider = SSlash->FindComponent<CColliderLine2D>("Collider");
	auto SSlashCollider = WSlashCollider.lock();

	FVector3 Dir = m_Line2DInfo.End - m_Line2DInfo.Start;
	float Dis = Dir.Length();

	SSlashCollider->SetRelativePos(m_Line2DInfo.Start);
	SSlashCollider->SetRelativeRotationZ(FVector3::GetAngle2D(FVector3::Axis[EAxis::Y], Dir));
	SSlashCollider->SetLineDistance(Dis);

}
