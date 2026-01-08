#include "pch.h"
#include "Asset.h"

CAsset::CAsset(EAssetType _AssetType) :
	CObject(),
	m_AssetType(_AssetType)
{
}

CAsset::CAsset(const CAsset& _Ref) :
	CObject(_Ref),
	m_AssetType(_Ref.m_AssetType)
{
}

CAsset::CAsset(CAsset&& _Ref) noexcept :
	CObject(_Ref),
	m_AssetType(_Ref.m_AssetType)
{
}

CAsset::~CAsset()
{

}
