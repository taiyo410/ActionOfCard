#include "../Utility/UtilityCommon.h"
#include "CardBase.h"
#include"CardDeck.h"
#include "CardSystem.h"

CardSystem::CardSystem(void):
	putCardPow_{ CARD_POW_NONE,CARD_POW_NONE },
	canPut_(true),
	isFirstAtk_{ false,false },
	playerResult_{BATTLE_RESULT::NONE,BATTLE_RESULT::NONE },
	preResult_{ BATTLE_RESULT::NONE,BATTLE_RESULT::NONE },
	cardDif_()

{
}

const CardSystem::BATTLE_RESULT CardSystem::GetResult(const int _cardPlayerNo) const
{
	//配列数より大きい数字を指定されたら何も返さない
	if (_cardPlayerNo >= ARRAY_NUM)return CardSystem::BATTLE_RESULT::NONE;

	CardSystem::BATTLE_RESULT result = playerResult_[_cardPlayerNo];

	return result;
}

void CardSystem::LoseInitPutCardPow(const int _playerNo)
{
	if (isFirstAtk_[_playerNo] == true)
	{
		//先出しのカードを消す
		putCardPow_[FIRST_ATK] = CARD_POW_NONE;
		isFirstAtk_[_playerNo] = false;
		playerResult_[_playerNo] = BATTLE_RESULT::NONE;

		//後出しのカードを先出しにする
		if (putCardPow_[SECOND_ATK] != CARD_POW_NONE)
		{
			//先出のカードの強さに後だしのカードの強さを移動
			putCardPow_[FIRST_ATK] = putCardPow_[SECOND_ATK];

			//後だしカードの強さは初期化
			putCardPow_[SECOND_ATK] = CARD_POW_NONE;

			//後だしを先出判定にする
			for(int i=0;i<ARRAY_NUM;i++)
			{
				if (i!=_playerNo)
				{
					isFirstAtk_[i] = true;
					break;
				}
			}
		}
	}
	else
	{
		putCardPow_[SECOND_ATK] = CARD_POW_NONE;
		playerResult_[_playerNo] = BATTLE_RESULT::NONE;
	}
}

void CardSystem::JudgeIsFirstAtk(const int _playerNo)
{
	//どちらも値が-1でカードが出されていない場合、先出フラグをtrueにする
	if (putCardPow_[FIRST_ATK] == CARD_POW_NONE && putCardPow_[SECOND_ATK] == CARD_POW_NONE)
	{
		isFirstAtk_[_playerNo] = true;
	}
}

#ifdef _DEBUG
void CardSystem::DrawDebug(void)
{

}
#endif // _DEBUG

void CardSystem::CompareCards(void)
{
	//どちらかにカードが入っていなければ引ける
	if (putCardPow_[FIRST_ATK] == -1 || putCardPow_[SECOND_ATK] == -1)
	{
		canPut_ = true;
	}
	//どちらも入っていれば引けない
	else if(putCardPow_[FIRST_ATK] != CARD_POW_NONE && putCardPow_[SECOND_ATK] != CARD_POW_NONE)
	{
		canPut_ = false;
	}
	//お互いに手札が0枚なら処理を抜ける
	if (putCardPow_[FIRST_ATK]== CARD_POW_NONE && putCardPow_[SECOND_ATK] == CARD_POW_NONE)
	{
		cardDif_ = 0;
		return;
	}

	//以下、2つのカードの強さを比較
	//どちらかのカードが出されていなければ先出し勝利
	BATTLE_RESULT result[ARRAY_NUM];
	if (putCardPow_[SECOND_ATK] == CARD_POW_NONE)
	{
		result[FIRST_ATK] = BATTLE_RESULT::SUCCESS_USE;
	}
	//先出しと後出しカードが同じ強さの時はドロー
	else if (putCardPow_[FIRST_ATK] == putCardPow_[SECOND_ATK])
	{
		//引き分け
		result[FIRST_ATK] = BATTLE_RESULT::BE_DRAW;
		result[SECOND_ATK] = BATTLE_RESULT::GIVE_DRAW;


		//カードを両方リセット
		putCardPow_[FIRST_ATK] = CARD_POW_NONE;
		putCardPow_[SECOND_ATK] = CARD_POW_NONE;
	}
	//先出しの勝ち
	else if (putCardPow_[FIRST_ATK] > putCardPow_[SECOND_ATK])
	{
		result[FIRST_ATK] = BATTLE_RESULT::SUCCESS_CARD_BREAK;
		result[SECOND_ATK] = BATTLE_RESULT::FAILURE_USE_BE_REFLECTED;

		//2枚のカードの強さを計算する(カードの差が低いほどダメージを上げるための計算)
		cardDif_ = putCardPow_[FIRST_ATK] - putCardPow_[SECOND_ATK];
	}
	//後出しの勝ち
	else if (putCardPow_[FIRST_ATK] < putCardPow_[SECOND_ATK])
	{
		result[FIRST_ATK] = BATTLE_RESULT::FAILURE_USE_BE_REFLECTED;
		result[SECOND_ATK] = BATTLE_RESULT::SUCCESS_CARD_BREAK;

		//2枚のカードの強さを計算する(カードの差が低いほどダメージを上げるための計算)
		cardDif_ = putCardPow_[SECOND_ATK] - putCardPow_[FIRST_ATK];
	}

	//各プレイヤーの結果に判定結果を反映する
	for (int i = 0; i < ARRAY_NUM; i++)
	{
		if (isFirstAtk_[i])
		{
			playerResult_[i] = result[FIRST_ATK];
		}
		else
		{
			playerResult_[i] = result[SECOND_ATK];
		}
		preResult_[i] = result[i];
	}
}

void CardSystem::PutCard(const int _pow, const int _playerNum)
{
	//すでに強さが代入されていれば処理を抜ける
	if (putCardPow_[PLAYER_NO] != CARD_POW_NONE && putCardPow_[CPU_NO] != CARD_POW_NONE)return;
	//カードの強さをセットする
	if (isFirstAtk_[_playerNum])
	{
		putCardPow_[FIRST_ATK] = _pow;
	}
	else
	{
		putCardPow_[SECOND_ATK] = _pow;
	}
}

