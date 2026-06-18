#pragma once
#include <functional>
#include <map>
#include <memory>
#include <vector>
#include <list>
#include "CardUiBase.h"
#include "./CardBase.h"
#include "../../Common/Vector2.h"
#include "../../Common/Vector2F.h"
#include "../Application.h"

class CardUI;
class CardUIController;
class PixelRenderer;
class PixelMaterial;

class PlayerCardUI
	:public CardUIBase
{

public:

	/// @brief コンストラクタ
	/// @param  
	PlayerCardUI(void);

	/// @brief デストラクタ
	/// @param  
	~PlayerCardUI(void)override;

	/// @brief ロード
	/// @param  
	void Load(void)override;

	/// @brief 初期化
	/// @param  
	void Init(void)override;

	/// @brief 更新
	/// @param  
	void Update(void)override;

	/// @brief プレイヤーのカードUI描画
	/// @param  
	void Draw(void)override;

	//カード初期化
	void InitCardUI(void)override;

#ifdef _DEBUG
	void DrawDebug(void);
#endif // _DEBUG

private:

#pragma region メンバー定数
	//見せるカード枚数
	static constexpr int VISIBLE_CARD_MAX = 6;

	//先頭に追加するときの戻る枚数
	static constexpr int PREV_CARD_COUNT = 2;

	//現在カードの前にある枚数
	static constexpr int CARDS_BEFORE_CURRENT = 1;

	//カード残り枚数ゲージシェーダ定数バッファサイズ
	static constexpr int CARD_NUM_GAUGE_CONST_BUF_SIZE = 3;

	//カード残り枚数ゲージシェーダ定数バッファインデックス
	static constexpr int CARD_NUM_GAUGE_CONST_BUF_IDX = 2;

	//選択カード番号
	static constexpr int SELECT_CARD_NO = 1;

	//カード角度関連
	static constexpr int ARROUND_NUM = 16;			//一周当たりの枚数		
	static constexpr int ARROUND_NUM_PER_QUAD = ARROUND_NUM / 4;//90度当たりの枚数
	static constexpr float ARROUND_PER_DEG = 360.0f / ARROUND_NUM;	//１枚当たりの角度
	static constexpr float ARROUND_PER_RAD = ARROUND_PER_DEG * DX_PI_F / 180.0f;//ラジアン変換
	static constexpr float ARROUND_PER_QUAD_DEG = ARROUND_PER_DEG * ARROUND_NUM_PER_QUAD;//90度当たりの枚数
	static constexpr float ARROUND_PER_QUAD_RAD = ARROUND_PER_QUAD_DEG * DX_PI_F / 180.0f;//90度当たりの枚数

#pragma endregion

#pragma region 外部ファイル読み込み
	Vector2F revolverEllipseRadius_{};				//リボルバー楕円の半径
	FLOAT4 cardNumGaugeLeftCol_{};					//カード枚数ゲージの左側の色
	FLOAT4 cardNumGaugeRightCol_{};					//カード枚数ゲージ右側の色
	Vector2F cardNumGaugePos_{};					//カード残り枚数のゲージ
	Vector2F cardNumGaugeSize_{};					//カード枚数ゲージサイズ
	Vector2F cardNumGaugeBGImgPos_{};				//カード枚数ゲージの背景の座標
	Vector2F cardNumGaugeBGImgSize_{};				//カード枚数ゲージの背景のサイズ
	Vector2F cardReactGoalCardPos_{};				//カード弾きの終点座標
	Vector2F revolverArrowLPos_{};					//リボルバーの左矢印UI座標
	Vector2F revolverArrowRPos_{};					//リボルバーの右矢印UI座標
	Vector2F revolverArrowSize_{};					//リボルバーの矢印サイズ
	float revolverArrowLAngle_ = 0.0f;				//リボルバー左矢印の傾け角度
	float revolverArrowRAngle_ = 0.0f;				//リボルバー右矢印の傾け角度
	float revolverButtonSize_ = 0.0f;				//リボルバーボタンサイズ
	float revolverArrowScale_ = 0.0f;				//リボルバーの大きさ倍率
	float revolverButtonFromArrowOffset_ = 0.0f;	//リボルバーボタンから矢印UIのオフセット
	int reloadFontSize_ = 0;						//リロード文字列のフォントサイズ
	Vector2F cardNumStringPos_{};					//カード残り枚数(文字)の座標
	float reloadStrOffsetYFromCard_=0.0f;			//リロードの文字列座標のオフセット
	std::wstring reloadStr_=L"";					//リロードの文字列
	int cardNumFontSize_ = 0;						//カード残り枚数のフォントサイズ
#pragma endregion

#pragma region メンバー変数
	//カード残り枚数のゲージ
	std::unique_ptr<PixelMaterial> cardGaugePSMaterial_;
	std::unique_ptr<PixelRenderer> cardGaugePSRenderer_;

	//見せるカード
	std::list<std::shared_ptr<CardUIController>>visibleCards_;
	//リロード用の現在のカードイテレータ
	std::list<std::shared_ptr<CardUIController>>::iterator reloadAnimCurr_;

	bool isReloadEnd_;		//リロード終了
	float cardNumPer_;		//残りカード枚数ゲージ
	int cardNumGaugeImg_;	//残りカード枚数マスク画像
	int cardNumBgImg_;		//残りカード枚数ゲージ背景
	int fontHandle_;		//フォントハンドル
	int reloadFontHandle_;	//リロードのフォントハンドル
	int imgRevolverArrow_;	//矢印
#pragma endregion

#pragma region メンバー関数
	//カード状態遷移
	void ChangeNone(void)override;			//通常
	void ChangeLeft(void)override;			//左に移動
	void ChangeRight(void)override;			//右に移動
	void ChangeDecision(void)override;		//決定
	void ChangeReloadWait(void)override;	//リロード待機(リロードゲージのチャージ)
	void ChangeReload(void);				//リロード

	//更新系
	void UpdateNone(void)override;			//通常
	void UpdateLeft(void)override;			//左に移動
	void UpdateRight(void)override;			//右に移動
	void UpdateDecision(void)override;		//決定
	void UpdateReloadWait(void)override;	//リロード待機(リロードゲージのチャージ)
	void UpdateReload(void);				//リロード

	//Jsonからパラメータ取得
	void LoadJsonData(void);

	//すべてのカードの移動
	void MoveCardAll(const float& _moveTImeMax);

	//カードUI描画オブジェクト更新
	void UpdateDrawCardUI(void) override;

	//角度を現在角度に設定
	void SetBasePosVisibleCards(void);

	//見せるカードの更新
	void UpdateVisibleCard(void);

	//手札の消去
	void EraseHandCard(void);

	//カード使用時のカード角度の更新
	void DecideGoalAngle(void);

	//リロードアニメーション
	void ReloadAnimation(void);

	//リロード時に配列に追加
	void ReloadCardArray(void);

	//上下に見せカードを動かす
	void MoveUpDownVisibleCards(void);

	//矢印とボタン描画
	void DrawArrowAndButton(void);

	//現在選択中のカードの配列を取得
	std::list<std::shared_ptr<CardUIController>>::iterator GetVisibleCurrentIt(void);
	std::list<std::shared_ptr<CardUIController>>::iterator GetSearchHandIt(std::shared_ptr<CardUIController> target);
#pragma endregion
};