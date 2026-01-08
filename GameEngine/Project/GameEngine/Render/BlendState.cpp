#include "pch.h"
#include "BlendState.h"
#include "../Device.h"

CBlendState::CBlendState()
{
}

CBlendState::~CBlendState()
{
}

void CBlendState::AddRenderTargetDesc(bool _BlendEnable,
	D3D11_BLEND _SrcBlend, D3D11_BLEND _DestBlend,
	D3D11_BLEND_OP _BlendOp, D3D11_BLEND _SrcBlendAlpha,
	D3D11_BLEND _DestBlendAlpha, D3D11_BLEND_OP _BlendOpAlpha,
	UINT8 _RenderTargetWriteMask)
{
	D3D11_RENDER_TARGET_BLEND_DESC	Desc = {};

	Desc.BlendEnable = _BlendEnable;
	Desc.SrcBlend = _SrcBlend;
	Desc.DestBlend = _DestBlend;
	Desc.BlendOp = _BlendOp;
	Desc.SrcBlendAlpha = _SrcBlendAlpha;
	Desc.DestBlendAlpha = _DestBlendAlpha;
	Desc.BlendOpAlpha = _BlendOpAlpha;
	Desc.RenderTargetWriteMask = _RenderTargetWriteMask;

	m_TargetDesc.push_back(Desc);
}

void CBlendState::SetBlendFactor(float _r, float _g, float _b, float _a)
{
	m_BlendFactor[0] = _r;
	m_BlendFactor[1] = _g;
	m_BlendFactor[2] = _b;
	m_BlendFactor[3] = _a;
}

void CBlendState::SetSampleMask(UINT _SampleMask)
{
	m_SampleMask = _SampleMask;
}

bool CBlendState::CreateState(bool _AlphaToCoverageEnable, bool _IndependentBlendEnable)
{
	if (m_TargetDesc.empty())
		return false;

	D3D11_BLEND_DESC	Desc = {};

	Desc.AlphaToCoverageEnable = _AlphaToCoverageEnable;
	Desc.IndependentBlendEnable = _IndependentBlendEnable;

	size_t	Size = m_TargetDesc.size();

	memcpy(Desc.RenderTarget, &m_TargetDesc[0], sizeof(D3D11_RENDER_TARGET_BLEND_DESC) * Size);

	for (size_t i = Size; i < 8; ++i)
	{
		Desc.RenderTarget[i].BlendEnable = false;
		Desc.RenderTarget[i].SrcBlend = D3D11_BLEND_ONE;
		Desc.RenderTarget[i].DestBlend = D3D11_BLEND_ZERO;
		Desc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
		Desc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
		Desc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
		Desc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		Desc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBlendState(&Desc, (ID3D11BlendState**)m_State.GetAddressOf())))
		return false;

	return true;
}

void CBlendState::SetState()
{
	// BlendFactor : float 4개 배열로 구성되며 0 ~ 1로 구성된다.
	// 블렌드 설정에서 D3D11_BLEND_FACTOR 또는 
	// D3D11_BLEND_INV_BLEND_FACTOR를 썼을 때 사용한다.
	CDevice::GetInst()->GetContext()->OMGetBlendState(
		(ID3D11BlendState**)m_PrevState.GetAddressOf(), m_PrevBlendFactor,
		&m_PrevSampleMask);

	CDevice::GetInst()->GetContext()->OMSetBlendState(
		(ID3D11BlendState*)m_State.Get(), m_BlendFactor, m_SampleMask);
}

void CBlendState::ResetState()
{
	CDevice::GetInst()->GetContext()->OMSetBlendState(
		(ID3D11BlendState*)m_PrevState.Get(), m_PrevBlendFactor,
		m_PrevSampleMask);
}
