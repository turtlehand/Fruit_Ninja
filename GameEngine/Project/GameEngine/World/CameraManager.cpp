#include "pch.h"
#include "CameraManager.h"
#include "../Component/CameraComponent.h"

CCameraManager::CCameraManager()
{

}

CCameraManager::~CCameraManager()
{
}

const FVector3& CCameraManager::GetMainCameraWorldPos() const
{
	auto Camera = m_MainCamera.lock();

	if (Camera)
		return Camera->GetWorldPos();
	return FVector3::Zero;
}

const FVector3& CCameraManager::GetMainCameraAxis(EAxis::Type _Axis) const
{
	auto Camera = m_MainCamera.lock();
	if (Camera)
		return Camera->GetAxis(_Axis);

	return FVector3::Zero;
}

const FMatrix& CCameraManager::GetViewMatrix() const
{
	auto Camera = m_MainCamera.lock();
	if (Camera)
		return Camera->GetViewMatrix();
	return FMatrix::IdentityMatrix;
}

const FMatrix& CCameraManager::GetProjMatrix() const
{
	auto Camera = m_MainCamera.lock();
	if (Camera)
		return Camera->GetProjMatrix();
	return FMatrix::IdentityMatrix;
}

void CCameraManager::AddCamera(const std::string& _Name, const std::weak_ptr<CCameraComponent>& _Camera)
{
	auto iter = m_CameraMap.find(_Name);

	// 같은 이름이 없을 경우에만 추가
	if (iter == m_CameraMap.end())
	{
		if (m_CameraMap.empty())
			m_MainCamera = _Camera;
		m_CameraMap.insert(std::make_pair(_Name, _Camera));
	}
}

void CCameraManager::ChangeMainCamera(const std::string& _Name)
{
	auto iter = m_CameraMap.find(_Name);
	if (iter != m_CameraMap.end())
	{
		m_MainCamera = iter->second;
	}
}

void CCameraManager::ChangeMainCamera(const std::weak_ptr<CCameraComponent>& _Camera)
{
	m_MainCamera = _Camera;
}

bool CCameraManager::Init()
{
	return true;
}

void CCameraManager::Update(double _DeltaTime)
{
	// 카메라 중 유효하지 않은 카메라는 제거한다.
	auto iter = m_CameraMap.begin();
	auto iterEnd = m_CameraMap.end();

	for (; iter != iterEnd;)
	{
		if (iter->second.expired())
		{
			iter = m_CameraMap.erase(iter);
			continue;
		}
		++iter;
	}

	if (m_MainCamera.expired())
	{
		if (!m_CameraMap.empty())
			m_MainCamera = m_CameraMap.begin()->second;
	}
}
