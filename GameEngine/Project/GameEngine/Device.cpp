#include "pch.h"
#include "Device.h"

CDevice::CDevice() :
	m_Device(nullptr)
{

}

CDevice::~CDevice()
{
	/*
	if (m_TargetView)
	{
		m_TargetView->Release();
	}
	if (m_DepthView)
	{
		m_DepthView->Release();
	}
	if (m_SwapChain)
	{
		m_SwapChain->Release();
	}
	if (m_Context)
	{
		m_Context->ClearState();
		m_Context->Release();
	}
	if (m_Device)
	{
		m_Device->Release();
	}
	*/
}

int CDevice::Init(HWND _hWnd, int _Width, int _Height, bool _WindowMode)
{
	m_hWnd = _hWnd;
	m_RS.Width = _Width;
	m_RS.Height = _Height;
	m_WindowMode = _WindowMode;

	// Device, Context를 생성한다.
#pragma region Device

	UINT Flag = 0;
#ifdef _DEBUG
	Flag = D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL FLevel = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL FLevelResult;

	// FAILED 매크로는 HRESULT로 반환된 결과에 대해 실해인지 체크하는 매크로이다.
	if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, Flag,
		&FLevel, 1, D3D11_SDK_VERSION, m_Device.GetAddressOf(), &FLevelResult, m_Context.GetAddressOf())))
	{
		return E_FAIL;
	}

	UINT SampleCount = 4;

	UINT Check = 0;
	m_Device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &Check);

	if (Check < 1)
		SampleCount = 1;

	SampleCount = 1;

#pragma endregion

	// SwapChain을 생성한다.
#pragma region SwapChain
	
	DXGI_SWAP_CHAIN_DESC SwapDesc = {};

	// 백버퍼의 가로, 세로 해상도를 지정한다.
	SwapDesc.BufferDesc.Width = _Width;
	SwapDesc.BufferDesc.Height = _Height;

	// 백버퍼의 픽셀 포멧을 지정한다.
	SwapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// 화면 주사율 지정
	SwapDesc.BufferDesc.RefreshRate.Numerator = 1;
	SwapDesc.BufferDesc.RefreshRate.Denominator = 60;

	// 지정한 해상도와 모니터 해상도가 맞지 않을 경우 어떻게 스케일링 할지를 결정한다.
	// DXGI_MODE_SCALING_UNSPECIFIED = 0	드라이버/OS에 맡김
	// DXGI_MODE_SCALING_CENTERED	 = 1	작은 해상도를 쓸 떄, 가운데 배치 + 주변은 검은 여백
	// DXGI_MODE_SCALING_STRETCHED	 = 2	해상도를 화면에 맞게 늘리거나 줄여서 사용
	SwapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// 화면에 그릴 떄 줄 선서를 어떻게 처리할지에 대한 옵션이다.
	//	DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED		= 0		신경 안씀
	//	DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE		= 1		순차 스캔
	//	DXGI_MODE_SCANLINE_ORDER_UPPER_FIELD_FIRST	= 2	
	//	DXGI_MODE_SCANLINE_ORDER_LOWER_FIELD_FIRST	= 3
	SwapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	// 백버퍼 수 지정
	SwapDesc.BufferCount = 2;

	// 백 버퍼를 출력용으로 지정한다.
	SwapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// 출력한 윈도우 핸들을 지정한다.
	SwapDesc.OutputWindow = m_hWnd;

	// 멀티 샘플링 퀄리티를 지정한다.
	SwapDesc.SampleDesc.Quality = 0;
	SwapDesc.SampleDesc.Count = 1;

	// 창모드인지 풀스크린인지 지정한다.
	SwapDesc.Windowed = _WindowMode;

	// 스왑체인에서 백버퍼 -> 화면으로 넘길 떄 백버퍼를 어떻게 처리할지를 지정하는 옵션이다.
	// DXGI_SWAP_EFFECT_DISCARD			= 0		프레임을 화면에 보낸 뒤, 버퍼 내용은 보장 안 한다.
	// DXGI_SWAP_EFFECT_SEQUENTIAL		= 1		버퍼가 순서대로 화면에 처리된다.
	// DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL = 3		순차 출력을 하며 플립 처리를 한다.
	// DXGI_SWAP_EFFECT_FLIP_DISCARD	= 4		플립 하며 백버퍼의 내용을 버린다. 사용 시 버퍼가 최소 2개 이상이어야 가능하다.

	SwapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	// IDXGIDevice* DXGIDevice = nullptr;
	ComPtr<IDXGIDevice> DXGIDevice = nullptr;

	// __uidof : 컴 타입의 ID를 얻어온다.
	m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)DXGIDevice.GetAddressOf());

	ComPtr<IDXGIAdapter> Adapter = nullptr;
	DXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)Adapter.GetAddressOf());

	ComPtr<IDXGIFactory> Factory = nullptr;
	Adapter->GetParent(__uuidof(IDXGIFactory), (void**)Factory.GetAddressOf());

	if (FAILED(Factory->CreateSwapChain(m_Device.Get(), &SwapDesc, m_SwapChain.GetAddressOf())))
	{
		return E_FAIL;
	}
#pragma endregion

	// SwapChain에 지정되어 있는 백버퍼를 이용해서 화면에 출력하기 위한 타겟을 생성한다.
#pragma region RenderTarget
	// SwapChain으로부터 백버퍼를 얻어온다.
	ComPtr<ID3D11Texture2D> BackBuffer = nullptr;

	// GetBuffer로 얻어오면 페퍼런스 카운트가 1 증가하기 때문에 다 썼다면
	// Release를 호출하여 카운트를 감소시켜주어야 한다.
	if (FAILED(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)BackBuffer.GetAddressOf())))
	{
		return E_FAIL;
	}

	// 백버퍼에 출력하기 윈한 렌더타겟을 얻어온다.
	// 렌더 타켓은 픽셀을 저장하는 픽셀을 출력하기 위한 용도이가.
	if (FAILED(m_Device->CreateRenderTargetView(BackBuffer.Get(), nullptr, m_TargetView.GetAddressOf())))
		return E_FAIL;

#pragma endregion

	// 깊이 버퍼를 생성한다.
#pragma region DepthStencil
	D3D11_TEXTURE2D_DESC DepthDesc = {};

	DepthDesc.Width = _Width;
	DepthDesc.Height = _Height;
	DepthDesc.ArraySize = 1;

	// D24 : 깊이를 저장하는 용으로 24비트(3바이트)를 사용한다.
	// S8 : 스텐실 값을 저장하는 용으로 8비트(1바이트)를 사용한다.
	// UNORM : 0 ~ 1사이의 실수값
	// UINT : 0 ~ 255 까지의 정수값
	DepthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 깊이, 스텐실 용으로 사용한다.
	DepthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DepthDesc.Usage = D3D11_USAGE_DEFAULT;

	DepthDesc.SampleDesc.Count = 1;
	DepthDesc.SampleDesc.Quality = 0;
	DepthDesc.MipLevels = 1;

	ComPtr<ID3D11Texture2D> DepthBuffer = nullptr;

	// 위에서 정의한 Texture 정보를 Texture를 생성한다.
	if(FAILED(m_Device->CreateTexture2D(&DepthDesc, nullptr, DepthBuffer.GetAddressOf())))
		return E_FAIL;

	// 깊이 뷰를 만들어준다.
	if (FAILED(m_Device->CreateDepthStencilView(DepthBuffer.Get(), nullptr, m_DepthView.GetAddressOf())))
		return E_FAIL;
#pragma endregion

	// 뷰 포트
#pragma region ViewPort
	D3D11_VIEWPORT VP = {};
	
	VP.TopLeftX = 0;
	VP.TopLeftY = 0;
	VP.Width = (float)_Width;
	VP.Height = (float)_Height;
	VP.MinDepth = 0.f;
	VP.MaxDepth = 1.f;

	m_Context->RSSetViewports(1, &VP);
#pragma endregion

	return S_OK;
}

void CDevice::ClearBackBuffer()
{
	// 지정된 색으로 백버퍼를 초기화한다.
	float Color[4] = {0.5f,0.5f ,0.5f ,1.f };
	m_Context->ClearRenderTargetView(m_TargetView.Get(), Color);
}

void CDevice::ClearDepthStencil(float Depth, unsigned char Stencil)
{
	// 깊이, 스텐실을 초기화한다.
	m_Context->ClearDepthStencilView(m_DepthView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, Depth, Stencil);
}

void CDevice::SetTarget()
{
	// 출력하기 위한 타겟을 지정한다
	m_Context->OMSetRenderTargets(1, m_TargetView.GetAddressOf(), m_DepthView.Get());
}

// Window를 초기화해준다.
void CDevice::BeginRender()
{
	ClearBackBuffer();		// 색상 초기화
	ClearDepthStencil();	// DepthStencil 초기화
	SetTarget();			// 그릴 타겟 세팅
}

// 백버퍼를 화면에 보여준다.
void CDevice::EndRender()
{
	m_SwapChain->Present(0, 0);
}
