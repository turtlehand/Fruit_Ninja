#include "pch.h"
#include "Timer.h"

//LARGE_INTEGER CTimer::m_Frequency;
std::chrono::steady_clock::time_point CTimer::m_PrevCount;
std::chrono::duration<double> CTimer::m_DeltaTime;
UINT CTimer::m_FPS;

void CTimer::Init()
{

    m_PrevCount = std::chrono::steady_clock::now();
}

double CTimer::Update()
{

    // 현재 프레임의 카운트를 구한다.
    std::chrono::steady_clock::time_point Count = std::chrono::steady_clock::now();

    // 현재 프레임의 카운트와 이전 프레임의 카운트의 차이를 구하고
    // 이 차이를 초당 카운트로 나누어서 흐른 시간을 구한다.
    m_DeltaTime = Count - m_PrevCount;
    m_FPS = (UINT)(1.0 / m_DeltaTime.count());

    // 현재 프레임의 카운트를 이전 프레임의 카운트를 저장하여 다음 프레임에서 
    // 시간을 구할 수 있게 해준다.
    m_PrevCount = Count;

    return m_DeltaTime.count();
}

/*

게임을 만들 때 병목현상 때문에 델타타입이 크게 나올 때가 있다.
이렇게 델타타입이 크게 나온다면 벽을 통과할 텐데 이럴 때 벽을 통과하지 않게 하는 방법
델타타임의 최대 값을 정해놓나?
*/
