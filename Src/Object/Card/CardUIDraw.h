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

	/// @brief 革命時、カードの色を反転
	/// @param  
	void DrawReverseColorCard(void);

	/// @brief リロードゲージ描画
	/// @param  
	void DrawReloadGauge(const float& _reloadPer);

private:

#pragma region メンバー定数
	//リロードカードの割合の定数バッファスロット
	static constexpr int RELOAD_PER_CONST_BUF_SIZE = 1;

	//通常カードのトータルタイムバッファスロット
	static constexpr int NORMAL_CARD_CONST_BUF = 0;

	//通常カードの色の定数バッファ
	static constexpr int NORMAL_CARD_CONST_BUN_NUM = 1;
#pragma endregion

#pragma region 外部ファイル読み込み
	float selectCardFogPow_;			//選択カードのフォグの強さ
	float selectCardFrameEaseTime_;		//選択カードイージング時間
	float selectCardFrameMoveAmount_;	//選択カード枠拡大縮小の移動量
#pragma endregion

#pragma region メンバー変数
	//通常カードピクセルシェーダー
	std::unique_ptr<PixelMaterial> normalCardPSMaterial_;	//マテリアル
	std::unique_ptr<PixelRenderer> normalCardPSRenderer_;	//レンダラー

	//リロードカード
	std::unique_ptr<PixelMaterial> reloadCardPSMaterial_;	//マテリアル
	std::unique_ptr<PixelRenderer> reloadCardPSRenderer_;	//レンダラー

	//選択カード枠
	std::unique_ptr<PixelMaterial> selectFramePSMaterial_;	//マテリアル
	std::unique_ptr<PixelRenderer> selectFramePSRenderer_;	//レンダラー

	std::unique_ptr<Easing>easing_;		//イージング

	Vector2F halfSize_;						//カードの半分大きさ
	Vector2F size_;							//カードの大きさ
	Vector2F& centerPos_;					//中心座標
	Vector2F rightTopPos_;					//左上座標
	Vector2F leftDownPos_;					//右下座標
	int& typeImg_;							//カードの種類画像
	float& scl_;							//サイズ
	float selectEaseCnt_;					//選択枠イージングカウント
	float shaderSetRevolutionCard_;			//革命時のシェーダーセット変数
#pragma endregion

#pragma region メンバー関数
	//カードの描画
	void DrawCard(void);

	//選択枠イージング
	void SelectFrameEasing(void);
#pragma endregion

	//選択中のフォグの強さ
	static constexpr float SELECT_FOG_STRENGTH = 0.4f;

	//選択カード枠の大きさイージング時間
	static constexpr float SELECT_CARD_FRAME_EASING_TIME = 1.0f;

	//選択カード枠移動量
	static constexpr float SELECT_CARD_FRAME_MOVE_AMOUNT = 10.0f;


};