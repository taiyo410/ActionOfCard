#pragma once
#include<map>
#include "../Manager/Resource/SoundManager.h"
#include "../../Card/CardBase.h"
#include "../../Card/CardPresenter.h"
#include "../Base/CharacterOnHitBase.h"
#include "../Player/ActionController.h"
#include "../Base/CardActionBase.h"
#include "../UIData/CharacterUIData.h"
#include "../Base/ActionBase.h"
#include "../Object/ObjectBase.h"

class AnimationController;
class UIManager;
class ActionController;
class CardDeck;
class CardUIBase;
class CardPresenter;
class InputBase;
class Capsule;
class LogicBase;
class PlayerOnHit;
class EnemyOnHit;
class EnemyRock;

class CharacterBase 
	: public ObjectBase
{

public:

	//ヒットストップ止めるフレーム数
	static constexpr int HIT_STOP_FRAME = 5;

	//アニメーション速度
	static constexpr float DEFAULT_ANIM_SPEED = 40.0f;

	// 回転完了までの時間
	static constexpr float TIME_ROT = 0.1f;

	//最大ＨＰ
	static constexpr float HP_MAX = 200.0f;

	//転がるアニメーション速度
	static constexpr float ROLL_ANIM_SPEED = 20.0f;

	//当たり判定
	static constexpr int CUPSULE_COL_NO = 0;
	static constexpr int MOVE_LINE_COL_NO = 1;
	static constexpr int UP_DOWN_LINE = 2;
	static constexpr int ATK_COL_NO = 3;

	// アニメーション種別
	enum class ANIM_TYPE
	{
		NONE,				//何もしない
		IDLE,				//アイドル
		RUN,				//走る
		REACT,				//ダメージリアクション
		DEATH,				//死亡
		ATTACK_1_MIDDLE,	//中距離攻撃
		ATTACK_1_SHORT,		//近距離攻撃
		ATTACK_2,			//攻撃2段目
		ATTACK_3,			//攻撃3段目
		JUMP,				//ジャンプ
		DODGE,				//回避
		CARD_RELOAD,		//カードリロード
		//敵
		STOMP_ATK,			//スタンプ攻撃
		ROAR_ATK,			//咆哮
		JUMP_ATK,			//ジャンプ攻撃
		RUSH_ATK			//突進

	};

	//角度
	struct ROTATION
	{
		//回転
		Quaternion playerRotY_;		//プレイヤーY角度
		Quaternion goalQuaRot_;		//目的の回転
		float stepRotTime_;			//補完時間
		VECTOR dir_;			//方向
	};

	//各ステータス
	struct STATUS
	{
		float speed;		//移動速度
		float hp = HP_MAX;	//体力
	};

	//アクションの種類
	enum class ACTION_TYPE
	{
		IDLE,		//何もしてない
		MOVE,		//移動
		DASHMOVE,	//ダッシュ
		REACT,	//パンチされた状態
		JUMP,		//ジャンプ
		CARD_ACTION	//カードアクション
	};

	//更新フェーズ
	enum class UPDATE_PHASE
	{
		NONE,
		NORMAL,
		DIRECTION,
		CLEAR_DIRECTION,
		OVER_DIRECTION,
		HIT_STOP
	};

	//ステータスの種類
	enum class STATUS_TYPE
	{
		HP,
		ATK,
		DEF,
		SPD,
	};

	/// @brief コンストラクタ
	/// @param  
	CharacterBase(void);

	/// @brief デストラクタ
	/// @param  
	virtual ~CharacterBase(void)override = 0;

	/// @brief 読み込み
	/// @param  
	virtual void Load(void)override = 0;

	/// @brief 初期化
	/// @param  
	virtual void Init(void)override = 0;

	/// @brief 更新
	/// @param  
	void Update(void)override;

	/// @brief 描画
	/// @param  
	virtual void Draw(void)override = 0;

	//2D関連の描画
	virtual void Draw2D(void) = 0;

	/// @brief 当たった時の処理
	/// @param _hitCol 
	virtual void OnHit(const std::weak_ptr<Collider> _hitCol)override = 0;
	
	/// @brief 攻撃の当たり判定生成
	/// @param _charaTag 自身のタグ
	/// @param _atkPos 作りたい攻撃の座標
	/// @param _radius 当たり判定の半径
	virtual void MakeAttackCol(const Collider::TAG _charaTag, const Collider::TAG _attackTag, const VECTOR& _atkPos, const float& _radius);

	/// @brief 攻撃の当たり判定削除
	/// @param _charaTag 自身のタグ
	virtual void DeleteAttackCol(const Collider::TAG& _charaTag, const Collider::TAG& _attackCol);
	
	/// @brief キャラタグの取得
	/// @param  
	/// @return 
	const Collider::TAG& GetCharaTag(void)const { return tag_; }
	
	/// @brief キャラタイプの取得
	/// @param  
	/// @return 
	const CHARACTER_TYPE& GetCharaType(void)const { return characterType_; }

	/// @brief 入力方向に応じて方向を決める
	/// @param  
	virtual void MoveDirFromInput(void);

	/// @brief ゴール角度をセット
	/// @param _deg 
	virtual void SetGoalRotate(void) = 0;
	
	/// @brief 方向関連の変数の取得
	/// @param  
	/// @return 方向関連の変数
	inline const ROTATION& GetRotation(void)const { return charaRot_; }

	/// @brief 当たり判定の大きさの更新
	/// @param _radius 半径
	void UpdateAttackCol(const float _radius);

	/// @brief 角度更新
	/// @param  
	void Rotate(void);

	/// @brief ダメージ処理
	/// @param _dam ダメージ数
	virtual void Damage(const int _dam);

	/// @brief ダメージを食らったか
	/// @param  
	/// @return 
	const bool GetIsDamage(void)const;

	/// @brief キャラクターの中心座標取得
	/// @param  
	/// @return キャラクターの中心座標
	VECTOR GetCharaCenterPos(void)const;

	/// @brief ダメージを与えたことを知らせる
	/// @param  
	void SetIsDamage(void);

	/// @brief のけぞり時間のセット
	/// @param _flichCnt のけぞり時間
	void SetFlinchCnt(const float _flichCnt);

	/// @brief ステータス取得
	/// @param  
	/// @return 
	inline const STATUS& GetStatus(void)const { return status_; }

	/// @brief 足音の間隔時間の取得
	/// @param  
	/// @return 足音の間隔時間
	inline const float& GetFootSEDisCount(void)const { return footSEDisCount_; }

	/// @brief 足音素材の取得
	/// @param  
	/// @return 足音素材
	inline const ResourceManager::SRC& GetFootSE(void)const { return footSE_; }

	/// @brief カードアクションの取得
	/// @param  
	/// @return 
	const CardActionBase::CARD_ACT_TYPE& GetCardAction(void)const;

	/// @brief 敵のジャンプチャージ中取得
	/// @param  
	/// @return true:敵のジャンプチャージ中
	const ActionBase& GetMainAction(void)const;

	/// @brief ヒットポイントを当たり判定から取得
	/// @param  
	/// @return ヒットポイント
	const CharacterOnHitBase::HIT_POINT& GetHitPoint(void)const;

	/// @brief ロジッククラスにターゲットをセット
	/// @param _targetChara ターゲット
	void SetLogicTargetCharacter(std::shared_ptr<CharacterBase>_targetChara);
	
	/// @brief 岩配列
	/// @param _num 岩の生成数
	/// @param _atkPos 攻撃座標
	void AddEnemyRock(const int _num,VECTOR& _atkPos);

	/// @brief 岩配列ロード
	/// @param  
	void LoadEnemyRock(void);

	/// @brief 岩の当たり判定削除
	/// @param  
	void DeleteEnemyRockCol(void);

	/// @brief 岩の生存フラグセット
	/// @param _isAlive セットしたいフラグ
	void SetIsAliveEnemyRock(const bool _isAlive);

	/// @brief 岩配列更新
	/// @param  
	void EnemyRockUpdate(void);

	/// @brief ターゲットと当たったかどうかの取得
	/// @param  
	/// @return ターゲットと当たったかどうか
	const bool GetIsHitTarget(void)const;

	/// @brief 操作可能かどうかセット
	/// @param _isCanMoveable true:操作可能	false:操作不可能
	void SetIsCanMoveable(const bool _isCanMoveable) { isMoveable_ = _isCanMoveable; }

	//カードを使用済みにする(自分の攻撃中、敵の攻撃に当たった時)
	void SetUsedCard(void);


	/// @brief 遷移先の更新フェーズ設定
	/// @param _phase どの更新フェーズにするか
	void ChangeUpdatePhase(const UPDATE_PHASE _phase);

	/// @brief 演出から通常時に移行する時に初期化するもの
	/// @param  
	void ChangeDirectToNormal(void);

	/// @brief クリア演出が終わったか
	/// @param  
	/// @return true:クリア演出終了 : false:演出終了していない
	const bool GetIsEndDirect(void) { return isEndClearDirect_; }

	/// @brief キャラクター種類の取得
	/// @param  
	/// @return キャラクター種類
	const CHARACTER_TYPE GetCharacterType(void){ return characterType_; }

protected:

	//移動量ラインオフセット
	static constexpr float MOVE_LINE_Y_OFFSET = - 1.0f;

	//移動量更新条件の移動ラインの長さ
	static constexpr float MOVE_LINE_Y_CHECK_VALUE =  1.5f;

	//リロードカードステータス
	static constexpr CardBase::CARD_STATUS RELOAD_CARD_STATUS = { -1,CardBase::CARD_TYPE::RELOAD };

	//中心からのZオフセット
	static constexpr float CENTER_POS_Z_OFFSET = 600.0f;

	//キャラステータスのデータパス
	const std::string PLAYER_STATUS_DATA = "PlayerStatus";	//プレイヤー
	const std::string ENEMY_STATUS_DATA = "EnemyStatus";	//敵

	//入力
	std::unique_ptr<LogicBase>logic_;

	//行動系
	std::unique_ptr<ActionController>action_;

	// アニメーション
	std::unique_ptr<AnimationController>animationController_;

	//デッキ
	std::shared_ptr<CardDeck>deck_;

	//当たった時の処理
	std::unique_ptr<CharacterOnHitBase>onHit_;

	//敵のスタンプ攻撃時に発生する岩
	std::vector<std::unique_ptr<EnemyRock>>rock_;

	//使う足音
	SoundManager::SRC footSE_;

	//足音の再生間隔のディスカウント
	float footSEDisCount_;	

	//当たり判定の要素
	VECTOR movedPos_;		//移動後座標
	VECTOR moveDiff_;		//移動前座標
	VECTOR movePow_;		// 移動量

	//角度関連
	ROTATION charaRot_;

	//ステータス
	STATUS status_;

	//更新フェーズ
	UPDATE_PHASE updatePhase_;

	//更新フェーズ変更
	std::map <UPDATE_PHASE, std::function<void(void)>>changeUpdate_;

	//更新フェーズの更新
	std::function<void(void)>phazeUpdate_;

	//UIマネージャ
	UIManager& uiMng_;

	//カードUIと内部のデッキクラスの結びつけクラス
	std::unique_ptr<CardPresenter>cardPresent_;

	//サウンドマネージャ
	SoundManager& soundMng_;

	//移動操作可能か
	bool isMoveable_;

	//ステータス
	STATUS maxStatus_;

	//エフェクト
	std::unique_ptr<EffectController>effect_;

	//攻撃によってダメージを与えたか(与えたら判定を抜ける)
	bool isDamage_;

	//ダメージ
	float damagePoint_;

	//カプセル半径
	float capRadius_;

	//クリア演出が終わったか
	bool isEndClearDirect_;

	//Hpのデータ
	HP_DATA hpData_;

	//キャラ種別
	CHARACTER_TYPE characterType_;

	//ヒットストップ用カウンタ(フレーム)
	int hitStopFrame_;

	//移動後座標などの更新
	void UpdatePost(void);

	//Jsonからステータスをロード
	void LoadStatus(void);

	//移動制限
	void MoveLimit(const VECTOR& _stagePos, const VECTOR& _stageSize);

	//アクションの追加
	virtual void AddAction(void) = 0;

	//アニメーションの追加
	virtual void AddAnimation(void) = 0;

	//コライダ作成
	virtual void MakeColliderGeometry(void) = 0;;

	//更新フェーズ	
	void UpdateNone(void);							//何もしない
	virtual void UpdateNormal(void) = 0;			//通常更新
	virtual void UpdateDirection(void) = 0;			//演出時更新
	virtual void UpdateClearDirection(void) = 0;	//クリア演出
	virtual void UpdateOverDirection(void) = 0;	//オーバー演出
	void UpdateHitStop(void);						//ヒットストップ更新


	//遷移先の更新フェーズ
	void ChangeUpdateNone(void);				//何もしない
	void ChangeUpdateNormal(void);				//通常
	void ChangeUpdateDirection(void);			//演出
	virtual void ChangeUpdateClearDirection(void);	//クリア演出
	virtual void ChangeUpdateOverDirection(void);	//ゲームオーバー演出
	void ChangeUpdateHitStop(void);				//ヒットストップ
};