#pragma once
#include<memory>
#include "../Base/CharacterBase.h"
#include"../../Card/CardBase.h"
#include"../../../Common/Vector2.h"

class CardDeck;
class EnemyInput;
class ActionController;
class AnimationController;
class PixelMaterial;
class PixelRenderer;
class Idle;
class Run;
class Jump;
class React;
class EnemyCardAction;

class Enemy :public CharacterBase
{

public:

	//敵のサイズ
	static constexpr int RADIUS = 25; 

	//プレイヤーのローカル角度
	static constexpr float MODEL_LOCAL_DEG = 180.0f;

	//敵の大きさの倍率
	static constexpr float MODEL_SIZE_MULTIPLITER = 5.0f;

	//敵の大きさ
	static constexpr VECTOR MODEL_SCL = { MODEL_SIZE_MULTIPLITER,MODEL_SIZE_MULTIPLITER,MODEL_SIZE_MULTIPLITER };

	//カード最大枚数
	static constexpr int CARD_NUM_MAX = 20;

	//アニメーション速度
	static constexpr float ANIM_SPEED = 10.0f;

	//咆哮アニメーション速度
	static constexpr float ROAR_ANIM_SPEED = 30.0f;

	//咆哮アニメーション咆哮開始位置
	static constexpr float ROAR_ANIM_START_ANIM = 50.0f;

	//咆哮アニメーション咆哮終了位置
	static constexpr float ROAR_ANIM_END_ANIM = 127.0f;

	//カメラシェイク強さ
	static constexpr float CAM_SHAKE_LIMIT = 10.0f;

	/// @brief デッキの取得
	/// @param  
	/// @return 
	const std::shared_ptr<CardDeck> GetHand(void)const { return deck_; }

	/// @brief コンストラクタ
	/// @param  
	Enemy(void);

	/// @brief デストラクタ
	/// @param  
	~Enemy(void)override;

	/// @brief ロード
	/// @param  
	void Load(void) override;

	/// @brief 初期化
	/// @param  
	void Init(void) override;

	/// @brief 描画
	/// @param  
	void Draw(void) override;

	/// @brief 2D描画
	/// @param  
	void Draw2D(void) override;

	/// <summary>
	/// オブジェクトに当たった時の処理
	/// </summary>
	void OnHit(const std::weak_ptr<Collider> _hitCol)override;

	/// <summary>
	/// 入力方向に応じて方向を決める
	/// </summary>
	/// <param name=""></param>
	void MoveDirFromInput(void)override;

	/// <summary>
	/// ゴール角度をセット
	/// </summary>
	/// <param name="_deg"></param>
	void SetGoalRotate(void)override;

	/// @brief 咆哮演出時の更新
	/// @param  
	void UpdateRoarDirection(void);

	/// @brief 咆哮状態の取得
	/// @param  
	/// @return true:咆哮状態 false:通常状態
	const bool GetIsRoar(void)const { return isRoar_; }

private:

	//定数
	//プレイヤーの腰のフレーム番号
	static constexpr int SPINE_FRAME_NO = 1;

	//胸のフレーム
	static constexpr int CHEST_FRAME_NO = 4;

	//敵番号(デッキで判定する用)
	static constexpr int ENEMY_NUM = 1;

	//足音間隔
	static constexpr float FOOT_SE_DIS = 0.6f;

	//カプセル関連
	static constexpr VECTOR CAP_LOCAL_TOP = { 0.0f, 150.0f*MODEL_SIZE_MULTIPLITER, 0.0f };	//トップ座標
	static constexpr VECTOR CAP_LOCAL_DOWN = { 0.0f,0.0f,0.0f };	//ダウン座標
	static constexpr float CAP_RADIUS = 30.0f* MODEL_SIZE_MULTIPLITER;				//カプセル球の半径

	//倒れるエフェクトのスケール
	static constexpr float DEATH_EFF_SCL = 100.0f;
	static constexpr VECTOR DEATH_EFF_SCL_VEC = { DEATH_EFF_SCL,DEATH_EFF_SCL,DEATH_EFF_SCL };

	//爆発発生アニメステップ
	static constexpr float DEATH_BLAST_ANIM_STEP = 93.0f;

	//カードの中心座標
	Vector2 cardCenterPos_; 

	//敵のスケール
	float modelScl_;

	//咆哮状態かどうか
	bool isRoar_;

	//コライダ作成
	void MakeColliderGeometry(void)override;

	//更新系
	void UpdateNormal(void)override;			//通常更新
	void UpdateDirection(void)override;			//演出時の更新
	void UpdateClearDirection(void)override;	//クリア時(敵が倒れる)
	void UpdateOverDirection(void)override;		//ゲームオーバー

	/// @brief クリア演出に変更
	/// @param  
	void ChangeUpdateClearDirection(void)override;

	//アクションの追加
	void AddAction(void)override;

	//アニメーションの追加
	void AddAnimation(void)override;

#ifdef _DEBUG
	void DrawDebug(void);
#endif // _DEBUG
};