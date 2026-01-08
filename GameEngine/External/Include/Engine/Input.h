#pragma once

// DInput과 WinAPI Input 모두 작업

enum class EInputType
{
	DInput,
	Window
};

class CInput
{
	friend class CWorld;

private:
	CInput();
public:
	~CInput();

private:
	EInputType				m_InputType;
	HINSTANCE				m_hInst;
	HWND					m_hWnd;
	IDirectInput8*			m_Input;
	IDirectInputDevice8*	m_Keyboard;
	IDirectInputDevice8*	m_Mouse;

	// 키보드 상태를 저정하기 위한 변수
	unsigned char			m_KeyState[256];
	DIMOUSESTATE			m_MouseState;

public:
	bool Init();
	void Update(double _Delta);

private:
	bool InitDevice();
	void UpdateKeyboard();
	void UpdateMouse();

};

