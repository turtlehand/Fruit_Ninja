#pragma once
class CRenderState
{
	friend class CRenderManager;

protected:
	CRenderState();

public:
	virtual ~CRenderState();

protected:
	ComPtr<ID3D11DeviceChild> m_State;
	ComPtr<ID3D11DeviceChild> m_PrevState;

public:
	virtual void SetState() = 0;
	virtual void ResetState() = 0;
};

