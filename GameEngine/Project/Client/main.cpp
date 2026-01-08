// WinTest.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#include "pch.h"
#include "Engine/Engine.h"
#include "World/World.h"
#include "World/WorldManager.h"
#include "World/MainWorld.h"

#include "Player/Player.h"
#include "Player/Bullet.h"
#include "Missile.h"
#include "Player/GuidedM.h"
#include "Monster/Monster.h"
#include "Monster/MonsterSpawnPoint.h"
#include "Component/StateComponent.h"

#include "EngineSetting.h"
#include "GlobalSetting.h"

#define MAX_LOADSTRING 100

// 전역 변수 :
HINSTANCE g_hInst = nullptr;						// 현재 인스턴스입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다.
//ATOM                MyRegisterClass(HINSTANCE hInstance);		// 윈도우 설정한다.
//BOOL				InitInstance(HINSTANCE, int);				// 윈도우를 초기화하여 창을 만든다.
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

// HINSTANCE : 이 프로그램의 식별번호이다.
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR IpCmdLine,		// 현재는 쓰이지 않는 인자다.
	_In_ int nCmdShow)			// 현재는 쓰이지 않는 인자다.
{

	CEngine::GetInst()->CreateEngineSetting<CGlobalSetting>();

	if (!CEngine::GetInst()->Init(hInstance, L"Window", 0, 0, 1280, 720, true ))
	{
		CEngine::GetInst()->DestroyInst();
		return 0;
	}

	CEngine::CreateCDO<CPlayer>();
	CEngine::CreateCDO<CBullet>();
	CEngine::CreateCDO<CMissile>();
	CEngine::CreateCDO<CMonster>();
	CEngine::CreateCDO<CMonsterSpawnPoint>();
	//CEngine::CreateCDO<CStateComponent>();

	// 시작 월드를 지정한다.
	if (CWorldManager::GetInst()->CreateWorld<CMainWorld>(false).expired())
		return 0;

	int Ret = CEngine::GetInst()->Run();


	//CEngine::GetInst()->DestroyInst();

	return Ret;

}