#pragma once

class CEngineSetting
{
	friend class CEngine;
protected:
	CEngineSetting();
public:
	virtual ~CEngineSetting();

public:
	virtual bool Init();
};

