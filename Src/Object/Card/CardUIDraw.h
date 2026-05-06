#pragma once
#include"./CardBase.h"

class Easing;
class PixelMaterial;
class PixelRenderer;
class CardUIController;

class CardUIDraw :
	public ObjectBase
{

public:

	/// @brief コンストラクタ
	/// @param _typeImg カード画像
	/// @param _centerPos 中心座標
	/// @param _scl 大きさ
	CardUIDraw(int& _typeImg,Vector2F& _centerPos, float& _scl);

	/// @brief デストラクタ
	/// @param  
	~CardUIDraw(void)override;

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

	/// @brief 選択カード枠描画
	/// @param  
	void DrawSelectedFrame(void);

	/// @brief 選択カード描画
	/// @param  
	void DrawSelectCard(void);

	/// @brief リロードゲージ描画
	/// @param  
	void DrawReloadGauge(const float& _reloadPer);

private:

	//通常カードのシェーダパス
	std::wstring NORMAL_CARD_SHADER_PATH = L"CardNormalPS.cso";

	//リロードカードのシェーダパス
	std::wstring RELOAD_CARD_SHADER_PATH = L"CardReloadPS.cso";

	//選択枠シェーダパス
	std::wstring SELECT_FRAME_SHADER_PATH = L"SelectCardPS.cso";

	//カード強さ番号の倍率
	static constexpr float CARD_SCL = 0.5f;

	//カード初期座標
	static constexpr VECTOR CARD_INIT_POS = { 50.0f,200.0f,0.0f };

	//選択中のフォグの強さ
	static constexpr float SELECT_FOG_STRENGTH = 0.3f;

	//選択カード枠の大きさイージング時間
	static constexpr float SELECT_CARD_FRAME_EASING_TIME = 1.0f;

	//選択カード枠移動量
	static constexpr float SELECT_CARD_FRAME_MOVE_AMOUNT = 10.0f;

	//フレーム固定座標
	static constexpr Vector2F LEFT_UP_FRAME_POS = { 19.5f,343.0f };
	static constexpr Vector2F RIGHT_DOWN_FRAME_POS = { 140.0f,509.0f };

	//シェーダの定数バッファスロット
	static constexpr int CONST_BUF_SLOT_NUM = 3;

	//選択カードの定数バッファスロット
	static constexpr int CARD_NUM_CONST_BUF_SIZE = 3;

	//リロードカードの割合の定数バッファスロット
	static constexpr int RELOAD_PER_CONST_BUF_SIZE = 1;

	//通常カードのトータルタイムバッファスロット
	static constexpr int NORMAL_CARD_CONST_BUF = 0;

	//通常カードの色の定数バッファ
	static constexpr int NORMAL_CARD_CONST_BUN_NUM = 1;

	//選択カードアウトラインサイズ
	static constexpr float SELECT_CARD_OUTLINE_SIZE = 100.0f;	

	//通常カードピクセルマテリアル
	std::unique_ptr<PixelMaterial> normalCardPSMaterial_;

	//通常カードピクセルレンダラー
	std::unique_ptr<PixelRenderer> normalCardPSRenderer_;

	//リロードカード
	std::unique_ptr<PixelMaterial> reloadCardPSMaterial_;
	std::unique_ptr<PixelRenderer> reloadCardPSRenderer_;

	//選択カード枠マテリアル
	std::unique_ptr<PixelMaterial> selectFramePSMaterial_;
	std::unique_ptr<PixelRenderer> selectFramePSRenderer_;

	//イージング
	std::unique_ptr<Easing>easing_;

	//カードの種類画像
	int& typeImg_;		

	//カードの半分大きさ
	Vector2F halfSize_;	

	//カードの大きさ
	Vector2F size_;			

	//中心座標
	Vector2F& centerPos_;	

	//左上座標
	Vector2F rightTopPos_;	

	//右下座標
	Vector2F leftDownPos_;	

	//サイズ
	float& scl_;			

	//選択枠イージングカウント
	float selectEaseCnt_;	

	//カードの描画
	void DrawCard(void);

	//選択枠イージング
	void SelectFrameEasing(void);
};