#pragma once

// 버퍼 데이터를 담는 추상 클래스
class CConstantBufferData abstract
{
public:
	CConstantBufferData();
	CConstantBufferData(const CConstantBufferData& _Origin);
	CConstantBufferData(CConstantBufferData&& _Origin) noexcept;
	virtual ~CConstantBufferData();

protected:
	// 실제 Shader로 보내줄 CBuffer를 공유하게 한다.
	std::weak_ptr<class CConstantBuffer> m_Buffer;

protected:
	void SetConstantBuffer(const std::string& _Name);

public:
	virtual bool Init() = 0;
	virtual void UpdateBuffer() = 0;
	virtual CConstantBufferData* Clone() = 0;
};

