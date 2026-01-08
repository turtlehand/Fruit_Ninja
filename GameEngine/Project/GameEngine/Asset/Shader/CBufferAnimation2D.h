#pragma once
#include "ConstantBufferData.h"
class CCBufferAnimation2D :
	public CConstantBufferData
{
public:
	CCBufferAnimation2D();
	CCBufferAnimation2D(const CCBufferAnimation2D& _Ref);
	CCBufferAnimation2D(CCBufferAnimation2D&& _Ref)	noexcept;
	virtual ~CCBufferAnimation2D();

private:
	FCBufferAnimation2DData	m_Data;

public:
	virtual bool Init();
	virtual void UpdateBuffer();
	virtual CCBufferAnimation2D* Clone();

public:
	void SetLTUV(const FVector2& _uv)
	{
		m_Data.LTUV = _uv;
	}

	void SetLTUV(float _u, float _v)
	{
		m_Data.LTUV.x = _u;
		m_Data.LTUV.y = _v;
	}

	void SetRBUV(const FVector2& _uv)
	{
		m_Data.RBUV = _uv;
	}

	void SetRBUV(float u, float v)
	{
		m_Data.RBUV.x = u;
		m_Data.RBUV.y = v;
	}

	void SetAnimation2DEnable(bool _Enable)
	{
		m_Data.Anim2DEnable = _Enable ? 1 : 0;
	}

	void SetAnimation2DTextureType(EAnimation2DTextureType _Type)
	{
		m_Data.Anim2DTextureType = (int)_Type;
	}

	void SetTextureSymmetry(bool _Symmetry)
	{
		m_Data.TextureSymmetry = _Symmetry ? 1 : 0;
	}
};

