#include "pch.h"
#include "CBufferTransform.h"
#include "ConstantBuffer.h"

CCBufferTransform::CCBufferTransform()
{
}

CCBufferTransform::CCBufferTransform(const CCBufferTransform& _Ref) :
	CConstantBufferData(_Ref)
{
	m_Data = _Ref.m_Data;
}

CCBufferTransform::CCBufferTransform(CCBufferTransform&& _Ref) noexcept : 
	CConstantBufferData(_Ref)
{
	m_Data = _Ref.m_Data;
}

CCBufferTransform::~CCBufferTransform()
{
}

bool CCBufferTransform::Init()
{
	SetConstantBuffer("Transform");
	return true;
}

// WV, WVP를 만들고 CConstantBuffer->Update로 GPU에게 보내준다.
void CCBufferTransform::UpdateBuffer()
{
	m_Data.WV = m_Data.World * m_Data.View;
	m_Data.WVP = m_Data.WV * m_Data.Proj;

	// Shader에서 이 행렬을 이용하여 연산을 할 수 있게 하기 위해 행과 열을 바꾼다.
	m_Data.World.Transpose();
	m_Data.View.Transpose();
	m_Data.Proj.Transpose();
	m_Data.WV.Transpose();
	m_Data.WVP.Transpose();

	std::shared_ptr<CConstantBuffer> CBuffer = m_Buffer.lock();

	if (CBuffer)
		CBuffer->Update(&m_Data);
}

CCBufferTransform* CCBufferTransform::Clone()
{
	return nullptr;
}
