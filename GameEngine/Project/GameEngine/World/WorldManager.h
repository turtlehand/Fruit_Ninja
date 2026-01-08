#pragma once
#include "World.h"

class CWorldManager : public CSingleton<CWorldManager>
{
private:
	std::shared_ptr<CWorld> m_World;
	std::shared_ptr<CWorld> m_NextWorld;

public:
    std::weak_ptr<CWorld> GetWorld() const { return m_World; }
public:
    int Init();
    void Update(double _DeltaTime);
	void PostUpdate(double _DeltaTime);
    //void Render();


public:
	template <typename T>
	std::weak_ptr<T> CreateWorld(bool Next = true)
	{
		// 다음 월드가 있다면 다음 월드를 반환
		if (Next)
		{
			m_NextWorld = std::make_shared<T>();

			m_NextWorld->SetSelf(m_NextWorld);

			if (!m_NextWorld->Init())
				return std::weak_ptr<T>();

			return std::dynamic_pointer_cast<T>(m_NextWorld);
		}

		// 기본 월드를 생성
		m_World = std::make_shared<T>();

		m_World->SetSelf(m_World);

		if (!m_World->Init())
			return std::weak_ptr<T>();

		return std::dynamic_pointer_cast<T>(m_World);
	}

private:
    friend class CSingleton<CWorldManager>;
    CWorldManager();
    ~CWorldManager();
};

