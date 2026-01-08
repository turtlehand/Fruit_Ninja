#pragma once
#include "Shader.h"

class CGraphicShader :
    public CShader
{
protected:
    // VertexShader, PixelShader, DomainShader, HullShader, GeometryShader
    ComPtr<ID3D11InputLayout> m_InputLayout;
    
    // D3D11_INPUT_ELEMENT_DESC : 입력 레지스터 요소 1개를 이 구조체 1개로 지정
    // 예를 들어 struct
    // float3 : POSITION
    // float4 : COLOR0 
    // 일 경우 이 구조체 2개가 있어야 지정이 가능하다.
    std::vector< D3D11_INPUT_ELEMENT_DESC> m_InputDesc;

    UINT m_Size;

    // VertexShader 객체
    ComPtr<ID3D11VertexShader> m_VS;
    // VertexShader 코드를 컴파일하고 바이트 코드를 저장하기 위한 버퍼
    ComPtr<ID3DBlob> m_VSBlob;

    // PixelShader 객체
    ComPtr<ID3D11PixelShader> m_PS;
    // PixelShader 코드를 컴파일하고 바이트 코드를 저장하기 위한 버퍼
    ComPtr<ID3DBlob> m_PSBlob;


public:
    virtual bool Init(const std::wstring& _PathName) = 0;
    virtual void SetShader() final;

    /*
    LPCSTR SemanticName        레지스터 이름
    UINT SemanticIndex         레지스터 번호
    DXGI_FORMAT Format         몇 바이트인지를 포멧으로 지정
    UINT InputSlot             버텍스 버퍼 번호
    UINT AlignedByteOffset     여기서 지정하는 요소가 몇 바이트 째에 있는지
    D3D11_INPUT_CLASSIFICATION InputSlotClass정점 데이터인지 인스턴싱인지
    UINT InstanceDataStepRate  인스턴싱일 경우에만 사용
    */
    void AddInputDesc(const char* _Semantic, UINT _SemanticeIndex, DXGI_FORMAT _Fmt, UINT _InputSlot,
        UINT _Size, D3D11_INPUT_CLASSIFICATION _InputSlotClass, UINT _InstanceDataStepRate);
    bool CreateInputLayout();
    bool LoadVertexShader(const std::string& _EntryName, const std::wstring& _FileName, const std::wstring& _PathName = L"Shader");
    bool LoadPixelShader(const std::string& _EntryName, const std::wstring& _FileName, const std::wstring& _PathName = L"Shader");

    friend class CShaderManager;
protected:
    CGraphicShader();

public:
    virtual ~CGraphicShader() = 0;


};

