#include "pch.h"
#include "ShaderMaterialColor2D.h"

CShaderMaterialColor2D::CShaderMaterialColor2D()
{
}

CShaderMaterialColor2D::~CShaderMaterialColor2D()
{
}

bool CShaderMaterialColor2D::Init(const std::wstring& _PathName)
{
	if (!LoadVertexShader("Color2DVS", L"GlobalShader2D.hlsl", _PathName))
		return false;
	if (!LoadPixelShader("MaterialColor2DPS", L"GlobalShader2D.hlsl", _PathName))
		return false;

	AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputDesc("COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	// 입력 레이아웃을 생성한다.
	if (!CreateInputLayout())
		return false;
	return true;
}