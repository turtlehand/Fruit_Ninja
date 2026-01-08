#include "pch.h"
#include "CBufferMaterial.h"
#include "ConstantBuffer.h"

CCBufferMaterial::CCBufferMaterial()
{
}

CCBufferMaterial::CCBufferMaterial(const CCBufferMaterial& _Ref) :
	CConstantBufferData(_Ref),
	m_Data(_Ref.m_Data)
{

}

CCBufferMaterial::CCBufferMaterial(CCBufferMaterial&& _Ref) noexcept :
	CConstantBufferData(std::move(_Ref)),
	m_Data(std::move(_Ref.m_Data))
{

}

CCBufferMaterial::~CCBufferMaterial()
{
}

bool CCBufferMaterial::Init()
{
	SetConstantBuffer("Material");
	return true;
}

void CCBufferMaterial::UpdateBuffer()
{
	std::shared_ptr<CConstantBuffer> CBuffer = m_Buffer.lock();

	if (CBuffer)
		CBuffer->Update(&m_Data);
}

CCBufferMaterial* CCBufferMaterial::Clone()
{
	return new CCBufferMaterial(*this);
}
