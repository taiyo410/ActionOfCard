#include <DxLib.h>
#include "Utility/UtilityCommon.h"
#include "Manager/Generic/SceneManager.h"
#include "Common/Easing.h"
#include "./AnimationController.h"

AnimationController::AnimationController(void) :
	scnMng_(SceneManager::GetInstance()),
	spineFrameNum_(),
	modelId_(),
	playType_(-1),
	isBlend_(false),
	invalidBlendPos_({}), 
	resetAnimPos_({})
{
	easing_ = std::make_unique<Easing>();
}

AnimationController::~AnimationController(void)
{
	for (const auto& anim : animations_)
	{
		MV1DeleteModel(anim.second.model);
	}
}

void AnimationController::LoadData(const std::string& animName)
{

}

void AnimationController::Add(int type,int modelId)
{

	ANIMATION_PARAMETER anim;
	if (modelId != -1)
	{
		//リソースマネージャでロードしたものを使う
		anim.model = modelId;
	}
	else
	{
		//持ち主のモデル
		anim.model = modelId_;
	}
	anim.animIndex = type;

	if (animations_.count(type) == 0)
	{
		// 入れ替え
		animations_.emplace(type, anim);
	}
	else
	{
		// 追加
		animations_[type].model = anim.model;
		animations_[type].animIndex = anim.animIndex;
		animations_[type].attachNo = anim.attachNo;
		animations_[type].variable.totalTime = anim.variable.totalTime;
		animations_[type].variable.speed = anim.variable.speed;
		animations_[type].variable.detachSpeed = anim.variable.detachSpeed;
		animations_[type].variable.isLoop = anim.variable.isLoop;
		animations_[type].variable.isMidLoop = anim.variable.isMidLoop;
		animations_[type].variable.switchLoopReverse = anim.variable.switchLoopReverse;
		animations_[type].variable.stepEndLoopStart = anim.variable.stepEndLoopStart;
		animations_[type].variable.stepEndLoopEnd = anim.variable.stepEndLoopEnd;
		animations_[type].variable.endLoopSpeed = anim.variable.endLoopSpeed	;
	}
}

void AnimationController::PlayBlend(int type, ANIMATION_VARIABLE animVariable)
{
	//優先アニメーションを探索
	int priNo = -1;
	for (const auto& data : animations_)
	{
		if (!data.second.isPriority)continue;

		//優先されているアニメーションID
		priNo = data.first;
	}

	//すでに最優先であれば、同じアニメーションを再生することになるため、処理を飛ばす
	if (priNo == type)return;

	// 最優先アニメーションに設定するブレンド率
	float animRate = 1.0f;

	//何も優先するアニメーションがない場合、アニメーション通常再生
	if (priNo == -1)
	{
		auto& pri = animations_.at(type);
		pri.attachNo = MV1AttachAnim(modelId_, 0, pri.model);
		pri.variable.step = animVariable.step;
		pri.blendRate = animRate;
		pri.variable.totalTime = animVariable.totalTime > 0.0f ? animVariable.totalTime : MV1GetAttachAnimTotalTime(modelId_, pri.attachNo);
		pri.variable.isLoop = animVariable.isLoop;
		pri.animPosMask_ = animVariable.animPosMask_;
		pri.variable.isMidLoop = false;
		pri.isStop = animVariable.isStop;
		pri.isPriority = true;

		pri.variable.speed = animVariable.speed;
		pri.variable.detachSpeed = animVariable.detachSpeed;
		currentAnimPosMask_ = pri.animPosMask_;	
	}
	else
	{
		//次に再生するアニメーションが決まっている
		//かつ、まだ再生中の場合は処理を飛ばす
		//if (/*animations_.at(priNo).nextAnimNo_ != -1 &&*/
		//	animations_.at(priNo).step < animations_.at(priNo).totalTime) return;

		//遷移フラグ
		isBlend_ = true;

		//すでに優先されているものがあれば、優先を切り替える
		auto& oldPri = animations_.at(priNo);
		oldPri.isPriority = false;

		//アニメーションブレンド率計算
		for (const auto& data : animations_)
		{
			//アタッチされていなければ処理を飛ばす
			if (data.second.attachNo == -1)continue;

			animRate -= data.second.blendRate;

			//0より小さくなることはないが、念のため
			if (animRate < 0.0f)
			{
				animRate = 0.0f;
			}
		}

		//最優先アニメーション設定

		//最優先ではないが、すでにアタッチされている場合
		auto& pri = animations_.at(type);
		if (pri.attachNo == -1)
		{
			//最優先アニメーションをアタッチ
			pri.attachNo = MV1AttachAnim(modelId_, 0, pri.model);
		}
		pri.variable.step = animVariable.step;
		pri.isPriority = true;
		pri.blendRate = animRate;
		pri.variable.totalTime = animVariable.totalTime > 0.0f ? animVariable.totalTime : MV1GetAttachAnimTotalTime(modelId_, pri.attachNo);
		pri.variable.isLoop = animVariable.isLoop;
		pri.animPosMask_ = animVariable.animPosMask_;
		pri.variable.speed = animVariable.speed;
		pri.variable.detachSpeed = animVariable.detachSpeed;
		currentAnimPosMask_ = pri.animPosMask_;
	}
}

void AnimationController::Update(const float _spdScl)
{
	//状態別更新
	UpdateNormal();

	//ブレンドアニメーション
	UpdateBlend();

	//アニメーションによる座標の移動を無効化
	FreezeMovementForAnimation();
}

void AnimationController::SetEndLoop(int type, float startStep, float endStep, float speed)
{
	animations_.at(type).variable.stepEndLoopStart = startStep;
	animations_.at(type).variable.stepEndLoopEnd = endStep;
	animations_.at(type).variable.endLoopSpeed = speed;
}

void AnimationController::SetMidLoop(int type, const float startStep, const float endStep, float _spd)
{
	ANIMATION_VARIABLE& var = animations_.at(type).variable;
	var.isMidLoop = true;
	if (var.step >= endStep)
	{
		var.speed = _spd;
		var.switchLoopReverse = -1.0f;
	}
	else if (var.switchLoopReverse == -1.0f && var.step < startStep)
	{
		var.speed = _spd;
		var.switchLoopReverse = 1.0f;
	}
}

void AnimationController::SetEndMidLoop(int type, const float _spd)
{
	animations_.at(type).variable.isMidLoop = false;
	animations_.at(type).variable.switchLoopReverse = 1.0f;
	animations_.at(type).variable.speed = _spd;
}

int AnimationController::GetPlayType(void) const
{
	return playType_;
}

const float AnimationController::GetAnimStep(const int animType) const
{
	return animations_.at(animType).variable.step;
}

void AnimationController::SetAnimSpeed(int type, const float _spd, const bool _isEase, const float _startSpd, const float _t, Easing::EASING_TYPE _easeType)
{
	if (_isEase)
	{
		animations_.at(type).variable.speed = easing_->EaseFunc(_startSpd, _spd, _t, _easeType);
		return;
	}

	//イージングを使用しないならば、そのままスピードを代入
	animations_.at(type).variable.speed = _spd;
}

bool AnimationController::IsEnd(int type) const
{

	bool ret = false;

	if (animations_.at(type).variable.isLoop)
	{
		// ループ設定されているなら、
		// 無条件で終了しないを返す
		return ret;
	}

	if (animations_.at(type).variable.step >= animations_.at(type).variable.totalTime)
	{
		// 再生時間を過ぎたらtrue
		return true;
	}

	return ret;

}

void AnimationController::SetFrameLocalMatrixPos(const int _modelId, const int _frameIdx, VECTOR& _pos)
{
	VECTOR tmpScl;
	MATRIX tmpMatRot;
	VECTOR tmpPos;

	// 対象フレームのワールド行列を大きさ・回転・位置に分解してを取得する
	GetFrameLocalMatrix(_modelId, _frameIdx, tmpScl, tmpMatRot, tmpPos);

	// 合成
	MATRIX ret = MGetIdent();
	ret = MMult(ret, MGetScale(tmpScl));
	ret = MMult(ret, tmpMatRot);
	ret = MMult(ret, MGetTranslate(_pos));

	// 対象フレームにワールド行列をセット
	MV1SetFrameUserLocalMatrix(_modelId, _frameIdx, ret); 
}

void AnimationController::SetFrameAnimAttachLocalMatrixPos(int modelId, int attachNo, int frameIdx, VECTOR& pos)
{
	VECTOR tmpScl;
	MATRIX tmpMatRot;
	VECTOR tmpPos;

	// 対象フレームのローカル行列を大きさ・回転・位置に分解してを取得する
	GetFrameAnimAttachLocalMatrix(modelId, attachNo, frameIdx, tmpScl, tmpMatRot, tmpPos);

	// 合成
	MATRIX ret = MGetIdent();
	ret = MMult(ret, MGetScale(tmpScl));
	ret = MMult(ret, tmpMatRot);
	ret = MMult(ret, MGetTranslate(pos));

	// 対象フレームにローカル行列をセット
	MV1SetFrameUserLocalMatrix(modelId, frameIdx, ret);
}

void AnimationController::SetModelInfo(const int _modelId, const int _spineFrameNum)
{
	modelId_ = _modelId;
	spineFrameNum_ = _spineFrameNum;

	// 対象フレームのローカル行列を初期値にリセットする
	MV1ResetFrameUserLocalMatrix(modelId_, spineFrameNum_);
	auto mat = MV1GetFrameLocalMatrix(modelId_, spineFrameNum_);
	resetAnimPos_ = MGetTranslateElem(mat);
}

void AnimationController::GetFrameLocalMatrix(const int _modelId, int _frameIdx, VECTOR& _scl, MATRIX& _matRot, VECTOR& _pos)
{
	// 対象フレームのローカル行列を取得する
	auto mat = MV1GetFrameLocalMatrix(_modelId, _frameIdx);

	// 拡大縮小成分
	_scl = MGetSize(mat);

	// 回転成分＋拡大縮小成分
	_matRot = MGetRotElem(mat);

	// 回転成分のみにする
	auto revScl = VGet(1.0f / _scl.x, 1.0f / _scl.y, 1.0f / _scl.z);
	_matRot = MMult(_matRot, MGetScale(revScl));

	// 移動成分
	_pos = MGetTranslateElem(mat);
}

void AnimationController::GetFrameAnimAttachLocalMatrix(int modelId, int attachNo, int frameIdx, VECTOR& scl, MATRIX& matRot, VECTOR& pos)
{
	// 対象フレームのローカル行列を取得する
	auto mat = MV1GetAttachAnimFrameLocalMatrix(modelId, attachNo, frameIdx);

	// 拡大縮小成分
	scl = MGetSize(mat);

	// 回転成分＋拡大縮小成分
	matRot = MGetRotElem(mat);

	// 回転成分のみにする
	auto revScl = VGet(1.0f / scl.x, 1.0f / scl.y, 1.0f / scl.z);
	matRot = MMult(matRot, MGetScale(revScl));

	// 移動成分
	pos = MGetTranslateElem(mat);
}

bool AnimationController::IsBlendAnim(const int type) const
{
	const float blendRate = animations_.at(type).blendRate;
	return blendRate > 0.0f && blendRate < 1.0f;
}

void AnimationController::DrawDebug(void)
{
}

void AnimationController::UpdateNone(void)
{
	//何もしない
}

void AnimationController::UpdateNormal(const float _spdScl)
{
	//アタッチされていなければ再生しない
	for (auto& anim : animations_)
	{
		//アタッチされていなければ再生しない
		if (anim.second.attachNo == -1)continue;

		//アニメーション可変の情報
		ANIMATION_VARIABLE& animVar = anim.second.variable;

		//アニメーション時間の進行
		animVar.step += animVar.speed * scnMng_.GetDeltaTime() * _spdScl * animVar.switchLoopReverse;

		//	// アニメーション終了判定
		bool isEnd = false;
		if (animVar.switchLoopReverse > 0.0f)
		{
			// 通常再生の場合
			if (animVar.step > animVar.totalTime)
			{
				isEnd = true;
			}
		}
		else
		{
			// 逆再生の場合
			if (animVar.step < animVar.totalTime && !animVar.isMidLoop)
			{
				isEnd = true;
			}
		}

		if (isEnd)
		{
			// アニメーションが終了したら
			if (animVar.isLoop)
			{
				// ループ再生
				if (animVar.stepEndLoopStart > 0.0f)
				{
					// アニメーション終了後の指定フレーム再生
					animVar.switchLoopReverse *= -1.0f;
					if (animVar.switchLoopReverse > 0.0f)
					{
						animVar.step = animVar.stepEndLoopStart;
						animVar.totalTime = animVar.stepEndLoopEnd;
					}
					else
					{
						animVar.step = animVar.stepEndLoopEnd;
						animVar.totalTime = animVar.stepEndLoopStart;
					}
					animVar.speed = animVar.endLoopSpeed;

				}
				else
				{
					// 通常のループ再生
					animVar.step = 0.0f;
				}
			}
			else
			{
				// ループしない
				animVar.step = animVar.totalTime;
			}
		}
		//アニメーション設定（進行）
		MV1SetAttachAnimTime(modelId_, anim.second.attachNo, animVar.step);
	}
}

void AnimationController::FreezeMovementForAnimation(void)
{
	// 対象フレーム(今回は0版)のローカル行列を初期値にリセットする
	MV1ResetFrameUserLocalMatrix(modelId_, spineFrameNum_);
	// 対象フレームのローカル行列(大きさ、回転、位置)を取得する
	auto mat = MV1GetFrameLocalMatrix(modelId_, spineFrameNum_);
	auto scl = MGetSize(mat);			// 行列から大きさを取り出す
	auto rot = MGetRotElem(mat);		// 行列から回転を取り出す
	auto pos = MGetTranslateElem(mat);	// 行列から移動値を取り出す
	//Y軸の移動値は調整しない
	invalidBlendPos_.y = pos.y;
	//invalidBlendPos_.x = pos.x * currentAnimPosMask_.x;
	//invalidBlendPos_.y = pos.y * currentAnimPosMask_.y;
	//invalidBlendPos_.z = pos.z * currentAnimPosMask_.z;

	//invalidBlendPos_.x = resetAnimPos_.x + (pos.x - resetAnimPos_.x) * currentAnimPosMask_.x;
	//invalidBlendPos_.y = resetAnimPos_.y + (pos.y - resetAnimPos_.y) * currentAnimPosMask_.y;
	//invalidBlendPos_.z = resetAnimPos_.z + (pos.z - resetAnimPos_.z) * currentAnimPosMask_.z;

	// 大きさ、回転、位置をローカル行列に戻す
	MATRIX mix = MGetIdent();
	mix = MMult(mix, MGetScale(scl));	// 大きさ
	mix = MMult(mix, rot);				// 回転
	mix = MMult(mix, MGetTranslate(invalidBlendPos_));

	// 合成した行列を対象フレームにセットし直して、
	// アニメーションの移動値を無効化
	MV1SetFrameUserLocalMatrix(modelId_, spineFrameNum_, mix);
}

void AnimationController::AnimationDetach(const int _type)
{
	auto& data = animations_.at(_type);

	MV1DetachAnim(modelId_, data.attachNo);

	// 値をリセット

	// アニメーション現在時間
	data.variable.step = 0.0f;

	// アニメーションアタッチNo(モデルとアニメーションの管理番号)
	data.attachNo = -1;

	// ブレンド用
	data.blendRate = 0.0f;

	// 再生優先させるか
	data.isPriority = false;
}

void AnimationController::UpdateBlend(void)
{
	if (!isBlend_)return;

	float blendRate = 1.0f;
	VECTOR blendPos = {};

	// 最優先アニメーションNo
	int priNo = -1;

	for (auto& anim : animations_)
	{
		//最優先アニメーションの探索
		if (anim.second.isPriority)
		{
			priNo = anim.first;
		}

		//アタッチされていない,
		//または最優先アニメーションであれば処理を飛ばす
		if (anim.second.attachNo == -1||anim.second.isPriority)continue;

		//とりあえず時間で引く
		anim.second.blendRate -= scnMng_.GetDeltaTime() * anim.second.variable.detachSpeed;
		if (anim.second.blendRate < 0.0f)
		{
			//ブレンド率初期化
			anim.second.blendRate = 0.0f;

			//アニメーションデタッチ
			AnimationDetach(anim.first);
		}
		else
		{
			// 遷移前アニメーションのブレンド率をセット
			MV1SetAttachAnimBlendRate(modelId_,
				anim.second.attachNo, anim.second.blendRate);

			//ブレンド率を引く
			blendRate -= anim.second.blendRate;
		}
	}

	//念のため
	if (priNo == -1)return;

	//最優先アニメーション設定
	auto& pri = animations_.at(priNo);

	//遷移後アニメーションのブレンド率セット
	pri.blendRate = blendRate;
	MV1SetAttachAnimBlendRate(modelId_, pri.attachNo, pri.blendRate);

	if (pri.blendRate>=1.0f)
	{
		pri.blendRate = 1.0f;

		// 最優先以外を完全除去
		for (auto& anim : animations_)
		{
			if (anim.first == priNo) continue;

			if (anim.second.attachNo != -1)
			{
				AnimationDetach(anim.first);
			}
		}
		isBlend_ = false;
		currentAnimPosMask_ = pri.animPosMask_;
	}
}
