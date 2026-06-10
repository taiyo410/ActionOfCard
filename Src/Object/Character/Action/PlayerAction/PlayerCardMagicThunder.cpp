#include "../pch.h"
#include "../Utility/Utility3D.h"
#include "../Manager//Generic/SceneManager.h"
#include "../ActionController.h"
#include "../Object/Character/Base/CharacterBase.h"
#include "../Object/Character/Player/PlayerMagicThunder.h"
#include "../Object/Character/Base/LogicBase.h"
#include "PlayerCardMagicThunder.h"

PlayerCardMagicThunder::PlayerCardMagicThunder(ActionController& _actCtrl, CharacterBase& _charaObj, CardPresenter& _deck):
	PlayerCardMagicBase(_actCtrl,_charaObj,_deck)
{
	thunder_ = std::make_shared<PlayerMagicThunder>(targetPos_);
}

PlayerCardMagicThunder::~PlayerCardMagicThunder(void)
{
}

void PlayerCardMagicThunder::Load(void)
{
	thunder_->Load();
}

void PlayerCardMagicThunder::InitAttack(void)
{
	targetPos_ = actionCtrl_.GetInput().GetTargetTransform().pos;
	anim_.PlayBlend(static_cast<int>(CharacterBase::ANIM_TYPE::MAGIC_FIRE), animVar_);

	changeState_[STATE::SPELL_CAST]();
}

void PlayerCardMagicThunder::AttackRelease(void)
{
	character_.DeleteItem();
}

void PlayerCardMagicThunder::LoadAnimVar(const ACTION_LOAD_DATA& _data)
{
	if (_data.name != "FireMagic")return;

	animVar_ = _data.animVariable;
	const auto& data = _data.jsonData;
	LoadAttackStatus(data, atk_);
	thunder_->LoadThunderData(data);
}

void PlayerCardMagicThunder::UpdateSpellCast(void)
{
	if (anim_.GetAnimStep(static_cast<int>(CharacterBase::ANIM_TYPE::MAGIC_FIRE)) > atk_.colStartStep)
	{
		changeState_[STATE::SPELL_CAST]();
	}
}

void PlayerCardMagicThunder::UpdateAttack(void)
{
	if (!thunder_->GetIsAlive())
	{
		actionCtrl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
	}
}

void PlayerCardMagicThunder::ChangeSpellCast(void)
{
	updateState_ = [this]() {UpdateSpellCast(); };
}

void PlayerCardMagicThunder::ChangeAttack(void)
{
	//ó‘Ô‘JˆÚ
	thunder_->Init();
	character_.DrawItem(thunder_);
	thunder_->MakeThunderCollider();

	updateState_ = [this]() {UpdateAttack(); };
}
