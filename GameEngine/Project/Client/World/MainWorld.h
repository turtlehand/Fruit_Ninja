#pragma once
#include "Engine/World/World.h"

class CMainWorld :
    public CWorld
{
public:
    CMainWorld();
    virtual ~CMainWorld();

public:
    virtual bool Init();

private:
    void LoadAnimation2D();
    void LoadTexture2D();
};

