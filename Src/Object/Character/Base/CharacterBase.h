#pragma once
#include <map>
#include <nlohmann/json.hpp>
#include "Manager/Resource/SoundManager.h"
#include "Object/Common/AnimationController.h"
#include "Object/Card/CardBase.h"
#include "Object/Card/CardPresenter.h"
#include "Object/Character/Base/CharacterOnHitBase.h"
#include "Object/Character/Base/CardActionBase.h"
#include "Object/Character/UIData/CharacterUIData.h"
#include "Object/Character/Base/ActionBase.h"
#include "Object/ObjectBase.h"

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
class ItemBase;

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
		MAGIC_FIRE,			//炎魔法
		JUMP,				//ジャンプ
		DODGE,				//回避
		CARD_RELOAD,		//カードリロード
		//敵
		STOMP_ATK,			//スタンプ攻撃
		ROAR_ATK,			//咆哮
		JUMP_ATK,			//ジャンプ攻撃
		RUSH_ATK,			//突進
		REVOLUTION			//革命開始アクション

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

	//角度
	struct ROTATION
	{
		//回転
		Quaternion playerRotY_=Quaternion();	//プレイヤーY角度
		Quaternion goalQuaRot_ = Quaternion();	//目的の回転
		float stepRotTime_=0.0f;				//補完時間
		VECTOR dir_ = {};						//方向
	};

	//各ステータス
	struct STATUS
	{
		float speed;		//移動速度
		float hp = HP_MAX;	//体力
	};

	/// @brief コンストラクタ
	/// @param  
	CharacterBase(void);

	/// @brief デストラクタ
	/// @param  
	virtual ~CharacterBase(void)override = 0;

	/// @brief 読み込み
	/// @param  
	virtual void Load(void)override;

	/// @brief 初期化
	/// @param  
	virtual void Init(void)override;

	/// @brief 更新
	/// @param  
	void Update(void)override;

	/// @brief 描画
	/// @param  
	void Draw(void)override;

	/// @brief 影を作るためのモデル描画
	/// @param  
	void DrawShadow(void);

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

	/// @brief ターゲットの方向に向く
	/// @param  
	void LookAtTargetVec(void);
	
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

	/// @brief カードアクションの取得
	/// @param  
	/// @return 
	const bool GetIsJumpAtk(void)const;

	/// @brief 敵のジャンプチャージ中取得
	/// @param  
	/// @return true:敵のジャンプチャージ中
	const ActionBase& GetMainAction(void)const;

	/// @brief ヒットポイントを当たり判定から取得
	/// @param  
	/// @return ヒットポイント
	const CharacterOnHitBase::HIT_POINT& GetHitPoint(void)const;

	/// @brief カプセルのTop座標を取得
	/// @param  
	/// @return カプセルのTop座標
	const VECTOR GetCapsuleTop(void)const;

	/// @brief カプセルのDown座標を取得
	/// @param  
	/// @return カプセルのTop座標
	const VECTOR GetCapsuleDown(void)const;

	/// @brief ロジッククラスにターゲットをセット
	/// @param _targetChara ターゲット
	void SetLogicTargetCharacter(std::shared_ptr<CharacterBase>_targetChara);

	/// @brief ターゲットと当たったかどうかの取得
	/// @param  
	/// @return ターゲットと当たったかどうか
	const bool GetIsHitTarget(void)const;

	/// @brief 操作可能かどうかセット
	/// @param _isCanMoveable true:操作可能	false:操作不可能
	void SetIsCanMoveable(const bool _isCanMoveable) { isMoveable_ = _isCanMoveable; }

	/// @brief カードを使用済みにする(自分の攻撃中、敵の攻撃に当たった時)
	/// @param  
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

	/// @brief スタンプ攻撃の岩の描画配列の登録
	/// @param _rock 
	void RegisterDrawableRocks(const std::weak_ptr<EnemyRock> _rock);

	/// @brief スタンプ攻撃の岩の描画配列の登録解除
	/// @param _rock 解除する岩
	void UnRegisterDrawableRocks(void);

	/// @brief 炎魔法の描画
	/// @param _fire 
	void DrawItem(const std::weak_ptr<ItemBase> _item);

	/// @brief 炎の描画をやめる
	/// @param  
	void DeleteItem(void);

protected:

#pragma region メンバー定数
	//移動量ラインオフセット
	static constexpr float MOVE_LINE_Y_OFFSET = -1.0f;

	//移動量更新条件の移動ラインの長さ
	static constexpr float MOVE_LINE_Y_CHECK_VALUE = 1.5f;

	//キャラステータスのデータパス
	const std::string PLAYER_STATUS_DATA = "Player";	//プレイヤー
	const std::string ENEMY_STATUS_DATA = "Enemy";		//敵
#pragma endregion

#pragma region 外部ファイル読み込み
	//使用するモデル
	ResourceManager::SRC useModelSrc_= ResourceManager::SRC::NONE;		
	STATUS maxStatus_;						//ステータス最大値
	VECTOR localDeg_{};						//ローカル回転
	VECTOR localPos_{};						//ローカル座標
	float modelScl_=0.0f;					//モデルの大きさ
	int spineFrameNo_ = 0;					//腰のボーン番号
	VECTOR battleStartPos_ = {};			//戦闘開始時のスタート座標		
#pragma endregion

#pragma region メンバー変数
	std::unique_ptr<EffectController>effect_;				//エフェクト
	std::unique_ptr<LogicBase>logic_;						//入力
	std::unique_ptr<ActionController>actionCtrl_;			//行動系
	std::unique_ptr<AnimationController>animCtrl_;			// アニメーション
	std::shared_ptr<CardDeck>deck_;							//デッキ
	std::unique_ptr<CharacterOnHitBase>onHit_;				//当たった時の処理
	std::unique_ptr<CardPresenter>cardPresent_;				//カードUIと内部のデッキクラスの結びつけクラス
	std::vector<std::weak_ptr<EnemyRock>>drawableRocks_;	//描画する岩の配列
	std::weak_ptr<ItemBase>drawableItem_;					//炎の描画

	std::unordered_map <UPDATE_PHASE, std::function<void(void)>>changePhase_;	//更新フェーズ変更
	std::function<void(void)>updatePhase_;										//更新フェーズの更新

	std::unordered_map<std::string, ANIM_TYPE>animStrTable_;					//アニメーションタイプの文字列対応表

	//アクションごとのアニメーション再生データテーブル
	std::unordered_map<ANIM_TYPE, AnimationController::ANIMATION_VARIABLE> actionAnimTable_;
	std::unordered_map<ANIM_TYPE, float> animParam_;	//アニメーションパラメータ
	std::vector<std::string> actionStr_;				//アクションの文字列

	UIManager& uiMng_;				//UIマネージャ
	SoundManager& soundMng_;		//サウンドマネージャ

	//当たり判定の要素
	VECTOR movedPos_;						//移動後座標
	VECTOR moveDiff_;						//移動前座標
	ROTATION charaRot_;						//角度関連
	STATUS status_;							//ステータス
	UPDATE_PHASE phase_;					//更新フェーズ
	bool isMoveable_;						//移動操作可能か
	bool isEndClearDirect_;					//クリア演出が終わったか
	HP_DATA hpData_;						//Hpのデータ
	CHARACTER_TYPE characterType_;			//キャラ種別
	int hitStopFrame_;						//ヒットストップ用カウンタ(フレーム)
	float animSpdScl_;						//アニメーション速度の倍率

	//演出用アニメーションのパラメータ
	AnimationController::ANIMATION_VARIABLE deathAnim_;		//死亡アニメーション
	AnimationController::ANIMATION_VARIABLE clearAnim_;		//クリア演出アニメーション
	AnimationController::ANIMATION_VARIABLE overAnim_;		//ゲームオーバー演出アニメーション
	AnimationController::ANIMATION_VARIABLE idleAnim_;		//アイドルアニメーション
#pragma endregion

#pragma region メンバー関数
	//各キャラクターの基本処理
	virtual void LoadCharacter(void) = 0;						//ロード
	virtual void InitCharacter(void) = 0;						//初期化
	virtual void UpdateDirectionCharacter(void) = 0;			//演出時更新
	virtual void UpdateNormalCharacter(void) = 0;				//更新
	virtual void UpdateClearDirectionCharacter(void) = 0;		//クリア演出
	virtual void UpdateOverDirectionCharacter(void) = 0;		//オーバー演出
	virtual void DrawCharacter(void) = 0;						//描画

	//移動後座標などの更新
	void UpdatePost(void);

	//移動制限
	void MoveLimit(const VECTOR& _stagePos, const VECTOR& _stageSize);

	//コライダ作成
	virtual void MakeColliderGeometry(void)=0;

	//アクション関連のデータを外部からロード
	using OnActionDataLoaded = std::function<void(const ACTION_LOAD_DATA&)>;
	void LoadActionData(OnActionDataLoaded callBack=nullptr);
	virtual void LoadModelDataCharacter(const nlohmann::json& _data) {};

	//アクションデータの呼び出し時のコールバック
	virtual void LoadCharacterActionDataCallBack(const ACTION_LOAD_DATA& _data) = 0;

	//ロジックによる操作を受け付ける
	virtual void AcceptLogicControl(void);

	//更新フェーズ	
	void UpdateNone(void);				//何もしない
	void UpdateNormal(void);			//通常更新
	void UpdateDirection(void);			//演出時更新
	void UpdateClearDirection(void);	//クリア演出
	void UpdateOverDirection(void);		//オーバー演出
	void UpdateHitStop(void);						//ヒットストップ更新

	//遷移先の更新フェーズ
	void ChangeUpdateNone(void);				//何もしない
	void ChangeUpdateNormal(void);				//通常
	void ChangeUpdateDirection(void);			//演出
	virtual void ChangeUpdateClearDirection(void);	//クリア演出
	virtual void ChangeUpdateOverDirection(void);	//ゲームオーバー演出
	void ChangeUpdateHitStop(void);				//ヒットストップ
#pragma endregion

private:

#pragma region メンバー変数
	//データロード関数テーブル
	std::unordered_map<std::string, std::function<void(const nlohmann::json& _data)>>loadDataFuncTable_;
#pragma endregion

#pragma region メンバー関数
	//キャラクター共通の処理
	void LoadCommon(void);						//ロード
	void LoadCommonData(void);					//キャラクター共通のデータを外部ファイルから読み込み
	void InitCommon(void);						//初期化
	void UpdateNormalCommon(void);				//更新
	void UpdateDirectionCommon(void);			//演出時の共通の更新
	void UpdateClearDirectionCommon(void);		//ゲームクリア時
	void UpdateOverDirectionCommon(void);		//ゲームオーバー時
	void DrawCommon(void);						//描画

	//外部からのロード関数
	void LoadStatus(const nlohmann::json& _data);					//ステータス
	virtual void LoadModelDataCommon(const nlohmann::json& _data);		//モデル情報
	void LoadBattleStartPos(const nlohmann::json& _data);			//バトル開始時の座標の読み込み
#pragma endregion
};