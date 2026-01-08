#pragma once
#include "../Object.h"

class CAsset :
    public CObject
{
    friend class CAssetManager;
    friend class CWorldAssetManager;

protected:
    CAsset(EAssetType _AssetType);
    CAsset(const CAsset& _Ref);
    CAsset(CAsset&& _Ref)noexcept;

public:
    virtual ~CAsset() = 0;

protected:
    std::string m_Name;
    const EAssetType m_AssetType;

public:
    EAssetType GetAssetType() const { return m_AssetType; }
    const std::string& GetName() const { return m_Name; }

    void SetName(const std::string& _Name) { m_Name = _Name; }
};

