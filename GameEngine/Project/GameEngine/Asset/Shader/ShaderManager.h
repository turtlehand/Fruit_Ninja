#pragma once
#include "Shader.h"
#include "ConstantBuffer.h"

class CShaderManager
{
	friend class CAssetManager;
private:
	CShaderManager();

public:
	~CShaderManager();

private:
	std::unordered_map<std::string, std::shared_ptr<CShader>> m_ShaderMap;
	std::unordered_map<std::string, std::shared_ptr<CConstantBuffer>> m_CBufferMap;

public:
	bool Init();

public:
	std::weak_ptr<CShader> FindShader(const std::string& _Name);
	std::weak_ptr<CConstantBuffer> FindCBuffer(const std::string& _Name);
	bool CreateCBuffer(const std::string& _Name, int _Size, int _Register, int _ShaderBuffer = EShaderBufferType::VP);
	void ReleaseShader(const std::string& _Name);
	void ReleaseCBuffer(const std::string& _Name);

public:
	template<typename T>
	bool CreateShader(const std::string& _Name, const std::wstring& _PathName)
	{
		std::weak_ptr<CShader> Check = FindShader(_Name);

		// 있을 경우
		if (!Check.expired())
			return true;
		T* Origin = new T;
		std::shared_ptr<CShader> Shader;
		Shader.reset(Origin);

		if (!Shader->Init(_PathName))
		{
			assert(false);
			return false;
		}

		Shader->SetName(_Name);

		m_ShaderMap.insert(std::make_pair(_Name, Shader));

		return true;
	}



};

