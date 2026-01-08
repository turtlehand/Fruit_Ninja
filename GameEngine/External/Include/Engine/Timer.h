#pragma once

class CTimer
{
private:
    static std::chrono::steady_clock::time_point    m_PrevCount;   // 이전 프레임의 초당 카운트 수
    static std::chrono::duration<double>            m_DeltaTime;
    static UINT                                     m_FPS;

public:
    static void Init();
    static double Update();

public:
    static double GetDeltaTime() { return m_DeltaTime.count(); }
    static UINT GetFPS() { return m_FPS; }
};

