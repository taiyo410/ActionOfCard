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

	/// @brief オブジェクトに当たった時の処理
	/// @param _hitCol 
	void OnHit(const std::weak_ptr<Collider> _hitCol)override;

	/// @brief 入力方向に応じて方向を決める
	/// @param  
	void MoveDirFromInput(void)override;

	/// @brief ゴール角度をセット
	/// @param  
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
	//Enemyの文字列
	const std::string ENEMY_STR = "Enemy";

	//敵の腰のフレーム番号
	static constexpr int SPINE_FRAME_NO = 1;

	//胸のフレーム
	static constexpr int CHEST_FRAME_NO = 4;

	//敵番号(デッキで判定する用)
	static constexpr int ENEMY_NUM = 1;

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

	//咆哮アニメーション
	AnimationController::ANIMATION_VARIABLE roarAnim_;

	//キャラクター単体でのロード
	void LoadCharacter(void)override;

	//コライダ作成
	void MakeColliderGeometry(void)override;

	//更新系
	void UpdateNormal(void)override;			//通常更新
	//void UpdateDirection(void)override;			//演出時の更新
	void UpdateDirectionCharacter(void)override;	
	//void UpdateClearDirection(void)override;	//クリア時(敵が倒れる)
	void UpdateClearDirectionCharacter(void) override;	
	void UpdateOverDirection(void)override;		//ゲームオーバー
	void UpdateOverDirectionCharacter(void) override;

	/// @brief クリア演出に変更
	/// @param  
	void ChangeUpdateClearDirection(void)override;

#ifdef _DEBUG
	void DrawDebug(void);
#endif // _DEBUG
};