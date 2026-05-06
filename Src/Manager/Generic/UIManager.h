#pragma once
#include <unordered_map>
#include "../Object/Character/UIData/CharacterUIData.h"
#include "../Common/Vector2F.h"
#include "../Template/Singleton.h"

class HpUI;
class CardUIBase;
class DirectionUI;

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

	//フォントサイズ
	static constexpr int FONT_SIZE = 20;

	//ボタンX座標
	static constexpr Vector2F INIT_BOTTON_POS = { 10.0f,60.0f };

	//ボタンサイズ
	static constexpr float BOTTON_SIZE = 50.0f;

	//ボタン同士の間隔
	static constexpr float BOTTON_DISTANCE = 10.0f;

	//攻撃ボタン文字列
	const std::wstring ATTACK_BTN_STR = L"攻撃(カード使用)";

	//回避ボタン文字列
	const std::wstring DODGE_BTN_STR = L"回避";

	//ボタン
	int imgBtns_;

	//キャラHPUI
	std::unordered_map<CHARACTER_TYPE,std::unique_ptr<HpUI>>characterHpUI_;

	//カードUI
	std::unordered_map<CHARACTER_TYPE,std::unique_ptr<CardUIBase>>cardUI_;

	//演出用UI
	std::unique_ptr<DirectionUI>directionUI_;

	//フォント
	int fontHandle_;

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
	void DrawAttackBottonAndDodgeBotton(void);
};