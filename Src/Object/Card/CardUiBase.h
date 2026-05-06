#pragma once
#include <functional>
#include <map>
#include <unordered_map>
#include <memory>
#include <vector>
#include <list>
#include "../Manager/Resource/SoundManager.h"
#include "../ObjectBase.h"
#include "CardUIBase.h"
#include "./CardBase.h"
#include "../../Common/Vector2.h"
#include "../../Common/Vector2F.h"
#include"../Application.h"

class PixelMaterial;
class PixelRenderer;
class CardUIController;
class CardUIDraw;
class Easing;

class CardUIBase
{

public:

	//カード状態()カード単体
	enum class CARD_STATE
	{
		DRAW_PILE		//山札
		, MOVE_DRAW		//山札から手札へ移動
		, USING			//使用中
		, REACT			//弾かれ中
		, USED			//使用済み
	};

	//カード選択状態(UI全体の状態)
	enum class CARD_SELECT
	{
		NONE
		, LEFT			//左回転
		, RIGHT			//右回転
		, DISITION		//決定
		, RELOAD_WAIT	//リロード待機
		, RELOAD		//リロード
	};

	//カードサイズ補完時間
	static constexpr double SCL_LERP_TIME = 0.5;
	
	//カードUIの情報
	struct CARD_UI_INFO
	{
		CardBase::CARD_STATUS status;																	//カードのステータス
		int typeImg = -1;																				//カードの種類画像
		Vector2F cardPos_ = { Application::SCREEN_SIZE_X + 180,Application::SCREEN_HALF_Y * 222 };		//カードの座標(画面外で初期化)
		Vector2F numPos_ = { Application::SCREEN_SIZE_X + 180,Application::SCREEN_HALF_Y * 222 };		//カードの強さ番号座標(画面外で初期化)
		float currentAngle_ = 0.0f;																		//カードの現在の角度
		float goalAngle_ = currentAngle_;																//カードの目標の角度
		double cardScl_ = 1.0;
		double sclCnt = SCL_LERP_TIME;
		float disitionCnt_;																				//決定カウント
		float reactCnt_;																				//はじかれるカウント
		CARD_STATE state_ = CARD_STATE::DRAW_PILE;														//カードの状態
	};

	/// @brief コンストラクタ
	/// @param  
	CardUIBase(void);

	/// @brief  デストラクタ
	/// @param  
	virtual ~CardUIBase(void);

	/// @brief ロード
	/// @param  
	virtual void Load(void);

	/// @brief 初期化
	/// @param  
	virtual void Init(void);

	/// @brief 更新
	/// @param  
	virtual void Update(void);

	/// @brief 描画
	/// @param
	virtual void Draw(void);

	/// @brief UI配列に追加
	/// @param _status 
	void AddCardUi(const CardBase::CARD_STATUS _status);

	/// @brief カード選択状態変更
	/// @param _select 変更したい状態
	void ChangeSelectState(const CARD_SELECT _select);

	/// @brief アクションカード配列の状態を使用済みにする
	/// @param  
	void ChangeUsedActionCard(void);

	/// @brief アクションカード配列の状態を弾かれ中にする(カード負けしたとき)
	/// @param  
	void ChangeReactActionCard(void);

	/// @brief リロードカウントセット
	/// @param _cnt 
	void SetReloadCount(const float _cnt) { reloadPer_ = _cnt; };

	/// @brief カードリボルバーの状態の取得
	/// @param  
	/// @return カードリボルバーの状態
	const CARD_SELECT GetSelectState(void)const { return selectState_; }

	/// @brief カード初期化
	/// @param  
	virtual void InitCardUI(void) = 0;

	/// @brief カード勝敗サウンド再生
	/// @param  
	void PlayCardSound(void);

	/// @brief カードのロード
	/// @param  
	void LoadCardData(void);

protected:

	//Json読み込みパス
	const std::string JSON_DATA_PATH = "Data/Json/CharaData.json";

	//Jsonで読み込むキャラクターカードの文字列
	const std::string JSON_PLAYER_STR = "PlayerCards";
	const std::string JSON_ENEMY_STR = "EnemyCards";

	//カードステータスの文字列
	const std::string CARD_POWER_PATH = "pow";	//カードの強さのデータ格納文字列
	const std::string CARD_TYPE_PATH = "type";//カードの種類のデータ格納文字列

	//決定したカードの座標
	static constexpr Vector2F DISITON_CARD_POS = { Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y + 200 };

	//弾かれる前のゴール座標
	static constexpr Vector2F REACT_START_CARD_POS = { Application::SCREEN_HALF_X-100.0f, Application::SCREEN_HALF_Y + 200.0f };

	//デルタ(処理が重くなった時にカードuiが壊れるので定数でカウントする)
	static constexpr float DELTA = 1.0f/60.0f;

	//カード強さ最大値
	static constexpr int MAX_CARD_POWER = 9;

	//リロードカードの強さ格納用
	static constexpr int RELOAD_CARD_POWER = -1;

	//カードUIの大きさ
	static constexpr int GRAPH_SIZE_X = 120;
	static constexpr int GRAPH_SIZE_Y = 160;

	//数字画像の大きさ
	static constexpr float NUM_SCL = 0.18f;

	//カードのナンバーローカル座標
	static constexpr Vector2F NUM_LOCAL_POS = { 89.0f,130.0f };



	//カード更新関数
	std::function<void(void)>cardUpdate_;

	//状態遷移
	std::unordered_map<CARD_SELECT, std::function<void(void)>>changeMoveState_;

	//タイプ画像
	std::unordered_map<CardBase::CARD_TYPE, int> cardTypeImgs_;

	//初期カード
	std::list<std::shared_ptr<CardUIController>>initialCards_;

	//手札
	std::list<std::shared_ptr<CardUIController>>handCards_;

	//手札の現在選択中カード
	std::list<std::shared_ptr<CardUIController>>::iterator handCurrent_;

	//アクション中カード
	std::list<std::shared_ptr<CardUIController>>actions_;

	//シェーダー関連
	std::unique_ptr<PixelMaterial>material_;
	std::unique_ptr<PixelRenderer>renderer_;

	//イージング
	std::unique_ptr<Easing>easing_;

	//キャラクター種別(jsonで読み込むため)
	std::string charaType_;

	//カードタイプと文字列の対応マップ
	std::unordered_map<std::string, CardBase::CARD_TYPE> cardTypeMap_;

	//円形UIの中心座標
	Vector2 centerPos_;

	//カード番号イメージ
	int* cardNoImg_;

	//攻撃カード画像
	int atkCardImg_;

	//リロードカード画像
	int reloadCardImg_;

	//リロードゲージ
	int reloadGauge_;		

	//カードナンバー座標
	Vector2F numPos_;

	//カードセレクトの動き時間
	float cardMoveCnt_;

	//決定後のカウント
	float disitionCnt_;

	//リロード割合(カードのゲージ計算用)
	float reloadPer_;

	//状態
	CARD_SELECT selectState_;

	//マネージャ関連
	SoundManager& soundMng_;		//サウンド
	ResourceManager& resMng_;		//リソース
	SceneManager& scnMng_;			//シーン

	//カード勝ったとき流すサウンド
	ResourceManager::SRC cardWinRes_;

	//手札選択カードの計算
	void AddHandCurrent(void);	//足し算
	void SubHandCurrent(void);	//引き算

	//カード状態遷移
	virtual void ChangeNone(void) {};		//通常
	virtual void ChangeLeft(void) {};		//左に移動
	virtual void ChangeRight(void) {};		//右に移動
	virtual void ChangeDecision(void) {};	//決定
	virtual void ChangeReloadWait(void) {};	//リロード待機(チャージ状態)

	//更新系
	virtual void UpdateNone(void) {};		//通常
	virtual void UpdateLeft(void) {};		//左に移動
	virtual void UpdateRight(void) {};		//右に移動
	virtual void UpdateDecision(void) {};	//決定
	virtual void UpdateReloadWait(void) {};	//リロード待機(チャージ状態)

	//UI描画更新
	virtual void UpdateDrawCardUI(void) = 0;;

	//アクション配列のカードをすべて決定移動させる
	void DecisionMoveCardAll(void);

	//使用済みのカードを消す
	void UpdateUsedCard(void);

	// カード弾かれ移動
	void ReactMoveCard(const Vector2F& _goalPos);

	////特定のカードを弾かれ移動させる
	void SetBasePosActionCards(void);

	//画像作成
	int MakeCardNumImg(const CardBase::CARD_STATUS& _status);

	//属性画像取得
	const int GetTypeImg(const CardBase::CARD_STATUS _status);

private:

	//カード画像の配列
	std::map<CardBase::CARD_STATUS, int> cardImgs_;
};

