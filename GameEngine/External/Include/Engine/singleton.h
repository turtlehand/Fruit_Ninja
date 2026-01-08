#pragma once

template<typename T>
class CSingleton
{
private :
	static T* m_Inst;

public:
	static T* GetInst()
	{
		if (m_Inst == nullptr)
			m_Inst = new T;
		return m_Inst;
	}

	static void DestroyInst()
	{
		if (m_Inst)
		{
			delete m_Inst;
			m_Inst = nullptr;
		}
	}

protected:
	CSingleton()
	{
		atexit(DestroyInst);
	}

	~CSingleton()
	{

	}
};

template<typename T>
T* CSingleton<T>::m_Inst = nullptr;