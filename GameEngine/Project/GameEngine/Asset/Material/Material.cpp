#include "pch.h"
#include "Material.h"
#include "../Shader/CBufferMaterial.h"
#include "../Texture/TextureManager.h"
#include "../AssetManager.h"
#include "../../Render/RenderManager.h"

#include "../../Render/RenderState.h"

CMaterial::CMaterial() :
	CObject(),
	m_BaseColor(FVector4::White),
	m_Opacity(1.f),
	m_MaterialCBuffer()
{
}

CMaterial::CMaterial(const CMaterial& _Ref)
	: CObject(_Ref)
	, m_BaseColor(_Ref.m_BaseColor)
	, m_Opacity(_Ref.m_Opacity)
	, m_MaterialCBuffer()
{
	Init();
}

CMaterial::~CMaterial()
{
}

bool CMaterial::Init()
{
	m_MaterialCBuffer.reset(new CCBufferMaterial);
	m_MaterialCBuffer->Init();

	m_MaterialCBuffer->SetBaseColor(m_BaseColor);
	m_MaterialCBuffer->SetOpacity(m_Opacity);

	return true;
}

CMaterial* CMaterial::Clone()
{
	return new CMaterial(*this);
}


void CMaterial::UpdateConstantBuffer()
{
	// 텍스쳐 바인딩
	size_t Size = m_TextureArray.size();

	for (size_t i = 0; i < Size; ++i)
	{
		auto Texture = m_TextureArray[i]->Texture.lock();

		if (Texture)
		{
			Texture->SetShader(m_TextureArray[i]->Register,
				m_TextureArray[i]->ShaderBufferType,
				0);
		}
	}

	// 상수 버퍼 바인딩
	m_MaterialCBuffer->UpdateBuffer();

	// 블렌딩 상태 등록
	if (!m_BlendState.expired())
	{
		auto State = m_BlendState.lock();
		State->SetState();
	}
}

void CMaterial::UpdateConstantBuffer(int _TextureIndex)
{
	// 텍스쳐 바인딩
	size_t Size = m_TextureArray.size();

	for (size_t i = 0; i < Size; ++i)
	{
		auto Texture = m_TextureArray[i]->Texture.lock();

		if (Texture)
		{
			Texture->SetShader(m_TextureArray[i]->Register,
				m_TextureArray[i]->ShaderBufferType,
				_TextureIndex);
		}
	}

	// 상수 버퍼 바인딩
	m_MaterialCBuffer->UpdateBuffer();

	// 블렌딩 상태 등록
	if (!m_BlendState.expired())
	{
		auto State = m_BlendState.lock();
		State->SetState();
	}
}

void CMaterial::Reset()
{
	// 블렌딩 상태 초기화
	if (!m_BlendState.expired())
	{
		auto State = m_BlendState.lock();
		State->ResetState();
	}
}

const std::weak_ptr<CTexture>& CMaterial::GetTexture(int _TextureIndex)
{
	// 잘못된 범위
	if (m_TextureArray.size() <= _TextureIndex)
	{
		return std::weak_ptr<CTexture>();
	}

	return m_TextureArray[_TextureIndex]->Texture;
}

void CMaterial::AddTexture(const std::weak_ptr<CTexture>& _Texture, int _Register, int _ShaderBufferType, int _Index)
{
	FMaterialTextureInfo* Origin = new FMaterialTextureInfo;

	std::shared_ptr<FMaterialTextureInfo> TexInfo(Origin);

	TexInfo->Texture = _Texture;
	TexInfo->Name = _Texture.lock()->GetName();
	TexInfo->Register = _Register;
	TexInfo->ShaderBufferType = _ShaderBufferType;
	TexInfo->Index = _Index;

	m_TextureArray.push_back(TexInfo);
}

void CMaterial::AddTexture(const std::string& _Name, int _Register, int _ShaderBufferType, int _Index)
{
	FMaterialTextureInfo* Origin = new FMaterialTextureInfo;

	std::shared_ptr<FMaterialTextureInfo> TexInfo(Origin);

	auto TextureManager = CAssetManager::GetInst()->GetTextureManager().lock();

	std::weak_ptr<CTexture> Tex = TextureManager->FindTexture(_Name);

	if (Tex.expired())
		return;

	TexInfo->Texture = Tex;
	TexInfo->Name = Tex.lock()->GetName();
	TexInfo->Register = _Register;
	TexInfo->ShaderBufferType = _ShaderBufferType;
	TexInfo->Index = _Index;

	m_TextureArray.push_back(TexInfo);
}

void CMaterial::AddTexture(const std::string& _Name, const wchar_t* _FileName, const std::wstring& _PathName, int _Register, int _ShaderBufferType, int _Index)
{
	FMaterialTextureInfo* Origin = new FMaterialTextureInfo;

	std::shared_ptr<FMaterialTextureInfo> TexInfo(Origin);

	auto TextureManager = CAssetManager::GetInst()->GetTextureManager().lock();

	if (!TextureManager->LoadTexture(_Name, _FileName))
		return;

	std::weak_ptr<CTexture> Tex = TextureManager->FindTexture(_Name);

	if (Tex.expired())
		return;

	TexInfo->Texture = Tex;
	TexInfo->Name = Tex.lock()->GetName();
	TexInfo->Register = _Register;
	TexInfo->ShaderBufferType = _ShaderBufferType;
	TexInfo->Index = _Index;

	m_TextureArray.push_back(TexInfo);
}

void CMaterial::AddTextureFullPath(const std::string& _Name, const wchar_t* _FullPath, int _Register, int _ShaderBufferType, int _Index)
{
	FMaterialTextureInfo* Origin = new FMaterialTextureInfo;

	std::shared_ptr<FMaterialTextureInfo> TexInfo(Origin);

	auto TextureManager = CAssetManager::GetInst()->GetTextureManager().lock();

	if (!TextureManager->LoadTextureFullPath(_Name, _FullPath))
		return;

	std::weak_ptr<CTexture> Tex = TextureManager->FindTexture(_Name);

	if (Tex.expired())
		return;

	TexInfo->Texture = Tex;
	TexInfo->Name = Tex.lock()->GetName();
	TexInfo->Register = _Register;
	TexInfo->ShaderBufferType = _ShaderBufferType;
	TexInfo->Index = _Index;

	m_TextureArray.push_back(TexInfo);
}

void CMaterial::AddTexture(const std::string& _Name, const std::vector<const wchar_t*>& _FileName, const std::string& _PathName, int _Register, int _ShaderBufferType, int _Index)
{
	FMaterialTextureInfo* Origin = new FMaterialTextureInfo;

	std::shared_ptr<FMaterialTextureInfo> TexInfo(Origin);

	auto TextureManager = CAssetManager::GetInst()->GetTextureManager().lock();

	if (!TextureManager->LoadTexture(_Name, _FileName))
		return;

	std::weak_ptr<CTexture> Tex = TextureManager->FindTexture(_Name);

	if (Tex.expired())
		return;

	TexInfo->Texture = Tex;
	TexInfo->Name = Tex.lock()->GetName();
	TexInfo->Register = _Register;
	TexInfo->ShaderBufferType = _ShaderBufferType;
	TexInfo->Index = _Index;

	m_TextureArray.push_back(TexInfo);
}

void CMaterial::AddTextureFullPath(const std::string& _Name, const std::vector<const wchar_t*>& _FullPath, int _Register, int _ShaderBufferType, int _Index)
{
	FMaterialTextureInfo* Origin = new FMaterialTextureInfo;

	std::shared_ptr<FMaterialTextureInfo> TexInfo(Origin);

	auto TextureManager = CAssetManager::GetInst()->GetTextureManager().lock();

	if (!TextureManager->LoadTextureFullPath(_Name, _FullPath))
		return;

	std::weak_ptr<CTexture> Tex = TextureManager->FindTexture(_Name);

	if (Tex.expired())
		return;

	TexInfo->Texture = Tex;
	TexInfo->Name = Tex.lock()->GetName();
	TexInfo->Register = _Register;
	TexInfo->ShaderBufferType = _ShaderBufferType;
	TexInfo->Index = _Index;

	m_TextureArray.push_back(TexInfo);
}

/// <summary>
/// _TextureIndex번째 텍스쳐를 _Texture로 설정해준다.
/// </summary>
/// <param name="_TextureIndex"></param>
/// <param name="_Texture"></param>
bool CMaterial::SetTexture(int _TextureIndex, const std::weak_ptr<CTexture>& _Texture)
{
	// 잘못된 범위
	if (m_TextureArray.size() <= _TextureIndex)
	{
		FMaterialTextureInfo* Origin = new FMaterialTextureInfo;

		std::shared_ptr<FMaterialTextureInfo>	TexInfo(Origin);

		auto	_Tex = _Texture.lock();

		TexInfo->Name = _Tex->GetName();
		TexInfo->Register = 0;
		TexInfo->ShaderBufferType = EShaderBufferType::Pixel;
		TexInfo->Index = 0;

		m_TextureArray.push_back(TexInfo);
	}

	m_TextureArray[_TextureIndex]->Texture = _Texture;

	return true;
}


void CMaterial::SetBaseColor(float _R, float _G, float _B, float _A)
{
	m_BaseColor = FVector4(_R, _G, _B, _A);

	m_MaterialCBuffer->SetBaseColor(m_BaseColor);
}

void CMaterial::SetBaseColor(unsigned char _R, unsigned char _G, unsigned char _B, unsigned char _A)
{
	m_BaseColor = FVector4(_R / 255.f, _G / 255.f, _B / 255.f, _A / 255.f);

	m_MaterialCBuffer->SetBaseColor(m_BaseColor);

}

void CMaterial::SetBaseColor(const FVector4& _Color)
{
	m_BaseColor = _Color;

	m_MaterialCBuffer->SetBaseColor(m_BaseColor);

}

void CMaterial::SetOpacity(float _Opacity)
{
	m_Opacity = _Opacity;
}

void CMaterial::SetBlendState(const std::string& _Name)
{
	m_BlendState = CRenderManager::GetInst()->FindRenderState(_Name);
}

void CMaterial::SetBlendState(const std::weak_ptr<class CRenderState>& _State)
{
	m_BlendState = _State;
}
