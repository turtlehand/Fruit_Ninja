#pragma once
#include "Component.h"

enum class EComponentRender :unsigned char
{
    None,
    Render
};

class CSceneComponent :
    public CComponent
{
    friend class CGameObject;
    friend class CObject;

protected:
    CSceneComponent();
    CSceneComponent(const CSceneComponent& _Ref);
    CSceneComponent(CSceneComponent&& _Ref) noexcept;

public:
    ~CSceneComponent();

protected:
    std::weak_ptr<CSceneComponent>                  m_Parent;
    // 컴포넌트는 하위 컴포넌트를 가르키기만 한다.
    // ActorComponent는 Actor가 관리한다.
    std::vector<std::shared_ptr<CSceneComponent>>   m_ChildList;
    EComponentRender                                m_RenderType;           // 이 컴포넌트가 그리는 타입인지
    int                                             m_RenderLayer = 0;      // 해당 컴포넌트의 Layer

public:
    void SetRenderLayer(int _Layer) { m_RenderLayer = _Layer; }

    EComponentRender GetRenderType() { return m_RenderType;}

    int GetRenderLayer() { return m_RenderLayer; }

public:
    virtual void SetEnable(bool _Enable) override;
    virtual void SetSelf(std::weak_ptr<CComponent> _Self) override;
    virtual void SetWorld(std::weak_ptr<CWorld> _World) override;
    virtual void SetOwner(std::weak_ptr<CGameObject> _Owner) override;

public:
    virtual bool Init() override;
    virtual void Begin() override;
    virtual void Update(double DeltaTime) override;
    virtual void PostUpdate(double _DeltaTime) override;
    virtual void Render() override;
    virtual void Destroy() override;
protected:
    virtual CSceneComponent* Clone() const;

protected:
    virtual void GameObjectEnable(bool _Enable) override;

protected :
    bool        m_InheritScale;        // true 시 부모의 Scale의 영향을 받는다.
    bool        m_InheritRot;          // true 시 부모의 Rot의 영향을 받는다

    FVector3    m_Pivot;

    FVector3    m_RelativeScale;
    FVector3    m_RelativeRot;
    FVector3    m_RelativePos;

    FVector3    m_WorldScale;
    FVector3    m_WorldRot;
    FVector3    m_WorldPos;

    FVector3    m_WorldAxis[EAxis::End];

    FMatrix     m_ScaleMatrix;
    FMatrix     m_RotMatrix;
    FMatrix     m_TranslateMatrix;
    FMatrix     m_WorldMatrix;

public:
    const FVector3& GetPivot() const { return m_Pivot; }

    const FVector3& GetAxis(EAxis::Type _Axis) const
    {
        return m_WorldAxis[_Axis];
    }

    const FVector3& GetRelativeScale() const
    {
        return m_RelativeScale;
    }

    const FVector3& GetRelativeRot() const
    {
        return m_RelativeRot;
    }

    const FVector3& GetRelativePos() const
    {
        return m_RelativePos;
    }

    const FVector3& GetWorldScale() const
    {
        return m_WorldScale;
    }

    const FVector3& GetWorldRot() const
    {
        return m_WorldRot;
    }

    const FVector3& GetWorldPos() const
    {
        return m_WorldPos;
    }

public:
    void AddChild(std::weak_ptr<CSceneComponent> _Child);
    void UpdateTransform();

public:
    void SetInheritScale(bool _Scale);
    void SetInheritRot(bool _Rot);

    void SetPivot(const FVector3& Pivot)
    {
        m_Pivot = Pivot;
    }

    void SetPivot(float x, float y, float z)
    {
        m_Pivot = FVector3(x, y, z);
    }

    void SetPivot(float x, float y)
    {
        m_Pivot = FVector3(x, y, 0.f);
    }
    
    void SetRelativeScale(const FVector3& _Scale);
    void SetRelativeScale(const FVector2& _Scale);
    void SetRelativeScale(float _x, float _y, float _z);
    void SetRelativeScale(float _x, float _y);

    void AddRelativeScale(const FVector3& Scale);
    void AddRelativeScale(const FVector2& Scale);
    void AddRelativeScale(float x, float y, float z);
    void AddRelativeScale(float x, float y);

    void SetRelativeRotation(const FVector3& _Rot);
    void SetRelativeRotation(const FVector2& _Rot);
    void SetRelativeRotation(float _x, float _y, float _z);
    void SetRelativeRotation(float _x, float _y);
    void SetRelativeRotationX(float _x);
    void SetRelativeRotationY(float _y);
    void SetRelativeRotationZ(float _z);

    void AddRelativeRotation(const FVector3& Rot);
    void AddRelativeRotation(const FVector2& Rot);
    void AddRelativeRotation(float x, float y, float z);
    void AddRelativeRotation(float x, float y);
    void AddRelativeRotationX(float x);
    void AddRelativeRotationY(float y);
    void AddRelativeRotationZ(float z);

    void SetRelativePos(const FVector3& _Pos);
    void SetRelativePos(const FVector2& _Pos);
    void SetRelativePos(float _x, float _y, float _z);
    void SetRelativePos(float _x, float _y);

    void AddRelativePos(const FVector3& Pos);
    void AddRelativePos(const FVector2& Pos);
    void AddRelativePos(float x, float y, float z);
    void AddRelativePos(float x, float y);

    void InheritScale();
    void InheritRotation();
    void InheritPos();

    /*
    void SetWorldScale(const FVector3& Scale);
    void SetWorldScale(const FVector2& Scale);
    void SetWorldScale(float x, float y, float z);
    void SetWorldScale(float x, float y);

    void AddWorldScale(const FVector3& Scale);
    void AddWorldScale(const FVector2& Scale);
    void AddWorldScale(float x, float y, float z);
    void AddWorldScale(float x, float y);

    void SetWorldRotation(const FVector3& Rot);
    void SetWorldRotation(const FVector2& Rot);
    void SetWorldRotation(float x, float y, float z);
    void SetWorldRotation(float x, float y);
    void SetWorldRotationX(float x);
    void SetWorldRotationY(float y);
    void SetWorldRotationZ(float z);

    void AddWorldRotation(const FVector3& Rot);
    void AddWorldRotation(const FVector2& Rot);
    void AddWorldRotation(float x, float y, float z);
    void AddWorldRotation(float x, float y);
    void AddWorldRotationX(float x);
    void AddWorldRotationY(float y);
    void AddWorldRotationZ(float z);

    void SetWorldPos(const FVector3& Pos);
    void SetWorldPos(const FVector2& Pos);
    void SetWorldPos(float x, float y, float z);
    void SetWorldPos(float x, float y);

    void AddWorldPos(const FVector3& Pos);
    void AddWorldPos(const FVector2& Pos);
    void AddWorldPos(float x, float y, float z);
    void AddWorldPos(float x, float y);

    void InheritWorldScale();
    void InheritWorldRotation();
    void InheritWorldPos();
    */
};

