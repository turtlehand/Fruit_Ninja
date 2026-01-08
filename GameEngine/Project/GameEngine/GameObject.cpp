#include "pch.h"
#include "GameObject.h"
#include "SceneComponent.h"

CGameObject::CGameObject()
{
}

CGameObject::CGameObject(const CGameObject& _Ref)
{
}

CGameObject::CGameObject(CGameObject&& _Ref)
{
}

CGameObject::~CGameObject()
{
}

void CGameObject::SetWorld(CWorld* _World)
{
}

bool SceneComponent::Init()
{
    return false;
}

void SceneComponent::Update(double DeletaTime)
{
}

void SceneComponent::Render()
{
}

SceneComponent* SceneComponent::Clone()
{
    return nullptr;
}

void SceneComponent::Destroy()
{
}

bool CGameObject::Init()
{

}

void CGameObject::Update(double DeletaTime) 
{

}
void CGameObject::Render()
{

}
CGameObject* CGameObject::Clone()
{
}

void CGameObject::Destroy()
{
}
