#pragma once
#include <unordered_map>
#include "Application.h"
#include "Object/Character/UIData/CharacterUIData.h"
#include "Object/Card/CardSystem.h"
#include "Common/Vector2F.h"
#include "Template/Singleton.h"

class HpUI;
class CardUIBase;
class DirectionUI;
class ResourceManager;
class SceneManager;
class PixelMaterial;
class PixelRenderer;

class UIManager :
	public Singleton<UIManager>
{
	//シングルトン
	friend class Singleton<UIManager>;

public:

	/// @brief ロード
	/// @param  
	void Load(void);

	/// @brief 初期化
	/// @param  
	void Init(void);
	
	/// @brief 更新
	/// @param 
	void Update(void);
	
	//演出時のUI更新
	void DirectionUpdate(void);

	/// @brief UIの描画
	/// @param  
	void Draw(void);

	/// @brief ポストエフェクトの削除
	/// @param  
	void DeleteRevolutionPostEffect(void);
	
	/// @brief 演出時のUI描画
	/// @param  
	void DirectionDraw(void);

	/// @brief HPUIのHpを更新
	/// @param _charaType 更新したいキャラクター
	/// @param _hpData 更新するHp
	void RefreshHpUI(const CHARACTER_TYPE _charaType,const HP_DATA _hpData);

	/// @brief カードUIの取得
	/// @param _charaType 取得したいキャラクター
	/// @return 
	CardUIBase& GetCardUI(const CHARACTER_TYPE _charaType);

	/// @brief スキップ割り
	/// @param _skipPer 
	void SetSkipPer(const float _skipPer);

	/// @brief 革命開始(演出開始)
	/// @param  
	void StartRevolution(void) { isStartRevolution_ = true; }

	/// @brief 革命終了フラグ
	/// @param  
	/// @return 
	const bool GetIsEndRevolutionDirection(void) { return isEndRevolution_; }

	/// @brief 革命演出の終了フラグ初期化
	/// @param  
	void InitIsEndRevolution(void) { isEndRevolution_ = false; }

	/// @brief WINUI演出の開始
	/// @param  
	void StartWINDirection(void);

private:

#pragma region メンバー定数
	//フォントサイズ
	static constexpr int FONT_SIZE = 20;

	//HIGHERの画像の座標
	static constexpr Vector2F WIN_RULE_UI_POS = { Application::SCREEN_HALF_X,80.0f };

	//LOWER画像の座標
	static constexpr Vector2F LOWER_IMG_POS = { Application::SCREEN_HALF_X + 120.0f,80.0f };

	//デフォルトの画像サイズ倍率
	static constexpr float DEFAULT_SIZE_SCALE =  1.0f;

	//WINUIの演出の補間時間
	static constexpr float WIN_DIRECTION_TIME = 0.3f;
#pragma endregion

#pragma region 外部ファイル読み込み
	//ボタン関連
	Vector2F topButtonPos_;				//ボタンのTop座標
	float buttonDistanceY_;				//ボタン同士のY間隔
	float buttonSize_;					//ボタンサイズ
	std::wstring attackStr_;			//攻撃ボタンの文字列
	std::wstring dodgeStr_;				//回避ボタン文字列

	//HIGHERとLOWERのUI関連
	Vector2F higherImgPos_;				//Higher画像の座標
	Vector2F lowerImgPos_;				//Lower画像の座標
	Vector2F arrowLocalPos_;			//勝敗する矢印の座標
	float defaultHigherAndLowerScale_;	//デフォルトのサイズ
	float easeGoalScl_;					//イージング時の最大サイズ
	float easeTime_;					//イージング時間

	//WINUIの補間時間
	static constexpr float WIN_UI_EASE_TIME = 0.2f;
	static constexpr float WIN_UI_START_SCL = 1.6f;
	static constexpr float WIN_UI_END_SCL = 1.0f;
	static constexpr float WIN_UI_WAIT_TIME = 0.2f;
	float winUIEaseTime_ = 0.0f;
	float winUIStartScl_ = 0.0f;
	float winUIEndScl_=0.0f;
	float winUIWaitTime_ = 0.0f;
	Vector2F winUIPos_ = {};
#pragma endregion

#pragma region メンバー定数
	//革命時間関連	
	static constexpr float FADE_TIME = 0.3f;	//フェード時間
	static constexpr float WAIT_TIME = 1.0f;	//フェードイン後の待機時間

#pragma endregion


#pragma region メンバー変数
	//キャラHPUI
	std::unordered_map<CHARACTER_TYPE, std::unique_ptr<HpUI>>characterHpUI_;
	//カードUI
	std::unordered_map<CHARACTER_TYPE, std::unique_ptr<CardUIBase>>cardUI_;
	//演出用UI
	std::unique_ptr<DirectionUI> directionUI_;
	//イージング
	std::unique_ptr<Easing> easing_;
	//ポストエフェクト
	std::unique_ptr<PixelMaterial> invertMaterial_;		//マテリアル
	std::shared_ptr<PixelRenderer> invertRenderer_;		//レンダラー

	ResourceManager& resMng_;		//リソース
	SceneManager& scnMng_;			//シーンマネージャ

	int imgBtns_;				//ボタン
	int fontHandle_;			//フォント
	int lowerImg_;				//数字の勝敗を表す画像(小さい)
	int higherImg_;				//数字の勝敗を表す画像(大きい)
	int upArrowImg_;			//上矢印画像
	int downArrowImg_;			//下矢印画像
	int winUIImg_;				//カード勝利画像
	Vector2F higherPos_;		//数字勝敗画像の座標
	float scaleEaseCnt_;		//サイズイージングカウント
	float higherImgScl_;		//数字の勝敗画像サイズ
	float lowerImgScl_;			//数字の勝敗画像サイズ
	float fadeCnt_;										//色反転フェードのカウント
	std::function<void(void)> revolutionFadeFunc_;		//反転フェード更新
	float waitCnt_;										//シーン待機時間
	int winAlpha_;										//WINUIのアルファ値
	float winUIScl_;									//WINUIのスケール
	float winEaseCnt_;									//WINUIの補完カウント
	float winUIWaitCnt_;								//WINUI表示時間
	std::function<void(void)> winUIDirectionFunc_;		//WINUI演出関数
	bool isStartRevolution_;							//革命開始フラグ
	bool isEndRevolution_;								//革命終了フラグ
	bool isWinDirection_;								//スタンプ演出開始フラグ
#pragma endregion

#pragma region メンバー関数
	//コピー禁止
	UIManager(void);
	UIManager(const UIManager& _copy) = delete;
	UIManager& operator=(const UIManager& _copy) = delete;

	// デストラクタも同様
	~UIManager(void)override = default;

	//HpUIの生成
	void CreateHpUI(void);

	//カードUIの生成
	void CreateCardUI(void);

	//攻撃ボタンと回避ボタンの表示
	void DrawAttackButtonAndDodgeButton(void);

	//HigherとLower画像のイージングなどの描画
	void DrawHigherAndLower(void);

	//カード勝利した際の演出
	void DrawWinDirection(void);

	//数字の勝敗を表すUIの表示
	void EasingWinnerUISize(void);

	//勝敗判定のUIの画像を補完
	void UISizeEasing(float& _scl,const Vector2F& _winnerPos);

	//革命開始のポストエフェクトのロード
	void LoadRevolutionPostEffect(void);

	//革命演出更新
	void UpdateRevolutionDirection(void);

	//WINUI演出の更新
	void UpdateWinUIDirection(void);

	//革命フェード更新
	void RevolutionInvertFadeNone(void);	//革命フェードなし
	void RevolutionInvertFadeIn(void);		//革命時の色反転フェードイン
	void RevolutionInvertFadeOut(void);		//革命時の色反転フェードアウト

	//WINUI状態遷移
	void WINUIStompNone(void);
	void WINUIStompIn(void);
	void WINUIStompOut(void);

	//スタンプのような演出
	void StompInDirection(float& _easeCnt, int& _alpha, float& _scl,const bool _isStompIn
		, const float _startSize = 1.6f, const float _endSize = 1.2f, const float _time = 0.2f);

	//Jsonからのロード
	void LoadJsonParameter(void);
#pragma endregion
};