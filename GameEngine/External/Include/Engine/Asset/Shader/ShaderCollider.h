#pragma once
#include "GraphicShader.h"
class CShaderCollider :
    public CGraphicShader
{
    friend class CShaderManager;
protected:
    CShaderCollider();
public:
    ~CShaderCollider();

public:
    virtual bool Init(const std::wstring& _PathName);
};

