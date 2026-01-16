#pragma once



// ComPtr
#include<wrl/client.h>
using namespace Microsoft::WRL;

// 기본 STL
#include <Windows.h>
#include <assert.h>
#include <format>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <map>
#include <unordered_map>
#include <string>
#include <functional>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <memory>
#include <queue>

// 메모리 릭 체크용
#include <crtdbg.h>

// DirectX
#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

// DirectXTex
#ifdef _DEBUG
#pragma comment(lib, "DirectXTex/DirectXTex_Debug.lib")
#else
#pragma comment(lib, "DirectXTex/DirectXTex.lib")
#endif

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#pragma comment(lib, "dinput8.lib")

// Math
#include "Matrix.h"

// 기본
#include "singleton.h"
#include "define.h"
#include "struct.h"
#include "func.h"
#include "enum.h"

// 기타
#include "Asset/Shader/CBufferContainer.h"


