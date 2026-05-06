#pragma once
#include<vector>
#include<memory>
#include "../../Template/Singleton.h"

class CardDeck;

class CardSystem : 
	public Singleton<CardSystem>
{
	//シングルトン
	friend class Singleton<CardSystem>;

public:

	//プレイヤーの配列番号
	static constexpr int PLAYER_NO = 0;

	//CPUのの配列番号
	static constexpr int CPU_NO = 1;

	//先出しの番号
	static constexpr int FIRST_ATK = 0;

	//後だしの番号
	static constexpr int SECOND_ATK = 1;

	//配列数
	static constexpr int ARRAY_NUM = 2;

	//返す勝敗判定
	enum class BATTLE_RESULT
	{
		NONE,
		SUCCESS_USE,		//成功
		SUCCESS_USE_CONTINUE,//成功(連続攻撃)
		SUCCESS_CARD_BREAK,	//成功(カード破壊)
		FAILURE_USE_BE_REFLECTED,	//失敗(反射された)
		SUCCESS_REFLECT,	//成功(反射)		
		GIVE_DRAW,		//引き分けにした
		BE_DRAW,		//引き分けにされた
		RELOAD			//リロード
	};

	/// @brief ストックカードのカードを比較する
	/// @param  
	void CompareCards(void);
	
	/// @brief カードを場に出す
	/// @param _pow カードの強さ
	/// @param _playerNum プレイヤー番号(誰の結果にセットするか)
	void PutCard(const int _pow,const int _playerNum);

	/// @brief 勝敗結果の取得
	/// @param _cardPlayerNo カード勝負プレイヤーの番号
	/// @return 勝敗結果
	const BATTLE_RESULT GetResult(const int _cardPlayerNo)const;

	/// @brief カード勝負に負けた時の強さの初期化
	/// @param _playerNo 
	void LoseInitPutCardPow(const int _playerNo);
	
	/// @brief 先出し後出しの判定
	/// @param _playerNo 
	void JudgeIsFirstAtk(const int _playerNo);

	/// @brief カード２枚のカード強さの差を取得
	/// @param  
	/// @return  カード２枚のカード強さの差
	const int GetCardDif(void)const { return cardDif_; }

#ifdef _DEBUG
	void DrawDebug(void);
#endif // _DEBUG

private:

	//カードが出ていない状態の強さ
	static constexpr int CARD_POW_NONE = -1;

	//デストラクタ
	CardSystem(void);

	//コピーの禁止
	CardSystem(const CardSystem& _copy) = delete;
	CardSystem& operator=(const CardSystem&) = delete;
	~CardSystem(void)override = default;

	//場に出ているカード
	int putCardPow_[ARRAY_NUM];

	//カードを場に出せるか
	bool canPut_;

	//結果返す時のプレイヤーの識別
	BATTLE_RESULT playerResult_[ARRAY_NUM];

	//攻撃前のカード勝利結果
	BATTLE_RESULT preResult_[ARRAY_NUM];

	//2枚のカードの強さの差
	int cardDif_;

	//先出しかどうか true:先出し
	bool isFirstAtk_[ARRAY_NUM];
};