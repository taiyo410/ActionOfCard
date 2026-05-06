#pragma once
#include<vector>
#include<memory>
#include "../Object/ObjectBase.h"
#include "../../Common/Vector2.h"
#include "../Object/Character/UIData/CharacterUIData.h"
#include "CardBase.h"

class CardBase;

class CardDeck :
	public ObjectBase
{

public:

	/// @brief コンストラクタ
	/// @param _charaType キャラタイプ
	/// @param _playerNum プレイヤー番号
	CardDeck(CHARACTER_TYPE& _charaType,int _playerNum);

	/// @brief デストラクタ
	/// @param  
	~CardDeck(void);

	/// @brief ロード
	/// @param  
	void Load(void)override;

	/// @brief 初期化
	/// @param  
	void Init(void)override;

	/// @brief 更新
	/// @param  
	void Update(void)override;

	/// @brief 描画
	/// @param  
	void Draw(void)override;
	
	/// @brief デュエルデッキにカード追加
	/// @param _status 追加したいカードの強さ
	void AddDuelDeck(const CardBase::CARD_STATUS& _status);

	/// @brief カード使用
	/// @param  
	void CardUseUpdate(void);

	/// @brief 使用中のカードを消す
	/// @param _isLose カード負けかどうか
	void EraseHandCard(const bool _isLose = false);

	/// @brief カード選択
	/// @param  
	void CardMoveLeft(void);
	void CardMoveRight(void);

	/// @brief カードを使用札に移動
	/// @param  
	void MoveUsingCardToDrawPile(void);

	/// @brief デュエルデッキから使用札へ
	/// @param  
	void MoveUsingCardToDuelDeck(void);

	/// @brief デュエルデッキの解放
	/// @param  
	void ClearDuelDeck(void);

	/// @brief デッキからカードを引く
	/// @param  
	void DrawCardFromDeck(void);
	
	/// @brief カード失敗フラグ
	/// @param  
	/// @return true:負け
	const bool IsCardFailure(void)const;

	/// @brief 何もカードが出ていない状態
	/// @param  
	/// @return 
	bool IsNone(void);

	//カード勝利フラグ	
	bool IsCardWin(void);
	
	/// @brief カードの種類(アタックカードか魔法カードか)の取得
	/// @param  
	/// @return カードの種類
	std::vector<CardBase::CARD_TYPE> GetHandCardType(void);

	/// @brief 現在選択中のカードがリロードカードかを返す
	/// @param  
	/// @return 現在選択中のカード
	const CardBase::CARD_TYPE GetDrawCardType(void);

	/// @brief リロードする
	/// @param  
	void Reload(void);

	/// @brief 使用中カードからチャージカードへ移動
	/// @param  
	void MoveChargeToUsingCard(void);

private:

	//チャージカード最大枚数
	static constexpr int CHARGE_MAX = 3;

	//リロードカードの強さ
	static constexpr int RELOAD_CARD_POW = -1;

	//プレイヤーのカードデータが入っている文字列
	const std::string PLAYER_CARD_PATH = "PlayerCards";

	//敵のカードデータが入っている文字列
	const std::string ENEMY_CARD_PATH = "EnemyCards";

	//カードの種類のデータが入っている文字列
	const std::string CARD_TYPE_PATH = "type";

	//カードパワーを読み込むパス
	const std::string CARD_POWER_PATH = "pow";
	
	//カードを選択したときの制限
	void CardMoveLimit(void);

	/// @brief 山札にカード追加
	/// @param _status 追加したいカードの強さ
	void AddDrawPile(const CardBase::CARD_STATUS& _status);

	//カードデータの読み込み
	void LoadCardData(void);

	//札関連
	//初期札
	std::vector<std::unique_ptr<CardBase>>initDeck_;

	//山札
	std::vector<std::unique_ptr<CardBase>>drawPile_;

	//今使っているカード
	std::vector<std::unique_ptr<CardBase>>usingCards_;

	//チャージ中カード
	std::vector<std::unique_ptr<CardBase>>chargeCard_;

	//捨て札
	std::vector<std::unique_ptr<CardBase>>disCard_;

	//敵用のデュエルデッキ
	std::vector<std::unique_ptr<CardBase>>enemyDuelDeck_;

	//現在選択中のカード
	int currentNum_;

	//カード使用者のプレイヤー番号
	int playerNum_;

	//敵デュエルデッキの現在選択中番号
	int duelNo_;

	//キャラクタータイプ
	CHARACTER_TYPE& charaType_;

	//カード種類の文字列との対応マップ
	std::unordered_map<std::string, CardBase::CARD_TYPE> cardTypeMap_;
};