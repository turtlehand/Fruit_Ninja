#include "pch.h"
#include "GraphicShader.h"
#include "Device.h"
#include "Asset/PathManager.h"


CGraphicShader::CGraphicShader()
	: m_Size(0)
{

}

CGraphicShader::~CGraphicShader()
{

}

bool CGraphicShader::Init(const std::wstring& _PathName)
{
	return true;
}

/// Vertex, Pixel Shader를 바인딩해준다.
void CGraphicShader::SetShader()
{
	CDevice::GetInst()->GetContext()->VSSetShader(m_VS.Get(), nullptr, 0);
	CDevice::GetInst()->GetContext()->PSSetShader(m_PS.Get(), nullptr, 0);

	CDevice::GetInst()->GetContext()->IASetInputLayout(m_InputLayout.Get());
}

void CGraphicShader::AddInputDesc(const char* _Semantic, UINT _SemanticIndex, DXGI_FORMAT _Fmt, UINT _InputSlot, UINT _Size, D3D11_INPUT_CLASSIFICATION _InputSlotClass, UINT _InstanceDataStepRate)
{
	D3D11_INPUT_ELEMENT_DESC	Desc = {};

	Desc.SemanticName = _Semantic;
	Desc.SemanticIndex = _SemanticIndex;
	Desc.Format = _Fmt;
	Desc.InputSlot = _InputSlot;
	Desc.InputSlotClass = _InputSlotClass;
	Desc.InstanceDataStepRate = _InstanceDataStepRate;
	Desc.AlignedByteOffset = m_Size;

	m_Size += _Size;

	m_InputDesc.push_back(Desc);
}

bool CGraphicShader::CreateInputLayout()
{
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateInputLayout(
		&m_InputDesc[0],
		(UINT)m_InputDesc.size(), m_VSBlob->GetBufferPointer(),
		m_VSBlob->GetBufferSize(), &m_InputLayout)))
		return false;

	return true;
}

bool CGraphicShader::LoadVertexShader(const std::string& _EntryName, const std::wstring& _FileName, const std::wstring& _PathName)
{
	const std::wstring& BasePath = *CPathManager::FindPath(_PathName);

	std::wstring FullPath = BasePath + _FileName;

	UINT	Flag = 0;

#ifdef _DEBUG
	Flag = D3DCOMPILE_DEBUG;
#endif // _DEBUG

	// 컴파일 에러 메세지를 전달받기 위한 버퍼
	ComPtr<ID3DBlob> ErrorBlob = nullptr;

	// 이 경로의 Shader파일을 컴파일한다.
	// 1. Shader 파일이 있는 경로.
	// 2. Macro
	// 3. Include 어떻게 할것인지를 지정.
	// 4. 컴파일 하려는 함수의 이름
	// 5. Shader Version 지정
	// 6, 7. 추가 옵션 지정.
	// 8. 컴파일된 Shader의 바이트코드
	// 9. 에러가 있을 경우 에러 메세지를 얻어오기 위한 버퍼
	if (FAILED(D3DCompileFromFile(FullPath.c_str(), nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, _EntryName.c_str(), "vs_5_0", Flag,
		0, m_VSBlob.GetAddressOf(), ErrorBlob.GetAddressOf())))
	{
#ifdef _DEBUG
		char	ErrorText[1024] = {};
		strcpy_s(ErrorText, (const char*)ErrorBlob->GetBufferPointer());
		strcat_s(ErrorText, "\n");
		// Visual Studio의 출력창에 문자열을 출력한다.
		OutputDebugStringA(ErrorText);
#endif // _DEBUG

		return false;
	}

	// VertexShader 객체를 생성한다.
	// 1. 컴파일된 Shader 바이트 코드
	// 2. 바이트 코드의 크기
	// 3. 링크
	// 4. 생성할 Pixel Shader 객체
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateVertexShader(
		m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize(),
		nullptr, m_VS.GetAddressOf())))
		return false;

	return true;
}

bool CGraphicShader::LoadPixelShader(const std::string& _EntryName, const std::wstring& _FileName, const std::wstring& _PathName)
{
	const std::wstring& BasePath = *CPathManager::FindPath(_PathName);

	std::wstring FullPath = BasePath + _FileName;

	UINT	Flag = 0;

#ifdef _DEBUG
	Flag = D3DCOMPILE_DEBUG;
#endif // _DEBUG

	// 컴파일 에러 메세지를 전달받기 위한 버퍼
	ComPtr<ID3DBlob> ErrorBlob = nullptr;

	// 이 경로의 Shader파일을 컴파일한다.
	// 1. Shader 파일이 있는 경로.
	// 2. Macro
	// 3. Include 어떻게 할것인지를 지정.
	// 4. 컴파일 하려는 함수의 이름
	// 5. Shader Version 지정
	// 6, 7. 추가 옵션 지정.
	// 8. 컴파일된 Shader의 바이트코드
	// 9. 에러가 있을 경우 에러 메세지를 얻어오기 위한 버퍼
	if (FAILED(D3DCompileFromFile(FullPath.c_str(), nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, _EntryName.c_str(), "ps_5_0", Flag,
		0, m_PSBlob.GetAddressOf(), ErrorBlob.GetAddressOf())))
	{
#ifdef _DEBUG
		char	ErrorText[1024] = {};
		strcpy_s(ErrorText, (const char*)ErrorBlob->GetBufferPointer());
		strcat_s(ErrorText, "\n");
		// Visual Studio의 출력창에 문자열을 출력한다.
		OutputDebugStringA(ErrorText);
#endif // _DEBUG

		return false;
	}

	// PixelShader 객체를 생성한다.
	// 1. 컴파일된 Shader 바이트 코드
	// 2. 바이트 코드의 크기
	// 3. 링크
	// 4. 생성할 Pixel Shader 객체
	if (FAILED(CDevice::GetInst()->GetDevice()->CreatePixelShader(
		m_PSBlob->GetBufferPointer(), m_PSBlob->GetBufferSize(),
		nullptr, m_PS.GetAddressOf())))
		return false;

	return true;
}
