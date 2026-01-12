#pragma once
#include "../Object.h"
#include "../Component/SceneComponent.h"
#include "../Component/ObjectComponent.h"

class CSceneComponent;
class CWorld;

class CGameObject :
    public CObject
{
    friend class CWorld;
    friend class CObject;
public:
    CGameObject();
    CGameObject(const CGameObject& _Ref);
    CGameObject(CGameObject&& _Ref) noexcept;
    ~CGameObject();

protected:
    std::weak_ptr<CGameObject>                          m_Self;
    std::weak_ptr<CWorld>                               m_World;
    std::string                                         m_Name;
    bool                                                m_Enable;   // 현재 작동하는지
    bool                                                m_Alive;    // 파괴되지 않고 살아있는지

    // 게임 오브젝트가 컴포넌트를 관리한다.
    std::shared_ptr<CSceneComponent>                    m_SceneComponent;
    std::vector<std::shared_ptr<CObjectComponent>>      m_ObjectComponentList;

public:
    bool GetEnble() const { return m_Enable; }
    bool GetAlive() const { return m_Alive; }
    std::weak_ptr<CSceneComponent> GetRootComponent() const{ return m_SceneComponent; }

    void SetEnble(bool _Enable);
    void SetSelf(std::weak_ptr<CGameObject> _Self);
    void SetWorld(std::weak_ptr<CWorld> _World);
    void SetName(const std::string& _Name);


public:
    virtual bool Init();
    virtual void Begin();       // Begin 함수는 오브젝트가 월드에 배치되고 처음 한번만 호출되는 함수이다.
    virtual void Update(double _DeltaTime);
    virtual void PostUpdate(double _DeltaTime);
    virtual void Render();
    virtual void Destroy();

protected:
    virtual CGameObject* Clone();

public:
    template<typename T>
    std::weak_ptr<T> CreateComponent(const std::string& _Name, const std::string& _ParentName = "Root")
    {
        std::shared_ptr<CComponent> Component;

        Component.reset(new T);
        Component->SetName(_Name);
        Component->SetWorld(m_World);
        Component->SetOwner(m_Self);
        Component->SetSelf(Component);

        // 생성 실패
        if (!Component->Init())
        {
            return std::weak_ptr<T>();
        }

        // Scenen Object일 때만
        if (Component->GetComponentType() == EComponentType::Scene)
        {
            std::shared_ptr<CSceneComponent> SC = std::dynamic_pointer_cast<CSceneComponent>(Component);
            // 이 GameObjcet는 컴포넌트가 존재하지 않는다.
            // 해당 컴포넌트가 Root이다.
            if (!m_SceneComponent)
                m_SceneComponent = SC;

            // 다른 컴포넌트가 존재한다.
            else
            {
                // 최상위 부모에게 자식으로 달아준다.
                if (_ParentName == "Root")
                {
                    if (m_SceneComponent.get())
                        m_SceneComponent->AddChild(SC);
                }
                else
                {
                    // 부모로 붙여줄 컴포넌트를 찾는다.
                    // BFS 순회를 한다.
                    std::queue<std::shared_ptr<CSceneComponent>> ComQue;
                    ComQue.push(m_SceneComponent);

                    std::shared_ptr<CSceneComponent> Parent;

                    while (!ComQue.empty())
                    {
                        std::shared_ptr<CSceneComponent> Temp(ComQue.front());
                        ComQue.pop();

                        if (Temp->m_Name == _ParentName)
                        {
                            Parent = Temp;
                            break;
                        }
                        size_t Size = Temp->m_ChildList.size();

                        for (size_t i = 0; i < Size; ++i)
                        {
                            ComQue.push(Temp->m_ChildList[i]);
                        }
                    }

                    // _ParentName인 부모가 존재하지 않는다.
                    // 최상위 부모에게 자식으로 달아준다.
                    if (!Parent)
                    {
                        if (m_SceneComponent.get() == nullptr)
                            m_SceneComponent->AddChild(SC);
                    }

                    // _ParentName인 부모가 존재한다.
                    // 해당 부모에게 자식으로 달아준다.
                    else
                    {
                        Parent->AddChild(SC);
                    }
                }
            }
        }
        else
        {
            m_ObjectComponentList.push_back(std::dynamic_pointer_cast<CObjectComponent>(Component));
        }

        // dynamic_pointer_cast를 통해 T 타입으로 변환한 shared_ptr이 나오고 그걸 weak_ptr로 변환해서 반환한다.
        return std::dynamic_pointer_cast<T>(Component);
    }

    template<typename T>
    std::weak_ptr<T> FindComponent(const std::string& _Name)
    {
        // BFS 순회를 한다.
        std::queue<std::shared_ptr<CSceneComponent>> SQ;
        SQ.push(m_SceneComponent);

        while (!SQ.empty())
        {
            std::shared_ptr<CSceneComponent> Temp(SQ.front());
            SQ.pop();

            if (Temp->GetName() == _Name)
            {
                return std::dynamic_pointer_cast<T>(Temp);
            }
            size_t Size = Temp->m_ChildList.size();

            for (size_t i = 0; i < Size; ++i)
            {
                SQ.push(Temp->m_ChildList[i]);
            }
        }

        auto Oiter = m_ObjectComponentList.begin();
        auto OiterEnd = m_ObjectComponentList.end();

        for (; Oiter != OiterEnd; ++Oiter)
        {
            if ((*Oiter)->GetName() == _Name)
            {
                return std::dynamic_pointer_cast<T>(*Oiter);
            }
        }

    }

public:
    const FVector3& GetAxis(EAxis::Type Axis)	const;
    const FVector3& GetRelativeScale()	const;
    const FVector3& GetRelativeRot()	const;
    const FVector3& GetRelativePos()	const;
    const FVector3& GetWorldScale()	const;
    const FVector3& GetWorldRot()	const;
    const FVector3& GetWorldPos()	const;

public:
    void SetInheritScale(bool Scale);
    void SetInheritRot(bool Rot);

    void SetRelativeScale(const FVector3& Scale);
    void SetRelativeScale(const FVector2& Scale);
    void SetRelativeScale(float x, float y, float z);
    void SetRelativeScale(float x, float y);

    void AddRelativeScale(const FVector3& Scale);
    void AddRelativeScale(const FVector2& Scale);
    void AddRelativeScale(float x, float y, float z);
    void AddRelativeScale(float x, float y);

    void SetRelativeRotation(const FVector3& Rot);
    void SetRelativeRotation(const FVector2& Rot);
    void SetRelativeRotation(float x, float y, float z);
    void SetRelativeRotation(float x, float y);
    void SetRelativeRotationX(float x);
    void SetRelativeRotationY(float y);
    void SetRelativeRotationZ(float z);

    void AddRelativeRotation(const FVector3& Rot);
    void AddRelativeRotation(const FVector2& Rot);
    void AddRelativeRotation(float x, float y, float z);
    void AddRelativeRotation(float x, float y);
    void AddRelativeRotationX(float x);
    void AddRelativeRotationY(float y);
    void AddRelativeRotationZ(float z);

    void SetRelativePos(const FVector3& Pos);
    void SetRelativePos(const FVector2& Pos);
    void SetRelativePos(float x, float y, float z);
    void SetRelativePos(float x, float y);

    void AddRelativePos(const FVector3& Pos);
    void AddRelativePos(const FVector2& Pos);
    void AddRelativePos(float x, float y, float z);
    void AddRelativePos(float x, float y);

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
    */
};

