#pragma once
#include "ConstantBufferData.h"

// Transform 정보를 담고 있는 클래스

class CCBufferTransform :
	public CConstantBufferData
{
public:
	CCBufferTransform();
	CCBufferTransform(const CCBufferTransform& _Ref);
	CCBufferTransform(CCBufferTransform&& _Ref)	noexcept;
	virtual ~CCBufferTransform();

private:
	FCBufferTransformData	m_Data;

public:
	virtual bool Init();
	virtual void UpdateBuffer();
	virtual CCBufferTransform* Clone();

public:
	void SetWorldMatrix(const FMatrix& _World)
	{
		m_Data.World = _World;
	}

	void SetViewMatrix(const FMatrix& _View)
	{
		m_Data.View = _View;
	}

	void SetProjMatrix(const FMatrix& _Proj)
	{
		m_Data.Proj = _Proj;
	}

	void SetPivotSize(const FVector3& _PivotSize)
	{
		m_Data.PivotSize = _PivotSize;
	}
};

