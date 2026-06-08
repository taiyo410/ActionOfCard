#pragma once

#include <functional>
#include "../Base/ItemBase.h"

class PlayerMagicFire :
    public ItemBase
{

public:

	enum class STATE
	{
		NORMAL,
		FADE_OUT
	};

    /// @brief コンストラクタ
    /// @param  
    PlayerMagicFire(VECTOR& _startPos,VECTOR& _dir);

    /// @brief デストラクタ
    /// @param  
    ~PlayerMagicFire(void);

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
    
	/// @brief 当たり判定
	/// @param _hitCol 当たったコライダー
	void OnHit(const std::weak_ptr<Collider> _hitCol) override;

	/// @brief jsonからのロード
	/// @param _jsonData 
	void LoadFireData(const nlohmann::json& _jsonData);

	/// @brief 炎の生存状態の取得
	/// @param  
	/// @return 炎の生存状態
	const bool GetIsAlive(void) { return isAlive_; }

	/// @brief 炎の当たり判定を作成
	/// @param  
	void MakeFireBallCollider(void);

	/// @brief 状態遷移
	/// @param _state 遷移したい状態
	void ChangeDelete(void);

private:

#pragma region 外部ファイル読み込み
	float moveSpd_;			//移動速度
	float atkPow_;			//攻撃力
	float colRadius_;		//当たり判定の半径
	VECTOR& dir_;			//方向
	float fireBallEffScl_;	//エフェクトのスケール
	float effDeleteTime_;	//エフェクトの消去時間
#pragma endregion

#pragma region メンバー変数
	//状態遷移
	std::function<void(void)>updateState_;			//状態更新

	bool isAlive_;				//生存フラグ
	VECTOR& startPos_;			//攻撃初期位置
	int fireEffPlayId_;			//エフェクトプレイID
	float effAlpha_;			//エフェクトのブレンド描画
	float effDeleteScl_;		//エフェクト削除時のスケールダウン用
	float effDeleteAlpha_;		//エフェクト削除時のアルファ値
	float effDeleteCnt_;		//エフェクトの消去カウント
#pragma endregion

#pragma region 状態更新
	void UpdateNormal(void);
	void UpdateFadeOut(void);
#pragma endregion

};

