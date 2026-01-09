#pragma once

enum class ERenderListSort
{
	None,
	Y,
	Alpha
};

enum class EAnimation2DTextureType
{
	None = -1,
	SpriteSheet,
	Frame,
	End
};

enum class EAssetType
{
	None = -1,
	Mesh,
	Shader,
	ConstantBuffer,
	Material,
	Texture,
	Animation2D,
	End
};

enum class EColliderType
{
	Box2D,
	Line2D,
	Sphere2D,
	Polygon2D,
};

namespace ECollisionChannel
{
	enum Type
	{
		Static,
		Player,
		Monster,
		Custom1,
		Custom2,
		Custom3,
		Custom4,
		Custom5,
		Custom6,
		Custom7,
		Custom8,
		Custom9,
		Custom10,
		End
	};
}

namespace ECollisionInteraction
{
	enum Type
	{
		Ignore,		// 무시
		Collision,	// 충돌
		End
	};
}

namespace ECCWResult
{
	enum Type
	{
		CCW = -1,	// 시계 방향
		None = 0,	// 한 직선에 존재
		CW = 1,		// 반시계 방향
	};

};

/*
Channel1 : Player
Enagle : true
Interaction[Static]		= Collision;
Interaction[Player]		= Ignore;
Interaction[Monster]	= Collision;
Interaction[Custom1]	= Collision;
Interaction[Custom2]	= Ignore;

2번 프로파일
Channel : Monster
Enable : true
Interaction[Static]		= Collision;
Interaction[Player]		= Ignore;
Interaction[Monster]	= Ignore;
Interaction[Custom1]	= Ignore;
Interaction[Custom2]	= Collision;

*/

