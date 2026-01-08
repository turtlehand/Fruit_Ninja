#pragma once
#include "../Asset.h"

// GPU에게 버퍼를 보내주는 클래스
class CConstantBuffer :
    public CAsset
{

    ComPtr<ID3D11Buffer> m_Buffer;
    int m_Size;
    // 상수 버퍼의 레지스터 번호
    int m_Register;
    // 어떤 Shader에서 이 상수 버퍼를 사용하는지 판단하는 용도
    int m_ShaderBuffer;
public:
    bool Init(int _Size, int _Register, int _ShaderBuffer);
    void Update(void* _Data);

    friend class CShaderManager;
private:
    CConstantBuffer();
public:
    ~CConstantBuffer();
};

