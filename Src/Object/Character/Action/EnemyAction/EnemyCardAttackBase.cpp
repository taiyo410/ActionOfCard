#include "../pch.h"
#include "../../../Common/AnimationController.h"
#include "../Object/Common/EffectController.h"
#include "../Utility/Utility3D.h"
#include "../Manager/Generic/Camera.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Resource/SoundManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../ActionController.h"
#include "../../Base/CharacterBase.h"
#include"../../Enemy/EnemyLogic.h"
#include "EnemyCardAttackBase.h"

EnemyCardAttackBase::EnemyCardAttackBase(ActionController& _actCntl, CharacterBase& _charaObj, CardPresenter& _deckPresent):
	CardActionBase(_actCntl, _charaObj, _deckPresent)
{
}

EnemyCardAttackBase::~EnemyCardAttackBase(void)
{
}

void EnemyCardAttackBase::Init(void)
{
	atk_.isDamage = false;
	isTurnable_ = false;
	atkCnt_ = 0.0f;	
	InitAttack();
}

void EnemyCardAttackBase::Update(void)
{
	//攻撃中にカード負けしたら処理を飛ばす
	if (IsCardFailure(Collider::TAG::NML_ATK))
	{
		//攻撃判定無効
		FinishFailureAttack(Collider::TAG::NML_ATK);
		return;
	}

	//攻撃処理
	AttackUpdate();

	//エフェクト更新
	effect_->Update();
}

void EnemyCardAttackBase::Release(void)
{
	//カメラシェイクを元に戻す
	scnMng_.GetCamera().lock()->ChangeSub(Camera::SUB_MODE::NONE);

	actionCtrl_.GetInput().SetIsActioning(false);

	//現在使っているカードを捨てる
	cardPresent_.FinishCard();

	//硬直時間セット
	actionCtrl_.GetInput().SetFreezeCntByAttackType();

	AttackRelease();
}
