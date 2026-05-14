#include <DxLib.h>
#include "../Utility/UtilityCommon.h"
#include "../../Manager/Generic/SceneManager.h"
#include "../Common/Easing.h"
#include "AnimationController.h"

AnimationController::AnimationController(const int _modelId, const int _hipNum) :
	scnMng_(SceneManager::GetInstance()),
	hipNum_(_hipNum),
	modelId_(_modelId),
	playType_(-1),
	isLoop_(false),
	isStop_(false),
	switchLoopReverse_(0.0f),
	endLoopSpeed_(0.0f),
	stepEndLoopStart_(0.0f),
	stepEndLoopEnd_(0.0f),
	isMidLoop_(false),
	isEase_(false),
	blendStep_(0.0f),
	blendPer_(0.0f),
	isBlend_(false)
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

void AnimationController::Add(int type, const float speed, int modelId)
{

	Animation anim;
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

	//anim.model = MV1LoadModel(path.c_str());
	anim.animIndex = type;
	anim.speed = speed;

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
		animations_[type].totalTime = anim.totalTime;
	}

}

void AnimationController::Play(int type, bool isLoop, 
	float startStep, float endStep, bool isStop, bool isForce)
{
	PlayBlend(type, 0.5f, isLoop, startStep, endStep, isStop, isForce);
	//if (isBlend_)
	//{
	//	int i = 0;
	//	MV1DetachAnim(
	//		modelId_,
	//		currentAnim_.attachNo);
	//}

	//if (playType_ != type || isForce) {

	//	if (playType_ != -1)
	//	{
	//		// モデルからアニメーションを外す
	//		currentAnim_.attachNo = MV1DetachAnim(modelId_, currentAnim_.attachNo);
	//	}

	//	// アニメーション種別を変更
	//	playType_ = type;
	//	currentAnim_ = animations_[type];

	//	// 初期化
	//	currentAnim_.step = startStep;

	//	// モデルにアニメーションを付ける
	//	int animIdx = 0;
	//	if (MV1GetAnimNum(currentAnim_.model) > 1)
	//	{
	//		// アニメーションが複数保存されていたら、番号1を指定
	//		animIdx = 1;
	//	}
	//	currentAnim_.attachNo = MV1AttachAnim(modelId_, animIdx, currentAnim_.model);

	//	// アニメーション総時間の取得
	//	if (endStep > 0.0f)
	//	{
	//		currentAnim_.totalTime = endStep;
	//	}
	//	else
	//	{
	//		currentAnim_.totalTime = MV1GetAttachAnimTotalTime(modelId_, currentAnim_.attachNo);
	//	}

	//	// アニメーションループ
	//	isLoop_ = isLoop;

	//	//途中ループ
	//	isMidLoop_ = false;

	//	// アニメーションしない
	//	isStop_ = isStop;

	//	stepEndLoopStart_ = -1.0f;
	//	stepEndLoopEnd_ = -1.0f;
	//	switchLoopReverse_ = 1.0f;
	//}
}

void AnimationController::PlayBlend(int type, float blendTime, bool isLoop, float startStep, float endStep, bool isStop, bool isForce)
{
	//優先アニメーションを探索
	int priNo = -1;
	for (const auto& data : animations_)
	{
		if (!data.second.isPriority)continue;

		//優先されているアニメーションID
		priNo = data.first;
	}

	//すでに最優先であれば処理しない
	if (priNo == type)return;

	// 最優先アニメーションに設定するブレンド率
	float animRate = 1.0f;

	//何も優先するアニメーションがない場合、アニメーション通常再生
	if (priNo == -1)
	{
		auto& pri = animations_.at(type);
		pri.attachNo = MV1AttachAnim(modelId_, 0, pri.model);
		pri.blendRate = animRate;
		pri.isPriority = true;
	}
	else
	{
		//次に再生するアニメーションが決まっている
		//かつ、まだ再生中の場合は処理を飛ばす
		if (/*animations_.at(priNo).nextAnimNo_ != -1 &&*/
			animations_.at(priNo).step < animations_.at(priNo).totalTime) return;

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
			if (animRate < 0.0f)animRate = 0.0f;
		}

		//最優先アニメーション設定

		//最優先ではないが、すでにアタッチされている場合
		auto& pri = animations_.at(type);
		if (pri.attachNo == -1)
		{
			//最優先アニメーションをアタッチ
			pri.attachNo = MV1AttachAnim(modelId_, 0, pri.model);
			pri.blendRate = animRate;
		}
		pri.isPriority = true;

	}
}

void AnimationController::Update(const float _spdScl)
{
	//状態別更新
	UpdateNormal();

	//ブレンドアニメーション
	UpdateBlend();
}

void AnimationController::SetEndLoop(float startStep, float endStep, float speed)
{
	stepEndLoopStart_ = startStep;
	stepEndLoopEnd_ = endStep;
	endLoopSpeed_ = speed;
}

void AnimationController::SetMidLoop(const float startStep, const float endStep, float _spd)
{
	isMidLoop_ = true;
	if (currentAnim_.step >= endStep)
	{
		currentAnim_.speed = _spd;
		switchLoopReverse_ = -1.0f;
	}
	else if (switchLoopReverse_ == -1.0f && currentAnim_.step < startStep)
	{
		currentAnim_.speed = _spd;
		switchLoopReverse_ = 1.0f;
	}
}

void AnimationController::SetEndMidLoop(const float _spd)
{
	isMidLoop_ = false;
	switchLoopReverse_ = 1.0f;
	currentAnim_.speed = _spd;
}

int AnimationController::GetPlayType(void) const
{
	return playType_;
}

const float AnimationController::GetAnimStep(void) const
{
	return currentAnim_.step;
}

void AnimationController::SetAnimSpeed(const float _spd, const bool _isEase, const float _startSpd, const float _t, Easing::EASING_TYPE _easeType)
{
	if (_isEase)
	{
		currentAnim_.speed = easing_->EaseFunc(_startSpd, _spd, _t, _easeType);
		return;
	}

	//イージングを使用しないならば、そのままスピードを代入
	currentAnim_.speed = _spd;
}

bool AnimationController::IsEnd(void) const
{

	bool ret = false;

	if (isLoop_)
	{
		// ループ設定されているなら、
		// 無条件で終了しないを返す
		return ret;
	}

	if (currentAnim_.step >= currentAnim_.totalTime)
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

		//アニメーション時間の進行
		anim.second.step += anim.second.speed * scnMng_.GetDeltaTime();

		if (anim.second.step > anim.second.totalTime)
		{
			//とりあえずループ再生
			anim.second.step = 0.0f;
		}

		//
		//if (anim.second.attachNo == -1) continue;

		MV1SetAttachAnimTime(modelId_, anim.second.attachNo, anim.second.step);
	}


	//if (!isStop_)
	//{
	//	// 経過時間の取得
	//	float deltaTime = scnMng_.GetDeltaTime();

	//	// 再生
	//	currentAnim_.step += (deltaTime * currentAnim_.speed * switchLoopReverse_) * _spdScl;

	//	// アニメーション終了判定
	//	bool isEnd = false;
	//	if (switchLoopReverse_ > 0.0f)
	//	{
	//		// 通常再生の場合
	//		if (currentAnim_.step > currentAnim_.totalTime)
	//		{
	//			isEnd = true;
	//		}
	//	}
	//	else
	//	{
	//		// 逆再生の場合
	//		if (currentAnim_.step < currentAnim_.totalTime && !isMidLoop_)
	//		{
	//			isEnd = true;
	//		}
	//	}

	//	if (isEnd)
	//	{
	//		// アニメーションが終了したら
	//		if (isLoop_)
	//		{
	//			// ループ再生
	//			if (stepEndLoopStart_ > 0.0f)
	//			{
	//				// アニメーション終了後の指定フレーム再生
	//				switchLoopReverse_ *= -1.0f;
	//				if (switchLoopReverse_ > 0.0f)
	//				{
	//					currentAnim_.step = stepEndLoopStart_;
	//					currentAnim_.totalTime = stepEndLoopEnd_;
	//				}
	//				else
	//				{
	//					currentAnim_.step = stepEndLoopEnd_;
	//					currentAnim_.totalTime = stepEndLoopStart_;
	//				}
	//				currentAnim_.speed = endLoopSpeed_;

	//			}
	//			else
	//			{
	//				// 通常のループ再生
	//				currentAnim_.step = 0.0f;
	//			}
	//		}
	//		else
	//		{
	//			// ループしない
	//			currentAnim_.step = currentAnim_.totalTime;
	//		}

	//	}

	//}

	//
	//// アニメーション設定（進行）
	//FreezeMovementForAnimation(currentAnim_);

	//// アニメーション設定（進行）
	//MV1SetAttachAnimTime(modelId_, currentAnim_.attachNo, currentAnim_.step);

	
}

void AnimationController::FreezeMovementForAnimation(Animation& _anim)
{
	//アニメーション進行前のルートのローカル座標
	VECTOR pre = MV1GetAttachAnimFrameLocalPosition(modelId_, _anim.attachNo, hipNum_);

	//アニメーション進行後のルートのローカル座標
	VECTOR post = MV1GetAttachAnimFrameLocalPosition(modelId_, _anim.attachNo, hipNum_);

	//アニメーション移動量を取得
	_anim.movePow = VSub(post, pre);

	// 腰の位置がずれるので補正
	_anim.firstPos.y = post.y;

	// 移動量を打ち消す
	//SetFrameLocalMatrixPos(modelId_, hipNum_, currentAnim_.firstPos);
	SetFrameAnimAttachLocalMatrixPos(modelId_, nextAnim_.attachNo, hipNum_, nextAnim_.firstPos);
}

void AnimationController::AnimationDettach(const int _type)
{
	auto& data = animations_.at(_type);

	MV1DetachAnim(modelId_, data.attachNo);

	// 値をリセット

	// アニメーション現在時間
	data.step = 0.0f;

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


	//次のアニメーションブレンド率
		// ブレンド率計算
	blendPer_ = (blendStep_ >= blendTime_)? UtilityCommon::RATIO_MAX: blendStep_ / blendTime_;

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

		//アタッチされていないなら処理しない
		if (anim.second.attachNo == -1/*||anim.second.isPriority*/)continue;

		//とりあえず時間で引く
		anim.second.blendRate -= scnMng_.GetDeltaTime();
		if (anim.second.blendRate < 0.0f)
		{
			//ブレンド率初期化
			anim.second.blendRate = 0.0f;

			//アニメーションデタッチ
			AnimationDettach(anim.first);
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
		isBlend_ = false;
	}

	//アニメーションブレンド率セット
		//カウント更新
	//blendStep_ > blendTime_ ? blendStep_ = blendTime_ : blendStep_ += scnMng_.GetDeltaTime();

	////現在アニメーションのブレンド率
	//float currentAnimBlendRate = UtilityCommon::RATIO_MAX - blendPer_;

	////現在アニメーションを更新
	//MV1SetAttachAnimTime(modelId_, currentAnim_.attachNo, currentAnim_.step);

	////次アニメーションを更新
	//MV1SetAttachAnimTime(modelId_, nextAnim_.attachNo, nextAnim_.step);

	//currentAnim_.step += scnMng_.GetDeltaTime() * currentAnim_.speed;
	//nextAnim_.step += scnMng_.GetDeltaTime() * nextAnim_.speed;

	////現在アニメーションのブレンド
	//MV1SetAttachAnimBlendRate(modelId_, currentAnim_.attachNo, currentAnimBlendRate);

	////次アニメーションのブレンド
	//MV1SetAttachAnimBlendRate(modelId_, nextAnim_.attachNo, blendPer_);


	////座標固定
	//FreezeMovementForAnimation(nextAnim_);


	////ブレンドを終えたら
	//if (blendStep_ >= blendTime_)
	//{
	//	blendStep_ = 0.0f;
	//	blendTime_ = 0.0f;

	//	//現在アニメーションをデタッチ
	//	MV1DetachAnim(modelId_, currentAnim_.attachNo);

	//	//次アニメーションをアタッチ
	//	MV1AttachAnim(modelId_, nextAnim_.attachNo);

	//	currentAnim_ = nextAnim_;

	//	blendStep_ = 0.0f;
	//	blendTime_ = 0.0f;
	//	isBlend_ = false;
	//	return;

	//}


}
