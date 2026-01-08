#include "pch.h"
#include "Object.h"

std::unordered_map<size_t, std::shared_ptr<CObject>> CObject::m_CDOMap;

CObject::CObject()
{
}

CObject::CObject(const CObject& _Origin)
{
}

CObject::CObject(CObject&& _Origin) noexcept
{
}

CObject::~CObject()
{

}

void CObject::DestroyCDO()
{
	m_CDOMap.clear();
}

