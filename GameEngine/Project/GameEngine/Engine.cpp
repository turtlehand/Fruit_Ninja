#include "pch.h"
#include "Engine.h"
#include "Timer.h"
#include "Device.h"
#include "Asset/PathManager.h"
#include "World/WorldManager.h"
#include "Asset/AssetManager.h"
#include "Asset/Mesh/Mesh.h"
#include "Asset/Mesh/MeshManager.h"
#include "Render/RenderManager.h"
#include "CollisionInfoManager.h"

#include "Asset/Shader/GraphicShader.h"
#include "Asset/Shader/ShaderManager.h"
#include "Asset/Shader/CBufferTransform.h"

#include "Object.h"
#include "Component/CameraComponent.h"
#include "Component/MeshComponent.h"
#include "Component/DynamicMeshComponent.h"
#include "Object/GameObject.h"

bool CEngine::m_Loop = true;

CEngine::CEngine()
    : m_hWnd(nullptr)
    , m_hInst(nullptr)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //atexit(DestroyInst);

    m_Setting.reset(new CEngineSetting);
}

CEngine::~CEngine()
{
    // GetDC로 얻어온 DC는 반드시 ReleaseDC를 해주어야 한다.
    // ReleaseDC(mhWnd, mhDC);
    // CDevice::DestroyInst();
    // CWorldManager::DestroyInst();
    // CAssetManager::DestroyInst();

    CObject::DestroyCDO();
}

bool CEngine::Init(HINSTANCE _hInst, const TCHAR* _WindowName, int _IconID, int _SmallIconID, int _Width, int _Height, bool _WindowMode)
{
    m_hInst = _hInst;

    // 윈도우 레지스터 클래스를 등록한다.
    InitRegisterClass(_WindowName, _IconID, _SmallIconID);

    // 윈도우 창을 생성한다.
    if (FAILED(Create(_WindowName, _Width, _Height)))
        return false;

    // 디바이스 초기화
    if (FAILED(CDevice::GetInst()->Init(m_hWnd, _Width, _Height, _WindowMode)))
    {
        return false;
    }

    if (!CRenderManager::GetInst()->Init())
        return false;

    // 충돌 정보 관리자 초기화
    if (!CCollisionInfoManager::GetInst()->Init())
        return false;


    if (!CPathManager::Init())
    {
        return false;
    }


    if (FAILED(CAssetManager::GetInst()->Init()))
    {
        return false;
    }

    CMeshComponent::CreateEmptyAnimCBuffer();
    CDynamicMeshComponent::CreateEmptyAnimCBuffer();

    if (!m_Setting->Init())
        return false;


    if (FAILED(CWorldManager::GetInst()->Init()))
    {
        return false;
    }


    // 생성한 윈도우 창에 출력할 수 있는 DC를 생성한다.
    //mhDC = GetDC(mhWnd);

    // 타이머 초기화
    //CTimer::Init();

    return true;
}

void CEngine::InitRegisterClass(const TCHAR* _WindowName, int _IconID, int _SmallIconID)
{
    // 운영체제에 등록할 창 정보를 지정하여 레지스터에 등록한다.
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    // 창의 가로, 세로 크기가 바뀔때 윈도우 전체를 다시 그리게 하는 옵션.
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    // 메세지 발생시 호출할 함수의 포인터를 등록한다.
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;

    // 이 프로그램의 인스턴스(식별번호)를 넣어준다.
    wcex.hInstance = m_hInst;
    // 실행파일 아이콘
    wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(_IconID));
    // 마우스 커서 모양.
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    // 배경 색상.
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    // 등록할 메뉴를 지정한다.
    wcex.lpszMenuName = nullptr;// MAKEINTRESOURCEW(IDC_WINTEST);
    // 레지스터에 등록할 윈도우 클래스 이름을 지정한다.
    // TEXT : 유니코드 문자열을 만들어줄 때 사용한다.
    // L"테스트";
    wcex.lpszClassName = _WindowName;
    // 창 좌측 상단의 아이콘
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(_SmallIconID));

    // 생성한 구조체 정보를 이용해서 레지스터에 창 클래스를 등록한다.
    RegisterClassExW(&wcex);
}

int CEngine::Create(const TCHAR* _WindowName, int _Width, int _Height)
{
    // 창을 만들어준다. 만들어준 창을 컨트롤 할 수 있는 핸들도 함께 반환된다.
    // 1번 : 윈도우 클래스 이름
    // 2번 : 윈도우 타이틀바에 출력할 이름
    // 3번 : 윈도우 창 스타일을 지정한다.
    // 4번 : 윈도우 창 시작 X좌표를 지정한다.
    // 5번 : 윈도우 창 시작 Y좌표를 지정한다.
    // 6번 : 윈도우 창 가로 크기
    // 7번 : 윈도우 창 세로 크기
    // 8번 : 부모 윈도우 핸들
    // 9번 : 메뉴 핸들
    // 10번 : 식별번호
    m_hWnd = CreateWindowW(_WindowName, _WindowName, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, _Width, _Height, nullptr, nullptr, m_hInst, nullptr);

    if (!m_hWnd)
    {
        return E_FAIL;
    }

    // 클라이언트 영역의 해상도를 RECT로 만든다.
    RECT WindowRC = { 0,0,_Width, _Height };
    // 클라이언트 영역이 해상도 크기로 생성되기 위해 ThickFrame이나 기타 요소들을 포함한 실제 크기를 얻어온다.
    AdjustWindowRect(&WindowRC, WS_OVERLAPPEDWINDOW, FALSE);

    // 윈도우 크기를 변경한다.
    SetWindowPos(m_hWnd, HWND_TOPMOST, 0,0, WindowRC.right- WindowRC.left, WindowRC.bottom- WindowRC.top, SWP_NOMOVE | SWP_NOZORDER);

    // 창을 보여줄지를 결정한다.
    ShowWindow(m_hWnd, SW_SHOW);

    // 이 윈도우를 즉시 다시 그리게 만들어준다.
    // WM_PAINT 메세지를 발생시켜준다.
    UpdateWindow(m_hWnd);

    return S_OK;
}

LRESULT CEngine::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
        EndPaint(hWnd, &ps);
    }
    break;
    // 창이 파괴될 때 들어온다.
    case WM_DESTROY:
        m_Loop = false;
        PostQuitMessage(0);
        break;
    default:
        // 위에서 지정한 메세지 외 다른 메세지가 발생할 경우 윈도우의 기본 동작으로
        // 처리하게 만들어준다.
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
 }

void CEngine::InitCDO()
{
    CObject::CreateCDO<CGameObject>();
    CObject::CreateCDO<CSceneComponent>();
    CObject::CreateCDO<CMeshComponent>();
    CObject::CreateCDO<CCameraComponent>();
}

int CEngine::Run()
{
    MSG msg = {};

    // 기본 메시지 루프입니다:
    // GetMessage 함수를 이용해 메세지 큐에 있는 메세지를 얻어온다.
    // 단, 메세지가 없을 경우 이 함수 내부에서 대기하게 된다.
    while (m_Loop)
    {
        // GetMessage(&msg, nullptr, 0, 0)
        // PeekMessage도 윈도우 메세지 큐에서 메세지를 꺼내온다.
        // 단, 메세지가 없으면 바로 빠져나온다.
        // PM_REMOVE 는 메세지 큐에서 메세지를 제거하고 가져오라는 의미이다.
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            // 키보드 입력은 WM_KEYDOWN이 발생하는데, 문자 입력을 했다면 아래 함수에서
            // WM_CHAR 메세지를 만들어준다. 키보드 관련이 아니면 건너뛴다.
            TranslateMessage(&msg);

            // 이 메세지를 등록된 윈도우 프로시저 함수에 전달해준다.
            // WndProc 함수를 호출하는 것이다.
            DispatchMessage(&msg);
        }

        // 메세지가 없을 경우
        else
        {
            Logic();
        }
    }

    return (int)msg.wParam;
}

void CEngine::Logic()
{
    double DeltaTime = CTimer::Update();

    Update(DeltaTime);

    Render();

}

void CEngine::Update(double _DeltaTime)
{
    char	Test[256] = {};
    sprintf_s(Test, "FPS : %d\t DT : %f\n", CTimer::GetFPS(), CTimer::GetDeltaTime());
    OutputDebugStringA(Test);
    CWorldManager::GetInst()->Update(_DeltaTime);

    CWorldManager::GetInst()->PostUpdate(_DeltaTime);

}

void CEngine::Render()
{
    CDevice::GetInst()->BeginRender();

    /*
    // 샘플 출력
    std::weak_ptr<CMeshManager> Weak_MeshMgr = CAssetManager::GetInst()->GetMeshManager();
    std::weak_ptr<CShaderManager> Weak_ShaderMgr = CAssetManager::GetInst()->GetShaderManager();
    
    std::shared_ptr<CMeshManager> MeshMgr = Weak_MeshMgr.lock();
    std::shared_ptr<CShaderManager> ShaderMgr = Weak_ShaderMgr.lock();

    if (MeshMgr && ShaderMgr)
    {
        std::weak_ptr<CShader>  Weak_Shader = ShaderMgr->FindShader("Color2D");
        std::weak_ptr<CMesh>  Weak_Mesh = MeshMgr->FindMesh("CenterCubeColor");

        std::shared_ptr<CShader>    Shader = Weak_Shader.lock();
        std::shared_ptr<CMesh>    Mesh = Weak_Mesh.lock();

        Shader->SetShader();

        Mesh->Render();
        
        // 월드행렬을 만든다.
        static FVector3 Rot(30.f, 0.f, 0.f);
        Rot.x += 60.f * CTimer::GetDeltaTime();
        Rot.y += 60.f * CTimer::GetDeltaTime();

        FVector3    Scale(1.f, 1.f, 1.f),  Pos;
        FMatrix ScaleMat = FMatrix::StaticScaling(Scale);
        FMatrix RotMat = FMatrix::StaticRotation(Rot);
        FMatrix TranslateMat = FMatrix::StaticTranslation(Pos);
        FMatrix WorldMat = ScaleMat * RotMat * TranslateMat;

        // View 행렬을 만든다.
        FVector3    CameraPos(0.f, 0.f, -2.f);      // 카메라의 위치
        FVector3    LookAt(0.f, 0.f, 0.f);          // 카메라가 바라보는 방향
        FVector3    CameraUp(0.f, 1.f, 0.f);        // 카메라의 위 방향

        FMatrix ViewMat = DirectX::XMMatrixLookAtLH(CameraPos.Convert(), LookAt.Convert(), CameraUp.Convert());

        // 투영 행렬을 만든다.
        float Angle = DirectX::XMConvertToRadians(60.f);

        FMatrix ProjMat = DirectX::XMMatrixPerspectiveFovLH(Angle,
            1280.f / 720.f, 0.1f, 1000.f);

        // 위치 버퍼
        std::shared_ptr<CCBufferTransform> CBuffer(new CCBufferTransform);

        CBuffer->Init();

        CBuffer->SetWorldMatrix(WorldMat);
        CBuffer->SetViewMatrix(ViewMat);
        CBuffer->SetProjMatrix(ProjMat);

        CBuffer->UpdateBuffer();
        
    }
    */

    // Object Render
    // CWorldManager::GetInst()->Render();
    CRenderManager::GetInst()->Render();

    CDevice::GetInst()->EndRender();
}


