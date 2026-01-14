#pragma once
#include "GraphicShader.h"
class CShaderWireFrame :
    public CGraphicShader
{
    friend class CShaderManager;
protected:
    CShaderWireFrame();
public:
    ~CShaderWireFrame();

public:
    virtual bool Init(const std::wstring& _PathName);
};

