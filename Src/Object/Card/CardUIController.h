#pragma once
#include <memory>
#include "../../Common/Vector2.h"
#include "../../Common/Vector2F.h"
#include"./CardBase.h"
#include"../Application.h"

class CardUIDraw;
class Easing;

class CardUIController
{

public:

	enum class CARD_STATE
	{
		DRAW_PILE		//山札
		, MOVE_DRAW		//山札から手札へ移動
		, USING			//使用中
		, REACT			//弾かれ中
		, USED			//使用済み
	};

	struct CARD_STATUS_DRAW
	{
		int typeImg = -1;
		CardBase::CARD_STATUS status;
	};

	//カードサイズ補完時間
	static constexpr float SCL_LERP_TIME = 0.5f;

	//リロード1枚あたりの時間
	static constexpr float RELOAD_MOVE_CARD_TIME_PER = 0.06f;

	//カードセレクト時間
	static constexpr float SELECT_MOVE_CARD_TIME = 0.1f;

	//カード決定UI時間
	static constexpr float DISITION_MOVE_CARD_TIME = SELECT_MOVE_CARD_TIME;

	//敵の選択カード初期位置
	static constexpr Vector2F ENEMY_SELECT_CARD_START_POS = { Application::SCREEN_SIZE_X - 200,Application::SCREEN_HALF_Y + 200 };

	//弾かれるUI時間
	static constexpr float REACT_MOVE_CARD_TIME = 0.3f;

	/// @brief コンストラクタ
	/// @param _cardNumImgs カード番号の複数画像
	CardUIController(int& _cardNumImgs);

	/// @brief デストラクタ
	/// @param  
	~CardUIController(void);

	/// @brief ロード
	/// @param  
	void Load(void);

	/// @brief 初期化
	/// @param  
	void Init(void);

	/// @brief 更新
	/// @param  
	void Update(void);

	/// @brief 描画
	/// @param  
	void Draw(void);

	/// @brief リロードゲージの描画
	/// @param  
	void DrawReloadGauge(const float& _reloadPer);

	/// @brief 選択カード枠描画
	/// @param  
	void SelectCardDrawFrame(void);

	/// @brief 選択カード描画
	/// @param  
	void DrawSelectCard(void);

	/// @brief 選択移動
	/// @param  
	void DecisionMove(void);

	/// @brief 弾かれ処理
	/// @param _goalPos 目標角度
	void ReactMove(const Vector2F& _goalPos);
	
	//使用済みカード削除
	void EraseUsedCard(void);

	/// @brief カード中心座標の取得
	/// @param  
	/// @return 中心座標
	Vector2F& GetCenterPos(void) { return cardPos_; }

	/// @brief カード大きさの取得
	/// @param  
	/// @return 
	float& GetScl(void) { return cardScl_; }

	/// @brief 現在角度を取得
	/// @param  
	/// @return 
	const float& GetCurrentAngle(void)const { return currentAngle_; }
	
	/// @brief カード座標初期化
	/// @param _num 何番目か
	/// @param _perRad 1枚の角度
	/// @param _center 楕円の中心座標
	/// @param _radius 半径
	void InitCard(const int& _num);

	/// @brief 使い済みにする
	/// @param  
	void ChangeUsedCard(void);

	//弾かれ状態
	void ChangeReactCard(void);

	/// @brief 使用中
	/// @param  
	void ChangeUsing(void);

	//弾かれ処理
	void ReactUpdate(const Vector2F& _goalPos);

	/// @brief リボルバー上を移動
	/// @param  
	void MoveOnRevolver(const float& _cnt,const float& moveTimeMax);

	/// @brief 上下にカードを動かす(見栄え用)
	/// @param  
	void MoveUpDown(void);

	/// @brief 敵のカード決定
	/// @param  
	void ChangeDicisionEnemyCardMove(void);

	/// @brief 拡大縮小カウントの取得
	/// @param  
	/// @return 拡大縮小カウント
	const float& GetSclCnt(void) { return sclCnt_; }

	/// @brief 決定した時のアニメーションカウントの取得
	/// @param  
	/// @return 
	const float& GetDecisionCnt(void)const { return disitionCnt_; }

	/// @brief ステータスの取得
	/// @param  
	/// @return ステータス
	const CardBase::CARD_STATUS GetStatus(void)const { return status_; }

	/// @brief イージングのためのスタート角度(角度補完終了後の現在角度)と目標の角度セット
	/// @param _goalrad 目標角度
	void SetStartAndGoalAngle(const float& _goalrad);

	/// @brief スタート角度に現在角度を代入
	/// @param  
	void SetStartAngle(void);

	/// @brief 現在の角度セット
	/// @param _rad セットしたいラジアン角度
	void SetCurrentAngle(const float& _rad) { currentAngle_ = _rad; }

	/// @brief 決定カウントセット
	/// @param _cnt セットしたいカウント
	void SetDecisionCount(const float& _cnt) { disitionCnt_ = _cnt; }

	/// @brief ステータスの取得
	/// @param _status ステータス
	void SetStatus(const CardBase::CARD_STATUS& _status) { status_ = _status; }

	/// @brief カードタイプ画像のセット
	/// @param _type 
	void SetTypeImg(const int& _type) { typeImg_ = _type; }

	/// @brief ガード画像のセット
	/// @param _img カード画像
	void SetCardImg(const int& _img) { cardImg_ = _img; }

	/// @brief 上下に動かす基準となる座標のセット(状態遷移時のみ)
	/// @param  
	void SetBaseCardPos(void) { baseCardPos_ = cardPos_; }

	/// @brief リセット
	/// @param  
	void ResetCount(void);

	/// @brief 状態の取得
	/// @param  
	/// @return 状態
	const CARD_STATE& GetState(void)const { return state_; }

#ifdef _DEBUG
	/// @brief 弾かれカウントの取得
	const float& GetReactCount(void)const { return reactCnt_; }
#endif // _DEBUG

private:

	//決定したカードの座標
	static constexpr Vector2F DISITON_CARD_POS = { Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y + 200 };

	//弾かれる前のゴール座標
	static constexpr Vector2F REACT_START_CARD_POS = { Application::SCREEN_HALF_X - 100.0f, Application::SCREEN_HALF_Y + 200.0f };

	//倍率1の時のカードの強さの描画座標
	static constexpr Vector2F NUM_LOCAL_POS = { 89.0f,130.0f };

	//楕円中心
	static constexpr float CENTER_X = 80.0f;
	static constexpr float CENTER_Y = 640.0f;

	//楕円の半径
	static constexpr float RADIUS_X = 186.0f;	//横半径
	static constexpr float RADIUS_Y = 214.0f;	//横半径

	//角度関連
	static constexpr int ARROUND_NUM = 16;			//一周当たりの枚数		
	static constexpr int ARROUND_NUM_PER_QUAD = ARROUND_NUM / 4;//90度当たりの枚数
	static constexpr float ARROUND_PER_DEG = 360.0f / ARROUND_NUM;	//１枚当たりの角度
	static constexpr float ARROUND_PER_RAD = ARROUND_PER_DEG * DX_PI_F / 180.0f;//ラジアン変換

	//控えの見せカードを上下に動かす関連
	static constexpr float UP_DOWN_MOVE_RADIUS = 5.0f;
	static constexpr float UP_DOWN_MOVE_SPEED = 0.05f;

	//カード描画
	std::unique_ptr<CardUIDraw>cardDraw_;

	//コントローラークラスが使われるイージングの参照
	std::unique_ptr<Easing>easing_;

	//カードの座標(画面外で初期化)
	Vector2F cardPos_;		

	//カード上下に動かす移動量
	float upDownMovePow_;	

	//上下動かし用の角度(sin波で動かすため)
	float upDownMoveAngle_;	

	//カードを上下に動かす基準の座標
	Vector2F baseCardPos_;	

	//カードの強さ番号座標(画面外で初期化)
	Vector2F numPos_;		

	//カードの現在の角度
	float currentAngle_;	

	//始まり角度
	float startAngle_;		

	//カードの目標の角度
	float goalAngle_;	

	//カードのスケール
	float cardScl_;

	//スケールのイージング用カウント
	float sclCnt_;

	//決定カウント
	float disitionCnt_;

	//はじかれるカウント
	float reactCnt_;													

	//カードの状態
	CARD_STATE state_;							

	//カードリボルバーの中心
	Vector2F centerPos_;						

	//カードの種類画像
	int typeImg_;		

	//カードのステータス
	CardBase::CARD_STATUS status_;	

	//カードの大きさ
	Vector2F halfSize_;		

	//カード画像
	int cardImg_;		

	//カード画像保管
	std::list<int>cardImgs_;	

	//カード番号イメージ
	int cardNoImg_;
};