#pragma once

class CPathManager
{
private:
	static std::unordered_map<std::wstring, std::wstring> m_PathMap;

public:
	static void Clear();
	static bool Init();
	static bool CreatePath(const std::wstring& _Name, const std::wstring _Path, const std::wstring& _BasePath = L"Root");
	static const std::wstring* FindPath(const std::wstring& _Name);
};

