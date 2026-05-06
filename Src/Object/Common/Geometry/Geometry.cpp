#include "../../../Common/Quaternion.h"
#include "Geometry.h"

Geometry::~Geometry(void)
{
}

inline void Geometry::SetHalfSize(const VECTOR& _halfSize)
{
	obb_.vMin = VScale(_halfSize, -1.0f);
	obb_.vMax = _halfSize;
}
const VECTOR Geometry::GetCenter(void) const
{
	VECTOR top = GetPosPoint1();
	VECTOR down = GetPosPoint2();

	VECTOR diff = VSub(top, down);
	return VAdd(down, VScale(diff, 0.5f));
}

Geometry::Geometry(const VECTOR& _pos, const Quaternion& _rot, const float& _radius,
	const VECTOR& _localPosPoint1, const VECTOR& _localPosPoint2,
	const OBB& _obb, const int& _parentModelId) :
	pos_(_pos),
	quaRot_(_rot),
	radius_(_radius),
	hitLineInfo_({}),
	hitInfo_({}),
	localPosPoint1_(_localPosPoint1),
	localPosPoint2_(_localPosPoint2),
	obb_(_obb),
	parentModelId_(_parentModelId)

{
}

const VECTOR Geometry::GetRotPos(const VECTOR& _localPos) const
{
	VECTOR localRotPos = quaRot_.PosAxis(_localPos);
	return VAdd(pos_, localRotPos);
}


void Geometry::HitAfter(void)
{
}
