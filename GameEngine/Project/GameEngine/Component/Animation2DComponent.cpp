#include "pch.h"
#include "Animation2DComponent.h"
#include "Component/MeshComponent.h"

#include "Animation2DSequence.h"
#include "../Asset/AssetManager.h"
#include "../Asset/Animation2D/Animation2D.h"
#include "../Asset/Animation2D/Animation2DManager.h"

#include "../Asset/Shader/CBufferAnimation2D.h"
#include "../Asset/Texture/Texture.h"

#include "../World/World.h"
#include "../World/WorldAssetManager.h"

CAnimation2DComponent::CAnimation2DComponent() :
	CObjectComponent(),
	m_UpdateComponent(),
	m_AnimationSequenceMap(),
	m_CurrentAnimationSequence(),
	m_UpdateEnable(false)
{
	SetClassType<CAnimation2DComponent>();
}

CAnimation2DComponent::CAnimation2DComponent(const CAnimation2DComponent& _Ref) :
	CObjectComponent(_Ref),
	m_UpdateComponent(_Ref.m_UpdateComponent),
	m_AnimationSequenceMap(),
	m_CurrentAnimationSequence(),
	m_UpdateEnable(_Ref.m_UpdateEnable)
{
}

CAnimation2DComponent::CAnimation2DComponent(CAnimation2DComponent&& _Ref) noexcept :
	CObjectComponent(std::move(_Ref)),
	m_UpdateComponent(std::move(_Ref.m_UpdateComponent)),
	m_AnimationSequenceMap(),
	m_CurrentAnimationSequence(),
	m_UpdateEnable(_Ref.m_UpdateEnable)
{
}

CAnimation2DComponent::~CAnimation2DComponent()
{
}

bool CAnimation2DComponent::Init()
{
	m_AnimCBuffer.reset(new CCBufferAnimation2D);

	m_AnimCBuffer->Init();

	m_AnimCBuffer->SetAnimation2DEnable(true);

	return true;
}

void CAnimation2DComponent::Update(double _DeltaTime)
{
	if (m_CurrentAnimationSequence)
	{
		auto	MeshComponent = m_UpdateComponent.lock();

		if (!m_UpdateEnable)
		{
			if (MeshComponent)
			{
				MeshComponent->SetAnimComponent(GetSelf<CAnimation2DComponent>());

				auto	Asset = m_CurrentAnimationSequence->GetAnimation().lock();

				if (Asset)
				{
					m_AnimCBuffer->SetAnimation2DTextureType(Asset->GetAnimationTextureType());
					if (MeshComponent->SetTexture(0, 0, Asset->GetTexture()))
					{
						m_UpdateEnable = true; 
					}
				}
			}
		}

		m_CurrentAnimationSequence->Update(_DeltaTime);

	}
}

void CAnimation2DComponent::PostUpdate(double _DeltaTime)
{

}

void CAnimation2DComponent::Destroy()
{

}

CAnimation2DComponent* CAnimation2DComponent::Clone() const
{
	return new CAnimation2DComponent(*this);
}

void CAnimation2DComponent::SetUpdateComponent(const std::weak_ptr<class CMeshComponent>& _Component)
{
	m_UpdateComponent = _Component;

	auto	MeshComponent = m_UpdateComponent.lock();

	if (MeshComponent && m_CurrentAnimationSequence)
	{
		// MeshComponent에게 AnimationComponent를 등록
		MeshComponent->SetAnimComponent(GetSelf<CAnimation2DComponent>());

		auto	Asset = m_CurrentAnimationSequence->GetAnimation().lock();

		if (Asset)
		{
			m_AnimCBuffer->SetAnimation2DTextureType(Asset->GetAnimationTextureType());
			if (MeshComponent->SetTexture(0, 0,
				Asset->GetTexture()))
				m_UpdateEnable = true;
		}
	}
}

/// <summary>
/// Animation을 가져와서 시퀀스로 추가한다.
/// </summary>
/// <param name="_Anim">추가할 애니메이션</param>
/// <param name="_PlayTime">재생 시간</param>
/// <param name="_PlayRate">재생 속도</param>
/// <param name="_Loop">반복 여부</param>
/// <param name="_Reverse">반전 여부</param>
void CAnimation2DComponent::AddAnimation(const std::weak_ptr<class CAnimation2D>& _Anim, float _PlayTime, float _PlayRate, bool _Loop, bool _Reverse)
{
	auto Anim = _Anim.lock();
	if (!Anim)
		return;

	auto iter = m_AnimationSequenceMap.find(Anim->GetName());

	if (iter != m_AnimationSequenceMap.end())
		return;

	std::shared_ptr<CAnimation2DSequence> Sequence(new CAnimation2DSequence);

	Sequence->SetAnimation2D(Anim);
	Sequence->SetPlayTime(_PlayTime);
	Sequence->SetPlayRate(_PlayRate);
	Sequence->SetLoop(_Loop);
	Sequence->SetReverse(_Reverse);

	m_AnimationSequenceMap.insert(std::make_pair(Anim->GetName(), Sequence));

	if (!m_CurrentAnimationSequence)
	{
		m_CurrentAnimationSequence = Sequence;

		auto	MeshComponent = m_UpdateComponent.lock();

		if (MeshComponent && m_CurrentAnimationSequence)
		{
			auto	Asset = m_CurrentAnimationSequence->GetAnimation().lock();

			if (Asset)
			{
				m_AnimCBuffer->SetAnimation2DTextureType(Asset->GetAnimationTextureType());
				if (MeshComponent->SetTexture(0, 0, Asset->GetTexture()))
					m_UpdateEnable = true;
			}
		}
	}
}

/// <summary>
/// 이름이 _Name인 Animation을 가져와서 시퀀스로 추가한다.
/// </summary>
/// <param name="_Name">이름 _Name인 Animation을 등록한다.</param>
/// <param name="_PlayTime">재생 시간</param>
/// <param name="_PlayRate">재생 속도</param>
/// <param name="_Loop">반복 여부</param>
/// <param name="_Reverse">반전 여부</param>
void CAnimation2DComponent::AddAnimation(const std::string& _Name, float _PlayTime, float _PlayRate, bool _Loop, bool _Reverse)
{
	// 월드에셋매니저에서 Asset 가져오기
	auto	World = m_World.lock();

	std::weak_ptr<CAnimation2D>	Anim;

	auto	AnimMgr = World->GetWorldAssetManager().lock();

	Anim = AnimMgr->FindAnimation(_Name);

	if (Anim.expired())
		return;

	// 가져온 에셋을 저장하기
	auto	_Anim = Anim.lock();

	auto	iter = m_AnimationSequenceMap.find(_Anim->GetName());

	if (iter != m_AnimationSequenceMap.end())
		return;

	std::shared_ptr<CAnimation2DSequence>	Sequence(new CAnimation2DSequence);

	Sequence->SetAnimation2D(Anim);
	Sequence->SetPlayTime(_PlayTime);
	Sequence->SetPlayRate(_PlayRate);
	Sequence->SetLoop(_Loop);
	Sequence->SetReverse(_Reverse);

	m_AnimationSequenceMap.insert(std::make_pair(_Anim->GetName(), Sequence));

	if (!m_CurrentAnimationSequence)
	{
		m_CurrentAnimationSequence = Sequence;

		auto	MeshComponent = m_UpdateComponent.lock();

		if (MeshComponent && m_CurrentAnimationSequence)
		{
			auto	Asset = m_CurrentAnimationSequence->GetAnimation().lock();

			if (Asset)
			{
				m_AnimCBuffer->SetAnimation2DTextureType(Asset->GetAnimationTextureType());
				if (MeshComponent->SetTexture(0, 0,
					Asset->GetTexture()))
					m_UpdateEnable = true;
			}
		}
	}
}

void CAnimation2DComponent::SetPlayTime(const std::string& _Name, float _PlayTime)
{
	std::string Key = "Animation2D_" + _Name;

	auto	iter = m_AnimationSequenceMap.find(Key);

	if (iter == m_AnimationSequenceMap.end())
		return;

	iter->second->SetPlayTime(_PlayTime);
}

void CAnimation2DComponent::SetPlayRate(const std::string& _Name, float _PlayRate)
{
	std::string Key = "Animation2D_" + _Name;

	auto	iter = m_AnimationSequenceMap.find(Key);

	if (iter == m_AnimationSequenceMap.end())
		return;

	iter->second->SetPlayRate(_PlayRate);
}

void CAnimation2DComponent::SetSymmetry(const std::string& _Name, bool _Symmetry)
{
	std::string Key = "Animation2D_" + _Name;

	auto	iter = m_AnimationSequenceMap.find(Key);

	if (iter == m_AnimationSequenceMap.end())
		return;

	iter->second->SetAnimationSymmetry(_Symmetry);
}

void CAnimation2DComponent::SetLoop(const std::string& _Name, bool _Loop)
{
	std::string Key = "Animation2D_" + _Name;

	auto	iter = m_AnimationSequenceMap.find(Key);

	if (iter == m_AnimationSequenceMap.end())
		return;

	iter->second->SetLoop(_Loop);
}

void CAnimation2DComponent::SetReverse(const std::string& _Name, bool _Reverse)
{
	std::string Key = "Animation2D_" + _Name;

	auto	iter = m_AnimationSequenceMap.find(Key);

	if (iter == m_AnimationSequenceMap.end())
		return;

	iter->second->SetReverse(_Reverse);
}

/// <summary>
/// 등록된 애니메이션 시퀀스 중 _Name 애니메이션시퀀스를 재생한다.
/// </summary>
/// <param name="_Name"></param>
void CAnimation2DComponent::ChangeAnimation(const std::string& _Name)
{
	std::string Key = "Animation2D_" + _Name;

	if (m_UpdateComponent.expired())
		return;

	else if (!m_CurrentAnimationSequence)
		return;

	else if (m_CurrentAnimationSequence->GetName() == Key)
		return;

	auto iter = m_AnimationSequenceMap.find(Key);

	if (iter == m_AnimationSequenceMap.end())
		return;

	// 애니메이션 정보를 초기화한다.
	m_CurrentAnimationSequence->Clear();

	// 현재 애니메이션을 교체한다.
	m_CurrentAnimationSequence = iter->second;

	m_CurrentAnimationSequence->Clear();

	// 등록된 MeshComponent가 가지고 있는 Material의 Texture를 애니메이션이 가지고 있는 Texture로 교체한다.
	// 메쉬컴포넌트를 가져오고
	auto MeshComponent = m_UpdateComponent.lock();

	if (MeshComponent)
	{
		// 현재 애니메이션 시퀀스의 시작 Frame으로 변경한다.
		auto Anim = m_CurrentAnimationSequence->GetAnimation().lock();

		if (Anim)
		{
			m_AnimCBuffer->SetAnimation2DTextureType(Anim->GetAnimationTextureType());
			MeshComponent->SetTexture(0, 0, Anim->GetTexture());
		}
	}
}

void CAnimation2DComponent::SetShader()
{
	int Frame = m_CurrentAnimationSequence->GetFrame();

	m_AnimCBuffer->SetTextureSymmetry(m_CurrentAnimationSequence->GetSymmetry());

	auto Anim = m_CurrentAnimationSequence->GetAnimation().lock();

	if (Anim)
	{
		if (Anim->GetAnimationTextureType() == EAnimation2DTextureType::SpriteSheet)
		{
			const FTextureFrame& TexFrame = Anim->GetFrame(Frame);

			auto	Texture = Anim->GetTexture().lock();

			if (Texture)
			{
				const FTextureInfo* TexInfo =
					Texture->GetTexture();

				m_AnimCBuffer->SetLTUV(
					TexFrame.Start.x / TexInfo->Width,
					TexFrame.Start.y / TexInfo->Height);
				m_AnimCBuffer->SetRBUV(
					(TexFrame.Start.x + TexFrame.Size.x) / TexInfo->Width,
					(TexFrame.Start.y + TexFrame.Size.y) / TexInfo->Height);
			}
		}
	}

	m_AnimCBuffer->UpdateBuffer();
}


EAnimation2DTextureType CAnimation2DComponent::GetTextureType() const
{
	auto Anim = m_CurrentAnimationSequence->GetAnimation().lock();

	return Anim->GetAnimationTextureType();;
}

int CAnimation2DComponent::GetAnimationFrame() const
{
	return m_CurrentAnimationSequence->GetFrame();
}
