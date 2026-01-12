#include "pch.h"
#include "GameObject.h"

CGameObject::CGameObject() :
	m_Enable(true),
	m_Alive(true)
{
	SetClassType<CGameObject>();
}

CGameObject::CGameObject(const CGameObject& _Ref):
	CObject(_Ref),
	m_Enable(_Ref.m_Enable),
	m_Alive(_Ref.m_Alive),
	m_Name(_Ref.m_Name),
	m_World()
{

	//CSceneComponent* SComponent = _Ref.m_SceneComponent->Clone();
	//m_SceneComponent.reset(SComponent);
}

CGameObject::CGameObject(CGameObject&& _Ref) noexcept: 
	CObject(_Ref),
	m_Enable(_Ref.m_Enable),
	m_Alive(_Ref.m_Alive),
	m_Name(_Ref.m_Name),
	m_World()
{
	//m_SceneComponent = std::move(_Ref.m_SceneComponent);
	//_Ref.m_SceneComponent.reset();

	//self와 World는 World의 CreateCloneGameObject에서 처리한다.
}

CGameObject::~CGameObject()
{
}

void CGameObject::SetEnble(bool _Enable)
{
	m_Enable = _Enable;

	// GameObject가 비활성되었다가 자식 컴포넌트에게 알려준다.
	if (m_SceneComponent.get())
		m_SceneComponent->GameObjectEnable(_Enable);

	auto	iter = m_ObjectComponentList.begin();
	auto	iterEnd = m_ObjectComponentList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->GameObjectEnable(_Enable);
	}
}

void CGameObject::SetSelf(std::weak_ptr<CGameObject> _Self)
{
	m_Self = _Self;

	if (m_SceneComponent.get())
		m_SceneComponent->SetOwner(m_Self);
}

void CGameObject::SetWorld(std::weak_ptr<CWorld> _World)
{
	m_World = _World;

	if(m_SceneComponent.get())
		m_SceneComponent->SetWorld(m_World);
}

void CGameObject::SetName(const std::string& _Name)
{
	m_Name = _Name;
}

bool CGameObject::Init()
{
	return true;
}

void CGameObject::Begin()       // Begin 함수는 오브젝트가 월드에 배치되고 처음 한번만 호출되는 함수이다.
{
	m_SceneComponent->Begin();

	auto	iter = m_ObjectComponentList.begin();
	auto	iterEnd = m_ObjectComponentList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Begin();
	}
}

void CGameObject::Update(double _DeltaTime)
{
	auto Root = m_SceneComponent.get();

	if (Root)
		Root->Update(_DeltaTime);

	auto iter = m_ObjectComponentList.begin();
	auto iterEnd = m_ObjectComponentList.end();

	for (; iter != iterEnd;)
	{
		if ((*iter).use_count() == 0)
		{
			iter = m_ObjectComponentList.erase(iter);
			iterEnd = m_ObjectComponentList.end();
			continue;
		}
		(*iter)->Update(_DeltaTime); 
		++iter;
	}


}

void CGameObject::PostUpdate(double _DeltaTime)
{
	auto Root = m_SceneComponent.get();

	if (Root)
		Root->PostUpdate(_DeltaTime);

	auto iter = m_ObjectComponentList.begin();
	auto iterEnd = m_ObjectComponentList.end();

	for (; iter != iterEnd;)
	{
		if ((*iter).use_count() == 0)
		{
			iter = m_ObjectComponentList.erase(iter);
			iterEnd = m_ObjectComponentList.end();
			continue;
		}
		(*iter)->PostUpdate(_DeltaTime);
		++iter;
	}

}

void CGameObject::Render()
{
	auto Root = m_SceneComponent.get();

	if (Root)
		Root->Render();
}

CGameObject* CGameObject::Clone()
{
	return new CGameObject(*this);
}

void CGameObject::Destroy()
{
	m_Alive = false;
}

const FVector3& CGameObject::GetAxis(EAxis::Type Axis)	const
{
	auto	Root = m_SceneComponent.get();

	if (Root)
		return Root->GetAxis(Axis);

	return FVector3::Zero;
}

const FVector3& CGameObject::GetRelativeScale()	const
{
	auto	Root = m_SceneComponent.get();

	if (Root)
		return Root->GetRelativeScale();

	return FVector3::Zero;
}

const FVector3& CGameObject::GetRelativeRot()	const
{
	auto	Root = m_SceneComponent.get();

	if (Root)
		return Root->GetRelativeRot();

	return FVector3::Zero;
}

const FVector3& CGameObject::GetRelativePos()	const
{
	auto	Root = m_SceneComponent.get();

	if (Root)
		return Root->GetRelativePos();

	return FVector3::Zero;
}

const FVector3& CGameObject::GetWorldScale()	const
{
	auto	Root = m_SceneComponent.get();

	if (Root)
		return Root->GetWorldScale();

	return FVector3::Zero;
}

const FVector3& CGameObject::GetWorldRot()	const
{
	auto	Root = m_SceneComponent.get();

	if (Root)
		return Root->GetWorldRot();

	return FVector3::Zero;
}

const FVector3& CGameObject::GetWorldPos()	const
{
	auto	Root = m_SceneComponent.get();

	if (Root)
		return Root->GetWorldPos();

	return FVector3::Zero;
}

void CGameObject::SetInheritScale(bool Scale)
{
	auto	Root = m_SceneComponent.get();

	if (Root)
		Root->SetInheritScale(Scale);
}

void CGameObject::SetInheritRot(bool Rot)
{
	auto	Root = m_SceneComponent.get();

	if (Root)
		Root->SetInheritRot(Rot);
}

void CGameObject::SetRelativeScale(const FVector3& Scale)
{
	auto	Root = m_SceneComponent.get();

	if (Root)
		Root->SetRelativeScale(Scale);
}

void CGameObject::SetRelativeScale(const FVector2& Scale)
{
	auto	Root = m_SceneComponent.get();

	if (Root)
		Root->SetRelativeScale(Scale);
}

void CGameObject::SetRelativeScale(float x, float y, float z)
{
	auto	Root = m_SceneComponent.get();

	if (Root)
		Root->SetRelativeScale(x, y, z);
}

void CGameObject::SetRelativeScale(float x, float y)
{
	auto	Root = m_SceneComponent.get();

	if (Root)
		Root->SetRelativeScale(x, y);
}

void CGameObject::AddRelativeScale(const FVector3& Scale)
{
	auto	Root = m_SceneComponent.get();

	if (Root)
		Root->AddRelativeScale(Scale);
}

void CGameObject::AddRelativeScale(const FVector2& Scale)
{
	auto	Root = m_SceneComponent.get();

	if (Root)
		Root->AddRelativeScale(Scale);
}

void CGameObject::AddRelativeScale(float x, float y, float z)
{
	auto	Root = m_SceneComponent.get();

	if (Root)
		Root->AddRelativeScale(x, y, z);
}

void CGameObject::AddRelativeScale(float x, float y)
{
	auto	Root = m_SceneComponent.get();

	if (Root)
		Root->AddRelativeScale(x, y);
}

void CGameObject::SetRelativeRotation(const FVector3& Rot)
{
	auto	Root = m_SceneComponent.get();

	if (Root)
		Root->SetRelativeRotation(Rot);
}

void CGameObject::SetRelativeRotation(const FVector2& Rot)
{
	auto	Root = m_SceneComponent.get();

	if (Root)
		Root->SetRelativeRotation(Rot);
}

void CGameObject::SetRelativeRotation(float x, float y, float z)
{
	auto	Root = m_SceneComponent.get();

	if (Root)
		Root->SetRelativeRotation(x, y, z);
}

void CGameObject::SetRelativeRotation(float x, float y)
{
	auto	Root = m_SceneComponent.get();

	if (Root)
		Root->SetRelativeRotation(x, y);
}

void CGameObject::SetRelativeRotationX(float x)
{
	auto	Root = m_SceneComponent.get();

	if (Root)
		Root->SetRelativeRotationX(x);
}

void CGameObject::SetRelativeRotationY(float y)
{
	auto	Root = m_SceneComponent.get();

	if (Root)
		Root->SetRelativeRotationY(y);
}

void CGameObject::SetRelativeRotationZ(float z)
{
	auto	Root = m_SceneComponent.get();

	if (Root)
		Root->SetRelativeRotationZ(z);
}

void CGameObject::AddRelativeRotation(const FVector3& Rot)
{
	auto	Root = m_SceneComponent.get();

	if (Root)
		Root->AddRelativeRotation(Rot);
}

void CGameObject::AddRelativeRotation(const FVector2& Rot)
{
	auto	Root = m_SceneComponent.get();

	if (Root)
		Root->AddRelativeRotation(Rot);
}

void CGameObject::AddRelativeRotation(float x, float y, float z)
{
	auto	Root = m_SceneComponent.get();

	if (Root)
		Root->AddRelativeRotation(x, y, z);
}

void CGameObject::AddRelativeRotation(float x, float y)
{
	auto	Root = m_SceneComponent.get();

	if (Root)
		Root->AddRelativeRotation(x, y);
}

void CGameObject::AddRelativeRotationX(float x)
{
	auto	Root = m_SceneComponent.get();

	if (Root)
		Root->AddRelativeRotationX(x);
}

void CGameObject::AddRelativeRotationY(float y)
{
	auto	Root = m_SceneComponent.get();

	if (Root)
		Root->AddRelativeRotationY(y);
}

void CGameObject::AddRelativeRotationZ(float z)
{
	auto	Root = m_SceneComponent.get();

	if (Root)
		Root->AddRelativeRotationZ(z);
}

void CGameObject::SetRelativePos(const FVector3& Pos)
{
	auto	Root = m_SceneComponent.get();

	if (Root)
		Root->SetRelativePos(Pos);
}

void CGameObject::SetRelativePos(const FVector2& Pos)
{
	auto	Root = m_SceneComponent.get();

	if (Root)
		Root->SetRelativePos(Pos);
}

void CGameObject::SetRelativePos(float x, float y, float z)
{
	auto	Root = m_SceneComponent.get();

	if (Root)
		Root->SetRelativePos(x, y, z);
}

void CGameObject::SetRelativePos(float x, float y)
{
	auto	Root = m_SceneComponent.get();

	if (Root)
		Root->SetRelativePos(x, y);
}

void CGameObject::AddRelativePos(const FVector3& Pos)
{
	auto	Root = m_SceneComponent.get();

	if (Root)
		Root->AddRelativePos(Pos);
}

void CGameObject::AddRelativePos(const FVector2& Pos)
{
	auto	Root = m_SceneComponent.get();

	if (Root)
		Root->AddRelativePos(Pos);
}

void CGameObject::AddRelativePos(float x, float y, float z)
{
	auto	Root = m_SceneComponent.get();

	if (Root)
		Root->AddRelativePos(x, y, z);
}

void CGameObject::AddRelativePos(float x, float y)
{
	auto	Root = m_SceneComponent.get();

	if (Root)
		Root->AddRelativePos(x, y);
}

/*
void CGameObject::SetWorldScale(const FVector3& Scale)
{
	auto	Root = m_Root.get();

	if (Root)
		Root->SetWorldScale(Scale);
}

void CGameObject::SetWorldScale(const FVector2& Scale)
{
	auto	Root = m_Root.get();

	if (Root)
		Root->SetWorldScale(Scale);
}

void CGameObject::SetWorldScale(float x, float y, float z)
{
	auto	Root = m_Root.get();

	if (Root)
		Root->SetWorldScale(x, y, z);
}

void CGameObject::SetWorldScale(float x, float y)
{
	auto	Root = m_Root.get();

	if (Root)
		Root->SetWorldScale(x, y);
}

void CGameObject::AddWorldScale(const FVector3& Scale)
{
	auto	Root = m_Root.get();

	if (Root)
		Root->AddWorldScale(Scale);
}

void CGameObject::AddWorldScale(const FVector2& Scale)
{
	auto	Root = m_Root.get();

	if (Root)
		Root->AddWorldScale(Scale);
}

void CGameObject::AddWorldScale(float x, float y, float z)
{
	auto	Root = m_Root.get();

	if (Root)
		Root->AddWorldScale(x, y, z);
}

void CGameObject::AddWorldScale(float x, float y)
{
	auto	Root = m_Root.get();

	if (Root)
		Root->AddWorldScale(x, y);
}

void CGameObject::SetWorldRotation(const FVector3& Rot)
{
	auto	Root = m_Root.get();

	if (Root)
		Root->SetWorldRotation(Rot);
}

void CGameObject::SetWorldRotation(const FVector2& Rot)
{
	auto	Root = m_Root.get();

	if (Root)
		Root->SetWorldRotation(Rot);
}

void CGameObject::SetWorldRotation(float x, float y, float z)
{
	auto	Root = m_Root.get();

	if (Root)
		Root->SetWorldRotation(x, y, z);
}

void CGameObject::SetWorldRotation(float x, float y)
{
	auto	Root = m_Root.get();

	if (Root)
		Root->SetWorldRotation(x, y);
}

void CGameObject::SetWorldRotationX(float x)
{
	auto	Root = m_Root.get();

	if (Root)
		Root->SetWorldRotationX(x);
}

void CGameObject::SetWorldRotationY(float y)
{
	auto	Root = m_Root.get();

	if (Root)
		Root->SetWorldRotationY(y);
}

void CGameObject::SetWorldRotationZ(float z)
{
	auto	Root = m_Root.get();

	if (Root)
		Root->SetWorldRotationZ(z);
}

void CGameObject::AddWorldRotation(const FVector3& Rot)
{
	auto	Root = m_Root.get();

	if (Root)
		Root->AddWorldRotation(Rot);
}

void CGameObject::AddWorldRotation(const FVector2& Rot)
{
	auto	Root = m_Root.get();

	if (Root)
		Root->AddWorldRotation(Rot);
}

void CGameObject::AddWorldRotation(float x, float y, float z)
{
	auto	Root = m_Root.get();

	if (Root)
		Root->AddWorldRotation(x, y, z);
}

void CGameObject::AddWorldRotation(float x, float y)
{
	auto	Root = m_Root.get();

	if (Root)
		Root->AddWorldRotation(x, y);
}

void CGameObject::AddWorldRotationX(float x)
{
	auto	Root = m_Root.get();

	if (Root)
		Root->AddWorldRotationX(x);
}

void CGameObject::AddWorldRotationY(float y)
{
	auto	Root = m_Root.get();

	if (Root)
		Root->AddWorldRotationY(y);
}

void CGameObject::AddWorldRotationZ(float z)
{
	auto	Root = m_Root.get();

	if (Root)
		Root->AddWorldRotationZ(z);
}

void CGameObject::SetWorldPos(const FVector3& Pos)
{
	auto	Root = m_Root.get();

	if (Root)
		Root->SetWorldPos(Pos);
}

void CGameObject::SetWorldPos(const FVector2& Pos)
{
	auto	Root = m_Root.get();

	if (Root)
		Root->SetWorldPos(Pos);
}

void CGameObject::SetWorldPos(float x, float y, float z)
{
	auto	Root = m_Root.get();

	if (Root)
		Root->SetWorldPos(x, y, z);
}

void CGameObject::SetWorldPos(float x, float y)
{
	auto	Root = m_Root.get();

	if (Root)
		Root->SetWorldPos(x, y);
}

void CGameObject::AddWorldPos(const FVector3& Pos)
{
	auto	Root = m_Root.get();

	if (Root)
		Root->AddWorldPos(Pos);
}

void CGameObject::AddWorldPos(const FVector2& Pos)
{
	auto	Root = m_Root.get();

	if (Root)
		Root->AddWorldPos(Pos);
}

void CGameObject::AddWorldPos(float x, float y, float z)
{
	auto	Root = m_Root.get();

	if (Root)
		Root->AddWorldPos(x, y, z);
}

void CGameObject::AddWorldPos(float x, float y)
{
	auto	Root = m_Root.get();

	if (Root)
		Root->AddWorldPos(x, y);
}

*/