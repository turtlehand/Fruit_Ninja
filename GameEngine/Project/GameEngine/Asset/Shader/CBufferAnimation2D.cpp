#include "pch.h"
#include "CBufferAnimation2D.h"
#include "ConstantBuffer.h"

CCBufferAnimation2D::CCBufferAnimation2D() :
	CConstantBufferData()
	, m_Data()
{
}

CCBufferAnimation2D::CCBufferAnimation2D(const CCBufferAnimation2D& _Ref) :
	CConstantBufferData(_Ref)
	, m_Data()
{
}

CCBufferAnimation2D::CCBufferAnimation2D(CCBufferAnimation2D&& _Ref) noexcept  :
	CConstantBufferData(std::move(_Ref))
	, m_Data(std::move(_Ref.m_Data))
{
}

CCBufferAnimation2D::~CCBufferAnimation2D()
{
}

bool CCBufferAnimation2D::Init()
{
	SetConstantBuffer("Animation2D");

	return true;
}

void CCBufferAnimation2D::UpdateBuffer()
{
	std::shared_ptr<CConstantBuffer> CBuffer =  m_Buffer.lock();

	if (CBuffer)
		CBuffer->Update(&m_Data);
}
CCBufferAnimation2D* CCBufferAnimation2D::Clone()
{
	return new CCBufferAnimation2D(*this);
}