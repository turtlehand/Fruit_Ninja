#pragma once

#include "../Asset/Animation2D/Animation2D.h"

struct FAnimation2DNotify
{
	std::string Name;
	bool Call;
	int Frame = 0;
	std::function<void()> Function;
};

class CAnimation2DSequence
{
	friend class CAnimation2DComponent;

private:
	CAnimation2DSequence();
	CAnimation2DSequence(const CAnimation2DSequence& _Ref);
	CAnimation2DSequence(CAnimation2DSequence&& _Ref) noexcept;

public:
	~CAnimation2DSequence();

private:
	// 현재 작동중인 애니메이션
	std::weak_ptr<CAnimation2D> m_Animation;

	// 여러 프레임중 몇번째 프레임인지를 판단.
	// 현재 프레임 인덱스
	int m_Frame;
	
	// 프레임을 증가 혹은 감소시키기 위해 누적되는 시간
	// 현재 프레임이 지속되고 있는 시간.
	float m_Time = 0.f;

	// 한 프레임이 변경되어야 하는 시간
	// 한 프레임이 유지되는 시간 m_Time이 m_FrameTime을 넘어가면 다음 Frame으로
	float m_FrameTime = 1.f;

	// 전체 프레임이 동작할 시간.
	// 이 시퀀스가 작동하는 시간
	float m_PlayTime = 0.f;

	// 재생속도 배율
	// 재생속도
	float m_PlayRate = 1.f;

	// 애니메이션을 반복재생 시킬 것인지 여부
	bool m_Loop = true;

	// 역재생
	bool m_Reverse = false;

	// 좌우 대칭
	bool m_Symmetry = false;

	// 프레임이 변경될때마다 호출되는 함수
	std::vector<FAnimation2DNotify> m_NotifyArray;

	// 애니메이션이 끝에 도달하면 호출되는 함수
	std::function<void()> m_FinishNotify;

public:
	EAnimation2DTextureType GetAnimationTextureType()	const
	{
		auto	Anim = m_Animation.lock();

		if (Anim)
			return Anim->GetAnimationTextureType();

		return EAnimation2DTextureType::End;
	}

	bool GetReverse()	const
	{
		return m_Reverse;
	}

	int GetFrameCount()	const
	{
		auto	Anim = m_Animation.lock();

		if (Anim)
			return Anim->GetFrameCount();

		return 0;
	}

	const std::weak_ptr<CAnimation2D>& GetAnimation() const { return m_Animation; }
	int GetFrame() const { return m_Frame; }
	float GetTime() const { return m_Time; }
	float GetFrameTime() const { return m_FrameTime; }
	bool GetSymmetry() const { return m_Symmetry; }
	const std::string& GetName() const;

public:
	void SetAnimation2D(const std::weak_ptr<CAnimation2D>& _Anim)
	{
		m_Animation = _Anim;

		// 만약 Animation이 6장의 Frame을 가지고 있고 m_PlayTime이 1초라면 한 프레임은 1/6초로 설정해준다.
		auto Animation = m_Animation.lock();
		if (Animation)
			m_FrameTime = m_PlayTime / Animation->GetFrameCount();
	}

	void SetPlayTime(float _Time)
	{
		m_PlayTime = _Time;

		// 만약 Animation이 6장의 Frame을 가지고 있고 m_PlayTime이 1초라면 한 프레임은 1/6초로 설정해준다.
		auto Animation = m_Animation.lock();
		if (Animation)
			m_FrameTime = m_PlayTime / Animation->GetFrameCount();
	}

	void SetPlayRate(float _Rate)
	{
		m_PlayRate = _Rate;
	}

	void SetLoop(bool _Loop)
	{
		m_Loop = _Loop;
	}

	void SetReverse(bool _Reverse)
	{
		m_Reverse = _Reverse;

		// 역방향 선택 시 현재 프레임을 끝으로
		// 정방향 선택 시 현재 프레임을 처음으로
		auto Anim = m_Animation.lock();

		if (Anim)
		{
			if (m_Reverse)
				m_Frame = Anim->GetFrameCount() - 1;
			else
				m_Frame = 0;
		}

	}

	void SetAnimationSymmetry(bool _Symmetry) { m_Symmetry = _Symmetry; }

public:
	void Update(double _DeltaTime);
	CAnimation2DSequence* Clone() const;

public:
	void Clear();

private:
	void CallNotify();

	/// <summary>
	/// _Name과 동일한 프레임으로 전환 시 함수를 호출할 수 있게 함수를 등록한다.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="_Name"></param>
	/// <param name="_Frame"></param>
	/// <param name="_Obj"></param>
	/// <param name="Func"></param>
	template<typename T>
	void AddNotify(const std::string& _Name, int _Frame, T* _Obj, void(T::* Func)())
	{
		FAnimation2DNotify Notify;
		Notify.Name = _Name;
		Notify.Frame = _Frame;
		Notify.Function = std::bind(Func, _Obj);

		m_NotifyArray.push_back(Notify);
	}

	/// <summary>
	/// 애니메이션 완전히 재생되었다면 호출하는 함수
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="_Obj"></param>
	/// <param name="_Func"></param>
	template<typename T>
	void SetFinishNotify(T* _Obj, void(T::* _Func)())
	{
		m_FinishNotify = std::bind(_Func, _Obj);
	}

};

