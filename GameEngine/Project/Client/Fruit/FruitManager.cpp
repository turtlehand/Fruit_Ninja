#include "pch.h"
#include "FruitManager.h"

#include "World/World.h"
#include "Component/MeshComponent.h"
#include "World/Input.h"
#include "Asset/Shader/Shader.h"
#include "Asset/Shader/ShaderManager.h"
#include "Component/ColliderLine2D.h"
#include "Component/DynamicMeshComponent.h"
#include "Fruit.h"


#include<cstdlib>
#include<ctime>

CFruitManager::CFruitManager() :
	m_Fruits(),
	m_FruitScale(300.f)
{
}

CFruitManager::CFruitManager(const CFruitManager& _Ref) :
	m_Fruits(),
	m_FruitScale(_Ref.m_FruitScale)
{
}

CFruitManager::CFruitManager(CFruitManager&& _Ref) noexcept :
	m_Fruits(),
	m_FruitScale(_Ref.m_FruitScale)
{
}

CFruitManager::~CFruitManager()
{
}

bool CFruitManager::Init()
{
	auto	World = m_World.lock();

	// 플레이어에서 사용할 키를 등록한다.

	auto	Input = World->GetInput().lock();

	Input->AddBindFunction<CFruitManager>("SpawnFruit", VK_SPACE, EInputType::Press, this, &CFruitManager::Spawn);

	std::srand((unsigned int)time(NULL));


	return true;
}

void CFruitManager::Update(double _DeltaTime)
{

}

void CFruitManager::Spawn()
{
	int Random = rand() % 3;

	switch (Random)
	{
	case 0:
		SpawnApple();
		break;
	case 1:
		SpawnBanana();
		break;
	case 2:
		SpawnStar();
		break;

	default:
		break;
	}
}

void CFruitManager::SpawnApple()
{
	auto	World = m_World.lock();

	// 사과 모양으로 만들기
	std::weak_ptr<CFruit> WFruit = World->CreateGameObject<CFruit>("Test Apple");
	auto Fruit = WFruit.lock();
	Fruit->CreateApplePolygon();

	// 위치, 방향 조정
	std::weak_ptr<CDynamicMeshComponent> WFMesh = Fruit->FindComponent<CDynamicMeshComponent>("Root");
	auto SFMesh = WFMesh.lock();
	SFMesh->AddTexture(0, "Apple", TEXT("Apple.png"));

	Random_Transform(WFMesh);
}

void CFruitManager::SpawnBanana()
{
	auto	World = m_World.lock();

	// 바나나 모양으로 만들기
	std::weak_ptr<CFruit> WFruit = World->CreateGameObject<CFruit>("Test Banana");
	auto Fruit = WFruit.lock();
	Fruit->CreateBananaPolygon();

	// 위치, 방향 조정
	std::weak_ptr<CDynamicMeshComponent> WFMesh = Fruit->FindComponent<CDynamicMeshComponent>("Root");
	auto SFMesh = WFMesh.lock();
	SFMesh->AddTexture(0, "Banana", TEXT("Banana.png"));


	Random_Transform(WFMesh);
}

void CFruitManager::SpawnStar()
{
	auto	World = m_World.lock();

	// 별 모양으로 만들기
	std::weak_ptr<CFruit> WFruit = World->CreateGameObject<CFruit>("Test Star");
	auto Fruit = WFruit.lock();
	Fruit->CreateStarPolygon();

	// 위치, 방향 조정
	std::weak_ptr<CDynamicMeshComponent> WFMesh = Fruit->FindComponent<CDynamicMeshComponent>("Root");
	auto SFMesh = WFMesh.lock();
	SFMesh->AddTexture(0, "Star", TEXT("Star.png"));


	Random_Transform(WFMesh);
}

void CFruitManager::Random_Transform(std::weak_ptr<class CSceneComponent> _SC)
{
	auto SC = _SC.lock();

	float Random = std::rand() % 200 - 100;
	Random /= 100.f;

	SC->SetRelativeScale(m_FruitScale, m_FruitScale);
	SC->SetRelativePos(Random * 500, -300.f);
	SC->AddForce(FVector3(10000.f * -Random, 33000.f, 0.f));
}
