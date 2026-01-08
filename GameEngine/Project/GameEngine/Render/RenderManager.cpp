#include "pch.h"
#include "RenderManager.h"
#include "BlendState.h"
#include "DepthStencilState.h"
#include "../Component/SceneComponent.h"

// y축이 작을수록 먼저 그려진다.
// y축이 클수록 나중에 그려진다.
bool SortYRenderList(std::weak_ptr<CSceneComponent>& _Left, std::weak_ptr<CSceneComponent>& _Right)
{
	if (_Left.lock()->GetWorldPos().y < _Right.lock()->GetWorldPos().y)
		return true;
	return false;
}

CRenderManager::CRenderManager()
{
}

CRenderManager::~CRenderManager()
{
	ResetState("DepthDisable");
}

bool CRenderManager::Init()
{
	// 기본 알파블렌딩 생성
	AddBlendRenderTargetDesc("AlphaBlend", true,
		D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP_ADD);

	CreateBlendState("AlphaBlend");

	// Depth 끄기
	CreateDepthStencilState("DepthDisable", false);

	CreateLayer("Default", 0, ERenderListSort::Y);

	SetState("DepthDisable");

	return true;
}

void CRenderManager::Render()
{
	auto iter = m_RenderLayerMap.begin();
	auto iterEnd = m_RenderLayerMap.end();

	// 레이어 전체를 렌더링한다.
	for (; iter != iterEnd; ++iter)
	{
		// 렌더링할 목록을 정렬한다.
		auto Com = iter->second.RenderList.begin();
		auto ComEnd = iter->second.RenderList.end();
	
		for (; Com != ComEnd;)
		{
			// 유효하지 않은 컴포넌트는 제거한다.
			if (Com->expired())
			{
				Com = iter->second.RenderList.erase(Com);
				continue;
			}
			++Com;
		}

		// 출력할 물체를 정렬한다.
		switch (iter->second.SortType)
		{
		case ERenderListSort::None:
			break;
		case ERenderListSort::Y:
		{
			// 2개 이상 있을 때
			if (iter->second.RenderList.size() > 1)
				// Y축을 기준으로 정렬한다.(Y가 낮을 수록 먼저 정렬)
				iter->second.RenderList.sort(SortYRenderList);
		}
		break;
		case ERenderListSort::Alpha:
			break;
		}

		// 렌더링한다.
		Com = iter->second.RenderList.begin();
		ComEnd = iter->second.RenderList.end();

		for (; Com != ComEnd; ++Com)
		{
			auto _Com = (*Com).lock();

			// 비활성화 상태라면
			if (!_Com->GetEnable())
			{
				continue;
			}
			
			Com->lock()->Render();
		}
	}
}

// _Name으로 레이어 생성
bool CRenderManager::CreateLayer(const std::string& _Name, int _RenderOrder, ERenderListSort _SortType)
{
	auto iter = m_RenderLayerMap.find(_RenderOrder);

	if (iter != m_RenderLayerMap.end())
		return false;

	FRenderLayer Layer;
	Layer.Name = _Name;
	Layer.SortType = _SortType;

	m_RenderLayerMap.insert(std::make_pair(_RenderOrder, Layer));

	return true;
}

void CRenderManager::AddRenderLayer(const std::weak_ptr<class CSceneComponent>& _Component)
{
	auto RenderComponent = _Component.lock();

	int RenderLayer = RenderComponent->GetRenderLayer();
	
	auto iter = m_RenderLayerMap.find(RenderLayer);

	if (iter == m_RenderLayerMap.end())
		return;

	iter->second.RenderList.push_back(_Component);
}

void CRenderManager::ClearRenderLayer(int _RenderLayer)
{
	auto iter = m_RenderLayerMap.begin();
	auto iterEnd = m_RenderLayerMap.end();

	for (; iter != iterEnd; ++iter)
	{
		iter->second.RenderList.clear();
	}
}

void CRenderManager::AddBlendRenderTargetDesc(
	const std::string& _Name, bool _BlendEnable,
	D3D11_BLEND _SrcBlend, D3D11_BLEND _DestBlend,
	D3D11_BLEND_OP _BlendOp, D3D11_BLEND _SrcBlendAlpha,
	D3D11_BLEND _DestBlendAlpha, D3D11_BLEND_OP _BlendOpAlpha,
	UINT8 _RenderTargetWriteMask)
{
	auto	iter = m_RenderStateMap.find(_Name);

	std::shared_ptr<CBlendState>	BlendState;

	if (iter == m_RenderStateMap.end())
	{
		std::shared_ptr<CRenderState>	State(new CBlendState);

		m_RenderStateMap.insert(std::make_pair(_Name, State));

		BlendState = std::dynamic_pointer_cast<CBlendState>(State);
	}

	else
	{
		BlendState = std::dynamic_pointer_cast<CBlendState>(
			iter->second);
	}

	BlendState->AddRenderTargetDesc(_BlendEnable, _SrcBlend, _DestBlend,
		_BlendOp, _SrcBlendAlpha, _DestBlendAlpha, _BlendOpAlpha,
		_RenderTargetWriteMask);
}

void CRenderManager::SetBlendFactor(const std::string& _Name,
	float _r, float _g, float _b, float _a)
{
	auto	iter = m_RenderStateMap.find(_Name);

	std::shared_ptr<CBlendState>	BlendState;

	if (iter == m_RenderStateMap.end())
	{
		std::shared_ptr<CRenderState>	State(new CBlendState);

		m_RenderStateMap.insert(std::make_pair(_Name, State));

		BlendState = std::dynamic_pointer_cast<CBlendState>(State);
	}

	else
	{
		BlendState = std::dynamic_pointer_cast<CBlendState>(
			iter->second);
	}

	BlendState->SetBlendFactor(_r, _g, _b, _a);
}

void CRenderManager::SetBlendSampleMask(const std::string& _Name,
	UINT _SampleMask)
{
	auto	iter = m_RenderStateMap.find(_Name);

	std::shared_ptr<CBlendState>	BlendState;

	if (iter == m_RenderStateMap.end())
	{
		std::shared_ptr<CRenderState>	State(new CBlendState);

		m_RenderStateMap.insert(std::make_pair(_Name, State));

		BlendState = std::dynamic_pointer_cast<CBlendState>(State);
	}

	else
	{
		BlendState = std::dynamic_pointer_cast<CBlendState>(
			iter->second);
	}

	BlendState->SetSampleMask(_SampleMask);
}

bool CRenderManager::CreateBlendState(const std::string& _Name, bool _AlphaToCoverageEnable, bool _IndependentBlendEnable)
{
	auto	iter = m_RenderStateMap.find(_Name);

	std::shared_ptr<CBlendState>	BlendState;

	if (iter == m_RenderStateMap.end())
	{
		std::shared_ptr<CRenderState>	State(new CBlendState);

		m_RenderStateMap.insert(std::make_pair(_Name, State));

		BlendState = std::dynamic_pointer_cast<CBlendState>(State);
	}

	else
	{
		BlendState = std::dynamic_pointer_cast<CBlendState>(iter->second);
	}

	if (!BlendState->CreateState(_AlphaToCoverageEnable, _IndependentBlendEnable))
	{
		m_RenderStateMap.erase(_Name);

		return false;
	}

	return true;
}

bool CRenderManager::CreateDepthStencilState(const std::string& _Name,
	bool _DepthEnable, D3D11_DEPTH_WRITE_MASK _DepthWriteMask,
	D3D11_COMPARISON_FUNC _DepthFunc, bool _StencilEnable,
	UINT8 _StencilReadMask, UINT8 _StencilWriteMask,
	D3D11_DEPTH_STENCILOP_DESC _FrontFace,
	D3D11_DEPTH_STENCILOP_DESC _BackFace)
{
	auto	iter = m_RenderStateMap.find(_Name);

	std::shared_ptr<CDepthStencilState>	DepthState;

	if (iter == m_RenderStateMap.end())
	{
		std::shared_ptr<CRenderState>	State(new CDepthStencilState);

		m_RenderStateMap.insert(std::make_pair(_Name, State));

		DepthState = std::dynamic_pointer_cast<CDepthStencilState>(
			State);
	}

	else
	{
		DepthState = std::dynamic_pointer_cast<CDepthStencilState>(
			iter->second);
	}

	if (!DepthState->CreateState(_DepthEnable, _DepthWriteMask,
		_DepthFunc, _StencilEnable, _StencilReadMask, _StencilWriteMask,
		_FrontFace, _BackFace))
	{
		m_RenderStateMap.erase(_Name);
		return false;
	}

	return true;
}

void CRenderManager::SetState(const std::string& _Name)
{
	auto	iter = m_RenderStateMap.find(_Name);

	if (iter == m_RenderStateMap.end())
		return;

	iter->second->SetState();
}

void CRenderManager::ResetState(const std::string& _Name)
{
	auto	iter = m_RenderStateMap.find(_Name);

	if (iter == m_RenderStateMap.end())
		return;

	iter->second->ResetState();
}

std::weak_ptr<CRenderState> CRenderManager::FindRenderState( const std::string& _Name)
{
	auto	iter = m_RenderStateMap.find(_Name);

	if (iter == m_RenderStateMap.end())
		return std::weak_ptr<CRenderState>();

	return iter->second;
}