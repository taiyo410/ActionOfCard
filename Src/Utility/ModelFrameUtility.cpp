#include "../Utility/Utility3D.h"
#include "../Object/Common/Transform.h"
#include "ModelFrameUtility.h"

void ModelFrameUtility::GetFrameWorldMatrix(
	int modelId, int frameIdx, VECTOR& scl, MATRIX& matRot, VECTOR& pos)
{
	// 対象フレームのワールド行列を取得
	auto mat = MV1GetFrameLocalWorldMatrix(modelId, frameIdx);

	// 拡大縮小成分
	scl = MGetSize(mat);

	// 回転成分＋拡大縮小成分
	matRot = MGetRotElem(mat);
	// 回転成分のみ
	auto revScl = VGet(1.0f / scl.x, 1.0f / scl.y, 1.0f / scl.z);
	matRot = MMult(matRot, MGetScale(revScl));

	// 移動成分
	pos = MGetTranslateElem(mat);
}

void ModelFrameUtility::SetFrameWorldMatrix(const Transform& follow, int followFrameIdx, 
	Transform& target, VECTOR localPos, VECTOR localRot)
{
	// フレームのワールド行列を取得
	MATRIX worldMatMix = MV1GetFrameLocalWorldMatrix(follow.modelId, followFrameIdx);

	// 回転調整
	VECTOR scl = follow.scl;
	scl = VGet(1.0f / scl.x, 1.0f / scl.y, 1.0f / scl.z);

	// 回転行列を取り出す際に、大きさを1に戻す
	MATRIX rotMat = MMult(MGetRotElem(worldMatMix), MGetScale(scl));

	// X軸回転
	rotMat = MMult(rotMat,
		MGetRotAxis(
			VNorm(VTransformSR(Utility3D::DIR_R, worldMatMix)),
			localRot.x
		)
	);
	// Y軸回転
	rotMat = MMult(rotMat,
		MGetRotAxis(
			VNorm(VTransformSR(Utility3D::DIR_U, worldMatMix)),
			localRot.y
		)
	);
	// Z軸回転
	rotMat = MMult(rotMat,
		MGetRotAxis(
			VNorm(VTransformSR(Utility3D::DIR_F, worldMatMix)),
			localRot.z
		)
	);
	// 回転行列反映
	target.matRot = rotMat;

	// 位置調整
	VECTOR followFramePos = MV1GetFramePosition(follow.modelId, followFrameIdx);
	target.pos = VAdd(followFramePos, VTransform(localPos, target.matRot));
}
