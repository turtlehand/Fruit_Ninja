#include "pch.h"
#include "SlashManager.h"

#include "Slash.h"

#include "World/World.h"
#include "Component/MeshComponent.h"
#include "World/Input.h"
#include "Asset/Shader/Shader.h"
#include "Asset/Shader/ShaderManager.h"
#include "Component/ColliderLine2D.h"

CSlashManager::CSlashManager() :
	m_SlashSimul(),
	m_Line2DInfo(),
	m_SlashObject(),
	m_Duration(0.01666666666666),
	m_Time(0),
	m_PrevPos(),
	m_CriteriaSpeed(500.f),
	m_IsBeyond(false)
{
}

CSlashManager::CSlashManager(const CSlashManager& _Ref) :
	m_SlashSimul(),
	m_Line2DInfo(),
	m_SlashObject(),
	m_PrevPos(),
	m_CriteriaSpeed(_Ref.m_CriteriaSpeed),
	m_IsBeyond(false)
{
}

CSlashManager::CSlashManager(CSlashManager&& _Ref) noexcept :
	m_SlashSimul(),
	m_Line2DInfo(),
	m_SlashObject(),
	m_PrevPos(),
	m_CriteriaSpeed(std::move(_Ref.m_CriteriaSpeed)),
	m_IsBeyond(false)
{
}

CSlashManager::~CSlashManager()
{
}

bool CSlashManager::Init()
{
	auto	World = m_World.lock();

	// 베기의 시뮬레이션
	m_SlashSimul = CreateComponent<CMeshComponent>("SlashSimul");

	auto MeshC = m_SlashSimul.lock();
	
	if (MeshC)
	{
		MeshC->SetMesh("LineUP2D_Material");
		MeshC->SetShader("MaterialColor2D");
		MeshC->SetMaterialBaseColor(0, FVector4::Green);
		MeshC->SetInheritScale(false);
		MeshC->SetEnable(false);
	}

	// 실제 베기 오브젝트
	m_SlashObject = World->CreateGameObject<CSlash>("Slash");

	auto SSlash = m_SlashObject.lock();

	if (SSlash)
	{
		SSlash->SetEnble(false);
	}

	//Slash_Mouse_Click();

	return true;
}

void CSlashManager::Update(double _DeltaTime)
{
	auto	World = m_World.lock();
	auto	Input = World->GetInput().lock();

	//if (m_SlashObject.lock()->GetEnble())
	//{
	//	if (!m_Tick)
	//		m_Tick = true;
	//	else
	//	{
	//		m_SlashObject.lock()->SetEnble(false);
	//		m_Tick = false;
	//	}
	//}
	if (m_SlashObject.lock()->GetEnble())
	{
		m_SlashObject.lock()->SetEnble(false);
	}
	Slash_Mouse_Speed(_DeltaTime);
}

void CSlashManager::Start_Simul()
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

void CSlashManager::Update_Simul()
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

void CSlashManager::Active_Slash()
{
	auto MeshC = m_SlashSimul.lock();
	auto World = m_World.lock();
	auto SSlash = m_SlashObject.lock();


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

/// <summary>
/// 마우스 속력에 따라 베기
/// </summary>
void CSlashManager::Slash_Mouse_Speed(double _DeltaTime)
{
	m_Time += _DeltaTime;

	if (m_Time < m_Duration)
		return;

	m_Time -= m_Duration;

	auto	World = m_World.lock();
	auto	Input = World->GetInput().lock();

	FVector2 DisPos = Input->GetMouseGamePos();

	float Dis = (m_PrevPos - DisPos).Length();

	double Speed = Dis / m_Duration;

	// 순간 속력이 기준 속력을 넘어가면 베기 시작 위치 지정
	if (!m_IsBeyond && m_CriteriaSpeed < Speed)
	{
		m_Line2DInfo.Start = FVector3(DisPos.x, DisPos.y, 0.f);
		m_IsBeyond = true;
	}
	// 순간 속력이 기준 속력아래로 줄었다면 베기 끝 위치 지정
	// 베기 활성화
	else if (m_IsBeyond && Speed < m_CriteriaSpeed)
	{
		m_Line2DInfo.End = FVector3(DisPos.x, DisPos.y, 0.f);
		Active_Slash();
		m_IsBeyond = false;
	}

	m_PrevPos = Input->GetMouseGamePos();
}

void CSlashManager::Slash_Mouse_Click()
{
	auto	World = m_World.lock();
	auto	Input = World->GetInput().lock();

	Input->AddBindFunction<CSlashManager>("Start_Simul", VK_LBUTTON, EInputType::Press, this, &CSlashManager::Start_Simul);
	Input->AddBindFunction<CSlashManager>("Update_Simul", VK_LBUTTON, EInputType::Hold, this, &CSlashManager::Update_Simul);
	Input->AddBindFunction<CSlashManager>("Active_Slash", VK_LBUTTON, EInputType::Release, this, &CSlashManager::Active_Slash);
}
