#include "Utility/Utility3D.h"
#include "Utility/UtilityCommon.h"
#include "pch.h"
#include "Sphere.h"
#include "Capsule.h"
#include "Cube.h"
#include "Line.h"
#include "ExternalLine.h"
#include "Plane.h"

Plane::Plane(const VECTOR& _centerPos, const Quaternion& _rot, float _halfW, float _halfH):
	Geometry(_centerPos, _rot, 0.0f, {}, {},{},-1)
{
	plane_.halfH = _halfH;
	plane_.halfW = _halfW;
}

Plane::Plane(const VECTOR& _centerPos, const Quaternion& _rot
    , const VECTOR _centerLocalPos, const VECTOR _localRotRad
    , const VECTOR& _min, const VECTOR& _max):
	Geometry(_centerPos, _rot, 0.0f, {}, {}, {}, -1)
{
    plane_.centerPos = VScale(VAdd(_min, _max), 0.5f);
    plane_.centerPos = VAdd(plane_.centerPos, _centerLocalPos);

    //ローカル回転
    plane_.quaRot = Quaternion::Euler(_localRotRad.x, _localRotRad.y, _localRotRad.z);

	plane_.halfH = (_max.x - _min.x) * 0.5f;
	plane_.halfW = (_max.z - _min.z) * 0.5f;
}

void Plane::Draw(void)
{
    // 4頂点を計算
    VECTOR rightVec = VScale(plane_.quaRot.GetRight(), plane_.halfW);
    VECTOR upVec = VScale(plane_.quaRot.GetUp(), plane_.halfH);

    VECTOR topLeft = VAdd(VAdd(plane_.centerPos, upVec), VScale(rightVec, -1));
    VECTOR topRight = VAdd(VAdd(plane_.centerPos, upVec), rightVec);
    VECTOR bottomLeft = VAdd(VAdd(plane_.centerPos, VScale(upVec, -1)), VScale(rightVec, -1));
    VECTOR bottomRight = VAdd(VAdd(plane_.centerPos, VScale(upVec, -1)), rightVec);

    // 三角形2枚で矩形を描画
    DrawTriangle3D(topLeft, topRight, bottomRight, UtilityCommon::RED, false);
    DrawTriangle3D(topLeft, bottomRight, bottomLeft, UtilityCommon::RED,false);
}

const bool Plane::IsHit(Geometry& _geometry)
{
	return _geometry.IsHit(*this);
}

const bool Plane::IsHit(Model& _model)
{
	return false;
}

const bool Plane::IsHit(Cube& _cube)
{
	return false;
}

const bool Plane::IsHit(Sphere& _sphere)
{
	return false;
}

const bool Plane::IsHit(Capsule& _capsule)
{
	return false;
}

const bool Plane::IsHit(Line& _line)
{
    VECTOR start = _line.GetPosPoint1();
    VECTOR end = _line.GetPosPoint2();
    VECTOR d = VSub(_line.GetPosPoint2(), _line.GetPosPoint1());

    float denom = VDot(plane_.quaRot.GetForward(), d);

    // 平行の衝突を無視
    if (fabsf(denom) < 1e-6f) return false;

    float t = VDot(plane_.quaRot.GetForward(), VSub(plane_.centerPos, start)) / denom;

    // 線分の範囲外
    if (t < 0.0f || t > 1.0f) return false;

    // 交点
    VECTOR hit = VAdd(start, VScale(d, t));

    // 矩形の内外判定
    VECTOR local = VSub(hit, plane_.centerPos);
    float projW = VDot(local, plane_.quaRot.GetRight());
    float projH = VDot(local, plane_.quaRot.GetUp());

    if (fabsf(projW) > plane_.halfW || fabsf(projH) > plane_.halfH)
        return false;

    //結果の反映
    planeHitInfo_ = { hit, t, plane_.quaRot.GetForward() };

    return true;
}

const bool Plane::IsHit(ExternalLine& _externalLine)
{
    VECTOR start = _externalLine.GetPosPoint1();
    VECTOR end = _externalLine.GetPosPoint2();
    VECTOR d = VSub(_externalLine.GetPosPoint2(), _externalLine.GetPosPoint1());

    float denom = VDot(plane_.quaRot.GetForward(), d);

    // 平行の衝突を無視
    if (fabsf(denom) < 1e-6f) return false;

    float t = VDot(plane_.quaRot.GetForward(), VSub(plane_.centerPos, start)) / denom;

    // 線分の範囲外
    if (t < 0.0f || t > 1.0f) return false;

    // 交点
    VECTOR hit = VAdd(start, VScale(d, t));

    // 矩形の内外判定
    VECTOR local = VSub(hit, plane_.centerPos);
    float projW = VDot(local, plane_.quaRot.GetRight());
    float projH = VDot(local, plane_.quaRot.GetUp());

    if (fabsf(projW) > plane_.halfW || fabsf(projH) > plane_.halfH)
        return false;

    //結果の反映
    planeHitInfo_ = { hit, t, plane_.quaRot.GetForward() };

    return true;
}

const bool Plane::IsHit(Plane& _line)
{
    return false;
}
