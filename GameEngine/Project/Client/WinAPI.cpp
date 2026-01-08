// WinTest.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#include "pch.h"
#include "framework.h"
#include "WinAPI.h"

#include "GameManager.h"

#define MAX_LOADSTRING 100

// 전역 변수 :
HINSTANCE g_hInst = nullptr;						// 현재 인스턴스입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);		// 윈도우 설정한다.
BOOL				InitInstance(HINSTANCE, int);				// 윈도우를 초기화하여 창을 만든다.
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

// HINSTANCE : 이 프로그램의 식별번호이다.
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR IpCmdLine,		// 현재는 쓰이지 않는 인자다.
	_In_ int nCmdShow)			// 현재는 쓰이지 않는 인자다.
{
	/*
	사용하지 않는 인자
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	*/

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	if (FAILED(CGameManager::GetInst()->Init(g_hInst)))
	{
		CGameManager::Destroy();
		return false;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINAPI));

    int Ret = CGameManager::GetInst()->Progress();

    CGameManager::Destroy();

    return Ret;
    
}