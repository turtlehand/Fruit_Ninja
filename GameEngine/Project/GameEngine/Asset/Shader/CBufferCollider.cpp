#include "pch.h"
#include "CBufferCollider.h"
#include "ConstantBuffer.h"

CCBufferCollider::CCBufferCollider()
{
}

CCBufferCollider::CCBufferCollider(const CCBufferCollider& _Ref) :
	CConstantBufferData(_Ref),
	m_Data(_Ref.m_Data)
{
}

CCBufferCollider::CCBufferCollider(CCBufferCollider&& _Ref) noexcept :
	CConstantBufferData(std::move(_Ref)),
	m_Data(std::move(_Ref.m_Data))
{
}

CCBufferCollider::~CCBufferCollider()
{
}

bool CCBufferCollider::Init()
{
	SetConstantBuffer("Collider");

	return true;
}

void CCBufferCollider::UpdateBuffer()
{
	std::shared_ptr<CConstantBuffer> CBuffer = m_Buffer.lock();

	if (CBuffer)
		CBuffer->Update(&m_Data);
}

CCBufferCollider* CCBufferCollider::Clone()
{
	return new CCBufferCollider(*this);
}