#pragma once
#include <memory>
#include "../Common/Vector2F.h"

class ResourceManager;
class PixelMaterial;
class PixelRenderer;

class DirectionUI
{

public:
	
	/// @brief コンストラクタ
	/// @param  
	DirectionUI(void);

	/// @brief デストラクタ
	/// @param  
	~DirectionUI(void);

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

	/// @brief スキップゲージの割合
	/// @param  
	void SetSkipGaugePer(const float _skipPer);

private:

	//ボタン長押しスキップの文字列
	const std::wstring SKIP_STR = L"ボタン長押しでスキップ";

	//円形ゲージシェーダのパス
	const std::wstring ARC_GAUGE_SHADER_PATH = L"ArcHpBarPS.cso";

	//フォントサイズ
	static constexpr int FONT_SIZE = 30;

	//スキップボタンゲージシェーダの定数バッファサイズ
	static constexpr int SKIP_GAUGE_CONST_BUFF_SIZE = 2;

	//割合定数バッファ番号
	static constexpr int SKIP_GAUGE_PER_CONSTBUF_NUM = 1;

	//スキップボタン画像の位置
	static constexpr Vector2F SKIP_BTN_POS = { 30.0f,30.0f };

	//スキップボタンサイズ
	static constexpr Vector2F SKIP_BTN_SIZE = { 64.0f,64.0f };

	//スキップボタン長押し時間
	static constexpr float SKIP_BTN_TIME = 1.0f;

	//ボタン長押し文字のY座標オフセット
	static constexpr float SKIP_BTN_STR_OFFSET_Y = 16.0f;

	//集中線アニメーション速度
	static constexpr int INTENSIVE_LINE_ANIM_SPEED = 5;

	//リソースマネージャ
	ResourceManager& resMng_;

	//円形ゲージシェーダ
	std::unique_ptr<PixelMaterial>skipArcGaugeMaterial_;
	std::unique_ptr<PixelRenderer>skipArcGaugeRenderer_;

	//スキップボタン画像
	int imgSkipButtomMask_;

	//フォント
	int fontHandle_;

	//集中線(交互に表示する
	int intensiveLineFirstImg_;			//交互表示1枚目
	int intensiveLineSecondImg_;		//交互表示1枚目

	//集中線アニメーション
	int intensiveLineAnimImg_;

	//集中線アニメーションフレーム
	int intensiveLineAnimFrame_;

	//スキップゲージの割合
	float skipGaugePer_;

	//集中線アニメーション更新
	void UpdateIntensiveLineAnim(void);

};