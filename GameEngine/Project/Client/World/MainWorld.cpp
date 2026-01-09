#include "pch.h"
#include "MainWorld.h"
#include "Player/Player.h"
#include "Monster/Monster.h"
#include "Monster/MonsterSpawnPoint.h"

#include "Asset/AssetManager.h"
#include "Asset/Animation2D/Animation2DManager.h"
#include "CollisionInfoManager.h"

CMainWorld::CMainWorld()
{
}

CMainWorld::~CMainWorld()
{
}

bool CMainWorld::Init()
{
	if (!CWorld::Init())
		return false;

	LoadAnimation2D();


	std::weak_ptr<CPlayer> Player = CreateGameObject<CPlayer>("Player");
	auto Player1 = Player.lock();
	Player1->SetRelativeScale(100.f, 100.f);


	//std::weak_ptr<CMonsterSpawnPoint> SpawnPoint1 = CreateGameObject<CMonsterSpawnPoint>("SpawnPoint");

	//std::shared_ptr<CMonsterSpawnPoint> Point = SpawnPoint1.lock();

	//if (Point)
	//{
	//	Point->SetRelativePos(-400.f, -300.f);
	//	Point->SetRelativeRotationZ(20.f);
	//	Point->SetSpawnClass<CMonster>();
	//	Point->SetSpawnTime(5.f);
	//}

	return true;
}

void CMainWorld::LoadAnimation2D()
{
	m_WorldAssetManager->CreateAnimation("PlayerIdle");
	m_WorldAssetManager->SetAnimation2DTextureType("PlayerIdle",
		EAnimation2DTextureType::Frame);

	std::vector<const TCHAR*>	TextureFileName;

	for (int i = 0; i < 7; ++i)
	{
		//TCHAR	FileName[MAX_PATH] = {};
		TCHAR* FileName = new TCHAR[MAX_PATH];
		memset(FileName, 0, sizeof(TCHAR) * MAX_PATH);
		wsprintf(FileName,
			TEXT("Player/PlayerFrame/adventurer-get-up-0%d.png"), i);
		TextureFileName.push_back(FileName);
	}

	m_WorldAssetManager->SetTexture("PlayerIdle", "PlayerIdle",
		TextureFileName);

	for (int i = 0; i < 7; ++i)
	{
		delete[] TextureFileName[i];
	}
	TextureFileName.clear();

	m_WorldAssetManager->AddFrame("PlayerIdle", 7, 0.f, 0.f, 50.f, 37.f);

	m_WorldAssetManager->CreateAnimation("PlayerWalk");
	m_WorldAssetManager->SetAnimation2DTextureType("PlayerWalk",
		EAnimation2DTextureType::SpriteSheet);

	m_WorldAssetManager->SetTexture("PlayerWalk", "PlayerSheet",
		TEXT("Player/Player.png"));

	m_WorldAssetManager->AddFrame("PlayerWalk", 200.f, 222.f, 50.f, 37.f);
	m_WorldAssetManager->AddFrame("PlayerWalk", 250.f, 222.f, 50.f, 37.f);
	m_WorldAssetManager->AddFrame("PlayerWalk", 300.f, 222.f, 50.f, 37.f);
	m_WorldAssetManager->AddFrame("PlayerWalk", 0.f, 259.f, 50.f, 37.f);
	m_WorldAssetManager->AddFrame("PlayerWalk", 50.f, 259.f, 50.f, 37.f);
	m_WorldAssetManager->AddFrame("PlayerWalk", 100.f, 259.f, 50.f, 37.f);

	m_WorldAssetManager->CreateAnimation("PlayerAttack");
	m_WorldAssetManager->SetAnimation2DTextureType("PlayerAttack",
		EAnimation2DTextureType::SpriteSheet);

	m_WorldAssetManager->SetTexture("PlayerAttack", "PlayerSheet",
		TEXT("Player/Player.png"));

	m_WorldAssetManager->AddFrame("PlayerAttack", 0.f, 0.f, 50.f, 37.f);
	m_WorldAssetManager->AddFrame("PlayerAttack", 50.f, 0.f, 50.f, 37.f);
	m_WorldAssetManager->AddFrame("PlayerAttack", 100.f, 0.f, 50.f, 37.f);
	m_WorldAssetManager->AddFrame("PlayerAttack", 150.f, 0.f, 50.f, 37.f);

	m_WorldAssetManager->CreateAnimation("MonsterIdle");
	m_WorldAssetManager->SetAnimation2DTextureType("MonsterIdle",
		EAnimation2DTextureType::SpriteSheet);

	m_WorldAssetManager->SetTexture("MonsterIdle", "MonsterSheet",
		TEXT("Monster.png"));

	for (int i = 0; i < 14; ++i)
	{
		m_WorldAssetManager->AddFrame("MonsterIdle", i * 45.f, 60.f,
			45.f, 60.f);
	}

	m_WorldAssetManager->CreateAnimation("MonsterAttack");
	m_WorldAssetManager->SetAnimation2DTextureType("MonsterAttack",
		EAnimation2DTextureType::SpriteSheet);

	m_WorldAssetManager->SetTexture("MonsterAttack", "MonsterSheet",
		TEXT("Monster.png"));

	for (int i = 0; i < 21; ++i)
	{
		m_WorldAssetManager->AddFrame("MonsterAttack", i * 45.f,
			180.f, 45.f, 60.f);
	}


	m_WorldAssetManager->CreateAnimation("Explosion");
	m_WorldAssetManager->SetAnimation2DTextureType("Explosion",
		EAnimation2DTextureType::Frame);

	for (int i = 1; i <= 89; ++i)
	{
		//TCHAR	FileName[MAX_PATH] = {};
		TCHAR* FileName = new TCHAR[MAX_PATH];
		memset(FileName, 0, sizeof(TCHAR) * MAX_PATH);
		wsprintf(FileName,
			TEXT("Explosion/Explosion%d.png"), i);
		TextureFileName.push_back(FileName);
	}

	m_WorldAssetManager->SetTexture("Explosion", "Explosion",
		TextureFileName);

	for (int i = 0; i < 89; ++i)
	{
		delete[] TextureFileName[i];
	}
	TextureFileName.clear();

	m_WorldAssetManager->AddFrame("Explosion", 89, 0.f, 0.f, 320.f, 240.f);

	m_WorldAssetManager->CreateAnimation("Charge");
	m_WorldAssetManager->SetAnimation2DTextureType("Charge",
		EAnimation2DTextureType::SpriteSheet);

	m_WorldAssetManager->SetTexture("Charge", "Charge",
		TEXT("HPPotion.png"));

	m_WorldAssetManager->AddFrame("Charge", 0.f, 0.f, 64.f, 64.f);
	m_WorldAssetManager->AddFrame("Charge", 64.f, 0.f, 64.f, 64.f);
	m_WorldAssetManager->AddFrame("Charge", 128.f, 0.f, 64.f, 64.f);
	m_WorldAssetManager->AddFrame("Charge", 192.f, 0.f, 64.f, 64.f);
	m_WorldAssetManager->AddFrame("Charge", 256.f, 0.f, 64.f, 64.f);

	m_WorldAssetManager->AddFrame("Charge", 0.f, 64.f, 64.f, 64.f);
	m_WorldAssetManager->AddFrame("Charge", 64.f, 64.f, 64.f, 64.f);
	m_WorldAssetManager->AddFrame("Charge", 128.f, 64.f, 64.f, 64.f);
	m_WorldAssetManager->AddFrame("Charge", 192.f, 64.f, 64.f, 64.f);
	m_WorldAssetManager->AddFrame("Charge", 256.f, 64.f, 64.f, 64.f);

	m_WorldAssetManager->AddFrame("Charge", 0.f, 128.f, 64.f, 64.f);
	m_WorldAssetManager->AddFrame("Charge", 64.f, 128.f, 64.f, 64.f);
	m_WorldAssetManager->AddFrame("Charge", 128.f, 128.f, 64.f, 64.f);
	m_WorldAssetManager->AddFrame("Charge", 192.f, 128.f, 64.f, 64.f);
	m_WorldAssetManager->AddFrame("Charge", 256.f, 128.f, 64.f, 64.f);

	m_WorldAssetManager->AddFrame("Charge", 0.f, 192.f, 64.f, 64.f);
	m_WorldAssetManager->AddFrame("Charge", 64.f, 192.f, 64.f, 64.f);
	m_WorldAssetManager->AddFrame("Charge", 128.f, 192.f, 64.f, 64.f);
	m_WorldAssetManager->AddFrame("Charge", 192.f, 192.f, 64.f, 64.f);
	m_WorldAssetManager->AddFrame("Charge", 256.f, 192.f, 64.f, 64.f);

}