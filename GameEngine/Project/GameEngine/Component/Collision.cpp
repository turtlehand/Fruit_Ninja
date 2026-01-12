#include "pch.h"
#include "Collision.h"

#include "ColliderBox2D.h"
#include "ColliderSphere2D.h"
#include "ColliderLine2D.h"
#include "ColliderPolygon2D.h"

/// <summary>
/// Box 콜라이더끼리 충돌 되었는지
/// </summary>
/// <param name="_HitPoint"></param>
/// <param name="_Src"></param>
/// <param name="_Dest"></param>
/// <returns></returns>
bool CCollision::CollisionBox2DToBox2D(std::vector<FVector3>& _HitPoint, CColliderBox2D* _Src, CColliderBox2D* _Dest)
{
	if (!_Src || !_Dest)
		return false;

	// 회전이 되어 있지 않다면 AABB로 충돌 검사
	if (_Src->GetWorldRot().IsZero() && _Dest->GetWorldRot().IsZero())
	{
		return CollisionAABB2DToAABB2D(_HitPoint, _Src->GetInfo(), _Dest->GetInfo());
	}

	// 그 외에는 OBB로 충돌 검사
	return CollisionOBB2DToOBB2D(_HitPoint, _Src->GetInfo(), _Dest->GetInfo());
}

bool CCollision::CollisionAABB2DToAABB2D(std::vector<FVector3>& _HitPoint, const FBox2DInfo& _Src, const FBox2DInfo& _Dest)
{
	FVector3 SrcMin, SrcMax, DestMin, DestMax;

	SrcMin = _Src.Center - _Src.Axis[EAxis::X] * _Src.HalfSize.x - _Src.Axis[EAxis::Y] * _Src.HalfSize.y;
	SrcMax = _Src.Center + _Src.Axis[EAxis::X] * _Src.HalfSize.x + _Src.Axis[EAxis::Y] * _Src.HalfSize.y;
	DestMin = _Dest.Center - _Dest.Axis[EAxis::X] * _Dest.HalfSize.x - _Dest.Axis[EAxis::Y] * _Dest.HalfSize.y;
	DestMax = _Dest.Center + _Dest.Axis[EAxis::X] * _Dest.HalfSize.x + _Dest.Axis[EAxis::Y] * _Dest.HalfSize.y;

	if (SrcMin.x > DestMax.x)
		return false;
	else if (DestMin.x > SrcMax.x)
		return false;
	else if (SrcMin.y > DestMax.y)
		return false;
	else if (DestMin.y > SrcMax.y)
		return false;

	// 충돌 위치 구하기
	FVector3 IntersectMin, IntersectMax;

	IntersectMin.x = SrcMin.x > DestMin.x ? SrcMin.x : DestMin.x;
	IntersectMin.y = SrcMin.y > DestMin.y ? SrcMin.y : DestMin.y;

	IntersectMax.x = SrcMax.x < DestMax.x ? SrcMax.x : DestMax.x;
	IntersectMax.y = SrcMax.y < DestMax.y ? SrcMax.y : DestMax.y;

	_HitPoint.push_back((IntersectMin + IntersectMax) * 0.5f);

	return true;
}

bool CCollision::CollisionOBB2DToOBB2D(std::vector<FVector3>& _HitPoint, const FBox2DInfo& _Src, const FBox2DInfo& _Dest)
{
	/*
	float ProjLength = 0.f;

	// FBox2DInfo.Axis[EAxiz::] 가 사각형의 방향 벡터이다.
	// FBox2DInfo.HalfSize 는 가로 세로 길이의 절반이다.
	// FBox2DInfo.Center는 사각형의 중심이다.

	ProjLength += abs(_Src.Axis[EAxis::X].Dot(_Src.Axis[EAxis::X] * _Src.HalfSize.x));
	ProjLength += abs(_Src.Axis[EAxis::X].Dot(_Src.Axis[EAxis::Y] * _Src.HalfSize.y));
	ProjLength += abs(_Src.Axis[EAxis::X].Dot(_Dest.Axis[EAxis::X] * _Dest.HalfSize.x));
	ProjLength += abs(_Src.Axis[EAxis::X].Dot(_Dest.Axis[EAxis::Y] * _Dest.HalfSize.y));

	float CenterLength = abs(_Src.Axis[EAxis::X].Dot(_Src.Center - _Dest.Center));

	if (ProjLength < CenterLength)
		return false;

	// ===========================================
	ProjLength = 0.f;

	ProjLength += abs(_Src.Axis[EAxis::Y].Dot(_Src.Axis[EAxis::X] * _Src.HalfSize.x));
	ProjLength += abs(_Src.Axis[EAxis::Y].Dot(_Src.Axis[EAxis::Y] * _Src.HalfSize.y));
	ProjLength += abs(_Src.Axis[EAxis::Y].Dot(_Dest.Axis[EAxis::X] * _Dest.HalfSize.x));
	ProjLength += abs(_Src.Axis[EAxis::Y].Dot(_Dest.Axis[EAxis::Y] * _Dest.HalfSize.y));

	CenterLength = abs(_Src.Axis[EAxis::Y].Dot(_Src.Center - _Dest.Center));


	if (ProjLength < CenterLength)
		return false;


	// ===========================================
	ProjLength = 0.f;

	ProjLength += abs(_Dest.Axis[EAxis::X].Dot(_Src.Axis[EAxis::X] * _Src.HalfSize.x));
	ProjLength += abs(_Dest.Axis[EAxis::X].Dot(_Src.Axis[EAxis::Y] * _Src.HalfSize.y));
	ProjLength += abs(_Dest.Axis[EAxis::X].Dot(_Dest.Axis[EAxis::X] * _Dest.HalfSize.x));
	ProjLength += abs(_Dest.Axis[EAxis::X].Dot(_Dest.Axis[EAxis::Y] * _Dest.HalfSize.y));

	CenterLength = abs(_Dest.Axis[EAxis::X].Dot(_Src.Center - _Dest.Center));


	if (ProjLength < CenterLength)
		return false;


	// ===========================================
	ProjLength = 0.f;

	ProjLength += abs(_Dest.Axis[EAxis::Y].Dot(_Src.Axis[EAxis::X] * _Src.HalfSize.x));
	ProjLength += abs(_Dest.Axis[EAxis::Y].Dot(_Src.Axis[EAxis::Y] * _Src.HalfSize.y));
	ProjLength += abs(_Dest.Axis[EAxis::Y].Dot(_Dest.Axis[EAxis::X] * _Dest.HalfSize.x));
	ProjLength += abs(_Dest.Axis[EAxis::Y].Dot(_Dest.Axis[EAxis::Y] * _Dest.HalfSize.y));

	CenterLength = abs(_Dest.Axis[EAxis::Y].Dot(_Src.Center - _Dest.Center));


	if (ProjLength < CenterLength)
		return false;
	*/

	FVector3 CenterLine = _Src.Center - _Dest.Center;

	// _Src의 X축 방향을 기준으로 투영시킨다고 할 때
	// _Src의 X축 사이즈 + _Dest의 사각형의 투영 길이만 구하면 된다.
	// _Src를 투영시켜 계산할 필요가 없다.
	if (!AxisProjection(CenterLine, _Src.Axis[EAxis::X], _Src.HalfSize.x, _Dest.Axis, _Dest.HalfSize))
		return false;

	if (!AxisProjection(CenterLine, _Src.Axis[EAxis::Y], _Src.HalfSize.y, _Dest.Axis, _Dest.HalfSize))
		return false;

	if (!AxisProjection(CenterLine, _Dest.Axis[EAxis::X], _Dest.HalfSize.x, _Src.Axis, _Src.HalfSize))
		return false;

	if (!AxisProjection(CenterLine, _Dest.Axis[EAxis::Y], _Dest.HalfSize.y, _Src.Axis, _Src.HalfSize))
		return false;

	FVector3    SrcMin, SrcMax, DestMin, DestMax;

	SrcMin = _Src.Center - _Src.Axis[EAxis::X] * _Src.HalfSize.x -
		_Src.Axis[EAxis::Y] * _Src.HalfSize.y;
	SrcMax = _Src.Center + _Src.Axis[EAxis::X] * _Src.HalfSize.x +
		_Src.Axis[EAxis::Y] * _Src.HalfSize.y;

	DestMin = _Dest.Center - _Dest.Axis[EAxis::X] * _Dest.HalfSize.x -
		_Dest.Axis[EAxis::Y] * _Dest.HalfSize.y;
	DestMax = _Dest.Center + _Dest.Axis[EAxis::X] * _Dest.HalfSize.x +
		_Dest.Axis[EAxis::Y] * _Dest.HalfSize.y;

	FVector3    IntersectMin, IntersectMax;

	IntersectMin.x = SrcMin.x > DestMin.x ? SrcMin.x : DestMin.x;
	IntersectMin.y = SrcMin.y > DestMin.y ? SrcMin.y : DestMin.y;

	IntersectMax.x = SrcMax.x < DestMax.x ? SrcMax.x : DestMax.x;
	IntersectMax.y = SrcMax.y < DestMax.y ? SrcMax.y : DestMax.y;

	_HitPoint.push_back((IntersectMin + IntersectMax) * 0.5f);

	return true;

}

/// <summary>
/// 
/// </summary>
/// <param name="CenterLine">중심 사이 선분</param>
/// <param name="_ProjAxis">투영할 방향 벡터</param>
/// <param name="_SrcHalfSize">투영 벡터를 가지고 있는 사각형의 절반 크기</param>
/// <param name="_DestAxis">다른 사각형의 방향 벡터</param>
/// <param name="_DestHalfSize">다른 사각형의 절반 크기</param>
/// <returns></returns>
bool CCollision::AxisProjection(const FVector3& _CenterLine, const FVector3& _ProjAxis, float _SrcHalfSize, const FVector3* _DestAxis, const FVector2& _DestHalfSize)
{
	// 센터 사이를 연결하는 벡터를 분리축 후보에 투영하여 구간의
	// 길이를 구한다. 단, 음수값은 필요 없으므로 절대값으로 처리한다.
	// abs : 절대값을 구해준다.
	float CenterProjectionDist = abs(_CenterLine.Dot(_ProjAxis));

	float DestProjectionDist =
		abs(_ProjAxis.Dot(_DestAxis[EAxis::X])) * _DestHalfSize.x +
		abs(_ProjAxis.Dot(_DestAxis[EAxis::Y])) * _DestHalfSize.y;

	if (_SrcHalfSize + DestProjectionDist > CenterProjectionDist)
		return true;

	return false;
}

bool CCollision::CollisionSphere2DToSphere2D(std::vector<FVector3>& _HitPoint, CColliderSphere2D* _Src, CColliderSphere2D* _Dest)
{
	if (!_Src || !_Dest)
		return false;

	if (!CollisionSphere2DToSphere2D(_HitPoint, _Src->GetInfo(), _Dest->GetInfo()))
		return false;

	return true;
}

bool CCollision::CollisionSphere2DToSphere2D(std::vector<FVector3>& _HitPoint, const FSphere2DInfo& _Src, const FSphere2DInfo& _Dest)
{
	// 센터와 센터 사이의 거리를 구한다.
	float Distance = _Src.Center.Distance(_Dest.Center);

	// 중심 사이 거리가 두 원의 반지름보다 크다면 충돌하지 않았다.
	if (Distance > _Src.Radius + _Dest.Radius)
		return false;

	float Gap = _Src.Radius + _Dest.Radius - Distance;
	Gap /= 2;
	
	FVector3 Dir = _Src.Center - _Dest.Center;
	Dir.Normalize();

	_HitPoint.push_back(_Src.Center + Dir * (_Src.Radius - Gap));

	return true;
}

bool CCollision::CollisionBox2DToSphere2D(std::vector<FVector3>& _HitPoint, CColliderBox2D* _Box, CColliderSphere2D* _Sphere)
{
	if (!_Box || !_Sphere)
		return false;

	if (!CollisionBox2DToSphere2D(_HitPoint, _Box->GetInfo(), _Sphere->GetInfo()))
		return false;

	return true;
}

bool CCollision::CollisionBox2DToSphere2D(std::vector<FVector3>& _HitPoint, const FBox2DInfo& _Box, const FSphere2DInfo& _Sphere)
{
	FVector3 CenterLine = _Box.Center - _Sphere.Center;

	FVector3 Axis = CenterLine;
	Axis.Normalize();

	if (!AxisProjection(CenterLine, Axis, _Sphere.Radius, _Box.Axis, _Box.HalfSize))
		return false;

	// 상자 X축으로 투영
	float CenterProjectionDist = abs(CenterLine.Dot(_Box.Axis[EAxis::X]));

	if (CenterProjectionDist > _Sphere.Radius + _Box.HalfSize.x)
		return false;

	// 상자 Y축으로 투영
	CenterProjectionDist = abs(CenterLine.Dot(_Box.Axis[EAxis::Y]));

	if (CenterProjectionDist > _Sphere.Radius + _Box.HalfSize.y)
		return false;

	return true;
}

bool CCollision::CollisionLine2DToLine2D(std::vector<FVector3>& _HitPoint, CColliderLine2D* _Src, CColliderLine2D* _Dest)
{
	if (!_Src || !_Dest)
		return false;

	if (!CollisionLine2DToLine2D(_HitPoint, _Src->GetInfo(), _Dest->GetInfo()))
		return false;

	return true;
}



ECCWResult::Type CCollision::CCW2D(const FVector3& _p1, const FVector3& _p2, const FVector3& _p3)
{
	// CCW(Counter Clock Wise) 알고리즘
	// 점 3개가 이루는 방향을 계산하는 알고리즘.
	// 점 A, B, C가 있을 경우 세 점이 이루는 방향을 계산.
	// CCW(A, B, C)
	// CCW(A, C, B);

	// 2d 벡터는 외적을 할 경우, x,y 값 0, 0이고, z값만 의미를 가진다.
	// z값이 1이나 -1이냐에 따라 외적 방향이 결정되기 때문이다.
	// (A X B).z = Ax * By - Ay * Bx;
	FVector3 v = _p2 - _p1;
	FVector3 w = _p3 - _p1;

	float Cross = v.x * w.y - v.y * w.x;

	if (Cross < 0.0f)
		return ECCWResult::CW;
	else if (Cross > 0.f)
		return ECCWResult::CCW;
	
	return ECCWResult::None;

}

bool CCollision::CollisionLine2DToLine2D(std::vector<FVector3>& _HitPoint, const FLine2DInfo& _Src, const FLine2DInfo& _Dest)
{

	// 두 선이 교차하는지 판단해야 한다.
	int ccw1 = CCW2D(_Src.Start, _Src.End, _Dest.Start);
	int ccw2 = CCW2D(_Src.Start, _Src.End, _Dest.End);
	int ccw3 = CCW2D(_Dest.Start, _Dest.End, _Src.Start);
	int ccw4 = CCW2D(_Dest.Start, _Dest.End, _Src.End);

	// 둘 다 음수( 1 * -1, -1 * 1)일 때 교차
	if (ccw1 * ccw2 < 0 && ccw3 * ccw4 < 0)
	{
		// 직선의 방정식 : ax + by = c
		// 점 A, B가 있을 경우
		// a1 = y1 - y2
		// b1 = x2 - x1
		// c1 = a1 * x1 + b1 * y1;
		// 두 직선이 평행인지를 구하는 행렬식을 구한다.
		// 두 직선을 외적한다.
		FVector3 v = _Src.Start - _Src.End;
		FVector3 w = _Dest.Start - _Dest.End;

		// 연립 방정식을 하여 충돌 지점을 구한다.
		/*
		|a1 b1|   |x| = |c1|
		|a2 b2|   |y|   |c2|

		2x2 행렬을 A, 미지수 벡터를 p = (x, y)
		c = (c1, c2)

		A * p = c

		p = A역행렬 * c

		크래머 공식을 이용해서 역행렬을 직접 만들지 않고 행렬식 만으로
		x, y를 구하는 방식이다.
		행렬식 Det가 0이면 두 점은 평행하여 무한한 교점이 나온다.
		행렬식 Det가 0이 아니면 두 직선의 방향이 달라서 한 점에서
		교차한다.

		a1 * x + b1 * y = c1
		a2 * x + b2 * y = c2

		a1 * b2 * x + b1 * b2 * y = c1 * b2
		a2 * b1 * x + b2 * b1 * y = c2  *b1
		*/

		// Det 값이 0일 경우 두 직선은 평행하다는 의미이다.
		// 직선1 : a1 * x + b1 * y = c1
		// 직선2 : a1 * x + b2 * y = c2
		float   Det = v.x * w.y - v.y * w.x;

		FVector3 HitPoint;

		HitPoint.x = ((_Src.Start.x * _Src.End.y - _Src.Start.y * _Src.End.x) * (_Dest.Start.x - _Dest.End.x) -
			(_Dest.Start.x * _Dest.End.y - _Dest.Start.y * _Dest.End.x) * (_Src.Start.x - _Src.End.x)) 
			/ Det;

		HitPoint.y = ((_Src.Start.x * _Src.End.y - _Src.Start.y * _Src.End.x) * (_Dest.Start.y - _Dest.End.y) -
			(_Dest.Start.x * _Dest.End.y - _Dest.Start.y * _Dest.End.x) * (_Src.Start.y - _Src.End.y)) 
			/ Det;

		_HitPoint.push_back(HitPoint);

		return true;
	}

	// _Dest.Start가 _Src 선분 위에 존재할 경우
	if (ccw1 == 0 && PointOnLine2D(_Src.Start, _Src.End, _Dest.Start))
	{
		_HitPoint.push_back(_Dest.Start);
		return true;
	}
	// _Dest.End가 _Src 선분 위에 존재할 경우
	else if (ccw2 == 0 && PointOnLine2D(_Src.Start, _Src.End, _Dest.End))
	{
		_HitPoint.push_back(_Dest.End);
		return true;
	}
	// _Src.Start가 _Dest 선분 위에 존재할 경우
	else if (ccw3 == 0 && PointOnLine2D(_Dest.Start, _Dest.End, _Src.Start))
	{
		_HitPoint.push_back(_Dest.Start);
		return true;
	}
	// _Src.End가 _Dest 선분 위에 존재할 경우
	else if (ccw4 == 0 && PointOnLine2D(_Dest.Start, _Dest.End, _Src.End))
	{
		_HitPoint.push_back(_Dest.End);
		return true;
	}

	return false;
}

// 점이 선분에 포함되어 있는지
bool CCollision::PointOnLine2D(const FVector3& _LineStart, const FVector3& _LineEnd, const FVector3& _Point)
{
	// 점이 선분의 Min, Max 범위 안에 없으면 우선 선분에 포함되지 않음을 의미한다.
	float MinX = _LineStart.x < _LineEnd.x ? _LineStart.x : _LineEnd.x;
	float MaxX = _LineStart.x > _LineEnd.x ? _LineStart.x : _LineEnd.x;
	float MinY = _LineStart.y < _LineEnd.y ? _LineStart.y : _LineEnd.y;
	float MaxY = _LineStart.y > _LineEnd.y ? _LineStart.y : _LineEnd.y;

	if (_Point.x < MinX || MaxX < _Point.x || _Point.y < MinY || MaxY < _Point.y)
		return false;

	// _Point - _LineStart 로 벡터를 만들고
	// 기존 선분과 외적하여 
	// 0이나오면 평행
	// 0이 아니라면 서로 다른 직선에 놓임을 의미한다.
	float crossProduct = (_LineEnd.y - _LineStart.y) * (_Point.x - _LineStart.x) -
		(_LineEnd.x - _LineStart.x) * (_Point.y - _LineStart.y);

	// 부동소수점 오차를 고려해 0에 아주 가까운지 확인 (Epsilon)
	if (std::abs(crossProduct) > 0.001f)
		return false;

	return true;
}

bool CCollision::CollisionBox2DToLine2D(std::vector<FVector3>& _HitPoint, CColliderBox2D* _Src, CColliderLine2D* _Dest)
{
	if (!_Src || !_Dest)
		return false;

	if (!CollisionBox2DToLine2D(_HitPoint, _Src->GetInfo(), _Dest->GetInfo()))
		return false;

	return true;
}

bool CCollision::CollisionBox2DToLine2D(std::vector<FVector3>& _HitPoint, const FBox2DInfo& _Box, const FLine2DInfo& _Line)
{
	// 선을 구성하는 점 2개 중 사각형 안에 들어오는 점이 있다면 무조건 충돌이다.
	if (CollisionBox2DToPoint(_HitPoint, _Box, _Line.Start))
		return true;
	if (CollisionBox2DToPoint(_HitPoint, _Box, _Line.End))
		return true;

	// 하지만 들어오지 않을 경우 사각형을 구성하는 4개의 변을 만들고 선을 교차하는 변이 있는지 체크하여 검사한다.
	// 사각형을 구상하는 4개의 꼭지점을 구한다.
	FVector3 Pos[4];

	// 왼쪽 하단
	Pos[0] = _Box.Center - _Box.Axis[EAxis::X] * _Box.HalfSize.x -
		_Box.Axis[EAxis::Y] * _Box.HalfSize.y;
	// 왼쪽 상단
	Pos[1] = _Box.Center - _Box.Axis[EAxis::X] * _Box.HalfSize.x +
		_Box.Axis[EAxis::Y] * _Box.HalfSize.y;
	// 오른쪽 하단
	Pos[2] = _Box.Center + _Box.Axis[EAxis::X] * _Box.HalfSize.x -
		_Box.Axis[EAxis::Y] * _Box.HalfSize.y;
	// 오른쪽 상단
	Pos[3] = _Box.Center + _Box.Axis[EAxis::X] * _Box.HalfSize.x +
		_Box.Axis[EAxis::Y] * _Box.HalfSize.y;

	// 박스의 선분을 만든다.
	FLine2DInfo BoxLine[4];

	// 왼쪽
	BoxLine[0].Start = Pos[0];
	BoxLine[0].End = Pos[1];

	// 위
	BoxLine[1].Start = Pos[1];
	BoxLine[1].End = Pos[3];

	// 오른쪽
	BoxLine[2].Start = Pos[3];
	BoxLine[2].End = Pos[2];

	// 아래
	BoxLine[3].Start = Pos[0];
	BoxLine[3].End = Pos[2];

	bool Result = false;
	float Dist = FLT_MAX;
	FVector3 HitResult;

	// _Line과 박스의 각 선분 충돌 검사를 한다.
	for (int i = 0; i < 4; ++i)
	{
		if (CollisionLine2DToLine2D(_HitPoint, _Line, BoxLine[i]))
		{
			Result = true;
		}
	}

	return Result;
}

bool CCollision::CollisionBox2DToPoint(std::vector<FVector3>& _HitPoint, const FBox2DInfo& _Box, const FVector3& _Point)
{
	// CenterLine을 상자의 X, Y축에 투영하여 구간을 비교한다.
	FVector3 CenterLine = _Point - _Box.Center;

	// CenterLine을 X축에 투영
	float Dist = abs(CenterLine.Dot(_Box.Axis[EAxis::X]));

	// 투영한 길이가 더 길다면 충돌 X
	if (Dist > _Box.HalfSize.x)
		return false;

	// CenterLine을 Y축에 투영
	Dist = abs(CenterLine.Dot(_Box.Axis[EAxis::Y]));

	// 투영한 길이가  더 길다면 충돌 X
	if (Dist > _Box.HalfSize.y)
		return false;

	_HitPoint.push_back(_Point);

	return true;
}

bool CCollision::CollisionSphere2DToLine2D(std::vector<FVector3>& _HitPoint, CColliderSphere2D* _Sphere, CColliderLine2D* _Line)
{
	if (!_Sphere || !_Line)
		return false;

	if (!CollisionSphere2DToLine2D(_HitPoint, _Sphere->GetInfo(), _Line->GetInfo()))
		return false;

	return true;;
}

bool CCollision::CollisionSphere2DToLine2D(std::vector<FVector3>& _HitPoint, const FSphere2DInfo& _Sphere, const FLine2DInfo& _Line)
{
	/*
	교점 P는 2개가 나올 수 있다.
	원 센터 : C 반지름 : r 교점 : P
	||P - C|| - r = 0
	직선의 시작점 : S 직선의 방향 : D 임의의 거리 : t
	교점 P = S + Dt
	||S - C + Dt|| - r = 0
	S - C : M
	||M + Dt|| - r = 0
	루트((M + Dt) * (M + Dt)) - r = 0
	(M + Dt) * (M + Dt) - r * r = 0
	Dt^2 + 2MDt + M^2 - r^2 = 0
	이차방정식 : Ax^2 + Bx + C = 0
	A : D, B : 2MD, C : M^2 - r^2
	-B +- 루트(B^2 - 4AC) / 2A
	A에 해당하는 D는 방향벡터이므로 제곱해도 1이다 무시한다.
	-B +- 루트(B^2 - 4C) / 2
	*/
	FVector3 Dir = _Line.End - _Line.Start;

	// 선의 길이를 구한다.
	float   LineLength = Dir.Length();

	// 방향만 남긴다.
	Dir.Normalize();

	FVector3    M = _Line.Start - _Sphere.Center;

	float   b = 2.f * M.Dot(Dir);
	float   c = M.Dot(M) -_Sphere.Radius * _Sphere.Radius;

	float   Det = b * b - 4.f * c;

	if (Det < 0.f)
		return false;

	Det = sqrtf(Det);

	float   t1, t2;

	t1 = (-b + Det) / 2.f;
	t2 = (-b - Det) / 2.f;

	// 교점이 둘 다 뒤에 있을 경우 충돌이 아니다.
	if (t1 < 0.f && t2 < 0.f)
		return false;

	// 구해준 두 길이중 하나라도 선의 길이보다 짧아야 한다.
	// 단, 음수는 제외한다.
	bool    Result = false;

	if (t1 > 0.f && t1 <= LineLength || t2 > 0.f && t2 <= LineLength)
		Result = true;

	else
	{
		// 선의 시작과 도착점이 원 안에 들어왔는지 판단한다.
		float   Length1 = _Line.Start.Distance(_Sphere.Center);
		float   Length2 = _Line.End.Distance(_Sphere.Center);

		if (Length1 <= _Sphere.Radius && Length2 <= _Sphere.Radius)
		{
			Result = true;
		}
	}

	if (Result)
	{
		float   HitDist = t1 < t2 ? t1 : t2;

		if (HitDist < 0.f)
			HitDist = t1 > t2 ? t1 : t2;

		_HitPoint.push_back(_Line.Start + Dir * HitDist);
	}

	return Result;

}

bool CCollision::CollisionPolygon2DToLine2D(std::vector<FVector3>& _HitPoint, CColliderPolygon2D* _Polygon, CColliderLine2D* _Line)
{
	if (!_Polygon || !_Line)
		return false;

	if (!CollisionPolygon2DToLine2D(_HitPoint, _Polygon->GetInfo(), _Line->GetInfo()))
		return false;

	return true;;
}

bool CCollision::CollisionPolygon2DToLine2D(std::vector<FVector3>& _HitPoint, const FPolygon2DInfo& _Polygon, const FLine2DInfo& _Line)
{
	// 하지만 들어오지 않을 경우 사각형을 구성하는 4개의 변을 만들고 선을 교차하는 변이 있는지 체크하여 검사한다.
	// 사각형을 구상하는 4개의 꼭지점을 구한다.
	std::vector<FVector3> Pos(_Polygon.Points.size());

	for (int i = 0; i < _Polygon.Points.size(); ++i)
	{
		Pos[i] = _Polygon.WorldPoint[i];
		
	}

	std::vector<FLine2DInfo> Line(_Polygon.Points.size());

	for (int i = 0; i < _Polygon.Points.size() - 1; ++i)
	{
		Line[i].Start = Pos[i];
		Line[i].End = Pos[i + 1];
	}

	Line[_Polygon.Points.size() - 1].Start = Pos[_Polygon.Points.size() - 1];
	Line[_Polygon.Points.size() - 1].End = Pos[0];

	bool Result = false;
	float Dist = FLT_MAX;
	FVector3 HitResult;

	// _Line과 박스의 각 선분 충돌 검사를 한다.
	for (int i = 0; i < _Polygon.Points.size(); ++i)
	{
		if (CollisionLine2DToLine2D(_HitPoint, _Line, Line[i]))
		{
			Result = true;
		}
	}

	return Result;

}
