#pragma once

class CColliderBox2D;
class CColliderSphere2D;
class CColliderLine2D;

class CCollision
{
	// Box To Box
public:
	static bool CollisionBox2DToBox2D(FVector3& _HitPoint, CColliderBox2D* _Src, CColliderBox2D* _Dest);
private:
	static bool CollisionAABB2DToAABB2D(FVector3& _HitPoint, const FBox2DInfo& _Src, const FBox2DInfo& _Dest);
	static bool CollisionOBB2DToOBB2D(FVector3& _HitPoint, const FBox2DInfo& _Src, const FBox2DInfo& _Dest);
	static bool AxisProjection(const FVector3& _CenterLine, const FVector3& _ProjAxis, float _SrcHalfSize, const FVector3* _DestAxis, const FVector2& _DestHalfSize);

	// Sphere To Sphere
public:
	static bool CollisionSphere2DToSphere2D(FVector3& _HitPoint, CColliderSphere2D* _Src, CColliderSphere2D* _Dest);
private:
	static bool CollisionSphere2DToSphere2D(FVector3& _HitPoint, const FSphere2DInfo& _Src, const FSphere2DInfo& _Dest);
	
	// Box To Sphere
public:
	static bool CollisionBox2DToSphere2D(FVector3& _HitPoint, CColliderBox2D* _Box, CColliderSphere2D* _Sphere);
private:
	static bool CollisionBox2DToSphere2D(FVector3& _HitPoint, const FBox2DInfo& _Box, const FSphere2DInfo& _Sphere);

	// Line To Line
public: 
	static bool CollisionLine2DToLine2D(FVector3& _HitPoint, CColliderLine2D* _Src, CColliderLine2D* _Dest);
private:
	static ECCWResult::Type CCW2D(const FVector3& _p1, const FVector3& _p2, const FVector3& _p3);
	static bool CollisionLine2DToLine2D(FVector3& _HitPoint, const FLine2DInfo& _Src, const FLine2DInfo& _Dest);
	static bool PointOnLine2D(const FVector3& _LineStart, const FVector3& _LineEnd, const FVector3& _Point);

public:
	static bool CollisionBox2DToLine2D(FVector3& _HitPoint, CColliderBox2D* _Src, CColliderLine2D* _Dest);
private:
	static bool CollisionBox2DToLine2D(FVector3& _HitPoint, const FBox2DInfo& _Box, const FLine2DInfo& _Line);
	static bool CollisionBox2DToPoint(FVector3& _HitPoint, const FBox2DInfo& _Box, const FVector3& _Point);

	// Sphere To Line
public:
	static bool CollisionSphere2DToLine2D(FVector3& _HitPoint, CColliderSphere2D* _Sphere, CColliderLine2D* _Line);
private:
	static bool CollisionSphere2DToLine2D(FVector3& _HitPoint, const FSphere2DInfo& _Sphere, const FLine2DInfo& _Line);

};

