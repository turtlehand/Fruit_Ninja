#include "pch.h"
#include "TextureManager.h"
#include "Device.h"

CTextureManager::CTextureManager()
{

}

CTextureManager::~CTextureManager()
{

}

bool CTextureManager::Init()
{
	CreateSampler("Point", D3D11_FILTER_MIN_MAG_MIP_POINT);
	CreateSampler("Linear", D3D11_FILTER_MIN_MAG_MIP_LINEAR);
	
	SetSampler("Point", 0, EShaderBufferType::All);
	SetSampler("Linear", 1, EShaderBufferType::All);

	return true;
}

bool CTextureManager::LoadTexture(const std::string& _Name, const wchar_t* _FileName, const std::wstring& _PathName)
{
	std::weak_ptr<CTexture> Check = FindTexture(_Name);

	// 있을 경우
	if (!Check.expired())
		return true;

	CTexture* Origin = new CTexture;

	std::shared_ptr<CTexture> Texture;

	Texture.reset(Origin);

	Texture->m_Name = _Name;

	if (!Texture->LoadTexture(_FileName, _PathName))
	{
		return false;
	}

	m_TextureMap.insert(std::make_pair(_Name, Texture));

	return true;
}

bool CTextureManager::LoadTextureFullPath(const std::string& _Name, const wchar_t* _FullPath)
{
	std::weak_ptr<CTexture> Check = FindTexture(_Name);

	// 있을 경우
	if (!Check.expired())
		return true;

	CTexture* Origin = new CTexture;

	std::shared_ptr<CTexture> Texture;

	Texture.reset(Origin);

	Texture->m_Name = _Name;

	if (!Texture->LoadTexture(_FullPath))
	{
		return false;
	}

	m_TextureMap.insert(std::make_pair(_Name, Texture));

	return true;
}

bool CTextureManager::LoadTexture(const std::string& _Name, const std::vector<const wchar_t*> _FileName, const std::wstring& _PathName)
{
	std::weak_ptr<CTexture>	Check = FindTexture(_Name);

	// 있을 경우
	if (!Check.expired())
		return true;

	CTexture* Origin = new CTexture;

	std::shared_ptr<CTexture> Texture;

	Texture.reset(Origin);

	Texture->m_Name = _Name;

	if (!Texture->LoadTexture(_FileName, _PathName))
	{
		return false;
	}

	m_TextureMap.insert(std::make_pair(_Name, Texture));

	return true;
}

bool CTextureManager::LoadTextureFullPath(const std::string& _Name, const std::vector<const wchar_t*> _FullPath)
{
	std::weak_ptr<CTexture>	Check = FindTexture(_Name);

	// 있을 경우
	if (!Check.expired())
		return true;

	CTexture* Origin = new CTexture;

	std::shared_ptr<CTexture> Texture;

	Texture.reset(Origin);

	Texture->m_Name = _Name;

	if (!Texture->LoadTexture(_FullPath))
	{
		return false;
	}

	m_TextureMap.insert(std::make_pair(_Name, Texture));

	return true;
}

std::weak_ptr<CTexture> CTextureManager::FindTexture(const std::string& _Name)
{
	std::unordered_map<std::string, std::shared_ptr<class CTexture>>::iterator iter = m_TextureMap.find(_Name);

	if(iter == m_TextureMap.end())
		return std::weak_ptr<CTexture>();

	return iter->second;
}

bool CTextureManager::CreateSampler(const std::string& _Name, D3D11_FILTER _Filter, D3D11_TEXTURE_ADDRESS_MODE _u, D3D11_TEXTURE_ADDRESS_MODE _v, D3D11_TEXTURE_ADDRESS_MODE _w, UINT _MaxAnisotropy, D3D11_COMPARISON_FUNC _Func)
{
	// 이미 있는 샘플러다.
	if (FindSampler(_Name))
		return false;

	D3D11_SAMPLER_DESC Desc = {};


	//D3D11_FILTER_MIN_MAG_MIP_POINT
	//	→ 포인트 샘플링(최근접 픽셀, 픽셀 아트에 적합)
	//	D3D11_FILTER_MIN_MAG_MIP_LINEAR
	//	→ 선형 보간(부드러운 텍스처)
	//	D3D11_FILTER_ANISOTROPIC
	//	→ 비등방성 필터링(비스듬한 면에서 선명)
	Desc.Filter = _Filter;

	//텍스처 좌표가 0~1 범위를 벗어났을 때 처리 방식입니다.
	Desc.AddressU = _u;
	Desc.AddressV = _v;
	Desc.AddressW = _w;

	// MipMap 선택 시 LOD(Level of Detail)에 가중치를 줍니다.
	// 특수한 효과가 아니라면 보통 0을 사용합니다.
	Desc.MipLODBias = 0.f;

	//비등방성 필터링 강도입니다.
	//유효 범위 : 1 ~16
	//	Filter가 D3D11_FILTER_ANISOTROPIC일 때만 의미 있음
	//값이 클수록 :
	//  비스듬한 텍스처가 선명해짐
	//	성능 비용 증가
	Desc.MaxAnisotropy = _MaxAnisotropy;

	// 비교 샘플링용 함수입니다.
	// 주로 Shadow Map에서 사용됩니다.
	Desc.ComparisonFunc = _Func;

	// AddressU / V / W 가 D3D11_TEXTURE_ADDRESS_BORDER일 때
	//범위 밖에서 사용할 색상(RGBA) 입니다.
	Desc.BorderColor[0] = 0.f;
	Desc.BorderColor[1] = 0.f;
	Desc.BorderColor[2] = 0.f;
	Desc.BorderColor[3] = 1.f;

	// 사용 가능한 최소 Mip Level입니다.
	Desc.MinLOD = 0.f;

	// 사용 가능한 최대 Mip Level입니다.
	Desc.MaxLOD = D3D11_FLOAT32_MAX;

	ComPtr<ID3D11SamplerState> Sampler = nullptr;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateSamplerState(
		&Desc, Sampler.GetAddressOf())))
		return false;

	m_SamplerMap.insert(std::make_pair(_Name, Sampler));

	return true;
}

ComPtr<ID3D11SamplerState> CTextureManager::FindSampler(const std::string& _Name)
{
	std::unordered_map<std::string, ComPtr<ID3D11SamplerState>>::iterator iter = m_SamplerMap.find(_Name);
	if (iter == m_SamplerMap.end())
		return ComPtr<ID3D11SamplerState>();

	return iter->second;
}

void CTextureManager::SetSampler(const std::string& _Name, int _Register, int _ShaderBufferType)
{
	ComPtr<ID3D11SamplerState> Sampler = FindSampler(_Name);

	if (!Sampler)
		return;

	if (_ShaderBufferType & EShaderBufferType::Vertex)
	{
		CDevice::GetInst()->GetContext()->VSSetSamplers(_Register, 1,
			Sampler.GetAddressOf());
	}

	if (_ShaderBufferType & EShaderBufferType::Pixel)
	{
		CDevice::GetInst()->GetContext()->PSSetSamplers(_Register, 1,
			Sampler.GetAddressOf());
	}

	if (_ShaderBufferType & EShaderBufferType::Domain)
	{
		CDevice::GetInst()->GetContext()->DSSetSamplers(_Register, 1,
			Sampler.GetAddressOf());
	}

	if (_ShaderBufferType & EShaderBufferType::Hull)
	{
		CDevice::GetInst()->GetContext()->HSSetSamplers(_Register, 1,
			Sampler.GetAddressOf());
	}

	if (_ShaderBufferType & EShaderBufferType::Geometry)
	{
		CDevice::GetInst()->GetContext()->GSSetSamplers(_Register, 1,
			Sampler.GetAddressOf());
	}

	if (_ShaderBufferType & EShaderBufferType::Compute)
	{
		CDevice::GetInst()->GetContext()->CSSetSamplers(_Register, 1,
			Sampler.GetAddressOf());
	}
}

void CTextureManager::ReleaseAsset(const std::string& _Name)
{
	auto iter = m_TextureMap.find(_Name);

	if (iter != m_TextureMap.end())
	{
		// 다른 월드에서 더 이상 들고 있지 않을 경우
		if (iter->second.use_count() == 1)
		{
			m_TextureMap.erase(iter);
		}
	}
}