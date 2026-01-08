#pragma once
#include "GraphicShader.h"
class CShaderColor2D :
    public CGraphicShader
{
    friend class CShaderManager;

public:
    virtual bool Init(const std::wstring& _PathName);

protected:
    CShaderColor2D();
public:
    ~CShaderColor2D();
};

