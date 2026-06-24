#include "pch.h"
#include"./Model.h"
#include"./Cube.h"
#include"./Sphere.h"
#include"./Capsule.h"
#include"./Line.h"
#include"./Plane.h"
#include "ExternalLine.h"

ExternalLine::ExternalLine(const VECTOR& _objPos1, const VECTOR& _objPos2):
	Geometry(_objPos1, Quaternion(), 0.0f, {}, {}, {}, -1),
	externalPoint1_(_objPos1),
	externalPoint2_(_objPos2)
{
}

ExternalLine::~ExternalLine(void)
{
}

void ExternalLine::Draw(void)
{
}

const bool ExternalLine::IsHit(Geometry& _geometry)
{
	return _geometry.IsHit(*this);
}

const bool ExternalLine::IsHit(Model& _model)
{
	return _model.IsHit(*this);
}

const bool ExternalLine::IsHit(Cube& _cube)
{
	return _cube.IsHit(*this);
}

const bool ExternalLine::IsHit(Sphere& _sphere)
{
	return _sphere.IsHit(*this);
}

const bool ExternalLine::IsHit(Capsule& _capsule)
{
	return _capsule.IsHit(*this);
}

const bool ExternalLine::IsHit(Line& _line)
{
	return _line.IsHit(*this);
}

const bool ExternalLine::IsHit(ExternalLine& _externalLine)
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

const bool ExternalLine::IsHit(Plane& _plane)
{
	return _plane.IsHit(*this);
}

const VECTOR ExternalLine::GetPosPoint1(void) const
{
	return externalPoint1_;
}

const VECTOR ExternalLine::GetPosPoint2(void) const
{
	return externalPoint2_;
}
