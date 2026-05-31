#include "../pch.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/Camera.h"
#include "../../Base/CharacterBase.h"
#include "../../../Common/AnimationController.h"
#include "../ActionController.h"
#include "../../Base/LogicBase.h"
#include "PlayerCardAttackOneMiddle.h"

PlayerCardAttackOneMiddle::PlayerCardAttackOneMiddle(ActionController& _actCntl, CharacterBase& _charaObj, CardPresenter& _cardPresenter):
	CardActionBase(_actCntl,_charaObj,_cardPresenter)
{
	easing_ = std::make_unique<Easing>();
}

PlayerCardAttackOneMiddle::~PlayerCardAttackOneMiddle(void)
{
}

void PlayerCardAttackOneMiddle::Load(void)
{
}

void PlayerCardAttackOneMiddle::Init(void)
{
	//カウントのセット
	midAtkCnt_ = ATTACK_ONE_MID_TIME;

	//近距離攻撃のアニメーションをセット
	anim_.PlayBlend(static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_1_MIDDLE), animVar_);

	midAtkOverCnt_ = ATTACK_ONE_MID_COMBO_TIME;

	//突き攻撃の方向に向く
	actionCntl_.GetInput().GetLookAtTargetDir();
	isTurnable_ = true;
}

void PlayerCardAttackOneMiddle::Update(void)
{
	//攻撃中にカード負けしたら処理を飛ばす
	if (IsCardFailure(Collider::TAG::NML_ATK))
	{
		return;
	}

	//コンボ先行受付
	actionCntl_.ComboInput();

	//キャラ同士で当たったか
	const bool isHitTarget = character_.GetIsHitTarget();
	const float delta = scnMng_.GetDeltaTime();

	//攻撃判定処理
	if (anim_.GetAnimStep(static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_1_MIDDLE)) >= atk_.colStartStep && midAtkCnt_ > 0.0f) { midAtkCnt_ -= delta; }

	//中距離突きカウントが０以上なら
	if (midAtkCnt_ > 0.0f && !atk_.isDamage)
	{
		//スピード減速
		speed_ = easing_->EaseFunc(ATTACK_ONE_MID_SPD, 0.0f
			, (ATTACK_ONE_MID_TIME - midAtkCnt_) / ATTACK_ONE_MID_TIME
			, Easing::EASING_TYPE::QUAD_OUT);

		//攻撃判定生成
		character_.MakeAttackCol(character_.GetCharaTag(), Collider::TAG::NML_ATK, {}, 0.0f);
	}
	else if (midAtkCnt_ <= 0.0f/* || character_.GetIsDamage()*/)		//アニメーション終了でアイドル状態変更
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
			actionCntl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
		}
		else
		{
			actionCntl_.ChangeComboCardAttack();
		}
	}
}

void PlayerCardAttackOneMiddle::Release(void)
{
	//現在使っているカードを捨てる
	cardPresent_.ChangeAction();

	////当たり判定削除
	//character_.DeleteAttackCol(Collider::TAG::PLAYER1, Collider::TAG::NML_ATK);
	//character_.SetIsCanMoveable(true);
}

void PlayerCardAttackOneMiddle::LoadAnimVar(const ACTION_LOAD_DATA& _data)
{
	if (_data.name != "Attack_1_Middle")return;

	animVar_ = _data.animVariable;
	LoadAttackStatus(_data.jsonData, atk_);
}
