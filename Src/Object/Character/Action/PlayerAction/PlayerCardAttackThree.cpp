#include "pch.h"
#include "Manager/Generic/SceneManager.h"
#include "Manager/Generic/Camera.h"
#include "Object/Common/AnimationController.h"
#include "Object/Character/Base/CharacterBase.h"
#include "Object/Character/Action/ActionController.h"
#include "./PlayerCardAttackThree.h"

PlayerCardAttackThree::PlayerCardAttackThree(ActionController& _actCntl, CharacterBase& _charaObj, CardPresenter& _cardPresenter):
	PlayerCardAttackBase(_actCntl, _charaObj, _cardPresenter),
	atkThreeEndCnt_(),
	atkAnimLerpCnt_()
{
}

PlayerCardAttackThree::~PlayerCardAttackThree(void)
{
}

void PlayerCardAttackThree::InitAttack(void)
{
	//攻撃3段階目のアニメーションを再生
	anim_.PlayBlend(static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_3), animVar_);

	//カウントの初期化
	atkThreeEndCnt_ = 0.0f;
	atkAnimLerpCnt_ = 0.0f;
}

void PlayerCardAttackThree::AttackUpdate(void)
{
	//ステップの取得
	const float animStep = anim_.GetAnimStep(static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_3));		//現在のアニメステップ

	//攻撃スタートカウント以下なら、アニメーションスピードを遅くする
	if (animStep < atk_.colStartStep)
	{
		anim_.SetAnimSpeed(static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_3), chargeAnimSpd_);
	}

	//攻撃判定処理
	else if (animStep >= atk_.colStartStep &&
		animStep <= atk_.colEndStep)
	{
		//速度を自然に見せるための補完
		atkAnimLerpCnt_ += scnMng_.GetDeltaTime();

		//アニメーション速度補完
		anim_.SetAnimSpeed(static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_3), CharacterBase::DEFAULT_ANIM_SPEED, true
			, chargeAnimSpd_, atkAnimLerpCnt_ / attackLerpTime_, Easing::EASING_TYPE::QUAD_IN);

		//攻撃判定有効
		isAliveAtkCol_ = true;
		character_.MakeAttackCol(character_.GetCharaTag(), Collider::TAG::NML_ATK, {}, 0.0f);

	}
	else if (anim_.IsEnd(static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_3)))		//アニメーション終了でアイドル状態変更
	{
		//攻撃終了時間以上なら、アイドル状態へ
		if (atkThreeEndCnt_ > attackEndTime_)
		{
			actionCtrl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
			return;
		}

		//攻撃３段階カのウント
		atkThreeEndCnt_ += scnMng_.GetDeltaTime();

		//カメラシェイク
		scnMng_.GetCamera().lock()->SetShakeStatus(atkThreeEndCnt_ / attackEndTime_, cameraShakeLimit_, Easing::EASING_TYPE::ELASTIC_BACK);
		scnMng_.GetCamera().lock()->ChangeSub(Camera::SUB_MODE::ONE_SHAKE);
	}
	else if (animStep > atk_.colEndStep)	//攻撃終了後
	{
		//攻撃判定無効
		character_.DeleteAttackCol(character_.GetCharaTag(), Collider::TAG::NML_ATK);
	}
}

void PlayerCardAttackThree::LoadAnimVar(const ACTION_LOAD_DATA& _data)
{
	if (_data.name != "Attack_3")return;

	animVar_ = _data.animVariable;
	LoadAttackStatus(_data.jsonData, atk_);

	//攻撃3段階目のアニメーション速度
	chargeAnimSpd_ = _data.jsonData.value("chargeAnimSpeed", 0.0f);

	//アニメスピード保管時間
	attackLerpTime_ = _data.jsonData.value("lerpTime", 0.0f);

	//カメラシェイクの強さ
	cameraShakeLimit_ = _data.jsonData.value("cameraShakeLimit", 0.0f); 

	//攻撃終了後の後隙
	attackEndTime_ = _data.jsonData.value("attackEndCount", 0.0f);
}
