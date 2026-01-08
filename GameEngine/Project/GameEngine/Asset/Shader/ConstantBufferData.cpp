#include "pch.h"
#include "ConstantBufferData.h"
#include "Asset/Shader/ShaderManager.h"
#include "Asset/AssetManager.h"

CConstantBufferData::CConstantBufferData()
{
}

CConstantBufferData::CConstantBufferData(const CConstantBufferData& _Origin)
{
	m_Buffer = _Origin.m_Buffer;
}

CConstantBufferData::CConstantBufferData(CConstantBufferData&& _Origin) noexcept
{
	m_Buffer = _Origin.m_Buffer;
	_Origin.m_Buffer.reset();
}

CConstantBufferData::~CConstantBufferData()
{
}

// 어떤 유형의 버퍼를 사용할 것인지 세팅해준다.
void CConstantBufferData::SetConstantBuffer(const std::string& _Name)
{
	std::weak_ptr<CShaderManager> Weak_ShaderMgr = CAssetManager::GetInst()->GetShaderManager();

	std::shared_ptr<CShaderManager> ShaderMgr = Weak_ShaderMgr.lock();

	if (ShaderMgr)
		m_Buffer = ShaderMgr->FindCBuffer(_Name);
}
