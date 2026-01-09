#pragma once
#include "enum.h"

struct FPoint
{
    float x;
    float y;
};

struct FResolution
{
    int Width = 0;
    int Height = 0;
};

struct FVertexColor
{
    // 위치
    FVector3 Pos;

    // 색상
    FVector4 Color;

    FVertexColor(FVector3 _Pos, FVector4 _Color) :
        Pos(_Pos),
        Color(_Color)
    {

    }

    FVertexColor(float x, float y, float z, float r, float g, float b,
        float a) :
        Pos(x, y, z),
        Color(r, g, b, a)
    {
    }
};

struct FVertexTex
{
    // 위치
    FVector3	Pos;

    // UV
    FVector2	UV;

    FVertexTex()
    {
    }

    FVertexTex(float x, float y, float z, float u, float v) :
        Pos(x, y, z),
        UV(u, v)
    {
    }
};

struct FTextureFrame
{
    FVector2 Start;
    FVector2 Size;
};

struct FBox2DInfo
{
    FVector3 Center;
    FVector3 Axis[2] = { FVector3::Axis[EAxis::X],FVector3::Axis[EAxis::Y] };
    FVector2 HalfSize = FVector2(1.f, 1.f);
};

struct FSphere2DInfo
{
    FVector3 Center;
    float Radius = 0.f;
};

struct FLine2DInfo
{
    FVector3 Start;
    FVector3 End;
};

struct FPolygon2DInfo
{
    FVector3 Center;
    std::vector<FVector3> Points;
};

struct FCollisionChannel
{
    std::string Name;
    ECollisionChannel::Type Channel;
};

struct FCollisionProfile
{
    std::string Name;
    // 프로파일이 어떤 채널을 사용하는지
    FCollisionChannel* Channel;
    bool Enable = true;
    ECollisionInteraction::Type Interaction[ECollisionChannel::End] = {};
};