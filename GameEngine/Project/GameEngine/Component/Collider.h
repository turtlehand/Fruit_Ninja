#pragma once
#include "SceneComponent.h"

class CCollider abstract:
	public CSceneComponent
{
	friend class CGameObject;
	friend class CObject;
protected:
	CCollider(EColliderType _ColliderType);
	CCollider(const CCollider& _Ref);
	CCollider(CCollider&& _Ref) noexcept;

public:
	~CCollider();

protected:
	EColliderType		m_ColliderType;
	FVector3			m_Min;
	FVector3			m_Max;
	FVector3			m_RenderScale;
	bool				m_DebugDraw;
	bool				m_Collision;
	FCollisionProfile* m_Profile = nullptr;

	// 이 충돌체와 부딪친 충돌체는 서로 떨어지기 전까지 이 맵에 보관해둔다.
	std::unordered_map<CCollider*, std::weak_ptr<CCollider>> m_CollisionObjectMap;

	std::weak_ptr<class CShader> m_Shader;
	std::weak_ptr<class CMesh> m_Mesh;
	std::shared_ptr<class CCBufferTransform> m_TransformCBuffer;
	std::shared_ptr<class CCBufferCollider> m_ColliderCBuffer;

	std::function<void(const FVector3&, CCollider*)> m_CollisionBeginFunc;
	std::function<void(CCollider*)> m_CollisionEndFunc;


public:
	EColliderType GetColliderType() const { return m_ColliderType; }
	bool GetDebugDraw() { return m_DebugDraw; }
	FCollisionProfile* GetCollisionProfile() const { return m_Profile; }
public:
	void virtual SetDebugDraw(bool _Draw);
	void SetCollisionProfile(const std::string& _Name);
	bool CheckCollisionObject(CCollider* _Collider);
	void EraseCollisionObject(CCollider* _Collider);

public:
	virtual bool Init() override;
	virtual void Update(double _DeltaTime) override;
	virtual void PostUpdate(double _DeltaTime) override;
	virtual void Render() override;

protected:
	virtual CCollider* Clone() const = 0;

public:
	void CallCollisionBegin(const FVector3& _HitPoint, std::weak_ptr<CCollider>& _Collider);
	void CallCollisionEnd(std::weak_ptr<CCollider>& _Collider);

public:
	virtual bool Collision(FVector3& _HitPoint, std::shared_ptr<CCollider> _Dest) = 0;

	/// <summary>
	/// 다른 Collider와 충돌 시 호출되는 함수를 등록한다.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="_Obj"></param>
	/// <param name="_Func"></param>
	template<typename T>
	void SetCollisionBeginFunction(T* _Obj, void(T::* _Func)(const FVector3&, CCollider*))
	{
		m_CollisionBeginFunc = std::bind(_Func, _Obj, std::placeholders::_1, std::placeholders::_2);
	}

	/// <summary>
	/// 다른 Collider와 충돌 해제 시 호출되는 함수를 등록한다.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="_Obj"></param>
	/// <param name="_Func"></param>
	template<typename T>
	void SetCollisionEndFunction(T* _Obj, void(T::* _Func)(CCollider*))
	{
		m_CollisionEndFunc = std::bind(_Func, _Obj, std::placeholders::_1);

	}
};

