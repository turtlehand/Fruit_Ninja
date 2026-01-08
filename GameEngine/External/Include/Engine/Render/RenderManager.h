#pragma once

struct FRenderLayer
{
	std::string Name;
	std::list<std::weak_ptr<class CSceneComponent>> RenderList;
	ERenderListSort SortType = ERenderListSort::Y;
};

class CRenderState;

class CRenderManager : public CSingleton<CRenderManager>
{
	friend class CSingleton<CRenderManager>;
private:
	CRenderManager();
	~CRenderManager();

private:
	std::unordered_map<std::string, std::shared_ptr<CRenderState>>	  m_RenderStateMap;
	std::map<int, FRenderLayer> m_RenderLayerMap;

public:
	bool CreateLayer(const std::string& _Name, int _RenderOrder, ERenderListSort _SortType);
	void AddRenderLayer(const std::weak_ptr<class CSceneComponent>& _Component);
	void ClearRenderLayer(int _RenderLayer);

public:
	void AddBlendRenderTargetDesc(const std::string& _Name,
		bool _BlendEnable = true,
		D3D11_BLEND _SrcBlend = D3D11_BLEND_ONE,
		D3D11_BLEND _DestBlend = D3D11_BLEND_ZERO,
		D3D11_BLEND_OP _BlendOp = D3D11_BLEND_OP_ADD,
		D3D11_BLEND _SrcBlendAlpha = D3D11_BLEND_ONE,
		D3D11_BLEND _DestBlendAlpha = D3D11_BLEND_ZERO,
		D3D11_BLEND_OP _BlendOpAlpha = D3D11_BLEND_OP_ADD,
		UINT8 _RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL);

	void SetBlendFactor(const std::string& _Name, float _r, float _g, float _b, float _a);

	void SetBlendSampleMask(const std::string& _Name, UINT _SampleMask);

	bool CreateBlendState(const std::string& _Name,
		bool _AlphaToCoverageEnable = false,
		bool _IndependentBlendEnable = false);

	bool CreateDepthStencilState(const std::string& _Name,
		bool _DepthEnable = true,
		D3D11_DEPTH_WRITE_MASK _DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL,
		D3D11_COMPARISON_FUNC _DepthFunc = D3D11_COMPARISON_LESS,
		bool StencilEnable = false,
		UINT8 _StencilReadMask = 0xff,
		UINT8 _StencilWriteMask = 0xff,
		D3D11_DEPTH_STENCILOP_DESC _FrontFace =
		{ D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP,
		D3D11_STENCIL_OP_KEEP , D3D11_COMPARISON_ALWAYS },
		D3D11_DEPTH_STENCILOP_DESC _BackFace =
		{ D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP,
		D3D11_STENCIL_OP_KEEP , D3D11_COMPARISON_ALWAYS });

	void SetState(const std::string& _Name);
	void ResetState(const std::string& _Name);

	std::weak_ptr<class CRenderState> FindRenderState(const std::string& _Name);

public:
	bool Init();
	void Render();
};

