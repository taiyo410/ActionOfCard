#include "../pch.h"
#include "../Utility/Utility3D.h"
#include "../Manager//Generic/SceneManager.h"
#include "../ActionController.h"
#include "../Object/Character/Base/CharacterBase.h"
#include "../Object/Character/Player/PlayerMagicFIre.h"
#include "../Object/Character/Base/LogicBase.h"
#include "PlayerCardMagicFire.h"

PlayerCardMagicFire::PlayerCardMagicFire(ActionController& _actCntl, CharacterBase& _charaObj, CardPresenter& _deck):
	PlayerCardMagicBase(_actCntl, _charaObj, _deck)
{
	fireBall_ = std::make_shared<PlayerMagicFire>(atk_.pos, toTargetDir_);
	useAnimNum_ = static_cast<int>(CharacterBase::ANIM_TYPE::MAGIC_FIRE);
}

PlayerCardMagicFire::~PlayerCardMagicFire(void)
{
}

void PlayerCardMagicFire::Load(void)
{
	fireBall_->Load();
}

void PlayerCardMagicFire::InitAttack(void)
{
	//アニメーションの再生
	anim_.PlayBlend(static_cast<int>(CharacterBase::ANIM_TYPE::MAGIC_FIRE),animVar_);

	//攻撃座標をセット
	const Transform& trans = character_.GetTransform();
	atk_.pos = Utility3D::AddPosRotate(trans.pos, trans.quaRot, fireLocalPos_);

	//敵への方向をセット
	toTargetDir_ = actionCtrl_.GetInput().GetToTargetDir();

	actCnt_ = 0.0f;

	//炎を出現させる
	fireBall_->Init();
	character_.DrawItem(fireBall_);
	changeState_[STATE::SPELL_CAST]();
}

void PlayerCardMagicFire::AttackRelease(void)
{
	character_.DeleteItem();
	fireBall_->Release();
}

void PlayerCardMagicFire::LoadAnimVar(const ACTION_LOAD_DATA& _data)
{
	if (_data.name != "FireMagic")return;

	animVar_ = _data.animVariable;
	const auto& data = _data.jsonData;
	LoadAttackStatus(data, atk_);

	//炎側に外部ファイルを取得させる
	fireBall_->LoadFireData(data);

	actMaxTime_ = data.value("actionMaxTime", 0.0f);

	//炎出現座標のローカル座標
	if (data.contains("fireStartLocalPos"))
	{
		const auto localPosData = data["fireStartLocalPos"];
		fireLocalPos_.x = localPosData.value("x", 0.0f);
		fireLocalPos_.y = localPosData.value("y", 0.0f);
		fireLocalPos_.z = localPosData.value("z", 0.0f);
	}
	
}

void PlayerCardMagicFire::UpdateSpellCast(void)
{
	if (anim_.GetAnimStep(static_cast<int>(CharacterBase::ANIM_TYPE::MAGIC_FIRE)) > atk_.colStartStep)
	{
		//状態遷移
		changeState_[STATE::ATTACK]();
	}
}

void PlayerCardMagicFire::UpdateMagicAttack(void)
{
	//アクション時間のカウント
	actCnt_ += scnMng_.GetDeltaTime();

	//ファイアボールが死ぬ、または、攻撃してから一定時間経過後にアイドルへ
	//ファイアボールがどこにも当たらずにずっとアクションを続けることを阻止するため
	if (!fireBall_->GetIsAlive() || actCnt_ > actMaxTime_)
	{
		actionCtrl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
		return;
	}
	fireBall_->Update();
}

void PlayerCardMagicFire::ChangeSpellCast(void)
{
	updateState_ = [this]() {UpdateSpellCast(); };
}

void PlayerCardMagicFire::ChangeMagicAttack(void)
{
	//炎の当たり判定作成
	fireBall_->MakeFireBallCollider();
	updateState_ = [this]() {UpdateMagicAttack(); };
}
