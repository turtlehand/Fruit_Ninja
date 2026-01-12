#pragma once
#include "Component/SceneComponent.h"
#include "Object/GameObject.h"
#include "WorldAssetManager.h"
#include "Input.h"

class CCameraManager;
class CGameObject;
class CWorldCollision;

class CWorld
{
protected:
	// 월드가 오브젝트를 관리하므로 shared
	std::unordered_multimap<std::string, std::shared_ptr<CGameObject>>	m_ObjectList;
	std::vector<std::weak_ptr<CGameObject>> m_StartObjList;
	std::weak_ptr<CWorld>												m_Self;
	std::shared_ptr<CCameraManager>										m_CameraManager;
	std::shared_ptr<CWorldAssetManager>									m_WorldAssetManager;
	std::shared_ptr<CInput>												m_Input;
	std::shared_ptr<CWorldCollision>									m_Collision;

public:
	std::weak_ptr<CCameraManager> GetCameraManager() const { return m_CameraManager; }
	void SetSelf(std::weak_ptr<CWorld> _Self) {	m_Self = _Self; }
	std::weak_ptr<CWorldAssetManager> GetWorldAssetManager() const { return m_WorldAssetManager; }
	std::weak_ptr<CInput> GetInput() const { return m_Input; }
	std::weak_ptr<CWorldCollision> GetCollision() const { return m_Collision; }

public:
	virtual bool Init();
	virtual void Update(double _DeltaTime);
	virtual void PostUpdate(double _DeltaTime);
	//virtual void Render();
private:
	void Begin();
	void MouseGamePos();
public:
	CWorld();
	virtual ~CWorld();
public:
	template <typename T>
	std::weak_ptr<T> CreateGameObject(const std::string& _Name)
	{
		std::shared_ptr<T> Obj;

		Obj.reset(new T);

		Obj->SetWorld(m_Self);
		Obj->SetSelf(Obj);
		Obj->SetName(_Name);

		if (!Obj->Init())
		{
			return std::weak_ptr<T>();
		}

		m_ObjectList.insert(std::make_pair(_Name,Obj));

		m_StartObjList.push_back(Obj);

		return Obj;
	}

	template <typename T>
	std::weak_ptr<T> CreateCloneGameObject(const std::string& _Name, const std::weak_ptr<T>& _Origin)
	{
		// _Origin이 존재하지 않는다.
		if (_Origin.expired())
			return std::weak_ptr<T>();

		auto CloneShared = _Origin.lock();

		T* CloneObj = nullptr;

		if (CloneShared)
			CloneObj = CloneShared->Clone();

		std::shared_ptr<T> Obj;
		
		Obj.reset(CloneObj);

		Obj->SetWorld(m_Self);
		Obj->SetSelf(Obj);
		Obj->SetName(_Name);

		if (!Obj->Init())
		{
			return std::weak_ptr<T>();
		}

		m_ObjectList.insert(std::make_pair(_Name, Obj));

		m_StartObjList.push_back(Obj);

		return Obj;
	}

	template<typename T>
	std::weak_ptr<T> FindObject(const std::string& _Name)
	{
		std::unordered_multimap<std::string, std::shared_ptr<CGameObject>>::iterator iter = m_ObjectList.find(_Name);

		if (iter == m_ObjectList.end())
			return std::weak_ptr<T>();
		return std::dynamic_pointer_cast<T>(iter->second);
	}

	template<typename T>
	bool FindObjectList(const std::string& _Name, std::list<std::weak_ptr<T>>& _ObjList)
	{
		_ObjList.clear();
		// pair에는 시작 iterator와, 끝 iterator를 받아오게 한다.
		std::pair<std::unordered_multimap<std::string, std::shared_ptr<CGameObject>>::iterator, std::unordered_multimap<std::string, std::shared_ptr<CGameObject>>::iterator>   Range = m_ObjectList.equal_range(_Name);
	
		// first와 second가 같다면 키를 못찾았다는 의미이다.
		if (Range.first == Range.second)
			return false;

		std::unordered_multimap<std::string, std::shared_ptr<CGameObject>>::iterator iter = Range.first;

		for (; iter != Range.second; ++iter)
		{
			_ObjList.push_back(std::dynamic_pointer_cast<T>(iter->second));
		}

		return true;
	}

};

