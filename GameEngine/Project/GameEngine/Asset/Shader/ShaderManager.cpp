#include "pch.h"
#include "ShaderManager.h"

#include "ShaderColor2D.h"
#include "ShaderMaterialColor2D.h"
#include "ShaderTexture2D.h"
#include "ShaderCollider.h"
#include "ShaderWireFrame.h"

#include "Device.h"

CShaderManager::CShaderManager()
{
}


CShaderManager::~CShaderManager()
{
}

bool CShaderManager::Init()
{
	D3D11_RASTERIZER_DESC Desc = {};
	Desc.CullMode = D3D11_CULL_FRONT;
	Desc.FillMode = D3D11_FILL_SOLID;
	if (CDevice::GetInst()->GetDevice()->CreateRasterizerState(&Desc, m_RSState[(UINT)0].GetAddressOf()))
		return false;

	Desc.CullMode = D3D11_CULL_NONE;
	Desc.FillMode = D3D11_FILL_WIREFRAME;
	if (CDevice::GetInst()->GetDevice()->CreateRasterizerState(&Desc, m_RSState[(UINT)1].GetAddressOf()))
		return false;

	// ============== 상수 버퍼 ============== 
	// Transform 정보를 0번 상수 레지스터에, Graphic 관련 셰이더에게 등록한다.
	if (!CreateCBuffer("Transform", sizeof(FCBufferTransformData), 0, EShaderBufferType::Graphic))
		return false;
 
	// 재질 정보를 1번 상수 레지스터에, Graphic 관련 셰이더에게 등록한다.
	if (!CreateCBuffer("Material", sizeof(FCBufferMaterialData), 1, EShaderBufferType::Pixel))
		return false;

	// 애니메이션 정보를 1번 상수 레지스터에, Graphic 관련 셰이더에게 등록한다.
	if (!CreateCBuffer("Animation2D", sizeof(FCBufferAnimation2DData), 2, EShaderBufferType::Vertex))
		return false;

	// 애니메이션 정보를 1번 상수 레지스터에, Graphic 관련 셰이더에게 등록한다.
	if (!CreateCBuffer("Collider", sizeof(FCBufferColliderData), 10, EShaderBufferType::Pixel))
		return false;



	//=============== Shader ========================
	if (!CreateShader<CShaderColor2D>("Color2D", L"EngineShader"))
		return false;

	if (!CreateShader<CShaderMaterialColor2D>("MaterialColor2D", L"EngineShader"))
		return false;

	if (!CreateShader<CShaderTexture2D>("DefaultTexture2D", L"EngineShader"))
		return false;

	if (!CreateShader<CShaderCollider>("Collider", L"EngineShader"))
		return false;

	if (!CreateShader<CShaderWireFrame>("WireFrame", L"EngineShader"))
		return false;


	return true;
}

std::weak_ptr<CShader> CShaderManager::FindShader(const std::string& _Name)
{
	auto iter = m_ShaderMap.find(_Name);

	if (iter == m_ShaderMap.end())
		return std::weak_ptr<CShader>();

	return iter->second;
}

std::weak_ptr<CConstantBuffer> CShaderManager::FindCBuffer(const std::string& _Name)
{
	auto iter = m_CBufferMap.find(_Name);

	if (iter == m_CBufferMap.end())
		return std::weak_ptr<CConstantBuffer>();
	return iter->second;
}

// 상수 버퍼를 생성한다.
bool CShaderManager::CreateCBuffer(const std::string& _Name, int _Size, int _Register, int _ShaderBuffer)
{
	std::weak_ptr<CConstantBuffer> Check = FindCBuffer(_Name);

	// 있을 경우
	if (!Check.expired())
		return true;

	CConstantBuffer* Origin = new CConstantBuffer;
	std::shared_ptr<CConstantBuffer> CBuffer;
	CBuffer.reset(Origin);

	if (!Origin->Init(_Size, _Register, _ShaderBuffer))
	{
		assert(false);
		return false;
	}

	m_CBufferMap.insert(make_pair(_Name, CBuffer));

	return true;
}

void CShaderManager::ReleaseShader(const std::string& _Name)
{
	auto iter = m_ShaderMap.find(_Name);

	if (iter != m_ShaderMap.end())
	{
		// 다른 월드에서 더 이상 들고 있지 않을 경우
		if (iter->second.use_count() == 1)
		{
			m_ShaderMap.erase(iter);
		}
	}
}

void CShaderManager::ReleaseCBuffer(const std::string& _Name)
{
	auto iter = m_CBufferMap.find(_Name);

	if (iter != m_CBufferMap.end())
	{
		// 다른 월드에서 더 이상 들고 있지 않을 경우
		if (iter->second.use_count() == 1)
		{
			m_CBufferMap.erase(iter);
		}
	}
}
