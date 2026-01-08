#pragma once
#include "../../Object.h"

struct FMaterialTextureInfo
{
	std::string                     Name;
	std::weak_ptr<class CTexture>   Texture;
	int                             Register = 0;
	int                             ShaderBufferType = EShaderBufferType::Pixel;
	int                             Index = 0;
};

class CMaterial :
	public CObject
{
	friend class CMaterialManager;
	friend class CMesh;
protected:
	CMaterial();
	CMaterial(const CMaterial& _Ref);

public:
	virtual ~CMaterial();

protected:
	std::vector<std::shared_ptr<FMaterialTextureInfo>>	m_TextureArray;
	FVector4											m_BaseColor;
	float												m_Opacity; // 불투명도 : 1. 불투명 0. 투명
	std::shared_ptr<class CCBufferMaterial>				m_MaterialCBuffer;
	std::weak_ptr<class CRenderState>					m_BlendState;

public:

	void AddTexture(const std::weak_ptr<CTexture>& _Texture,
		int _Register, int _ShaderBufferType = EShaderBufferType::Pixel,
		int _Index = 0);

	void AddTexture(const std::string& _Name,
		int _Register, int _ShaderBufferType = EShaderBufferType::Pixel,
		int _Index = 0);

	void AddTexture(const std::string& _Name,
		const wchar_t* _FileName,
		const std::wstring& _PathName = L"Texture",
		int _Register = 0, int _ShaderBufferType = EShaderBufferType::Pixel,
		int _Index = 0);

	void AddTextureFullPath(const std::string& _Name,
		const wchar_t* _FullPath,
		int _Register = 0, int _ShaderBufferType = EShaderBufferType::Pixel,
		int _Index = 0);

	void AddTexture(const std::string& _Name,
		const std::vector<const wchar_t*>& _FileName,
		const std::string& _PathName = "Texture",
		int _Register = 0, int _ShaderBufferType = EShaderBufferType::Pixel,
		int _Index = 0);

	void AddTextureFullPath(const std::string& _Name,
		const std::vector<const wchar_t*>& _FullPath,
		int _Register = 0, int _ShaderBufferType = EShaderBufferType::Pixel,
		int _Index = 0);

	bool SetTexture(int _TextureIndex, const std::weak_ptr<CTexture>& _Texture);

	void SetBaseColor(float _R, float _G, float _B, float _A);
	void SetBaseColor(unsigned char _R, unsigned char _G, unsigned char _B, unsigned char _A);
	void SetBaseColor(const FVector4& _Color);

	void SetOpacity(float _Opacity);

	void SetBlendState(const std::string& _Name);
	void SetBlendState(const std::weak_ptr<class CRenderState>& _State);

public :
	bool Init();
	void UpdateConstantBuffer();
	void UpdateConstantBuffer(int _TextureIndex);
	void Reset();

public:
	CMaterial* Clone();

};

