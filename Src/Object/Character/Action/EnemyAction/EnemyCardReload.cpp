#include "pch.h"
#include "Object/Card/CardPresenter.h"
#include "Object/Character/Action/ActionController.h"
#include "./EnemyCardReload.h"

EnemyCardReload::EnemyCardReload(ActionController& _actCtrl, CharacterBase& _character, CardPresenter& _cardPresent):
	CardActionBase(_actCtrl,_character,_cardPresent)
{
}

EnemyCardReload::~EnemyCardReload(void)
{
}

void EnemyCardReload::Init(void)
{
	//カードをリロードする
	cardPresent_.EnemyCardReload();
}

void EnemyCardReload::Update(void)
{
	actionCtrl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
}

void EnemyCardReload::Release(void)
{
}
