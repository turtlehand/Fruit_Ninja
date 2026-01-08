#pragma once
#include "ConstantBufferData.h"


class CCBufferMaterial :
    public CConstantBufferData
{
public:
	CCBufferMaterial();
	CCBufferMaterial(const CCBufferMaterial& _Ref);
	CCBufferMaterial(CCBufferMaterial&& _Ref)	noexcept;
	virtual ~CCBufferMaterial();

private:
	FCBufferMaterialData	m_Data;

public:
	virtual bool Init();
	virtual void UpdateBuffer();
	virtual CCBufferMaterial* Clone();

public:
	void SetBaseColor(const FVector4& Color)
	{
		m_Data.BaseColor = Color;
	}

	void SetOpacity(float Opacity)
	{
		m_Data.Opacity = Opacity;
	}
};

