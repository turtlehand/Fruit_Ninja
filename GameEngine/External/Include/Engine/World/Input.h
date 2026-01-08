#pragma once

// DInput과 WinAPI Input 모두 작업
enum class EInputSystemType
{
	DInput,
	Window
};

#define DIK_MOUSELB 0xfc
#define DIK_MOUSERB 0xfd
#define DIK_MOUSEWB 0xfe

namespace EMouseType
{
	enum
	{
		LB,		// 왼쪽
		RB,		// 오른쪽
		WB,		// 휠
		End
	};
}

namespace EInputType
{
	enum Type
	{
		Press,		// 누르는 순간
		Hold,		// 누르고 있을 때
		Release,	// 키 해제 시
		End
	};
}

struct FKeyState
{
	unsigned char	Key = 0;
	bool			Press = false;
	bool			Hold = false;
	bool			Release = false;
};

struct FBindKey
{
	// 어떤 키를 이용하는지
	FKeyState*				Key = nullptr;

	// Key와 아래 키들이 동시에 입력 시 Func 호출
	bool					Ctrl = false;
	bool					Alt = false;
	bool					Shift = false;
	std::function<void()>	Func[EInputType::End];
	bool					KeyHold = false;
};

class CInput
{
	friend class CWorld;

private:
	CInput();
public:
	~CInput();

private:
	EInputSystemType		m_InputType;
	HINSTANCE				m_hInst;
	HWND					m_hWnd;
	IDirectInput8*			m_Input;
	IDirectInputDevice8*	m_Keyboard;
	IDirectInputDevice8*	m_Mouse;

	// 키보드 상태를 저정하기 위한 변수
	unsigned char			m_KeyState[256];
	DIMOUSESTATE			m_MouseState;

private:
	std::unordered_map<unsigned char, FKeyState*> m_KeyStateMap;				// 필요한 키만 추가한다.
	std::unordered_map<std::string, std::unique_ptr<FBindKey>> m_BindKeyMap;	// 키에 함수를 추가한다.

	bool m_CtrlState[EInputType::End] = {};
	bool m_AltState[EInputType::End] = {};
	bool m_ShiftState[EInputType::End] = {};
	
	bool m_MouseButton[EMouseType::End][EInputType::End] = {};

public:
	bool Init();
	void Update(double _Delta);

private:
	bool InitDevice();
	void UpdateKeyboard();
	void UpdateMouse();
	void UpdateInput(double _DeltaTime);
	void UpdateDInput(double _DeltaTime);
	void UpdateWInput(double _DeltaTime);
	void UpdateBindKey(double _DeltaTime);

public:
	void AddBindKey(const std::string& _Name, unsigned char _Key);
	void SetKeyCtrl(const std::string& _Name, bool _Ctrl);
	void SetKeyAlt(const std::string& _Name, bool _Alt);
	void SetKeyShift(const std::string& _Name, bool _Shift);
private:
	FKeyState* FindKeyState(unsigned char _Key);
	FBindKey* FindBindKey(const std::string& _Name);

	unsigned char ConvertKey(unsigned char _Key);

public:
	/// <summary>
	/// _Name인 추가한 Key를 찾아서 함수를 추가한다.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="_Name"></param>
	/// <param name="_Type"></param>
	/// <param name="_Object"></param>
	/// <param name="_Func"></param>
	template<typename T>
	void SetBindFunction(const std::string& _Name, EInputType::Type _Type, T* _Object, void (T::* _Func)())
	{
		FBindKey* Key = FindBindKey(_Name);

		if (!Key)
			return;

		Key->Func[_Type] = std::bind(_Func, _Object);
	}

	template<typename T>
	void AddBindFunction(const std::string& _Name, unsigned char _Key, EInputType::Type _Type, T* _Object, void (T::* _Func)())
	{
		auto iter = m_BindKeyMap.find(_Name);

		// 이미 키를 등록했다.
		if (iter != m_BindKeyMap.end())
			return;

		std::unique_ptr<FBindKey> NewKey = std::make_unique<FBindKey>();

		FKeyState* State = FindKeyState(_Key);

		// 키가 없을 경우 새로 생성해서 등록한다.

		if (!State)
		{
			State = new FKeyState;
			State->Key = ConvertKey(_Key);
			m_KeyStateMap.insert(std::make_pair(_Key, State));
		}

		NewKey->Key = State;
		NewKey->Func[_Type] = std::bind(_Func, _Object);

		// std::move로 소유권을 map으로 이동시킨다.
		m_BindKeyMap.insert(std::make_pair(_Name, std::move(NewKey)));
	}

};

