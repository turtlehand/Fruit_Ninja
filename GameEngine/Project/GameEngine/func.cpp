#include "pch.h"

std::string ToString(const std::wstring& _String)
{
	return std::string(_String.begin(), _String.end());
}

std::wstring ToWString(const std::string& _String)
{
	return std::wstring(_String.begin(), _String.end());
}