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

CFruitManager::CFruitManager()
{
}

CFruitManager::CFruitManager(const CFruitManager& _Ref)
{
}

CFruitManager::CFruitManager(CFruitManager&& _Ref) noexcept
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

	Input->AddBindFunction<CFruitManager>("SpawnFruit", VK_SPACE, EInputType::Press, this, &CFruitManager::SpawnFruit);

	std::srand((unsigned int)time(NULL));


	return true;
}

void CFruitManager::Update(double _DeltaTime)
{

}

void CFruitManager::SpawnFruit()
{
	auto	World = m_World.lock();

	float Random = std::rand() % 200 - 100;
	Random /= 100.f;

	std::weak_ptr<CFruit> WFruit = World->CreateGameObject<CFruit>("Test Fruit");
	auto Fruit = WFruit.lock();
	Fruit->SetRelativeScale(300.f, 300.f);
	Fruit->SetRelativePos(Random * 500, -300.f);
	Fruit->CreateApplePolygon();

	std::weak_ptr<CDynamicMeshComponent> WFMesh = Fruit->FindComponent<CDynamicMeshComponent>("Root");
	auto FMesh = WFMesh.lock();

	Random = std::rand() % 200 - 100;
	Random /= 100.f;

	FMesh->AddForce(FVector3(20000.f * Random, 33000.f, 0.f));
}
