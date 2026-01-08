#pragma once

#include "RenderState.h"

class CBlendState :
	public CRenderState
{
	friend class CRenderManager;

protected:
	CBlendState();

public:
	virtual ~CBlendState();

private:
	std::vector<D3D11_RENDER_TARGET_BLEND_DESC>	m_TargetDesc;
	float	m_BlendFactor[4] = { 1.f, 1.f, 1.f, 1.f };
	float	m_PrevBlendFactor[4] = {};
	UINT	m_SampleMask = 0xffffffff;
	UINT	m_PrevSampleMask = 0xffffffff;

public:
	// SrcRGB * SrcBlend BlendOp DestRGB * DestBlend = 최종 RGB
	// SrcA * SrcBlendAlpha BlendOpAlpha DestA * DestBlendAlpha = 
	// 최종 Alpha
	// src : 출력할 물체의 색
	// dest : 렌더타겟에 출력되어 있는 색
	// src : 0, 1, 0, 0.5
	// dest : 1, 0, 0, 1
	// 0, 1, 0 * 0.5 + 1, 0, 0 * (1 - 0.5) = 0.5, 0.5, 0
	void AddRenderTargetDesc(bool _BlendEnable = true,
		D3D11_BLEND _SrcBlend = D3D11_BLEND_ONE,
		D3D11_BLEND _DestBlend = D3D11_BLEND_ZERO,
		D3D11_BLEND_OP _BlendOp = D3D11_BLEND_OP_ADD,
		D3D11_BLEND _SrcBlendAlpha = D3D11_BLEND_ONE,
		D3D11_BLEND _DestBlendAlpha = D3D11_BLEND_ZERO,
		D3D11_BLEND_OP _BlendOpAlpha = D3D11_BLEND_OP_ADD,
		UINT8 _RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL);
	void SetBlendFactor(float _r, float _g, float _b, float _a);
	void SetSampleMask(UINT _SampleMask);

	// AlphaToCoverageEnable : MSAA 사용 시 알파값을 샘플 마스크로
	// 바꿔서 투명 경계를 부드럽게 해준다. 나뭇잎, 풀 같은 곳에서
	// 많이 활용된다.
	// IndependentBlendEnable : false로 하면 RenderTarget[0]의 값이
	// 1 ~ 7번까지 모두 복제되고 true를 하면 각자 따로 지정이 가능하다.
	bool CreateState(bool _AlphaToCoverageEnable = false, bool _IndependentBlendEnable = false);

	/*
	BOOL AlphaToCoverageEnable;
	BOOL IndependentBlendEnable;
	D3D11_RENDER_TARGET_BLEND_DESC RenderTarget[ 8 ];
	*/

public:
	virtual void SetState();
	virtual void ResetState();
};

