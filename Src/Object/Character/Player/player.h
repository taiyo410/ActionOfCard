#pragma once
#include <map>
#include <functional>
#include <memory>
//#include"../../Character/Player/InputController.h"
#include"./ActionController.h"
#include"../../Card/CardBase.h"
#include "../Base/CharacterBase.h"

#define DEBUG_ON
class AnimationController;
class ActionController;
class CardDeck;
class InputController;
class Camera;
class Geometry;
class Cupsule;
class Line;
class PlayerOnHit;
class PlayerCardUI;
class Weapon;
class ModelMaterial;
class ModelRenderer;
class PlayerHpUI;
class Idle;
class Run;
class Jump;
class React;
class Dodge;
class PlayerCardAction;


class EffectController;

class Player :public CharacterBase
{

public:

	//半径
	static constexpr float CAP_RADIUS = 25.0f;

	//デフォルトのアニメーションスピード
	static constexpr float DEFAULT_ANIM_SPD = 60.0f;

	//ドッジアニメーション速度
	static constexpr float DODGE_ANIM_SPD = 80.0f;

	//中距離攻撃アニメ速度
	static constexpr float ATK_MID_ANIM_SPD = 40.0f;

	//カプセル関連
	static constexpr VECTOR CAP_LOCAL_TOP = { 0.0f, 200.0f, 0.0f };	//トップ座標
	static constexpr VECTOR CAP_LOCAL_DOWN = { 0.0f,50.0f,0.0f };	//ダウン座標

	/// @brief コンストラクタ
	/// @param  
	Player(void);

	/// @brief  デストラクタ
	/// @param  
	~Player(void)override;
	
	/// @brief 読み込み
	/// @param  
	void Load(void)override;
	
	/// @brief 初期化
	/// @param  
	void Init(void)override;
	
	/// @brief 描画
	/// @param  
	void Draw(void)override;

	/// @brief 2D関連の描画
	/// @param  
	void Draw2D(void)override;
	
	/// @brief 手札
	/// @param  
	/// @return 手札
	const std::weak_ptr<CardDeck>& GetHand(void) { return deck_; }

	/// @brief 当たった時の処理
	/// @param _hitCol 相手のコライダ
	void OnHit(const std::weak_ptr<Collider> _hitCol)override;
	
	/// @brief 入力方向に応じて方向を決める
	/// @param  
	void MoveDirFromInput(void)override;

	/// @brief ゴール角度をセット
	/// @param  
	void SetGoalRotate(void)override;

	/// @brief 攻撃の当たり判定作成
	/// @param _charaTag キャラクタータグ
	/// @param _attackTag 攻撃タグ
	/// @param _atkPos 攻撃座標(球の当たり判定用)
	/// @param _radius 半径(球の当たり判定用)
	void MakeAttackCol(const Collider::TAG _charaTag, const Collider::TAG _attackTag, const VECTOR& _atkPos, const float& _radius)override;

	/// @brief 攻撃の当たり判定削除
	/// @param _charaTag 自身のタグ
	void DeleteAttackCol(const Collider::TAG& _charaTag, const Collider::TAG& _attackCol)override;

	/// @brief ダメージ処理
	/// @param _dam ダメージ数
	virtual void Damage(const int _dam);

private:

	//足音間隔
	static constexpr float FOOT_SE_DIS = 0.2f;

	//重力の割合
	static constexpr float GRAVITY_PER = 20.0f;

	//プレイヤー１のX座標
	static constexpr float PLAYER_ONE_POS_X = -300.0f;

	//座標の間隔
	static constexpr float DISTANCE_POS = 50.0f;

	//プレイヤーのローカル角度
	static constexpr float MODEL_LOCAL_DEG = 180.0f;

	//プレイヤーの大きさ
	static constexpr VECTOR MODEL_SCL = { 1.0f,1.0f,1.0f };

	//プレイヤーの初期のZ座標
	static constexpr float INIT_POS_Z = -500.0f;

	//プレイヤーナンバー(デッキクラスで判定用)
	static constexpr int PLAYER_NUM = 0;

	//プレイヤーの腰のフレーム番号
	static constexpr int SPINE_FRAME_NO = 0;

	//プレイヤーHPバー
	static constexpr Vector2 START_HPBAR_POS = { 10,10 };
	static constexpr Vector2 HPBAR_SIZE = { 200,30 };

	//敵ヒットSEボリューム
	static constexpr float ENEMY_HIT_SE_VOL = 0.8f;

	//カード最大枚数
	static constexpr int CARD_NUM_MAX = 15;

	//手のフレーム番号
	static constexpr int HAND_FRAME_NO = 36;

	//当たり判定を行う範囲
	static constexpr int COL_RANGE = 1;

	//入力デバイス
	InputManager::CONTROLL_TYPE cntl_;

	//ゲームパッド番号
	InputManager::JOYPAD_NO padNum_;

	//カメラ
	std::weak_ptr<Camera>camera_;

	//プレイヤー単体が持っているもの
	int playerNum_;			//プレイヤー番号

	//状態更新
	std::function<void(void)>stateUpdate_;

	//武器オブジェクト
	std::unique_ptr<Weapon>weapon_;

#ifdef _DEBUG
	//デバッグ描画
	void DrawDebug(void);
#endif // _DEBUG

	//アニメーションの追加
	void AddAnimation(void)override;

	//アクションの追加
	void AddAction(void)override;

	//当たり判定初期化
	void MakeColliderGeometry(void)override;

	//更新
	void UpdateDirection(void) override;		//演出
	void UpdateNormal(void)override;			//通常(ゲーム中)
	void UpdateClearDirection(void)override;	//クリア演出
	void UpdateOverDirection(void)override;		//ゲームオーバー

	//ゲームオーバーに遷移
	void ChangeUpdateOverDirection(void)override;
	
	//アクション関係
	void Action(void);
};

