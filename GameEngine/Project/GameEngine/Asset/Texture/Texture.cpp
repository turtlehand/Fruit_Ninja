#include "pch.h"
#include "Texture.h"
#include "../PathManager.h"
#include "../../Device.h"

CTexture::CTexture() :
	CAsset(EAssetType::Texture)
{
}

CTexture::~CTexture()
{
	size_t Size = m_TextureList.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(m_TextureList[i]->Image);
		delete m_TextureList[i];
	}
}

bool CTexture::LoadTexture(const wchar_t* _FileName, const std::wstring& _PathName)
{
	const std::wstring* BasePath = CPathManager::FindPath(_PathName);

	std::wstring FullPath = *BasePath;
	FullPath += _FileName;

	return LoadTexture(FullPath.c_str());
}



bool CTexture::LoadTexture(const wchar_t* _FullPath)
{

	// 파일 확장자마다 로딩하는 함수가 다르다.
	// 1. .dds
	// 2. .tga
	// 3. 그 외의 파일확장자
	// 전체 경로에서 파일 확장자를 얻어오다.
	wchar_t FileExt[_MAX_EXT] = {};

	// 파일 확장자를 얻어온다.
	// _wsplitpath_s 함수는 경로를 나누어주는 함수이다.
	_wsplitpath_s(_FullPath, nullptr, 0, nullptr, 0, nullptr, 0, FileExt, _MAX_EXT);

	// 위에서 얻어온 유니코드 문자열 파일 확장자를 멀티바이트 문자열로 변경한다.
	char ConvertExt[_MAX_EXT] = {};

	// 멀티바이트로 변경하기 위한 길이를 얻어온다.
	int Count = WideCharToMultiByte(CP_ACP, 0, FileExt, -1, nullptr, 0, nullptr, nullptr);
	WideCharToMultiByte(CP_ACP, 0, FileExt, -1, ConvertExt, Count, nullptr, nullptr);

	// 확장자를 무조건 대문자로 비교하기 위해 대문자로 변경한다.
	_strupr_s(ConvertExt);

	// 픽셀정보를 지정하기 위한 객체를 생성한다.
	DirectX::ScratchImage* Image = new DirectX::ScratchImage;

	// 확장자 체크
	if (strcmp(ConvertExt, ".DDS") == 0)
	{
		if (FAILED(DirectX::LoadFromDDSFile(_FullPath, DirectX::DDS_FLAGS_NONE, nullptr, *Image)))
		{
			SAFE_DELETE(Image);
			return false;
		}
	}
	else if (strcmp(ConvertExt, ".TGA") == 0)
	{
		if (FAILED(DirectX::LoadFromTGAFile(_FullPath, nullptr, *Image)))
		{
			SAFE_DELETE(Image);

			return false;
		}
	}
	else
	{
		if (FAILED(DirectX::LoadFromWICFile(_FullPath, DirectX::WIC_FLAGS_NONE, nullptr, *Image)))
		{
			SAFE_DELETE(Image);
			return false;
		}
	}

	FTextureInfo* Texture = new FTextureInfo;

	Texture->Image = Image;

	m_TextureList.push_back(Texture);

	// 로딩한 이미지 정보를 이용해서 출력용 ShaderResourceView를 생성한다.

	return CreateResourceView((int)m_TextureList.size() - 1);
}

bool CTexture::LoadTexture(const std::vector<const wchar_t*> _FileName, const std::wstring& _PathName)
{
	size_t	Size = _FileName.size();

	for (size_t i = 0; i < Size; ++i)
	{
		if (!LoadTexture(_FileName[i], _PathName))
			return false;
	}

	return true;
}

bool CTexture::LoadTexture(const std::vector<const wchar_t*> _FullPath)
{
	size_t	Size = _FullPath.size();

	for (size_t i = 0; i < Size; ++i)
	{
		if (!LoadTexture(_FullPath[i]))
			return false;
	}

	return true;
}

bool CTexture::CreateResourceView(int _Index)
{
	if (FAILED(DirectX::CreateShaderResourceView(
		CDevice::GetInst()->GetDevice().Get(),
		m_TextureList[_Index]->Image->GetImages(),
		m_TextureList[_Index]->Image->GetImageCount(),
		m_TextureList[_Index]->Image->GetMetadata(),
		&m_TextureList[_Index]->SRV)))
	{
		return false;
	}

	m_TextureList[_Index]->Width = (unsigned int)(m_TextureList[_Index]->Image->GetImages()[0].width);
	m_TextureList[_Index]->Height = (unsigned int)(m_TextureList[_Index]->Image->GetImages()[0].height);

	return true;
}

void CTexture::SetShader(int _Register, int _ShaderBufferType, int _TextureIndex)
{
	if (_ShaderBufferType & EShaderBufferType::Vertex)
	{
		CDevice::GetInst()->GetContext()->VSSetShaderResources(
			_Register, 1, m_TextureList[_TextureIndex]->SRV.GetAddressOf());
	}

	// PixelShader
	if (_ShaderBufferType & EShaderBufferType::Pixel)
	{
		CDevice::GetInst()->GetContext()->PSSetShaderResources(
			_Register, 1, m_TextureList[_TextureIndex]->SRV.GetAddressOf());
	}

	// DomainShader
	if (_ShaderBufferType & EShaderBufferType::Domain)
	{
		CDevice::GetInst()->GetContext()->DSSetShaderResources(
			_Register, 1, m_TextureList[_TextureIndex]->SRV.GetAddressOf());
	}

	// HullShader
	if (_ShaderBufferType & EShaderBufferType::Hull)
	{
		CDevice::GetInst()->GetContext()->HSSetShaderResources(
			_Register, 1, m_TextureList[_TextureIndex]->SRV.GetAddressOf());
	}

	// GeometryShader
	if (_ShaderBufferType & EShaderBufferType::Geometry)
	{
		CDevice::GetInst()->GetContext()->GSSetShaderResources(
			_Register, 1, m_TextureList[_TextureIndex]->SRV.GetAddressOf());
	}

	// ComputeShader
	if (_ShaderBufferType & EShaderBufferType::Compute)
	{
		CDevice::GetInst()->GetContext()->CSSetShaderResources(
			_Register, 1, m_TextureList[_TextureIndex]->SRV.GetAddressOf());
	}
}