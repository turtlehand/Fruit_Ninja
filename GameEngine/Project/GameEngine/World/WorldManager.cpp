#include "pch.h"
#include "WorldManager.h"
#include "World.h"

CWorldManager::CWorldManager()
{

}

CWorldManager::~CWorldManager()
{
}

int CWorldManager::Init()
{
	m_World = std::make_shared<CWorld>();

	m_World->SetSelf(m_World);

	if (!m_World->Init())
		return E_FAIL;

	return S_OK;
}

void CWorldManager::Update(double _DeltaTime)
{
	m_World->Update(_DeltaTime);
}

void CWorldManager::PostUpdate(double _DeltaTime)
{
	m_World->PostUpdate(_DeltaTime);
}

//void CWorldManager::Render() 
//{
//	m_World->Render();
//}