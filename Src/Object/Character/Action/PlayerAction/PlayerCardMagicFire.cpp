#include "../pch.h"
#include "../ActionController.h"
#include "PlayerCardMagicFire.h"

PlayerCardMagicFire::PlayerCardMagicFire(ActionController& _actCntl, CharacterBase& _charaObj, CardPresenter& _deck):
	PlayerCardAttackBase(_actCntl, _charaObj, _deck)
{
	//fireBall_ = std::make_unique<PlayerMagicFire>(character_, resMng_, scnMng_, soundMng_);
}

PlayerCardMagicFire::~PlayerCardMagicFire(void)
{
}

void PlayerCardMagicFire::Load(void)
{
}

void PlayerCardMagicFire::InitAttack(void)
{
}

void PlayerCardMagicFire::AttackUpdate(void)
{
	//Ç¢Ç¡ÇΩÇÒÉAÉCÉhÉãèÛë‘Ç÷
	actionCntl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
}

void PlayerCardMagicFire::AttackRelease(void)
{
}

void PlayerCardMagicFire::LoadAnimVar(const ACTION_LOAD_DATA& _data)
{
	if (_data.name != "FireMagic")return;

	animVar_ = _data.animVariable;
	LoadAttackStatus(_data.jsonData, atk_);
}
