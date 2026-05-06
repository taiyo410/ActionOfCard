#include "../pch.h"
#include "../Card/CardUiBase.h"
#include "../Card/CardDeck.h"
#include "../../Manager/Generic/UIManager.h"
#include "CardPresenter.h"

CardPresenter::CardPresenter(CHARACTER_TYPE& _type, CardDeck& _deck):
	deck_(_deck),
	type_(_type),
	uiMng_(UIManager::GetInstance())
{
}

CardPresenter::~CardPresenter(void)
{
}

void CardPresenter::FinishCard(void)
{
	//使用済みへ移行
	uiMng_.GetCardUI(type_).ChangeUsedActionCard();
	deck_.EraseHandCard();
	uiMng_.GetCardUI(type_).ChangeSelectState(CardUIBase::CARD_SELECT::NONE);
}

void CardPresenter::FailureCard(void)
{
	//弾き状態へ移行
	uiMng_.GetCardUI(type_).ChangeReactActionCard();
	deck_.EraseHandCard();
}

void CardPresenter::PutCard(void)
{
	//手札からドロー中に移動
	deck_.MoveUsingCardToDrawPile();

	//決定状態に移行
	uiMng_.GetCardUI(type_).ChangeSelectState(CardUIBase::CARD_SELECT::DISITION);
}


void CardPresenter::RoleRevolver(const CardUIBase::CARD_SELECT _moveLR)
{
	//左右以外のときは処理しない
	if (_moveLR != CardUIBase::CARD_SELECT::LEFT && _moveLR != CardUIBase::CARD_SELECT::RIGHT)return;

	//UIのカードリボルバー回転
	uiMng_.GetCardUI(type_).ChangeSelectState(_moveLR);

	//内部のカードを動かす
	_moveLR == CardUIBase::CARD_SELECT::LEFT ? deck_.CardMoveLeft() : deck_.CardMoveRight();
}

const CardBase::CARD_TYPE CardPresenter::GetCardType(void) const
{
	return deck_.GetDrawCardType();
}

const std::vector<CardBase::CARD_TYPE> CardPresenter::GetHandCardType(void) const
{
	return deck_.GetHandCardType();
}

void CardPresenter::DeckReload(void)
{
	deck_.Reload();
	uiMng_.GetCardUI(type_).ChangeSelectState(CardUIBase::CARD_SELECT::RELOAD);
}

void CardPresenter::ChangeCard(void)
{
	//現在使っているカードを捨てる
	deck_.EraseHandCard();

	//手札に移動
	deck_.MoveUsingCardToDrawPile();

	//UIを使用済み状態へ移行
	uiMng_.GetCardUI(type_).ChangeUsedActionCard();

	//決定状態へ移行
	uiMng_.GetCardUI(type_).ChangeSelectState(CardUIBase::CARD_SELECT::DISITION);
}

void CardPresenter::EnemyCardReload(void)
{
	//内部的なリロード
	deck_.Reload();

	//UIをリロード状態に遷移
	uiMng_.GetCardUI(type_).ChangeSelectState(CardUIBase::CARD_SELECT::RELOAD_WAIT);
}

void CardPresenter::ChangeAction(void)
{
	//使用済みへ移行
	uiMng_.GetCardUI(type_).ChangeUsedActionCard();

	//アクション中カードを消す
	deck_.EraseHandCard();
}

const bool CardPresenter::IsCardFailure(void)const
{
	return deck_.IsCardFailure();
}

void CardPresenter::SetUIReloadCount(const float _per)
{
	uiMng_.GetCardUI(type_).SetReloadCount(_per);
}

void CardPresenter::ChangeUIState(const CardUIBase::CARD_SELECT _select)
{
	uiMng_.GetCardUI(type_).ChangeSelectState(_select);
}

const CardUIBase::CARD_SELECT CardPresenter::GetCardUIState(void) const
{
	return uiMng_.GetCardUI(type_).GetSelectState();
}
