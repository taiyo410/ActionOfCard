#pragma once
#include <vector>
#include <memory>
#include <unordered_map>
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

	//
	enum class ORDER_RULE
	{
		NORMAL,			//通常
		REVOLUTION		//革命(小さい値が勝つルール)
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

	/// @brief カードの勝敗判定を変更する
	/// @param  
	void ChangeJudgeRule(void);

	/// @brief カード判定ルールの取得
	/// @param  
	/// @return 
	const ORDER_RULE GetJudgeRule(void) { return rule_; }

#ifdef _DEBUG
	void DrawDebug(void);
#endif // _DEBUG

private:

#pragma region メンバー定数
	//カードが出ていない状態の強さ
	static constexpr int CARD_POW_NONE = -1;
#pragma endregion


#pragma region メンバー変数
	std::function<std::array<BATTLE_RESULT, ARRAY_NUM>(void)>battleRuleFunc_;	//カード比較
	std::unordered_map<ORDER_RULE, std::function<void(void)>>changeBattleRule_;
	int putCardPow_[ARRAY_NUM];				//場に出ているカード
	bool canPut_;							//カードを場に出せるか
	BATTLE_RESULT playerResult_[ARRAY_NUM];	//結果返す時のプレイヤーの識別
	BATTLE_RESULT preResult_[ARRAY_NUM];	//攻撃前のカード勝利結果
	ORDER_RULE rule_;						//勝敗ルール
	int cardDif_;							//2枚のカードの強さの差
	bool isFirstAtk_[ARRAY_NUM];			//先出しかどうか true:先出し
#pragma endregion

#pragma region メンバー関数
	//デストラクタ
	CardSystem(void);

	//コピーの禁止
	CardSystem(const CardSystem& _copy) = delete;
	CardSystem& operator=(const CardSystem&) = delete;
	~CardSystem(void)override = default;

	//カード勝敗判定の取得
	const std::array<BATTLE_RESULT,ARRAY_NUM> GetNormalResult(void);

	//ルール
	std::array<BATTLE_RESULT, ARRAY_NUM> NormalRuleFunc(void);		//通常
	std::array<BATTLE_RESULT, ARRAY_NUM> RevolutionRuleFunc(void);	//革命ルール

	std::wstring str[2];
#pragma endregion
};