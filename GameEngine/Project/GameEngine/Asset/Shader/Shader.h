#pragma once
#include "../Asset.h"

class CShader :
	public CAsset
{

public:
	virtual bool Init(const std::wstring& _PathNam) = 0;
	virtual void SetShader() = 0;

	friend class CShaderManager;
protected:
	CShader();
public:
	virtual ~CShader() = 0;


};

