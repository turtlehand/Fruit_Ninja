#include "pch.h"

std::string ToString(const std::wstring& _String)
{
	return std::string(_String.begin(), _String.end());
}

std::wstring ToWString(const std::string& _String)
{
	return std::wstring(_String.begin(), _String.end());
}

FVector2 GetUVFromPos(const FVector3& _Pos)
{
    // 기준 범위 (CenterRectTexture 사양: -0.5f ~ 0.5f)
    const float MinX = -0.5f;
    const float MaxX = 0.5f;
    const float MinY = -0.5f;
    const float MaxY = 0.5f;

    // UV 계산
    // x가 -0.5면 0.0, 0.5면 1.0
    float U = _Pos.x + 0.5f;

    // y가 0.5(상단)면 0.0, -0.5(하단)면 1.0 (DirectX/CenterRect 사양)
    float V = 0.5f - _Pos.y;

    return FVector2(U, V);
}
