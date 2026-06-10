#include "../pch.h"
#include "PlayerCardMagicBase.h"

PlayerCardMagicBase::PlayerCardMagicBase(ActionController& _actCntl, CharacterBase& _charaObj, CardPresenter& _deck):
	PlayerCardAttackBase(_actCntl, _charaObj, _deck)
{
	changeState_ = {
	{STATE::SPELL_CAST,[this]() {ChangeSpellCast(); }},
	{STATE::ATTACK,[this]() {ChangeAttack(); }}
	};
}

PlayerCardMagicBase::~PlayerCardMagicBase(void)
{
}

void PlayerCardMagicBase::AttackUpdate(void)
{
	updateState_();
}
