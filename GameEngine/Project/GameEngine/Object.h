#pragma once

class CObject
{
	friend class CEngine;
public:
	CObject();
	CObject(const CObject& _Origin);
	CObject(CObject&& _Origin) noexcept;
	virtual ~CObject() = 0;

protected:
	size_t m_ClassType = 0;

protected:
	template <typename T>
	void SetClassType()
	{
		m_ClassType = typeid(T).hash_code(); 
	}

private:
	static std::unordered_map<size_t, std::shared_ptr<CObject>> m_CDOMap;

public:
	static void DestroyCDO();

	template<typename T>
	static std::weak_ptr<T> CreateCDO()
	{
		size_t HashCode = typeid(T).hash_code();

		std::unordered_map<size_t, std::shared_ptr<CObject>>::iterator iter = m_CDOMap.find(HashCode);

		// 같은게 있을 경우 다시 생성하고 추가하면 안된다.
		if (iter != m_CDOMap.end())
			return std::weak_ptr<T>();

		std::shared_ptr<T> Obj;
		Obj.reset(new T);
		Obj->SetSelf(Obj);

		m_CDOMap.insert(std::make_pair(HashCode, Obj));

		return std::dynamic_pointer_cast<T>(Obj);
	}

	template<typename T>
	static std::weak_ptr<T> FindCDO()
	{
		size_t HashCode = typeid(T).hash_code();

		std::unordered_map<size_t, std::shared_ptr<CObject>>::iterator iter = m_CDOMap.find(HashCode);

		if (iter == m_CDOMap.end())
			return std::weak_ptr<T>();

		return std::dynamic_pointer_cast<T>(iter->second);
	}

	template<typename T>
	static std::weak_ptr<CObject> FindCDO(size_t _Key)
	{
		std::unordered_map<size_t, std::shared_ptr<CObject>>::iterator iter = m_CDOMap.find(_Key);

		if (iter == m_CDOMap.end())
			return std::weak_ptr<CObject>();

		return iter->second;

	}
};

