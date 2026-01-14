#include "pch.h"

std::string ToString(const std::wstring& _String)
{
	return std::string(_String.begin(), _String.end());
}

std::wstring ToWString(const std::string& _String)
{
	return std::wstring(_String.begin(), _String.end());
}

bool MaybeEqual(float a, float b)
{
	// 보통 1e-5f (0.00001f) 정도를 사용합니다.
	return fabsf(a - b) < 0.001f;
}
