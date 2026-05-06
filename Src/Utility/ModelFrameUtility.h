#pragma once
#include <DxLib.h>
class Transform;

class ModelFrameUtility
{

public:

	// 対象フレームのワールド行列を大きさ・回転・位置に分解してを取得する
	
	/// @brief 対象フレームのワールド行列を大きさ・回転・位置に分解してを取得する
	/// @param modelId モデルID
	/// @param frameIdx フレーム番号
	/// @param scl スケール
	/// @param matRot 回転行列
	/// @param pos 座標
	static void GetFrameWorldMatrix(
		int modelId, int frameIdx, VECTOR& scl, MATRIX& matRot, VECTOR& pos);
	
	/// @brief 対象フレームの位置にtargetを配置し、対象フレームの回転に加え、指定した相対座標・回転を加える
	/// @param follow 追従対象のモデル情報
	/// @param followFrameIdx 追従対象の対象フレーム
	/// @param target ターゲット
	/// @param localPos ローカル座標
	/// @param localRot ローカル座標
	static void SetFrameWorldMatrix(
		const Transform& follow, int followFrameIdx, Transform& target, VECTOR localPos, VECTOR localRot);

private:
};
