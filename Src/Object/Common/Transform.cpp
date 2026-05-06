#include <DxLib.h>
#include "../../Utility/Utility3D.h"
#include "Transform.h"

Transform::Transform(void)
{
	modelId = -1;

	scl = Utility3D::VECTOR_ONE;
	rot = Utility3D::VECTOR_ZERO;
	pos = Utility3D::VECTOR_ZERO;
	localPos = Utility3D::VECTOR_ZERO;

	matScl = MGetIdent();
	matRot = MGetIdent();
	matPos = MGetIdent();
	quaRot = Quaternion();
	quaRotLocal = Quaternion();

	collider = nullptr;
}

Transform::Transform(int model)
{
	modelId = model;

	scl = Utility3D::VECTOR_ONE;
	rot = Utility3D::VECTOR_ZERO;
	pos = Utility3D::VECTOR_ZERO;
	localPos = Utility3D::VECTOR_ZERO;

	matScl = MGetIdent();
	matRot = MGetIdent();
	matPos = MGetIdent();
	quaRot = Quaternion();
	quaRotLocal = Quaternion();

	collider = nullptr;
}

Transform::~Transform(void)
{
}

void Transform::Update(void)
{

	// 大きさ
	matScl = MGetScale(scl);

	// 回転
	rot = quaRot.ToEuler();
	matRot = quaRot.ToMatrix();


	//総合位置
	overAllPos = VAdd(pos, localPos);

	// 位置
	matPos = MGetTranslate(overAllPos);

	// 行列の合成
	MATRIX mat = MGetIdent();
	mat = MMult(mat, matScl);
	Quaternion q = quaRot.Mult(quaRotLocal);
	mat = MMult(mat, q.ToMatrix());
	mat = MMult(mat, matPos);

	// 行列をモデルに判定
	if (modelId != -1)
	{
		MV1SetMatrix(modelId, mat);
	}

	// 衝突判定の更新
	MV1RefreshCollInfo(modelId);


}

void Transform::SetModel(int model)
{
	modelId = model;
}


VECTOR Transform::GetForward(void) const
{
	return GetDir(Utility3D::DIR_F);
}

VECTOR Transform::GetBack(void) const
{
	return GetDir(Utility3D::DIR_B);
}

VECTOR Transform::GetRight(void) const
{
	return GetDir(Utility3D::DIR_R);
}

VECTOR Transform::GetLeft(void) const
{
	return GetDir(Utility3D::DIR_L);
}

VECTOR Transform::GetUp(void) const
{
	return GetDir(Utility3D::DIR_U);
}

VECTOR Transform::GetDown(void) const
{
	return GetDir(Utility3D::DIR_D);
}

VECTOR Transform::GetDir(const VECTOR& vec) const
{
	return quaRot.PosAxis(vec);
}
