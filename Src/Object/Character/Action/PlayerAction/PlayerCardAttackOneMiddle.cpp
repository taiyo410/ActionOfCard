#include "pch.h"
#include "Manager/Generic/SceneManager.h"
#include "Manager/Generic/Camera.h"
#include "Object/Common/AnimationController.h"
#include "Object/Character/Base/LogicBase.h"
#include "Object/Character/Base/CharacterBase.h"
#include "Object/Character/Action/ActionController.h"
#include "PlayerCardAttackOneMiddle.h"

PlayerCardAttackOneMiddle::PlayerCardAttackOneMiddle(ActionController& _actCntl, CharacterBase& _charaObj, CardPresenter& _cardPresenter):
	PlayerCardAttackBase(_actCntl,_charaObj,_cardPresenter)
{
	easing_ = std::make_unique<Easing>();
}

PlayerCardAttackOneMiddle::~PlayerCardAttackOneMiddle(void)
{
}

void PlayerCardAttackOneMiddle::InitAttack(void)
{
	//カウントのセット
	midAtkCnt_ = attackTime_;
	//近距離攻撃のアニメーションをセット
	anim_.PlayBlend(static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_1_MIDDLE), animVar_);
	midAtkOverCnt_ = comboTIme_;
	//突き攻撃の方向に向く
	character_.LookAtTargetVec();
}

void PlayerCardAttackOneMiddle::AttackUpdate(void)
{
	//コンボ先行受付
	actionCtrl_.ComboInput();

	//キャラ同士で当たったか
	const bool isHitTarget = character_.GetIsHitTarget();
	const float delta = scnMng_.GetDeltaTime();

	//攻撃判定処理
	if (anim_.GetAnimStep(static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_1_MIDDLE)) >= atk_.colStartStep && midAtkCnt_ > 0.0f) { midAtkCnt_ -= delta; }

	//中距離突きカウントが０以上なら
	if (midAtkCnt_ > 0.0f && !atk_.isDamage)
	{
		//スピード減速
		speed_ = easing_->EaseFunc(initVelocity_, 0.0f
			, (attackTime_ - midAtkCnt_) / attackTime_
			, Easing::EASING_TYPE::QUAD_OUT);

		//攻撃判定生成
		character_.MakeAttackCol(character_.GetCharaTag(), Collider::TAG::NML_ATK, {}, 0.0f);
	}
	else if (midAtkCnt_ <= 0.0f || isHitTarget)		//アニメーション終了でアイドル状態変更
	{
		//カウントダウン
		midAtkOverCnt_ -= delta;

		//攻撃判定無効
		speed_ = 0.0f;

		//攻撃判定消去
		character_.DeleteAttackCol(character_.GetCharaTag(), Collider::TAG::NML_ATK);

		//中距離突きが終了したら
		if (midAtkOverCnt_ < 0.0f)
		{
			actionCtrl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
		}
		else
		{
			actionCtrl_.ChangeComboCardAttack();
		}
	}
}

void PlayerCardAttackOneMiddle::AttackRelease(void)
{

}

void PlayerCardAttackOneMiddle::LoadAnimVar(const ACTION_LOAD_DATA& _data)
{
	if (_data.name != "Attack_1_Middle")return;

	animVar_ = _data.animVariable;
	LoadAttackStatus(_data.jsonData, atk_);
	initVelocity_ = _data.jsonData.value("initVelocity", 0.0f);
	attackTime_ = _data.jsonData.value("attackTime", 0.0f);
	comboTIme_ = _data.jsonData.value("comboTime", 0.0f);
}
