#pragma once
#pragma once
#include <map>
#include <functional>
#include "../../Object/Common/Transform.h"
#include "../ObjectBase.h"

class Easing;
class ModelMaterial;
class ModelRenderer;

class SkyDome : public ObjectBase
{

public:

	//回転速度
	static constexpr float ROT_SPEED = 0.05f;

	//大きさ
	static constexpr float SCALE = 60.0f;
	static constexpr VECTOR SCALES = { SCALE, SCALE, SCALE };

	// 状態
	enum class FOLLOW_STATE
	{
		NONE,
		STAY,
		FOLLOW
	};

	enum class PHASE
	{
		NONE,
		BATTLE,
		CLEAR
	};

	/// @brief コンストラクタ
	/// @param  
	SkyDome(void);

	/// @brief デストラクタ
	/// @param  
	~SkyDome(void)override = default;

	/// @brief ロード
	/// @param  
	void Load(void) override;

	/// @brief 初期化
	/// @param  
	void Init(void) override;

	/// @brief 更新
	/// @param  
	void Update(void) override;

	/// @brief 描画
	/// @param  
	void Draw(void) override;

	/// @brief 衝突後の処理
	/// @param _hitCol 相手側の当たり判定
	void OnHit(const std::weak_ptr<Collider> _hitCol)override {};

	/// @brief フェーズ遷移
	/// @param _phase 遷移したいフェーズ
	void ChangePhase(const PHASE _phase);

private:	

	//頂点シェーダの定数バッファ
	static constexpr int CONST_BUF_SIZE = 1;

	//スカイドームのサイズスケール
	static constexpr float SIZE_SCL = 1.0f;

	//スカイドームローカル回転角度
	static constexpr float LOCAL_DEG = 180.0f;

	//バトル中のカラースケール
	static constexpr float COLOR_SCL_BATTLE_R = 1.7f;		
	static constexpr float COLOR_SCL_BATTLE_G = 0.2f;	
	static constexpr float COLOR_SCL_BATTLE_B = 0.5f;
	static constexpr VECTOR COLOR_SCL_BATTLE = { COLOR_SCL_BATTLE_R ,COLOR_SCL_BATTLE_G ,COLOR_SCL_BATTLE_B };

	//通常時のカラースケール
	static constexpr float COLOR_SCL_DEFAULT_R = 1.0f;
	static constexpr float COLOR_SCL_DEFAULT_G = 1.0f;
	static constexpr float COLOR_SCL_DEFAULT_B = 1.0f;
	static constexpr VECTOR COLOR_SCL_DEFAULT = { COLOR_SCL_DEFAULT_R ,COLOR_SCL_DEFAULT_G ,COLOR_SCL_DEFAULT_B };

	//カラー補完時間
	static constexpr float COLOR_EASE_TIME = 7.0f;

	//追従状態
	FOLLOW_STATE followState_;

	//シェーダー関連
	std::unique_ptr<ModelMaterial> material_;	//マテリアル
	std::unique_ptr<ModelRenderer> renderer_;	//レンダラー

	//状態
	PHASE phase_;

	//状態遷移更新
	std::function<void(void)>updatePhase_;

	//状態管理(状態遷移時初期処理)
	std::map<PHASE, std::function<void(void)>> phaseChanges_;

	//イージング
	std::unique_ptr<Easing>easing_;
	//イージング時間
	float colEasingCnt_;

	//更新系
	void UpdateNone(void);			 //何もしない
	void UpdateClear(void);			 //クリア
};