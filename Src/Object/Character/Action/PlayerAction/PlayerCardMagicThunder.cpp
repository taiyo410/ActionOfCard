#include "../pch.h"
#include "../Utility/Utility3D.h"
#include "../Manager//Generic/SceneManager.h"
#include "../ActionController.h"
#include "../Object/Character/Base/CharacterBase.h"
#include "../Object/Character/Player/PlayerMagicThunder.h"
#include "../Object/Character/Base/LogicBase.h"
#include "PlayerCardMagicThunder.h"

PlayerCardMagicThunder::PlayerCardMagicThunder(ActionController& _actCtrl, CharacterBase& _charaObj, CardPresenter& _deck):
	PlayerCardAttackBase(_actCtrl,_charaObj,_deck)
{
	thunder_ = std::make_shared<PlayerMagicThunder>(targetPos_);
}

PlayerCardMagicThunder::~PlayerCardMagicThunder(void)
{
}

void PlayerCardMagicThunder::Load(void)
{
}

void PlayerCardMagicThunder::InitAttack(void)
{
	targetPos_ = actionCtrl_.GetInput().GetTargetTransform().pos;
}

void PlayerCardMagicThunder::AttackUpdate(void)
{
	if (anim_.GetAnimStep(static_cast<int>(CharacterBase::ANIM_TYPE::MAGIC_FIRE)) > atk_.colStartStep)
	{
		//ó‘Ô‘JˆÚ
		thunder_->MakeThunderCollider();
		return;
	}
}

void PlayerCardMagicThunder::AttackRelease(void)
{
}

void PlayerCardMagicThunder::LoadAnimVar(const ACTION_LOAD_DATA& _data)
{
	if (_data.name != "FireMagic")return;

	animVar_ = _data.animVariable;
	const auto& data = _data.jsonData;
	LoadAttackStatus(data, atk_);
}
