#include "../pch.h"
#include "PlayerCardMagicFire.h"

PlayerCardMagicFire::PlayerCardMagicFire(ActionController& _actCntl, CharacterBase& _charaObj, CardPresenter& _deck):
	CardActionBase(_actCntl, _charaObj, _deck)
{
	//fireBall_ = std::make_unique<PlayerMagicFire>(character_, resMng_, scnMng_, soundMng_);
}

PlayerCardMagicFire::~PlayerCardMagicFire(void)
{
}

void PlayerCardMagicFire::Load(void)
{
}

void PlayerCardMagicFire::Init(void)
{
}

void PlayerCardMagicFire::Update(void)
{
	//if(anim_.)
}

void PlayerCardMagicFire::Release(void)
{
}

void PlayerCardMagicFire::LoadAnimVar(const ACTION_LOAD_DATA& _data)
{
	if (_data.name != "FireMagic")return;

	animVar_ = _data.animVariable;
	LoadAttackStatus(_data.jsonData, atk_);
}
