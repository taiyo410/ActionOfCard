#include "../pch.h"
#include"../../Application.h"
#include "CardBase.h"
#include"./CardSystem.h"
#include "../Manager/Generic/DataBank.h"
#include "../Manager/Resource/ResourceManager.h"
#include"../Utility/UtilityTemplates.h"
#include "../Utility/UtilityCommon.h"
#include "CardDeck.h"

CardDeck::CardDeck(CHARACTER_TYPE& _charaType, int _playerNum):
	drawPile_(),
	currentNum_(0),
	charaType_(_charaType),
	playerNum_(_playerNum),
	duelNo_()
{
	cardTypeMap_= {
		{"Attack", CardBase::CARD_TYPE::ATTACK},
	};
}

CardDeck::~CardDeck(void)
{
	drawPile_.clear();
	usingCards_.clear();
	chargeCard_.clear();
	disCard_.clear();
	initDeck_.clear();
}

void CardDeck::Load(void)
{
}

void CardDeck::Init(void)
{
	//カードデータの読み込み
	LoadCardData();

	//カードUIの選択番号が1番なので1に初期化する
	currentNum_ = 0;
}

void CardDeck::CardUseUpdate(void)
{
	CardSystem& cardSystem = CardSystem::GetInstance();
	//カード同士を比べる
	cardSystem.CompareCards();
}

void CardDeck::EraseHandCard(const bool _isLose)
{
	usingCards_.clear();

	//使ったカードの配列を消す
	UtilityTemplates::EraseVectorArray(usingCards_);
	CardSystem::GetInstance().LoseInitPutCardPow(playerNum_);
}

void CardDeck::CardMoveRight(void)
{
	//指令はプレイヤーやエネミーから出す
	currentNum_--;
	//カード制限
	CardMoveLimit();
}

void CardDeck::CardMoveLeft(void)
{
	//指令はプレイヤーやエネミーから出す
	currentNum_++;
	//カード制限
	CardMoveLimit();
}

void CardDeck::Update(void)
{
}

void CardDeck::Draw(void)
{
}

void CardDeck::AddDrawPile(const CardBase::CARD_STATUS& _status)
{
	std::unique_ptr<CardBase>card = std::make_unique<CardBase>(_status);
	std::unique_ptr<CardBase>initCard = std::make_unique<CardBase>(_status);
	drawPile_.emplace_back(std::move(card)); 
	initDeck_.emplace_back(std::move(initCard));
}

void CardDeck::LoadCardData(void)
{
	std::string charaTypeStr;
	//キャラタイプを判断して、jsonのどこからデータを取るか決める
	charaType_ == CHARACTER_TYPE::PLAYER ? charaTypeStr = PLAYER_CARD_PATH : charaTypeStr = ENEMY_CARD_PATH;
	using json = nlohmann::json;

	//Jsonのデータを取得
	json j = resMng_.Load(ResourceManager::SRC::CHARA_DATA).jsonData;
	std::vector<CardBase::CARD_STATUS> cards;

	//カードデータの読み込み
	for (const auto& card : j[charaTypeStr])
	{
		CardBase::CARD_STATUS status;
		//カードの強さ
		if (card.contains(CARD_POWER_PATH))
		{
			status.pow = card.value(CARD_POWER_PATH, 0);
		}

		//カードの種類
		if (card.contains(CARD_TYPE_PATH))
		{
			std::string typeStr = card[CARD_TYPE_PATH];
			status.type = cardTypeMap_[typeStr];
		}

		//配列の中に追加
		AddDrawPile(status);
	}

	//末尾に必ずリロードカードを追加する
	AddDrawPile(CardBase::CARD_STATUS{ RELOAD_CARD_POW,CardBase::CARD_TYPE::RELOAD });
}

void CardDeck::AddDuelDeck(const CardBase::CARD_STATUS& _status)
{
	std::unique_ptr<CardBase>card = std::make_unique<CardBase>(_status);
	enemyDuelDeck_.emplace_back(std::move(card));
}

void CardDeck::MoveUsingCardToDrawPile(void)
{
	//山札にあるカードを手札に加える
	usingCards_.emplace_back(std::move(drawPile_[currentNum_]));

	//山札からカードを削除する
	UtilityTemplates::EraseVectorArray(drawPile_);
	if (currentNum_ > static_cast<int>(drawPile_.size()) - 1)
	{
		currentNum_ = 0;
	}
	//システム側の処理
	DrawCardFromDeck();
}

void CardDeck::MoveUsingCardToDuelDeck(void)
{
	//山札にあるカードを手札に加える
	usingCards_.emplace_back(std::move(enemyDuelDeck_[currentNum_]));

	//山札からカードを削除する
	UtilityTemplates::EraseVectorArray(enemyDuelDeck_);
	if (currentNum_ > static_cast<int>(enemyDuelDeck_.size()) - 1)
	{
		currentNum_ = 0;
	}
	//システム側の処理
	DrawCardFromDeck();
}

void CardDeck::ClearDuelDeck(void)
{
	enemyDuelDeck_.clear();
}

void CardDeck::DrawCardFromDeck(void)
{
	//使うカードを手札に加える
	int cardPow = 0;

	//合計値を出す
	cardPow = std::accumulate(usingCards_.begin(), usingCards_.end(), 0,
		[](int acc, const std::unique_ptr<CardBase>& it)
		{
			return acc + it->GetCardStatus().pow;
		});

	//カードを場に出してシステム側で処理をする
	CardSystem& cardSystem = CardSystem::GetInstance();

	//先出し後だしの判定
	cardSystem.JudgeIsFirstAtk(playerNum_);

	//カードを場に出す
	cardSystem.PutCard(cardPow, playerNum_);
}

std::vector<CardBase::CARD_TYPE> CardDeck::GetHandCardType(void)
{
	std::vector<CardBase::CARD_TYPE>handCardTypes;
	for (const auto& h : usingCards_)
	{
		CardBase::CARD_TYPE type = h->GetCardStatus().type;
		handCardTypes.emplace_back(type);
	}
	return handCardTypes;
}

const CardBase::CARD_TYPE CardDeck::GetDrawCardType(void)
{
	return drawPile_[currentNum_]->GetCardStatus().type;
}

void CardDeck::Reload(void)
{
	drawPile_.clear();

	//手札にデッキを入れていく
	for (const auto& deck : initDeck_)
	{
		CardBase* newCard = new CardBase(*deck);
		std::unique_ptr<CardBase>deckPtr(newCard);
		drawPile_.emplace_back(std::move(deckPtr));
	}

	//それぞれの番号の初期化
	currentNum_ = 0;
}

void CardDeck::MoveChargeToUsingCard(void)
{
	const int cardSize = static_cast<int>(usingCards_.size());
	if (cardSize >= CHARGE_MAX)
	{
		//ここは絶対通らない
		return;
	}
	//チャージ札に移動
	chargeCard_.emplace_back(std::move(usingCards_[0]));
	usingCards_.clear();

	//負けた時の初期化
	CardSystem::GetInstance().LoseInitPutCardPow(playerNum_);
}

void CardDeck::CardMoveLimit(void)
{
	int cardMax = static_cast<int>(drawPile_.size());
	//現在の番号が０未満なら
	if (currentNum_ < 0)
	{
		//最後の数にする
		currentNum_ = cardMax - 1;
	}
	else if (currentNum_ > cardMax - 1)
	{
		//最大数になったら初めに戻す
		currentNum_ = 0;
	}
}

const bool CardDeck::IsCardFailure(void)const
{
	CardSystem::BATTLE_RESULT result = CardSystem::GetInstance().GetResult(playerNum_);
	using RESULT = CardSystem::BATTLE_RESULT;
	return result == RESULT::FAILURE_USE_BE_REFLECTED || result == RESULT::BE_DRAW
		|| result == RESULT::GIVE_DRAW;
}

bool CardDeck::IsNone(void)
{
	CardSystem::BATTLE_RESULT result = CardSystem::GetInstance().GetResult(playerNum_);
	using RESULT = CardSystem::BATTLE_RESULT;
	return result == RESULT::NONE;
}

bool CardDeck::IsCardWin(void)
{
	CardSystem::BATTLE_RESULT result = CardSystem::GetInstance().GetResult(playerNum_);
	using RESULT = CardSystem::BATTLE_RESULT;
	return result == RESULT::SUCCESS_CARD_BREAK || result == RESULT::SUCCESS_REFLECT
		|| result == RESULT::SUCCESS_USE|| result == RESULT::SUCCESS_USE_CONTINUE;
}


