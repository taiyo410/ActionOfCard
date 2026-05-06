#include"../../../Utility/Utility3D.h"
#include "../../../Common/Quaternion.h"
#include"Model.h"
#include"Cube.h"
#include"Sphere.h"
#include"Line.h"
#include"Capsule.h"

//***************************************************
//カプセル
//***************************************************

Capsule::Capsule(const VECTOR& _pos, const Quaternion& _rot, const VECTOR _localPosTop, const VECTOR _localPosDown, const float _radius) : 
	Geometry(_pos,_rot,_radius,_localPosTop,_localPosDown,{},-1)
{
	std::memset(&hitInfo_, 0, sizeof(hitInfo_));
}

Capsule::Capsule(const Capsule& _copyBase, const VECTOR& _pos, const Quaternion& _rot) : Geometry(_pos, _rot, _copyBase.GetRadius(), _copyBase.GetLocalPosPoint1(), _copyBase.GetLocalPosPoint2(), {}, -1)
{
	std::memset(&hitInfo_, 0, sizeof(hitInfo_));
}

Capsule::~Capsule(void)
{
}

void Capsule::Draw(void)
{
	// 上の球体
	VECTOR pos1 = GetPosPoint1();
	DrawSphere3D(pos1, radius_, 5, NORMAL_COLOR, NORMAL_COLOR, false);

	// 下の球体
	VECTOR pos2 = GetPosPoint2();
	DrawSphere3D(pos2, radius_, 5, NORMAL_COLOR, NORMAL_COLOR, false);

	VECTOR dir;
	VECTOR s;
	VECTOR e;

	// 球体を繋ぐ線(X+)
	dir = quaRot_.PosAxis(Utility3D::DIR_R);
	s = VAdd(pos1, VScale(dir, radius_));
	e = VAdd(pos2, VScale(dir, radius_));
	DrawLine3D(s, e, NORMAL_COLOR);

	// 球体を繋ぐ線(X-)
	dir = quaRot_.PosAxis(Utility3D::DIR_L);
	s = VAdd(pos1, VScale(dir, radius_));
	e = VAdd(pos2, VScale(dir, radius_));
	DrawLine3D(s, e, NORMAL_COLOR);

	// 球体を繋ぐ線(Z+)
	dir = quaRot_.PosAxis(Utility3D::DIR_F);
	s = VAdd(pos1, VScale(dir, radius_));
	e = VAdd(pos2, VScale(dir, radius_));
	DrawLine3D(s, e, NORMAL_COLOR);

	// 球体を繋ぐ線(Z-)
	dir = quaRot_.PosAxis(Utility3D::DIR_B);
	s = VAdd(pos1, VScale(dir, radius_));
	e = VAdd(pos2, VScale(dir, radius_));
	DrawLine3D(s, e, NORMAL_COLOR);

	// カプセルの中心
	DrawSphere3D(GetCenter(), 5.0f, 10, NORMAL_COLOR, NORMAL_COLOR, true);
}

const bool Capsule::IsHit(Geometry& _geometry)
{
	return _geometry.IsHit(*this);
}

const bool Capsule::IsHit(Model& _model)
{
	return _model.IsHit(*this);
}

const bool Capsule::IsHit(Cube& _cube)
{
	return _cube.IsHit(*this);
}

const bool Capsule::IsHit(Sphere& _sphere)
{
	return _sphere.IsHit(*this);
}

const bool Capsule::IsHit(Capsule& _capsule)
{
	VECTOR d1 = VSub(GetPosPoint2(), GetPosPoint1());					// 線分1の方向ベクトル
	VECTOR d2 = VSub(_capsule.GetPosPoint2(), _capsule.GetPosPoint1());	// 線分2の方向ベクトル
	VECTOR r = VSub(GetPosPoint1(), _capsule.GetPosPoint1());

	//それぞれの内積を求める
	float a = VDot(d1, d1); // d1・d1
	float e = VDot(d2, d2); // d2・d2
	float f = VDot(d2, r);

	float s, t;

	float c = VDot(d1, r);
	float b = VDot(d1, d2);

	//2つの線分がどれだけ平行であるか
	float denom = a * e - b * b;

	if (denom != 0.0f)
	{
		s = (b * f - c * e) / denom;
		s = std::clamp(s, 0.0f, 1.0f);
	}
	else
	{
		s = 0.0f;
	}

	t = (b * s + f) / e;
	if (t < 0.0f)
	{
		t = 0.0f;
		s = std::clamp(-c / a, 0.0f, 1.0f);
	}
	else if (t > 1.0f)
	{
		t = 1.0f;
		s = std::clamp((b - c) / a, 0.0f, 1.0f);
	}

	VECTOR c1 = VAdd(GetPosPoint1(), VScale(d1, s));
	VECTOR c2 = VAdd(_capsule.GetPosPoint1(), VScale(d2, t));
	float distance = VSize(VSub(c1, c2));

	//衝突したか
	return distance <= (GetRadius() + _capsule.GetRadius());
}

const bool Capsule::IsHit(Line& _line)
{
	VECTOR u = VSub(_line.GetPosPoint2(), _line.GetPosPoint1());
	VECTOR v = VSub(GetPosPoint2(), GetPosPoint1());
	VECTOR w = VSub(_line.GetPosPoint1(), GetPosPoint1());

	float a = VDot(u, u);
	float b = VDot(u, v);
	float c = VDot(v, v);
	float d = VDot(u, w);
	float e = VDot(v, w);

	//どれだけ平行かを求める
	float denom = a * c - b * b;
	float s = 0.0f, t = 0.0f;

	//平行でなければ
	if (denom != 0.0f) 
	{
		s = std::clamp((b * e - c * d) / denom, 0.0f, 1.0f);
	}

	t = (b * s + e) / c;
	t = std::clamp(t, 0.0f, 1.0f);

	s = (b * t - d) / a;
	s = std::clamp(s, 0.0f, 1.0f);

	VECTOR closest1 = VAdd(_line.GetPosPoint1(), VScale(u, s));
	VECTOR closest2 = VAdd(GetPosPoint1(), VScale(v, t));
	VECTOR diff = VSub(closest1, closest2);

	float distance = sqrt(VDot(diff, diff));

	return distance <= GetRadius();
}

void Capsule::HitAfter(void)
{
	if (hitInfo_.HitNum > 0 && hitInfo_.Dim != nullptr)
	{
		//当たり判定情報の解放
		MV1CollResultPolyDimTerminate(hitInfo_);

		//再初期化
		std::memset(&hitInfo_, 0, sizeof(hitInfo_));
	}
}
