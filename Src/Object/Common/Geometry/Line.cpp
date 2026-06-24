#include "Common/Quaternion.h"
#include"./Model.h"
#include"./Cube.h"
#include"./Sphere.h"
#include"./Capsule.h"
#include"./Line.h"
#include"./ExternalLine.h"
#include"./Plane.h"

//***************************************************
//ê¸
//***************************************************

Line::Line(const VECTOR& _pos, const Quaternion& _rot, const VECTOR& _localPosPoint1, const VECTOR& _localPosPoint2) : 
	Geometry(_pos, _rot, 0.0f, _localPosPoint1, _localPosPoint2, {},-1)
{
	hitInfo_ = {};
}

Line::Line(const VECTOR* _objPos1, const VECTOR* _objPos2):
	Geometry({}, Quaternion(), 0.0f, {}, {}, {}, -1),
	externalPoint1_(_objPos1),
	externalPoint2_(_objPos2)
{
}

Line::Line(const Line& _copyBase, const VECTOR& _pos, const Quaternion& _rot) : 
	Geometry(_pos, _rot, 0.0f, _copyBase.GetLocalPosPoint1(), _copyBase.GetLocalPosPoint2(), {}, -1)
{
	hitInfo_ = {};
}

Line::~Line(void)
{
	externalPoint1_ = nullptr; 
	externalPoint2_ = nullptr;
}

void Line::Draw(void)
{
	VECTOR point1 = GetRotPos(localPosPoint1_);
	VECTOR point2 = GetRotPos(localPosPoint2_);

	DrawLine3D(point1, point2, NORMAL_COLOR);

	if (externalPoint1_ != nullptr && externalPoint2_ != nullptr)
	{
		DrawLine3D(*externalPoint1_, *externalPoint2_, NORMAL_COLOR);
	}
}

const bool Line::IsHit(Geometry& _geometry)
{
	return _geometry.IsHit(*this);
}

const bool Line::IsHit(Model& _model)
{
	return _model.IsHit(*this);
}

const bool Line::IsHit(Cube& _cube)
{
	return _cube.IsHit(*this);
}

const bool Line::IsHit(Sphere& _sphere)
{
	return _sphere.IsHit(*this);
}

const bool Line::IsHit(Capsule& _capsule)
{
	return _capsule.IsHit(*this);
}

const bool Line::IsHit(Line& _line)
{
	//å¿ÇËÇ»Ç≠è¨Ç≥Ç¢êî
	const float EPSILON = 1e-6f;

	VECTOR u = VSub(_line.GetPosPoint2(), _line.GetPosPoint1());
	VECTOR v = VSub(GetPosPoint2(), GetPosPoint1());
	VECTOR w = VSub(_line.GetPosPoint1(), GetPosPoint1());

	float a = VDot(u, u);
	float b = VDot(u, v);
	float c = VDot(v, v);
	float d = VDot(u, w);
	float e = VDot(v, w);

	float denom = a * c - b * b;
	float s = 0.0f, t = 0.0f;

	if (denom != 0.0f) {
		s = std::clamp((b * e - c * d) / denom, 0.0f, 1.0f);
	}

	t = (b * s + e) / c;
	t = std::clamp(t, 0.0f, 1.0f);

	s = (b * t - d) / a;
	s = std::clamp(s, 0.0f, 1.0f);

	VECTOR closest1 = VAdd(_line.GetPosPoint1(), VScale(u, s));
	VECTOR closest2 = VAdd(GetPosPoint1(), VScale(v, t));
	VECTOR diff = VSub(closest1, closest2);

	//ãóó£
	float distance = sqrt(VDot(diff, diff));

	//åç∑ÇµÇΩ
	return distance <= EPSILON;
}

const bool Line::IsHit(ExternalLine& _externalLine)
{
	//å¿ÇËÇ»Ç≠è¨Ç≥Ç¢êî
	const float EPSILON = 1e-6f;

	VECTOR u = VSub(_externalLine.GetPosPoint2(), _externalLine.GetPosPoint1());
	VECTOR v = VSub(GetPosPoint2(), GetPosPoint1());
	VECTOR w = VSub(_externalLine.GetPosPoint1(), GetPosPoint1());

	float a = VDot(u, u);
	float b = VDot(u, v);
	float c = VDot(v, v);
	float d = VDot(u, w);
	float e = VDot(v, w);

	float denom = a * c - b * b;
	float s = 0.0f, t = 0.0f;

	if (denom != 0.0f) {
		s = std::clamp((b * e - c * d) / denom, 0.0f, 1.0f);
	}

	t = (b * s + e) / c;
	t = std::clamp(t, 0.0f, 1.0f);

	s = (b * t - d) / a;
	s = std::clamp(s, 0.0f, 1.0f);

	VECTOR closest1 = VAdd(_externalLine.GetPosPoint1(), VScale(u, s));
	VECTOR closest2 = VAdd(GetPosPoint1(), VScale(v, t));
	VECTOR diff = VSub(closest1, closest2);

	//ãóó£
	float distance = sqrt(VDot(diff, diff));

	//åç∑ÇµÇΩ
	return distance <= EPSILON;
}

const bool Line::IsHit(Plane& _plane)
{
	return _plane.IsHit(*this);
}

const VECTOR Line::GetPosPoint1(void) const
{
	if (externalPoint1_) return *externalPoint1_;
	return GetRotPos(localPosPoint1_);
}

const VECTOR Line::GetPosPoint2(void) const
{
	if (externalPoint2_) return *externalPoint2_;
	return GetRotPos(localPosPoint2_);
}
