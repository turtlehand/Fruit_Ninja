#pragma once

class CDevice : public CSingleton<CDevice>
{
private:
    // 리소스 생성할 때 주로 사용한다.
    ComPtr<ID3D11Device>           m_Device;

    // 렌더링 명령 등에 주로 사용된다.
    ComPtr<ID3D11DeviceContext>    m_Context;

    ComPtr<IDXGISwapChain>          m_SwapChain;

    ComPtr<ID3D11RenderTargetView> m_TargetView;
    ComPtr<ID3D11DepthStencilView> m_DepthView;

    // 해상도
    FResolution m_RS;

    bool m_WindowMode;

    HWND m_hWnd;

public:
    ComPtr<ID3D11Device> GetDevice() { return m_Device; }
    ComPtr< ID3D11DeviceContext> GetContext() { return m_Context; }
    const FResolution& GetResolution() const { return m_RS; }
    bool GetWindowMode() const { return m_WindowMode; }

public:
    int Init(HWND _hWnd, int _Width, int _Height, bool _WindowMode);
    void ClearBackBuffer();
    void ClearDepthStencil(float Depth = 1.f, unsigned char Stencil = 0);
    void SetTarget();
    void BeginRender();
    void EndRender();

private:
    friend class CSingleton<CDevice>;
    CDevice();
    ~CDevice();
};