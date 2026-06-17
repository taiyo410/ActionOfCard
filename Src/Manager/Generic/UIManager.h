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

private:

#pragma region メンバー定数
	//フォントサイズ
	static constexpr int FONT_SIZE = 20;

	////ボタン座標
	//static constexpr Vector2F INIT_BOTTON_POS = { 10.0f,60.0f };

	////ボタンサイズ
	//static constexpr float BOTTON_SIZE = 50.0f;

	////ボタン同士の間隔
	//static constexpr float BOTTON_DISTANCE = 10.0f;

	////攻撃ボタン文字列
	//const std::wstring ATTACK_BTN_STR = L"攻撃(カード使用)";

	////回避ボタン文字列
	//const std::wstring DODGE_BTN_STR = L"回避";

	//勝敗の矢印のローカル座標
	//static constexpr Vector2F ARROW_LOCAL_POS = { 350.0f,0.0f };

	//HIGHERの画像の座標
	static constexpr Vector2F HIGHER_IMG_POS = { Application::SCREEN_HALF_X - 120.0f,80.0f };

	//LOWER画像の座標
	static constexpr Vector2F LOWER_IMG_POS = { Application::SCREEN_HALF_X + 120.0f,80.0f };

	//デフォルトの画像サイズ倍率
	static constexpr float DEFAULT_SIZE_SCALE = 0.2f;
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

	ResourceManager& resMng_;		//リソース

	int imgBtns_;				//ボタン
	int fontHandle_;			//フォント
	int lowerImg_;				//数字の勝敗を表す画像(小さい)
	int higherImg_;				//数字の勝敗を表す画像(大きい)
	int upArrowImg_;			//上矢印画像
	int downArrowImg_;			//下矢印画像
	Vector2F higherPos_;		//数字勝敗画像の座標
	Vector2F lowerPos_;			//数字勝敗画像の座標
	Vector2F winnerArrowPos_;	//勝ちを表す矢印
	Vector2F loserArrowPos_;	//負けを表す矢印
	float scaleEaseCnt_;		//サイズイージングカウント
	float higherImgScl_;		//数字の勝敗画像サイズ
	float lowerImgScl_;			//数字の勝敗画像サイズ
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

	//数字の勝敗を表すUIの表示
	void EasingWinnerUISize(void);

	/// @brief 勝敗判定のUIの画像を補完
	/// @param  
	void UISizeEasing(float& _scl,Vector2F& _arrowPos,const Vector2F& _winnerPos);

	void LoadJsonParameter(void);
#pragma endregion
};