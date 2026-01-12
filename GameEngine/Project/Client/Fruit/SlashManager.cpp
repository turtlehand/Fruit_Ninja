#include "pch.h"
#include "SlashManager.h"

#include "World/World.h"
#include "Component/MeshComponent.h"
#include "World/Input.h"
#include "Asset/Shader/Shader.h"
#include "Asset/Shader/ShaderManager.h"

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


	auto MeshC = m_SlashSimul.lock();
	
	if (MeshC)
	{
		MeshC->SetMesh("LineUP2D_Material");
		MeshC->SetShader("MaterialColor2D");
		MeshC->SetMaterialBaseColor(0, FVector4::Green);
		MeshC->SetInheritScale(false);
	}

	// 플레이어에서 사용할 키를 등록한다.

	auto	Input = World->GetInput().lock();

	Input->AddBindFunction<CSlashManager>("MousePress", VK_LBUTTON, EInputType::Press, this, &CSlashManager::MousePress);
	Input->AddBindFunction<CSlashManager>("MouseHold", VK_LBUTTON, EInputType::Hold, this, &CSlashManager::MouseHold);
	Input->AddBindFunction<CSlashManager>("MouseRelease", VK_LBUTTON, EInputType::Release, this, &CSlashManager::MouseRelease);

	return true;
}

void CSlashManager::Update(double _DeltaTime)
{
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

	MeshC->SetEnable(false);
	FVector3 Scale = MeshC->GetWorldScale();

}
