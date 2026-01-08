#pragma once
#include "RenderState.h"

class DepthStencilState :
    public CRenderState
{

public:
    // DepthEnable : 깊이를 쓸지 말지 결정한다.
    // D3D11_DEPTH_WRITE_MASK : zero, all로 구성, zero를 하면 깊이 버퍼에 깊이값을 안쓰고 all로 하면 깊이값을 쓴다.
    // DepthFunc : 깅피값을 비교할 함수를 지정한다.
};

