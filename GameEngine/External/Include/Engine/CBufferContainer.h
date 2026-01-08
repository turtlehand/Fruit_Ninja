#pragma once

// 어떤 종료의 셰이더를 사용하는 버퍼인지
namespace EShaderBufferType
{
	enum Type : unsigned int
	{
		Vertex = 0x1,
		Pixel = 0x2,
		Domain = 0x4,
		Hull = 0x8,
		Geometry = 0x10,
		Compute = 0x20,
		VP = Vertex | Pixel,
		Graphic = Vertex | Pixel | Domain | Hull | Geometry,
		All = Graphic | Compute
	};
}

struct FCBufferTransformData
{
	FMatrix World;
	FMatrix View;
	FMatrix Proj;
	FMatrix WV;
	FMatrix WVP;

};


struct FCBufferMaterialData
{
	FVector4 BaseColor;
	float Opacity;
	FVector3 Padding;
};