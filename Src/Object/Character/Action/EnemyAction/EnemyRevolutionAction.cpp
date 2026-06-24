#include "pch.h"
#include "Object/CHaracter/Base/CharacterBase.h"
#include "Object/Character/Action/ActionController.h"
#include "EnemyRevolutionAction.h"

EnemyRevolutionAction::EnemyRevolutionAction(ActionController& _actCntl, CharacterBase& _character):
	ActionBase(_actCntl,_character)
{
}

EnemyRevolutionAction::~EnemyRevolutionAction(void)
{
}

void EnemyRevolutionAction::Load(void)
{
}

void EnemyRevolutionAction::Init(void)
{
	anim_.PlayBlend(static_cast<int>(CharacterBase::ANIM_TYPE::REVOLUTION), animVar_);
}

void EnemyRevolutionAction::Update(void)
{
	if()
}

void EnemyRevolutionAction::LoadAnimVar(const ACTION_LOAD_DATA& _data)
{
	if (_data.name != "StompAttack")return;

	animVar_ = _data.animVariable;
}
