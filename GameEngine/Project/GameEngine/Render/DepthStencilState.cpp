#include "pch.h"
#include "DepthStencilState.h"
#include "../Device.h"

CDepthStencilState::CDepthStencilState()
{
}

CDepthStencilState::~CDepthStencilState()
{
}

bool CDepthStencilState::CreateState(bool _DepthEnable,
	D3D11_DEPTH_WRITE_MASK _DepthWriteMask,
	D3D11_COMPARISON_FUNC _DepthFunc, bool _StencilEnable,
	UINT8 _StencilReadMask, UINT8 _StencilWriteMask,
	D3D11_DEPTH_STENCILOP_DESC _FrontFace,
	D3D11_DEPTH_STENCILOP_DESC _BackFace)
{
	D3D11_DEPTH_STENCIL_DESC	Desc = {};

	Desc.DepthEnable = _DepthEnable;
	Desc.DepthWriteMask = _DepthWriteMask;
	Desc.DepthFunc = _DepthFunc;
	Desc.StencilEnable = _StencilEnable;
	Desc.StencilReadMask = _StencilReadMask;
	Desc.StencilWriteMask = _StencilWriteMask;
	Desc.FrontFace = _FrontFace;
	Desc.BackFace = _BackFace;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateDepthStencilState(&Desc, (ID3D11DepthStencilState**)m_State.GetAddressOf())))
		return false;

	return true;
}

void CDepthStencilState::SetState()
{
	// 기존에 지정되어 있던 DepthStencilState를 얻어온다.
	CDevice::GetInst()->GetContext()->OMGetDepthStencilState(
		(ID3D11DepthStencilState**)m_PrevState.GetAddressOf(), &m_PrevStencilRef);

	// State를 지정한다.
	CDevice::GetInst()->GetContext()->OMSetDepthStencilState(
		(ID3D11DepthStencilState*)m_State.Get(), m_StencilRef);
}

void CDepthStencilState::ResetState()
{
	// 원래 상태로 되돌린다.
	CDevice::GetInst()->GetContext()->OMSetDepthStencilState(
		(ID3D11DepthStencilState*)m_PrevState.Get(), m_PrevStencilRef);
}
