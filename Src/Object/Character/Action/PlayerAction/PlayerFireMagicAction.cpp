#include "../pch.h"
#include "PlayerFireMagicAction.h"

PlayerFireMagicAction::PlayerFireMagicAction(ActionController& _actCntl, CharacterBase& _charaObj, CardPresenter& _deck):
	CardActionBase(_actCntl, _charaObj, _deck)
{
	//fireBall_ = std::make_unique<PlayerMagicFire>(character_, resMng_, scnMng_, soundMng_);
}

PlayerFireMagicAction::~PlayerFireMagicAction(void)
{
}

void PlayerFireMagicAction::Load(void)
{
}

void PlayerFireMagicAction::Init(void)
{
}

void PlayerFireMagicAction::Update(void)
{
	//if(anim_.)
}

void PlayerFireMagicAction::Release(void)
{
}

void PlayerFireMagicAction::LoadAnimVar(const ACTION_LOAD_DATA& _data)
{
	if (_data.name != "FireMagic")return;

	animVar_ = _data.animVariable;
	LoadAttackStatus(_data.jsonData, atkStatus_);
}
