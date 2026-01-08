#pragma once
#include "ConstantBufferData.h"

class CCBufferCollider :
    public CConstantBufferData
{
public:
	CCBufferCollider();
	CCBufferCollider(const CCBufferCollider& _Ref);
	CCBufferCollider(CCBufferCollider&& _Ref)	noexcept;
	virtual ~CCBufferCollider();

private:
	FCBufferColliderData	m_Data;

public:
	virtual bool Init();
	virtual void UpdateBuffer();
	virtual CCBufferCollider* Clone();

public:
	void SetColor(const FVector4& _Color) { m_Data.Color = _Color; }
};

