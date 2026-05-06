#pragma once
#include "../Application.h"
#include "../Common/Vector2F.h"
#include "../UIData/CharacterUIData.h"
#include "../Base/UIBase2D.h"
class Easing;
class ResourceManager;
class PixelRenderer;
class PixelMaterial;

class HpUI
	: public UIBase2D
{

public:

	/// @brief コンストラクタ
	/// @param  
	HpUI(CHARACTER_TYPE _charaType);

	/// @brief デストラクタ
	/// @param  
	~HpUI(void)override;

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

	//シェイク時間セット
	void SetShakeTime(void);

	/// @brief Hpの更新
	/// @param  
	void RefreshHp(const HP_DATA& _hpData);

	/// @brief Jsonからデータを取得
	/// @param  
	void LoadJsonHpUI(void);

private:

	//シェーダ定数バッファ
	static constexpr int ARC_CONST_BUF = 2;			//円形
	static constexpr int LINE_CONST_BUF = 3;		//線形


	//減る前のHP保存用定数バッファの番号
	static constexpr int PRE_HP_CONST_BUF = 2;

	//補間時間
	static constexpr float LERP_TIME = 0.3f;
	static constexpr float WAIT_TIME = 0.5f;

	//シェイク
	static constexpr float SHAKE_CNT = 0.3f;
	static constexpr float COVER_SHAKE_POW_Y = 30.0f;
	static constexpr float BAR_SHAKE_POW_Y = 30.0f;

	//Hpバーサイズ
	static constexpr Vector2F BAR_COVER_SIZE = { 500.0f ,20.0f };
	static constexpr Vector2F BAR_SIZE = { 470.0f ,16.0f };

	//Hpバーカバーハンドル
	int barCoverHandle_;

	//HPバーフレーム
	int barFrame_;

	//Hpマスクハンドル
	int hpMask_;

	//バーカバー座標
	Vector2F barCoverPos_;

	//バー座標
	Vector2F barPos_;

	//HPUIデータ
	HP_DATA hpData_;

	//前フレームとのHpの差
	float hpDis_;

	//円形アウトライン
	int arcOutLineImg_;
	int lineOutLineImg_;

	//固定座標
	Vector2F initBarPos_;
	Vector2F initCoverPos_;

	//リソースマネージャー
	ResourceManager& resMng_;

	//レンダラー
	std::unique_ptr<PixelRenderer>renderer_;
	//マテリアル
	std::unique_ptr<PixelMaterial>material_;

	//補間カウント
	float lerpCnt_;

	//補間待機時間
	float waitCnt_;

	//シェイクカウント
	float shakeCnt_;

	//キャラクタータイプ
	const CHARACTER_TYPE charaType_;

	//バーの左の色
	FLOAT4 barColorL_;

	//バーの右の色
	FLOAT4 barColorR_;
};