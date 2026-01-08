#include "pch.h"
#include "SceneComponent.h"
#include "MeshComponent.h"
#include "Render/RenderManager.h"

CSceneComponent::CSceneComponent() :
	m_InheritRot(true),
	m_InheritScale(true),
	m_RenderType(EComponentRender::None),
	m_RelativePos(),
	m_RelativeScale(FVector3::One),
	m_RelativeRot(),
	m_WorldPos(),
	m_WorldScale(FVector3::One),
	m_WorldRot(),
	m_WorldAxis{ FVector3::Axis[EAxis::X], FVector3::Axis[EAxis::Y], FVector3::Axis[EAxis::Z], }
{
	SetClassType<CSceneComponent>();
	m_ComponentType = EComponentType::Scene;
}

CSceneComponent::CSceneComponent(const CSceneComponent& _Ref) :
	CComponent(_Ref),
	m_InheritRot(_Ref.m_InheritRot),
	m_InheritScale(_Ref.m_InheritScale),
	m_RenderType(_Ref.m_RenderType),
	m_RelativeScale(_Ref.m_RelativeScale),
	m_RelativeRot(_Ref.m_RelativeRot),
	m_RelativePos(_Ref.m_RelativePos),

	m_WorldScale(),
	m_WorldRot(),
	m_WorldPos(),
	m_WorldAxis{_Ref.m_WorldAxis[0],_Ref.m_WorldAxis[1],_Ref.m_WorldAxis[2] },

	m_ScaleMatrix(),
	m_RotMatrix(),
	m_TranslateMatrix(),
	m_WorldMatrix()
{
	m_ComponentType = EComponentType::Scene;

	// SceneComponent를 복제하여 가지고 있게 한다.
	size_t Size = _Ref.m_ChildList.size();

	for (int i = 0; i < Size; ++i)
	{
		// 컴포넌트를 복제하여 넣어주기
		CSceneComponent* Component = _Ref.m_ChildList[i]->Clone();

		std::shared_ptr<CSceneComponent> Com(Component);

		Com->m_Parent = std::dynamic_pointer_cast<CSceneComponent>(m_Self.lock());
		Com->m_Owner = m_Owner;
		Com->m_World = _Ref.m_World;
		m_ChildList.push_back(Com);
	}
}

CSceneComponent::CSceneComponent(CSceneComponent&& _Ref) noexcept :
	CComponent(_Ref),
	m_InheritRot(_Ref.m_InheritRot),
	m_InheritScale(_Ref.m_InheritScale),
	m_RenderType(_Ref.m_RenderType),
	m_RelativeScale(_Ref.m_RelativeScale),
	m_RelativeRot(_Ref.m_RelativeRot),
	m_RelativePos(_Ref.m_RelativePos),

	m_WorldScale(),
	m_WorldRot(),
	m_WorldPos(),
	m_WorldAxis{ _Ref.m_WorldAxis[0],_Ref.m_WorldAxis[1],_Ref.m_WorldAxis[2] },

	m_ScaleMatrix(),
	m_RotMatrix(),
	m_TranslateMatrix(),
	m_WorldMatrix()
{
	m_ComponentType = EComponentType::Scene;

	m_ChildList = std::move(_Ref.m_ChildList);
	_Ref.m_ChildList.clear();

	size_t Size = m_ChildList.size();

	for (int i = 0; i < Size; ++i)
	{
		m_ChildList[i]->m_Parent = std::dynamic_pointer_cast<CSceneComponent>(m_Self.lock());
		m_ChildList[i]->m_Owner = m_Owner;
		// world는 이동
	}

	_Ref.m_Self.reset();
	_Ref.m_Parent.reset();
}

CSceneComponent::~CSceneComponent()
{
}

void CSceneComponent::SetSelf(std::weak_ptr<CComponent> _Self)
{
	m_Self = _Self;

	size_t Size = m_ChildList.size();
	for (size_t i = 0; i < Size; ++i)
	{
		m_ChildList[i]->SetSelf(m_ChildList[i]);
	}
}

void CSceneComponent::SetWorld(std::weak_ptr<CWorld> _World)
{
	m_World = _World;

	size_t Size = m_ChildList.size();
	for (size_t i = 0; i < Size; ++i)
	{
		m_ChildList[i]->SetWorld(_World);
	}
}

void CSceneComponent::SetOwner(std::weak_ptr<CGameObject> _Owner)
{
	m_Owner = _Owner;
	
	size_t Size = m_ChildList.size();
	for (size_t i = 0; i < Size; ++i)
	{
		m_ChildList[i]->SetOwner(_Owner);
	}
}

bool CSceneComponent::Init()
{
	// 자신을 RenderManager에게 등록한다.
	if (m_RenderType == EComponentRender::Render)
	{
		auto Self = std::dynamic_pointer_cast<CSceneComponent>(m_Self.lock());

		CRenderManager::GetInst()->AddRenderLayer(Self);
	}

	return true;
}

void CSceneComponent::Begin()
{
	size_t	Size = m_ChildList.size();

	InheritScale();

	InheritRotation();

	InheritPos();

	for (size_t i = 0; i < Size; ++i)
	{
		auto Child = m_ChildList[i].get();

		if (Child)
			Child->Begin();
	}
}

void CSceneComponent::Update(double DeltaTime)
{

	size_t	Size = m_ChildList.size();

	for (size_t i = 0; i < Size; ++i)
	{
		auto Child = m_ChildList[i].get();

		if (Child)
			Child->Update(DeltaTime);
	}
}

void CSceneComponent::PostUpdate(double _DeltaTime)
{
	UpdateTransform();

	size_t	Size = m_ChildList.size();

	for (size_t i = 0; i < Size; ++i)
	{
		auto Child = m_ChildList[i].get();

		if (Child)
			Child->PostUpdate(_DeltaTime);
	}
}

void CSceneComponent::Render()
{
	/*
	size_t	Size = m_ChildList.size();

	for (size_t i = 0; i < Size; ++i)
	{
		auto Child = m_ChildList[i].get();

		if (Child)
			Child->Render();
	}
	*/
}

CSceneComponent* CSceneComponent::Clone()	const
{
	return new CSceneComponent(*this);
}

void CSceneComponent::Destroy()
{
	m_Alive = false;
}

void CSceneComponent::AddChild(std::weak_ptr<CSceneComponent> _Child)
{
	auto Child = _Child.lock();

	if (Child)
	{
		Child->m_Parent = std::dynamic_pointer_cast<CSceneComponent>(m_Self.lock());
		m_ChildList.push_back(Child);

		//Child->SetRelativeScale(m_RelativeScale);
		//Child->SetRelativeRotation(m_RelativeRot);
		//Child->SetRelativePos(m_RelativePos);
	}
}

void CSceneComponent::UpdateTransform()
{
	m_ScaleMatrix.Scaling(m_WorldScale);
	m_RotMatrix.Rotation(m_WorldRot);
	m_TranslateMatrix.Translation(m_WorldPos);

	m_WorldMatrix = m_ScaleMatrix * m_RotMatrix * m_TranslateMatrix;
}

void CSceneComponent::SetInheritScale(bool Scale)
{
	m_InheritScale = Scale;
}

void CSceneComponent::SetInheritRot(bool Rot)
{
	m_InheritRot = Rot;
}

void CSceneComponent::SetRelativeScale(const FVector3& Scale)
{
	m_RelativeScale = Scale;

	InheritScale();
}

void CSceneComponent::SetRelativeScale(const FVector2& Scale)
{
	SetRelativeScale(FVector3(Scale.x, Scale.y, m_RelativeScale.z));
}

void CSceneComponent::SetRelativeScale(float x, float y, float z)
{
	SetRelativeScale(FVector3(x, y, z));
}

void CSceneComponent::SetRelativeScale(float x, float y)
{
	SetRelativeScale(FVector3(x, y, m_RelativeScale.z));
}

void CSceneComponent::AddRelativeScale(const FVector3& Scale)
{
	SetRelativeScale(Scale + m_RelativeScale);
}

void CSceneComponent::AddRelativeScale(const FVector2& Scale)
{
	AddRelativeScale(FVector3(Scale.x, Scale.y, 0.f));
}

void CSceneComponent::AddRelativeScale(float x, float y, float z)
{
	AddRelativeScale(FVector3(x, y, z));
}

void CSceneComponent::AddRelativeScale(float x, float y)
{
	AddRelativeScale(FVector3(x, y, 0.f));
}

void CSceneComponent::SetRelativeRotation(const FVector3& Rot)
{
	m_RelativeRot = Rot;

	InheritRotation();
}

void CSceneComponent::SetRelativeRotation(const FVector2& Rot)
{
	SetRelativeRotation(FVector3(Rot.x, Rot.y, m_RelativeRot.z));
}

void CSceneComponent::SetRelativeRotation(float x, float y, float z)
{
	SetRelativeRotation(FVector3(x, y, z));
}

void CSceneComponent::SetRelativeRotation(float x, float y)
{
	SetRelativeRotation(FVector3(x, y, m_RelativeRot.z));
}

void CSceneComponent::SetRelativeRotationX(float x)
{
	SetRelativeRotation(FVector3(x, m_RelativeRot.y, m_RelativeRot.z));
}

void CSceneComponent::SetRelativeRotationY(float y)
{
	SetRelativeRotation(FVector3(m_RelativeRot.x, y, m_RelativeRot.z));
}

void CSceneComponent::SetRelativeRotationZ(float z)
{
	SetRelativeRotation(FVector3(m_RelativeRot.x, m_RelativeRot.y, z));
}

void CSceneComponent::AddRelativeRotation(const FVector3& Rot)
{
	SetRelativeRotation(Rot + m_RelativeRot);
}

void CSceneComponent::AddRelativeRotation(const FVector2& Rot)
{
	AddRelativeRotation(FVector3(Rot.x, Rot.y, 0.f));
}

void CSceneComponent::AddRelativeRotation(float x, float y, float z)
{
	AddRelativeRotation(FVector3(x, y, z));
}

void CSceneComponent::AddRelativeRotation(float x, float y)
{
	AddRelativeRotation(FVector3(x, y, 0.f));
}

void CSceneComponent::AddRelativeRotationX(float x)
{
	AddRelativeRotation(FVector3(x, 0.f, 0.f));
}

void CSceneComponent::AddRelativeRotationY(float y)
{
	AddRelativeRotation(FVector3(0.f, y, 0.f));
}

void CSceneComponent::AddRelativeRotationZ(float z)
{
	AddRelativeRotation(FVector3(0.f, 0.f, z));
}

void CSceneComponent::SetRelativePos(const FVector3& Pos)
{
	m_RelativePos = Pos;

	InheritPos();
}

void CSceneComponent::SetRelativePos(const FVector2& Pos)
{
	SetRelativePos(FVector3(Pos.x, Pos.y, m_RelativePos.z));
}

void CSceneComponent::SetRelativePos(float x, float y, float z)
{
	SetRelativePos(FVector3(x, y, z));
}

void CSceneComponent::SetRelativePos(float x, float y)
{
	SetRelativePos(FVector3(x, y, m_RelativePos.z));
}

void CSceneComponent::AddRelativePos(const FVector3& Pos)
{
	SetRelativePos(Pos + m_RelativePos);
}

void CSceneComponent::AddRelativePos(const FVector2& Pos)
{
	AddRelativePos(FVector3(Pos.x, Pos.y, 0.f));
}

void CSceneComponent::AddRelativePos(float x, float y, float z)
{
	AddRelativePos(FVector3(x, y, z));
}

void CSceneComponent::AddRelativePos(float x, float y)
{
	AddRelativePos(FVector3(x, y, 0.f));
}

void CSceneComponent::InheritScale()
{
	if (m_InheritScale)
	{
		auto	Parent = m_Parent.lock();

		if (Parent)
		{
			m_WorldScale = m_RelativeScale * Parent->m_WorldScale;
		}

		else
		{
			m_WorldScale = m_RelativeScale;
		}
	}

	else
	{
		m_WorldScale = m_RelativeScale;
	}

	// 자식들의 WorldScale 크기를 조정해준다.
	size_t	Size = m_ChildList.size();

	for (size_t i = 0; i < Size; ++i)
	{
		auto	Child = m_ChildList[i].get();

		if (Child)
			Child->InheritScale();
	}

	for (size_t i = 0; i < Size; ++i)
	{
		auto	Child = m_ChildList[i].get();
		if (Child)
			Child->InheritPos();
	}
}

void CSceneComponent::InheritRotation()
{
	if (m_InheritRot)
	{
		auto	Parent = m_Parent.lock();

		if (Parent)
		{
			m_WorldRot = m_RelativeRot + Parent->m_WorldRot;
		}

		else
		{
			m_WorldRot = m_RelativeRot;
		}
	}

	else
	{
		m_WorldRot = m_RelativeRot;
	}

	// 회전값을 구해준다.
	// 3개의 축을 구해진 회전값으로 회전시켜준다.
	FMatrix	RotationMatrix;

	RotationMatrix.Rotation(m_WorldRot);

	for (int i = 0; i < EAxis::End; ++i)
	{
		m_WorldAxis[i] = FVector3::Axis[i].TransformNormal(RotationMatrix);

		m_WorldAxis[i].Normalize();
	}

	// 자식들의 회전값도 조정해준다.
	size_t	Size = m_ChildList.size();

	for (size_t i = 0; i < Size; ++i)
	{
		auto	Child = m_ChildList[i].get();

		if (Child)
			Child->InheritRotation();
	}

	for (size_t i = 0; i < Size; ++i)
	{
		auto	Child = m_ChildList[i].get();

		if (Child)
			Child->InheritPos();
	}
}

void CSceneComponent::InheritPos()
{
	auto	Parent = m_Parent.lock();

	if (Parent)
	{
		FMatrix	ParentMatrix;

		// 부모의 회전을 우선 받는다.
		if (m_InheritScale)
		{
			FMatrix ParentMScale;
			FVector3 ParentScale = Parent->GetWorldScale();
			ParentMScale.Scaling(ParentScale);

			ParentMatrix = ParentMatrix * ParentMScale;
		}

		// 부모의 회전을 우선 받는다.
		if (m_InheritRot)
		{
			FMatrix ParentMRot;
			FVector3	ParentRot = Parent->GetWorldRot();
			ParentMRot.Rotation(ParentRot);
			ParentMatrix = ParentMatrix * ParentMRot;
		}

		// 부모의 위치를 받는다.
		FVector3	ParentPos = Parent->GetWorldPos();

		// 부모의 회전 + 위치를 저장한 행렬을 만든다.
		memcpy(&ParentMatrix._41, &ParentPos, sizeof(FVector3));

		// 3, 0, 0 떨어져 있을 경우
		// 부모 위치가 10, 10, 10 일경우
		/*
		3, 0, 0, 1 * 1  0  0  0 = 13, 10, 10
					 0  1  0  0
					 0  0  1  0
					 10 10 10 1
		*/


		// 부모의 행렬과 자식의 위치를 곱하여 WorldPos를 구한다.
		m_WorldPos = m_RelativePos.TransformCoord(ParentMatrix);
	}

	else
	{
		m_WorldPos = m_RelativePos;
	}

	size_t	Size = m_ChildList.size();

	for (size_t i = 0; i < Size; ++i)
	{
		auto	Child = m_ChildList[i].get();

		if (Child)
			Child->InheritPos();
	}
}

/*
void CSceneComponent::SetWorldScale(const FVector3& Scale)
{
	m_WorldScale = Scale;

	InheritWorldScale();
}

void CSceneComponent::SetWorldScale(const FVector2& Scale)
{
	SetWorldScale(FVector3(Scale.x, Scale.y, m_WorldScale.z));
}

void CSceneComponent::SetWorldScale(float x, float y, float z)
{
	SetWorldScale(FVector3(x, y, z));
}

void CSceneComponent::SetWorldScale(float x, float y)
{
	SetWorldScale(FVector3(x, y, m_WorldScale.z));
}

void CSceneComponent::AddWorldScale(const FVector3& Scale)
{
	SetWorldScale(Scale + m_WorldScale);
}

void CSceneComponent::AddWorldScale(const FVector2& Scale)
{
	AddWorldScale(FVector3(Scale.x, Scale.y, 0.f));
}

void CSceneComponent::AddWorldScale(float x, float y, float z)
{
	AddWorldScale(FVector3(x, y, z));
}

void CSceneComponent::AddWorldScale(float x, float y)
{
	AddWorldScale(FVector3(x, y, 0.f));
}

void CSceneComponent::SetWorldRotation(const FVector3& Rot)
{
	m_WorldRot = Rot;

	InheritWorldRotation();
}

void CSceneComponent::SetWorldRotation(const FVector2& Rot)
{
	SetWorldRotation(FVector3(Rot.x, Rot.y, m_WorldRot.z));
}

void CSceneComponent::SetWorldRotation(float x, float y, float z)
{
	SetWorldRotation(FVector3(x, y, z));
}

void CSceneComponent::SetWorldRotation(float x, float y)
{
	SetWorldRotation(FVector3(x, y, m_WorldRot.z));
}

void CSceneComponent::SetWorldRotationX(float x)
{
	SetWorldRotation(FVector3(x, m_WorldRot.y, m_WorldRot.z));
}

void CSceneComponent::SetWorldRotationY(float y)
{
	SetWorldRotation(FVector3(m_WorldRot.x, y, m_WorldRot.z));
}

void CSceneComponent::SetWorldRotationZ(float z)
{
	SetWorldRotation(FVector3(m_WorldRot.x, m_WorldRot.y, z));
}

void CSceneComponent::AddWorldRotation(const FVector3& Rot)
{
	SetWorldRotation(Rot + m_WorldRot);
}

void CSceneComponent::AddWorldRotation(const FVector2& Rot)
{
	AddWorldRotation(FVector3(Rot.x, Rot.y, 0.f));
}

void CSceneComponent::AddWorldRotation(float x, float y, float z)
{
	AddWorldRotation(FVector3(x, y, z));
}

void CSceneComponent::AddWorldRotation(float x, float y)
{
	AddWorldRotation(FVector3(x, y, 0.f));
}

void CSceneComponent::AddWorldRotationX(float x)
{
	AddWorldRotation(FVector3(x, 0.f, 0.f));
}

void CSceneComponent::AddWorldRotationY(float y)
{
	AddWorldRotation(FVector3(0.f, y, 0.f));
}

void CSceneComponent::AddWorldRotationZ(float z)
{
	AddWorldRotation(FVector3(0.f, 0.f, z));
}

void CSceneComponent::SetWorldPos(const FVector3& Pos)
{
	m_WorldPos = Pos;

	InheritWorldPos();
}

void CSceneComponent::SetWorldPos(const FVector2& Pos)
{
	SetWorldPos(FVector3(Pos.x, Pos.y, m_WorldPos.z));
}

void CSceneComponent::SetWorldPos(float x, float y, float z)
{
	SetWorldPos(FVector3(x, y, z));
}

void CSceneComponent::SetWorldPos(float x, float y)
{
	SetWorldPos(FVector3(x, y, m_WorldPos.z));
}

void CSceneComponent::AddWorldPos(const FVector3& Pos)
{
	SetWorldPos(Pos + m_WorldPos);
}

void CSceneComponent::AddWorldPos(const FVector2& Pos)
{
	AddWorldPos(FVector3(Pos.x, Pos.y, 0.f));
}

void CSceneComponent::AddWorldPos(float x, float y, float z)
{
	AddWorldPos(FVector3(x, y, z));
}

void CSceneComponent::AddWorldPos(float x, float y)
{
	AddWorldPos(FVector3(x, y, 0.f));
}

void CSceneComponent::InheritWorldScale()
{
	if (m_InheritScale)
	{
		auto	Parent = m_Parent.lock();

		if (Parent)
		{
			m_RelativeScale = m_WorldScale / Parent->m_WorldScale;
		}

		else
		{
			m_RelativeScale = m_WorldScale;
		}
	}

	else
	{
		m_RelativeScale = m_WorldScale;
	}

	size_t	Size = m_ChildList.size();

	for (size_t i = 0; i < Size; ++i)
	{
		auto Child = m_ChildList[i].get();

		if (Child)
			Child->InheritWorldScale();
	}
}

void CSceneComponent::InheritWorldRotation()
{
	if (m_InheritRot)
	{
		auto	Parent = m_Parent.lock();

		if (Parent)
		{
			m_RelativeRot = m_WorldRot - Parent->m_WorldRot;
		}

		else
		{
			m_RelativeRot = m_WorldRot;
		}
	}

	else
	{
		m_RelativeRot = m_WorldRot;
	}

	// 3개의 축을 구해진 회전값으로 회전시켜준다.
	FMatrix	RotationMatrix;

	RotationMatrix.Rotation(m_WorldRot);

	for (int i = 0; i < EAxis::End; ++i)
	{
		m_WorldAxis[i] = FVector3::Axis[i].TransformNormal(RotationMatrix);

		m_WorldAxis[i].Normalize();
	}

	size_t	Size = m_ChildList.size();

	for (size_t i = 0; i < Size; ++i)
	{
		auto Child = m_ChildList[i].get();

		if (Child)
			Child->InheritWorldRotation();
	}

	for (size_t i = 0; i < Size; ++i)
	{
		auto	Child = m_ChildList[i].get();

		if (Child)
			Child->InheritWorldPos();
	}
}

void CSceneComponent::InheritWorldPos()
{
	auto Parent = m_Parent.lock();

	if (Parent)
	{
		FMatrix	ParentMatrix;

		if (m_InheritRot)
		{
			FVector3	ParentRot = Parent->GetWorldRot();
			ParentMatrix.Rotation(ParentRot);
		}

		FVector3	ParentPos = Parent->GetWorldPos();
		memcpy(&ParentMatrix._41, &ParentPos, sizeof(FVector3));

		ParentMatrix.Inverse();

		m_RelativePos = m_WorldPos.TransformCoord(ParentMatrix);
	}

	else
	{
		m_RelativePos = m_WorldPos;
	}

	size_t	Size = m_ChildList.size();

	for (size_t i = 0; i < Size; ++i)
	{
		auto Child = m_ChildList[i].get();

		if (Child)
			Child->InheritWorldPos();
	}
}
*/