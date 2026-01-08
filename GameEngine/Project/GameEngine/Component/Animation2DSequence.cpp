#include "pch.h"
#include "Animation2DSequence.h"

CAnimation2DSequence::CAnimation2DSequence() :
	m_Animation(),
	m_Frame(0),
	m_Time(0.f),
	m_FrameTime(0.f),
	m_PlayTime(1.f),
	m_PlayRate(1.f),
	m_Loop(false),
	m_Reverse(false),
	m_NotifyArray()
{

}

CAnimation2DSequence::CAnimation2DSequence(const CAnimation2DSequence& _Ref) :
	m_Animation(_Ref.m_Animation),
	m_Frame(_Ref.m_Frame),
	m_Time(_Ref.m_Time),
	m_FrameTime(_Ref.m_FrameTime),
	m_PlayTime(_Ref.m_PlayTime),
	m_PlayRate(_Ref.m_PlayRate),
	m_Loop(_Ref.m_Loop),
	m_Reverse(_Ref.m_Reverse),
	m_NotifyArray()
{

}

CAnimation2DSequence::CAnimation2DSequence(CAnimation2DSequence&& _Ref) noexcept :
	m_Animation(std::move(_Ref.m_Animation)),
	m_Frame(std::move(_Ref.m_Frame)),
	m_Time(std::move(_Ref.m_Time)),
	m_FrameTime(std::move(_Ref.m_FrameTime)),
	m_PlayTime(std::move(_Ref.m_PlayTime)),
	m_PlayRate(std::move(_Ref.m_PlayRate)),
	m_Loop(std::move(_Ref.m_Loop)),
	m_Reverse(std::move(_Ref.m_Reverse)),
	m_NotifyArray(std::move(_Ref.m_NotifyArray))
{

}

CAnimation2DSequence::~CAnimation2DSequence()
{

}

const std::string& CAnimation2DSequence::GetName() const
{
	auto Anim = m_Animation.lock();

	if (!Anim)
		return std::string();

	return Anim->GetName();
}

/// <summary>
/// 시간에 따라 다음 Frame을 변경한다.
/// </summary>
/// <param name="_DeltaTime"></param>
void CAnimation2DSequence::Update(double _DeltaTime)
{
	if (m_Animation.expired())
		return;

	auto Anim = m_Animation.lock();

	m_Time += (float)_DeltaTime * m_PlayRate;

	while (m_Time >= m_FrameTime)
	{
		m_Time -= m_FrameTime;

		// 반대로
		if (m_Reverse)
		{
			--m_Frame;

			if (m_Frame < 0)
			{
				if (m_FinishNotify)
					m_FinishNotify();

				if (m_Loop)
				{
					m_Frame = Anim->GetFrameCount() - 1;
					for (size_t i = 0; i < m_NotifyArray.size(); ++i)
					{
						m_NotifyArray[i].Call = false;
					} 
				}
				// 반복 재생이 아닐 경우 마지막 프레임으로 고정.
				else
					m_Frame = 0;
			}
		}
		else
		{
			// 프레임을 증가시킨다.
			++m_Frame;

			if (m_Frame == Anim->GetFrameCount())
			{
				if (m_FinishNotify)
					m_FinishNotify();

				// 반복 재생 해야 할 경우
				if (m_Loop)
				{
					m_Frame = 0;

					for (size_t i = 0; i < m_NotifyArray.size();++i)
					{
						m_NotifyArray[i].Call = false;
					}
				}
				// 반복 재생이 아닐 경우 마지막 프레임으로 고정.
				else
					m_Frame = Anim->GetFrameCount() - 1;
			}
		}
	
	}

	// 0번 프레임에 대한 노티파이 처리
	CallNotify();

}

CAnimation2DSequence* CAnimation2DSequence::Clone() const
{
	return new CAnimation2DSequence(*this);
}


/// <summary>
/// 등록함수 호출을 초기화해주고
/// 애니메이션 시퀀스를 초기상태로 만든다.
/// </summary>
void CAnimation2DSequence::Clear()
{
	size_t Size = m_NotifyArray.size();
	for (size_t i = 0; i < Size; ++i)
	{
		m_NotifyArray[i].Call = false;
	}

	auto Anim = m_Animation.lock();

	if (Anim)
	{
		if (m_Reverse)
			m_Frame = Anim->GetFrameCount() - 1;
		else
			m_Frame = 0;
		m_Time = 0.f;
	}
}

/// <summary>
/// FAnimation2DNotify.Name과 동일한 이름의 Frame이 있다면 등록한 함수를 호출해준다.
/// </summary>
void CAnimation2DSequence::CallNotify()
{
	size_t Size = m_NotifyArray.size();
	for (size_t i = 0; i < Size; ++i)
	{
		if (m_NotifyArray[i].Frame == m_Frame && !m_NotifyArray[i].Call)
		{
			m_NotifyArray[i].Call = true;

			if (m_NotifyArray[i].Function)
				m_NotifyArray[i].Function();
		}
	}
}