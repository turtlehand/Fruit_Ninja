#include "pch.h"
#include "PathManager.h"

std::unordered_map<std::wstring, std::wstring> CPathManager::m_PathMap;


void CPathManager::Clear()
{

}

bool CPathManager::Init()
{
	// 루트 경로를 만들어둔다.
	// 실행파일 이름까지 포함한 실행파일이 있는 전체 경로를 얻어온다.
	std::vector<wchar_t> Path(MAX_PATH);
	GetModuleFileName(0, Path.data(), MAX_PATH);

	// 뒷부분의 실행파일 이름은 제거한다.
	int Length = (int)lstrlen(Path.data());

	for (int i = Length - 1; i >= 0; --i)
	{
		if (Path[i] == '\\' || Path[i] == '/')
		{
			memset(&Path[i + 1], 0, (Length - i - i));
			break;
		}
	}

	std::wstring stringPath = Path.data();

	m_PathMap.insert(std::make_pair(L"Root", stringPath));

	return true;
}

bool CPathManager::CreatePath(const std::wstring& _Name, const std::wstring _Path, const std::wstring& _BasePath)
{
	if (FindPath(_Name))
		return false;

	// BasePath를 얻어온다.
	const std::wstring& BasePath = *FindPath(_BasePath);

	// BasePath와 인자로 들어온 패스를 붙여준다.
	std::wstring FullPath = BasePath + _Path;

	m_PathMap.insert(std::make_pair(_Name, FullPath));

	return true;
}

const std::wstring* CPathManager::FindPath(const std::wstring& _Name)
{
	auto iter = m_PathMap.find(_Name);

	if (iter == m_PathMap.end())
		return nullptr;

	return &(iter->second);
}
