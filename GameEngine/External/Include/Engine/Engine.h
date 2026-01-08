#pragma once
#include "Object.h"
#include "EngineSetting.h"

class CEngine : public CSingleton<CEngine>
{
private:
    friend class CSingleton<CEngine>;
    CEngine();
    ~CEngine();

private:
    HINSTANCE   m_hInst;
    HWND        m_hWnd;
    std::shared_ptr<CEngineSetting>	m_Setting;

    static bool   m_Loop;

public:
    HINSTANCE GetWindowInstance() const { return m_hInst; }
    HWND GetWindowHandle() const { return m_hWnd; }


public:
    bool Init(HINSTANCE _hInst, const TCHAR* _WindowName, int _IconID, int _SmallIconID, int _Width, int _Height, bool _WindowMode = true);
    int Run();

public:
    template <typename T>
    std::weak_ptr<T> CreateEngineSetting()
    {
        m_Setting.reset(new T);

        return std::dynamic_pointer_cast<T>(m_Setting);
    }

private:
    void Logic();

    //물체들의 정보를 갱신하는 함수
    void Update(double _DeltaTime);

    // 갱신된 정보를 화면에 출력하는 함수
    void Render();

private:
    // 윈도우 레지스터 클래스 등록
    void InitRegisterClass(const TCHAR* _WindowName, int _IconID, int _SmallIconID);
    // 윈도우 창 생성
    int Create(const TCHAR* _WindowName, int _Width, int _Height);

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
    void InitCDO();
public:
    template<typename T>
    static void CreateCDO()
    {
        CObject::CreateCDO<T>();
    }

};

