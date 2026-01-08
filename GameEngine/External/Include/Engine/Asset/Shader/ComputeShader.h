#pragma once
#include "Shader.h"
class CComputeShader :
    public CShader
{
public:
    virtual bool Init() = 0;
    virtual void SetShader() final;


    friend class CShaderManager;
private:
    CComputeShader();

public:
    virtual ~CComputeShader() = 0;


};


