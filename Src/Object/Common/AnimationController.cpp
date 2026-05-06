#include <DxLib.h>
#include "../../Manager/Generic/SceneManager.h"
#include "../Common/Easing.h"
#include "AnimationController.h"

AnimationController::AnimationController(const int _modelId, const int _hipNum) :
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
	isEase_(false)
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

	if (playType_ != type || isForce) {

		if (playType_ != -1)
		{
			// モデルからアニメーションを外す
			playAnim_.attachNo = MV1DetachAnim(modelId_, playAnim_.attachNo);
			//MV1ResetFrameUserLocalWorldMatrix(modelId_, HIP_FRAME_NO);
		}

		// アニメーション種別を変更
		playType_ = type;
		playAnim_ = animations_[type];

		// 初期化
		playAnim_.step = startStep;

		// モデルにアニメーションを付ける
		int animIdx = 0;
		if (MV1GetAnimNum(playAnim_.model) > 1)
		{
			// アニメーションが複数保存されていたら、番号1を指定
			animIdx = 1;
		}
		playAnim_.attachNo = MV1AttachAnim(modelId_, animIdx, playAnim_.model);

		// アニメーション総時間の取得
		if (endStep > 0.0f)
		{
			playAnim_.totalTime = endStep;
		}
		else
		{
			playAnim_.totalTime = MV1GetAttachAnimTotalTime(modelId_, playAnim_.attachNo);
		}

		// アニメーションループ
		isLoop_ = isLoop;

		//途中ループ
		isMidLoop_ = false;

		// アニメーションしない
		isStop_ = isStop;

		stepEndLoopStart_ = -1.0f;
		stepEndLoopEnd_ = -1.0f;
		switchLoopReverse_ = 1.0f;
	}

}

void AnimationController::Update(const float _spdScl)
{


	if (!isStop_)
	{

		// 経過時間の取得
		float deltaTime = SceneManager::GetInstance().GetDeltaTime();

		// 再生
		playAnim_.step += (deltaTime * playAnim_.speed * switchLoopReverse_) * _spdScl;

		// アニメーション終了判定
		bool isEnd = false;
		if (switchLoopReverse_ > 0.0f)
		{
			// 通常再生の場合
			if (playAnim_.step > playAnim_.totalTime)
			{
				isEnd = true;
			}
		}
		else
		{
			// 逆再生の場合
			if (playAnim_.step < playAnim_.totalTime&&!isMidLoop_)
			{
				isEnd = true;
			}
		}

		if (isEnd)
		{
			// アニメーションが終了したら
			if (isLoop_)
			{
				// ループ再生
				if (stepEndLoopStart_ > 0.0f)
				{
					// アニメーション終了後の指定フレーム再生
					switchLoopReverse_ *= -1.0f;
					if (switchLoopReverse_ > 0.0f)
					{
						playAnim_.step = stepEndLoopStart_;
						playAnim_.totalTime = stepEndLoopEnd_;
					}
					else
					{
						playAnim_.step = stepEndLoopEnd_;
						playAnim_.totalTime = stepEndLoopStart_;
					}
					playAnim_.speed = endLoopSpeed_;
					
				}
				else
				{
					// 通常のループ再生
					playAnim_.step = 0.0f;
				}
			}
			else
			{
				// ループしない
				playAnim_.step = playAnim_.totalTime;
			}

		}

	}
	//アニメーション進行前のルートのローカル座標
	VECTOR pre = MV1GetAttachAnimFrameLocalPosition(modelId_, playAnim_.attachNo, hipNum_);

	// アニメーション設定（進行）
	MV1SetAttachAnimTime(modelId_, playAnim_.attachNo, playAnim_.step);

	//アニメーション進行後のルートのローカル座標
	VECTOR post = MV1GetAttachAnimFrameLocalPosition(modelId_, playAnim_.attachNo, hipNum_);

	//アニメーション移動量を取得
	playAnim_.movePow = VSub(post, pre);

	// 腰の位置がずれるので補正
	playAnim_.firstPos.y = post.y;
	//playAnim_.firstPos = post;

	// 移動量を打ち消す
	//SetFrameLocalMatrixPos(modelId_, hipNum_, playAnim_.firstPos);
	SetFrameAnimAttachLocalMatrixPos(modelId_, playAnim_.attachNo, hipNum_, playAnim_.firstPos);

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
	if (playAnim_.step >= endStep)
	{
		playAnim_.speed = _spd;
		switchLoopReverse_ = -1.0f;
	}
	else if (switchLoopReverse_ == -1.0f && playAnim_.step < startStep)
	{
		playAnim_.speed = _spd;
		switchLoopReverse_ = 1.0f;
	}
}

void AnimationController::SetEndMidLoop(const float _spd)
{
	isMidLoop_ = false;
	switchLoopReverse_ = 1.0f;
	playAnim_.speed = _spd;
}

int AnimationController::GetPlayType(void) const
{
	return playType_;
}

const float AnimationController::GetAnimStep(void) const
{
	return playAnim_.step;
}

void AnimationController::SetAnimSpeed(const float _spd, const bool _isEase, const float _startSpd, const float _t, Easing::EASING_TYPE _easeType)
{
	if (_isEase)
	{
		playAnim_.speed = easing_->EaseFunc(_startSpd, _spd, _t, _easeType);
		return;
	}

	//イージングを使用しないならば、そのままスピードを代入
	playAnim_.speed = _spd;
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

	if (playAnim_.step >= playAnim_.totalTime)
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
