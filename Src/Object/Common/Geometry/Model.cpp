#include "../../../Common/Quaternion.h"
#include"Sphere.h"
#include"Capsule.h"
#include"Cube.h"
#include"Line.h"
#include"Model.h"

//***************************************************
//モデル
//***************************************************

Model::Model(const VECTOR& _pos, const Quaternion& _rot, const int _modelId) : 
	Geometry(_pos, _rot,0.0f,{},{},{},_modelId)
	//hitLineInfo_({})/*, parentModelId_(_modelId)*/
{
	std::memset(&hitInfo_, 0, sizeof(hitInfo_)); 
}

Model::Model(const Model& _copyBase, const VECTOR& _pos, const Quaternion& _rot) : 
	Geometry(_pos, _rot, 0.0f, {}, {}, {}, _copyBase.GetParentModel())
{
	//parentModelId_ = _copyBase.GetParentModel();
	//hitLineInfo_ = {};
	std::memset(&hitInfo_, 0, sizeof(hitInfo_)); 
}

Model::~Model(void)
{
	HitAfter();
}

void Model::Draw(void)
{
}

const bool Model::IsHit(Geometry& _geometry)
{
	return _geometry.IsHit(*this);
}

const bool Model::IsHit(Model& _model)
{
	return false;
}

const bool Model::IsHit(Cube& _cube)
{
	return false;
}

const bool Model::IsHit(Sphere& _sphere)
{
	//判定
	auto col1 = MV1CollCheck_Sphere(GetParentModel(), -1, _sphere.GetColPos(), _sphere.GetRadius());
	auto col2 = MV1CollCheck_Sphere(GetParentModel(), -1, _sphere.GetColPos(), _sphere.GetRadius());

	//当たったか
	bool ret = col1.HitNum >= 1;

	//当たっていたら情報更新
	if (ret)
	{
		_sphere.SetHitInfo(col1);
		SetHitInfo(col2);
	}

	return ret;
}

const bool Model::IsHit(Capsule& _capsule)
{
	//判定
	auto col1 = MV1CollCheck_Capsule(GetParentModel(), -1, _capsule.GetPosPoint1(), _capsule.GetPosPoint2(), _capsule.GetRadius());
	auto col2 = MV1CollCheck_Capsule(GetParentModel(), -1, _capsule.GetPosPoint1(), _capsule.GetPosPoint2(), _capsule.GetRadius());

	//当たったか
	bool ret = col1.HitNum >= 1;

	//当たっていたら情報更新
	if (ret)
	{
		_capsule.SetHitInfo(col1);
		SetHitInfo(col2);
	}

	return ret;
}

const bool Model::IsHit(Line& _line)
{
	VECTOR pos1 = _line.GetPosPoint1();
	VECTOR pos2 = _line.GetPosPoint2();
	//判定
	auto col = MV1CollCheck_Line(GetParentModel(), -1, pos1, pos2);
	auto colDim = MV1CollCheck_LineDim(GetParentModel(), -1, pos1, pos2);

	//当たっていたら情報更新
	if (col.HitFlag)
	{
		_line.SetHitLineInfo(col);
		SetHitLineInfo(col);

		_line.SetHitInfo(colDim);
		SetHitInfo(colDim);
	}
	return col.HitFlag;
}

void Model::HitAfter(void)
{
	if (hitInfo_.HitNum > 0 && hitInfo_.Dim != nullptr) 
	{
		//当たり判定情報の解放(球、カプセル)
		MV1CollResultPolyDimTerminate(hitInfo_);

		//再初期化
		std::memset(&hitInfo_, 0, sizeof(hitInfo_));
	}
}
