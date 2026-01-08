#pragma once

class CCameraComponent;

class CCameraManager
{
	friend class CWorld;

private:
	CCameraManager();
public:
	~CCameraManager();

private:
	std::unordered_multimap<std::string, std::weak_ptr<CCameraComponent>> m_CameraMap;
	std::weak_ptr<CCameraComponent> m_MainCamera;

public:
	const std::weak_ptr<CCameraComponent>& GetMainCamera() const { return m_MainCamera; }
	const FVector3& GetMainCameraWorldPos() const;
	const FVector3& GetMainCameraAxis(EAxis::Type _Axis) const;
	const FMatrix& GetViewMatrix() const;
	const FMatrix& GetProjMatrix() const;


public:
	void AddCamera(const std::string& _Name, const std::weak_ptr<CCameraComponent>& _Camera);
	void ChangeMainCamera(const std::string& _Name);
	void ChangeMainCamera(const std::weak_ptr<CCameraComponent>& _Camera);

	bool Init();
	void Update(double _DeltaTime);
};

