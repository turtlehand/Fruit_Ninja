#pragma once
#include "SceneComponent.h"
#include "../Asset/Shader/CBufferContainer.h"

class CShader;
class CMesh;
class CTexture;
class CCBufferTransform;
class CMaterial;

class CAnimation2DComponent;
class CCBufferAnimation2D;

class CDynamicMeshComponent :
	public CSceneComponent
{
	friend class CGameObject;
	friend class CObject;

protected:
	CDynamicMeshComponent();
	CDynamicMeshComponent(const CDynamicMeshComponent& _Ref);
	CDynamicMeshComponent(CDynamicMeshComponent&& _Ref) noexcept;

public:
	~CDynamicMeshComponent();

protected:
	std::weak_ptr<CShader>							m_Shader;
	std::vector<std::vector<FVector3>>				m_VertexPath;
	std::shared_ptr<CMesh>							m_DynamicMesh;
	std::vector<std::shared_ptr<CMaterial>>			m_MaterialSlot;
	std::shared_ptr<CCBufferTransform>				m_TransformCBuffer;

	std::weak_ptr<CAnimation2DComponent>			m_AnimComponent;
	static std::shared_ptr<CCBufferAnimation2D>		m_EmptyAnimCBuffer;

	int                                             m_MaxPoint = DYNAMIC_MESH_MAX_POINT;
	int												m_PathSize;

public:
	int GetVertexCount(int _Path) const { return m_VertexPath[_Path].size(); }
	int GetAllVertexCount() const;

	void ResizePath(int _PathSize);
	void AddVertex(const FVector3& _Point, int _Path = 0);
	void SetVertex(int _Index, const FVector3& _Point, int _Path = 0);
	void RemoveVertex(int _Path = 0);

public:
	void SetShader(const std::weak_ptr<CShader>& _Shader);
	void SetShader(const std::string& _Name);

	void SetMaterial(int _SlotIndex);
	void SetMaterialBaseColor(int _SlotIndex, float _r, float _g, float _b, float _a);
	void SetMaterialBaseColor(int _SlotIndex, unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a);
	void SetMaterialBaseColor(int _SlotIndex, const FVector4& _Color);
	void SetMaterialOpacity(int _SlotIndex, float _Opacity);

	bool SetTexture(int _SlotIndex, int _TextureIndex, const std::weak_ptr<CTexture> _Texture);
	const std::weak_ptr<CTexture>& GetTexture(int _SlotIndex, int _TextureIndex);

	void AddTexture(int _SlotIndex,
		const std::weak_ptr<CTexture>& _Texture,
		int _Register, int _ShaderBufferType = EShaderBufferType::Pixel,
		int _Index = 0);

	void AddTexture(int _SlotIndex,
		const std::string& _Name,
		int _Register, int _ShaderBufferType = EShaderBufferType::Pixel,
		int _Index = 0);

	void AddTexture(int _SlotIndex,
		const std::string& _Name,
		const wchar_t* _FileName,
		const std::wstring& _PathName = L"Texture",
		int _Register = 0, int _ShaderBufferType = EShaderBufferType::Pixel,
		int _Index = 0);
	void AddTextureFullPath(int _SlotIndex,
		const std::string& _Name,
		const wchar_t* _FullPath,
		int _Register = 0, int _ShaderBufferType = EShaderBufferType::Pixel,
		int _Index = 0);
	void AddTexture(int _SlotIndex,
		const std::string& _Name,
		const std::vector<const wchar_t*>& _FileName,
		const std::wstring& _PathName = L"Texture",
		int _Register = 0, int _ShaderBufferType = EShaderBufferType::Pixel,
		int _Index = 0);
	void AddTextureFullPath(int _SlotIndex,
		const std::string& _Name,
		const std::vector<const wchar_t*>& _FullPath,
		int _Register = 0, int _ShaderBufferType = EShaderBufferType::Pixel,
		int _Index = 0);

	void SetBlendState(int _SlotIndex, const std::string& _Name);
	void SetBlendState(int _SlotIndex, const std::weak_ptr<class CRenderState>& _State);

public:
	static void CreateEmptyAnimCBuffer();
	static void ClearEmptyAnimCBuffer();

public:
	void SetAnimComponent(const std::weak_ptr<class CAnimation2DComponent>& AnimComponent)
	{
		m_AnimComponent = AnimComponent;
	}
public:
	virtual bool Init() override;
	virtual void Render() override;
	virtual void Destroy() override;

protected:
	virtual CDynamicMeshComponent* Clone() const;

protected:

	std::weak_ptr<class CMesh> CreateDynamicMesh();
	void UpdateMesh();
	void UpdateMesh(const std::vector<FTriangle2DInfo>& _Triangle);

};

