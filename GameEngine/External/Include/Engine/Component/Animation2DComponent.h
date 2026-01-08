#pragma once
#include "ObjectComponent.h"
#include "Animation2DSequence.h"

class CAnimation2D;

class CAnimation2DComponent :
    public CObjectComponent
{
    friend class CGameObject;
protected:
    CAnimation2DComponent();
    CAnimation2DComponent(const CAnimation2DComponent& _Ref);
    CAnimation2DComponent(CAnimation2DComponent&& _Ref) noexcept;

public:
    ~CAnimation2DComponent();

protected:
    std::weak_ptr<class CMeshComponent>                                     m_UpdateComponent;
    std::unordered_map<std::string, std::shared_ptr<CAnimation2DSequence>>  m_AnimationSequenceMap;
    std::shared_ptr<CAnimation2DSequence>                                   m_CurrentAnimationSequence;
    std::shared_ptr<class CCBufferAnimation2D>                              m_AnimCBuffer;
    bool	                                                                m_UpdateEnable;

public:
    void SetUpdateComponent(const std::weak_ptr<class CMeshComponent>& _Component);
    void AddAnimation(const std::weak_ptr<class CAnimation2D>& _Anim, float _PlayTime = 1.f, float _PlayRate = 1.f, bool _Loop = false, bool _Reverse = false);
    void AddAnimation(const std::string& _Name, float _PlayTime = 1.f, float _PlayRate = 1.f, bool _Loop = false, bool _Reverse = false);
    void SetPlayTime(const std::string& _Name, float _PlayTime);
    void SetPlayRate(const std::string& _Name, float _PlayRate);
    void SetSymmetry(const std::string& _Name, bool _Symmetry);
    void SetLoop(const std::string& _Name, bool _Loop);
    void SetReverse(const std::string& _Name, bool _Reverse);
    void ChangeAnimation(const std::string& _Name);

    void SetShader();
    EAnimation2DTextureType GetTextureType() const;
    int GetAnimationFrame() const;

public:
    virtual bool Init() override;
    virtual void Update(double _DeltaTime) override;
    virtual void PostUpdate(double _DeltaTime) override;
    virtual void Destroy() override;
protected:
    virtual CAnimation2DComponent* Clone() const ;

public:
    template <typename T>
    void AddNotify(const std::string& _SequenceName, const std::string& _Name, int _Frame, T* _Obj, void (T::* _Func)())
    {
        std::string Key = "Animation2D_" + _SequenceName;

        auto iter = m_AnimationSequenceMap.find(Key);

        if (iter == m_AnimationSequenceMap.end())
            return;

        iter->second->AddNotify<T>(_Name, _Frame, _Obj, _Func);
    }

    template <typename T>
    void SetFinishNotify(const std::string& _SequenceName, T* _Obj, void (T::* _Func)())
    {
        std::string Key = "Animation2D_" + _SequenceName;

        auto iter = m_AnimationSequenceMap.find(Key);

        if (iter == m_AnimationSequenceMap.end())
            return;

        iter->second->SetFinishNotify<T>(_Obj, _Func);
    }
};

