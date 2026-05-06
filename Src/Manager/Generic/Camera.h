#pragma once
#include <DxLib.h>
#include <functional>
#include <map>
#include "../../Common/Quaternion.h"
#include "../../Object/ObjectBase.h"
#include "../Template/Singleton.h"
#include "../Common/Easing.h"

class Transform;
class Easing;

class Camera
{
public:

	// カメラスピード(度)
	static constexpr float SPEED = 1.0f;

	//カメラのスピードパッド時
	static constexpr float SPEED_PAD = 0.0015f;	

	// カメラクリップ：NEAR
	static constexpr float CAMERA_NEAR = 10.0f;

	// カメラクリップ：FAR
	static constexpr float CAMERA_FAR = 30000.0f;

	// カメラの初期座標
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };

	// 追従位置からカメラ位置までの相対座標
	static constexpr VECTOR LOCAL_F2C_POS = { 0.0f, 800.0f, -1100.0f };
	static constexpr VECTOR ENEMY_ONLY_LOCAL_F2C_POS = { 0.0f, 100.0f, -800.0f };
	static constexpr VECTOR TARGET_CAM_LOCAL_F2C_POS = { 0.0f, 500.0f, -700.0f };

	// 追従位置から注視点までの相対座標
	static constexpr VECTOR LOCAL_F2T_POS = { 0.0f, 0.0f, 200.0f };

	// カメラのX回転上限度角
	static constexpr float LIMIT_X_UP_RAD = 15.0f * (DX_PI_F / 180.0f);
	static constexpr float LIMIT_X_DW_RAD = -30.0f * (DX_PI_F / 180.0f);

	// ターゲットカメラ遷移時の補完時間
	static constexpr double CHANGE_TARGET_LERP_TIME = 0.7;

	// カメラ感度
	static constexpr float FOV_PER = 0.2f;

	// 1シェイクにかかる時間
	static constexpr float SHAKE_PER = 0.1f;
	
	//当たり判定の半径
	static constexpr float HIT_RADIUS = 30.0f;

	//当たった時の押し出し回数
	static constexpr int COL_TRY_CNT_MAX = 10;

	//ヒットした法線方向へのオフセット
	static constexpr float HIT_NORMAL_OFFSET = 2.0f;

	// 衝突時の押し戻し試行回数
	static constexpr int CNT_TRY_COLLISION_CAMERA = 30;

	// 衝突判定用球体半径
	static constexpr float COL_CAPSULE_SPHERE = 50.0f;
	static constexpr float COL_SPHERE_SPHERE = 40.0f;

	// 衝突時の押し戻し量
	static constexpr float COLLISION_BACK_DIS = 2.0f;

	// カメラモード
	enum class MODE
	{
		NONE,
		FIXED_POINT,
		FOLLOW,
		SELF_SHOT,
		CHANGE_TARGET,
		TARGET_POINT,
		START_DIRECTION,
	};

	//カメラ演出
	enum class DIRECTION_MODE
	{
		NONE,
		PLAYER_AND_ENEMY_VIEW,
		ENEMY_ONLY_VIEW,
		ENEMY_ROAR_VIEW,
		PLAYER_ONLY_VIEW,
		END
	};

	//イージングモード
	enum class SUB_MODE
	{
		NONE,		//何もない
		SHAKE,		//連続シェイク
		ONE_SHAKE,	//一回のみシェイク
	};

	/// @brief コンストラクタ
	/// @param  
	Camera(void);

	/// @brief デストラクタ
	/// @param  
	~Camera(void);

	/// @brief 初期化
	/// @param  
	void Init(void);

	/// @brief 更新
	/// @param  
	void Update(void);

	/// @brief カメラの設定
	/// @param  
	void SetBeforeDraw(void);

	/// @brief 描画
	/// @param  
	void Draw(void);

	/// @brief カメラ位置の取得
	/// @param  
	/// @return カメラ位置
	const VECTOR GetPos(void) const { return pos_; }

	/// @brief カメラの操作角度
	/// @param  
	/// @return カメラ角度
	const VECTOR GetAngles(void) const { return angles_; }

	/// @brief カメラの注視点の角度
	/// @param  
	/// @return カメラの注視点
	const VECTOR GetTargetPos(void) const {return pos_;}

	/// @brief カメラ角度の取得
	/// @param  
	/// @return カメラ角度
	const Quaternion GetQuaRot(void) const {return rot_;}

	/// @brief X回転を抜いたカメラ角度の取得
	/// @param  
	/// @return X回転を抜いたカメラ角度
	const Quaternion GetQuaRotOutX(void) const { return rotOutX_; }

	/// @brief カメラの前方方向の取得
	/// @param  
	/// @return カメラの前方方向
	VECTOR GetForward(void) const;

	/// @brief カメラモードの変更
	/// @param mode 変更後のカメラモード
	void ChangeMode(const MODE mode);
	
	/// @brief サブ処理の変更
	/// @param _submode サブ処理(イージングなど)
	void ChangeSub(const SUB_MODE _subMode);

	/// @brief 追従対象の設定
	/// @param _follow 追従したい対象のTransform
	/// @param _localCenterPos 対象の中心位置からのローカル座標
	void SetFollow(const Transform* _follow,const VECTOR _localCenterPos);

	/// @brief ターゲットとする対象の設定
	/// @param _target ターゲット
	void SetTarget(const Transform* _target);

	/// @brief ターゲットの座標セット
	/// @param _targetPos ターゲットの座標
	void SetTargetPos(const VECTOR _targetPos);
	
	/// @brief シェイク時にセットするカウント(割合)
	/// @param t 現在の時間割合(1回のみのシェイクにする場合は‐1を入れる)
	/// @param limit 範囲の加減
	/// @param _easeType 使用したいイージング
	/// @param shakeTime 1回のシェイク時間(1回シェイクをしたい場合のみ)
	void SetShakeStatus(const float t, const float limit = 0.0f
		, const Easing::EASING_TYPE _easeType = Easing::EASING_TYPE::COS_BACK, const float shakeTime=0.0f);

	/// @brief カメラとステージの当たり判定のためのステージのTransformのセット
	/// @param _stageTrans ステージのTransform
	void SetStageTransform(const Transform* _stageTrans)
	{
		stageTransform_ = _stageTrans;
	}

	/// @brief 演出カメラの終了判定の取得
	/// @param  
	/// @return 演出カメラの終了判定
	const bool IsEndDirectionMode(void)
	{
		return mode_ == MODE::FOLLOW;
	}

	/// @brief 演出カメラのモードの取得
	/// @param  
	/// @return 演出カメラのモード
	const DIRECTION_MODE GetDirectionMode(void) const
	{
		return directionMode_;
	}

private:

	//カメラの初期角度
	static constexpr float DEFAULT_CAMERA_ANGLES_RAD_X = 30.0f;

	//追従対象のフレームナンバー
	static constexpr int FOLLOW_FRAME_NUM = 1;

	//プレイヤーと敵を両方映す時間
	static constexpr float PLAYER_AND_ENEMY_VIEW_TIME = 2.0f;

	//敵の咆哮を映す時間
	static constexpr float ENEMY_ROAR_VIEW_TIME = 6.0f;

	//プレイヤーの頭上位置
	static constexpr VECTOR PLAYER_HEAD_POS = { 0.0f,160.0f,0.0f };

	//敵の頭上位置
	static constexpr VECTOR PLAYER_WAIST = { 0.0f,80.0f,0.0f };

	//プレイヤーと敵を移す時のイージング追従位置から注視店までの相対座標
	static constexpr VECTOR PLAYER_AND_ENEMY_LOCAL_F2C_POS = { 0.0f, -600, -1400.0f };

	//敵のみを移す演出時の追従位置から注視点までの相対座標
	static constexpr VECTOR ENEMY_ONLY_LOCAL_F2T_POS = { 0.0f, 500.0f, 200.0f };

	//プレイヤーのみを移す演出時の追従位置からカメラまでのイージング初期の相対座標
	static constexpr VECTOR PLAYER_ONLY_LOCAL_F2C_START_POS = { 500.0f, 100.0f, -800.0f };

	//プレイヤーのみを移す演出時の追従位置からカメラまでのイージング終端の相対座標
	static constexpr VECTOR PLAYER_ONLY_LOCAL_F2C_GOAL_POS = { 0.0f,-150.0f,-170.0f };

	//プレイヤーのみを移す演出時のカメラY角度
	static constexpr float PLAYER_ONLY_CAMERA_ANGLE_Y = -145.0f;

	//敵咆哮演出時のカメラ振動範囲
	static constexpr float ENEMY_ROAR_SHAKE_LIMIT = 5.0f;

	//演出終了のゴール角度
	static constexpr float END_DIRECTION_GOAL_ANGLE = -10.0f;

	// カメラが追従対象とするTransform
	const Transform* followTransform_;
	VECTOR followLocalCenterPos_;

	//カメラの注視点とするターゲットTransform
	const Transform* targetTransform_;

	//ターゲットの座標(フレーム座標)
	VECTOR targetPoses_;

	//イージング
	std::unique_ptr<Easing>easing_;

	//ステージのTransform
	const Transform* stageTransform_;

	// カメラモード
	MODE mode_;

	//サブモード
	SUB_MODE subMode_;

	//カメラ更新
	std::function<void(void)>modeUpdate_;

	//イージングなど、、同時に動かしたい更新
	std::function<void(void)>subUpdate_;

	//カメラモード遷移
	std::map<MODE, std::function<void(void)>>changeMode_;
	std::map<SUB_MODE, std::function<void(void)>>changeSub_;

	//演出カウント
	float directionCnt_;

	//イージング初期値
	float startF2CPosZ_;

	//イージング終端値
	float goalF2CPosZ_;

	//イージングカウント
	float directionEaseCnt_;

	//カメラ演出モード
	DIRECTION_MODE directionMode_;

	//カメラ演出更新
	std::function<void(void)>directionUpdate_;

	//カメラ演出遷移
	std::map<DIRECTION_MODE, std::function<void(void)>>changeDirectionMode_;

	//カメラの位置
	VECTOR pos_;

	//移動後座標
	VECTOR goalPos_;

	//カメラ角度(rad)
	VECTOR angles_;

	//X軸回転が無い角度
	Quaternion rotOutX_;

	//カメラ角度
	Quaternion rot_;

	//注視点
	VECTOR targetPos_;

	// カメラの上方向
	VECTOR cameraUp_;
	
	//カメラシェイクのステータス
	Easing::EASING_TYPE easeType_;	//イージングの種類
	float shekePerCnt_;//1シェイクのカウント
	float initLimit_;	//初めに設定する範囲
	float limit_;		//動かす範囲
	float easePer_;		//シェイク全体時間
	float initPosY_;		//動かす前のカメラ座標Y
	
	//1回のみのシェイク時間
	float oneShakeTime_;

	//ターゲットカメラ遷移カウント
	double changeTargetLerpCnt_;

	//ターゲットカメラ遷移中フラグ
	bool isChangingCamera_;

	//追従対象フレーム座標
	VECTOR followFramePos_;

	//追従対象の中心座標
	VECTOR followCenterPos_;

	//追従位置から注視点までの相対座標
	VECTOR localF2TPos_;

	//追従位置からカメラ位置までの相対座標
	VECTOR localF2CPos_;

	//イージングスタートF2C位置
	VECTOR easingStartF2CPos_;

	//イージングゴールF2C位置
	VECTOR easingGoalF2CPos_;

	//イージングスタートF2C位置
	VECTOR easingStartF2TPos_;

	//イージングゴールF2C位置
	VECTOR easingGoalF2TPos_;

	//演出用ローカル中心座標
	VECTOR startFollowLocalCenterPos_;
	VECTOR goalFollowLocalCenterPos_;

	//イージング角度
	VECTOR startAngles_;
	VECTOR goalAngles_;

	//イージング演出時、次の状態格納
	DIRECTION_MODE nextDirectionMode_;

	//イージング演出次のタイプ格納
	float directionEasingTime_;

	// カメラを初期位置に戻す
	void SetDefault(void);

	// 追従対象との位置同期を取る
	void SyncFollow(const Transform* _followTransform);

	//ターゲットカメラの追従
	//void SyncTargetFollow(void);
	void SyncTargetFollow(void);

	// カメラ操作
	void ProcessRot(void);

	//スムーズにターゲットカメラに変わる
	void SmoothChangeCamera(void);

	/// @brief 当たり判定
	/// @param  
	void Collision(void);

	/// @brief 演出モード変更
	/// @param _mode 切り替えたい演出モード
	void ChangeDirectionMode(const DIRECTION_MODE _mode);

	// モード別更新ステップ
	void SetBeforeDrawFixedPoint(void);
	void SetBeforeDrawFollow(void);
	void SetBeforeDrawSelfShot(void);
	void SetBeforeDrawLerpCamera(void);
	void SetBeforeDrawTargetPoint(void);
	void SetBeforeDrawStartDirection(void);

	//遷移
	void ChangeFixedPoint(void);
	void ChangeFollow(void);
	void ChangeSelfShot(void);
	void ChangeTargetLerp(void);
	void ChangeTargetCamera(void);
	void ChangeStartDirection(void);

	//サブモード別更新
	void UpdateNone(void);
	void UpdateShake(void);
	void UpdateShakeOne(void);

	//サブモード遷移
	void ChangeNone(void);
	void ChangeShake(void);
	void ChangeShakeOne(void);

	//カメラ演出
	//更新
	void DirectionNone(void);
	void DirectionPlayerAndTarget(void);
	void DirectionEnemyOnly(void);
	void DirectionEnemyRoar(void);
	void DirectionPlayerOnly(void);
	void EndDirection(void);

	//遷移
	void ChangeDirectionNone(void);
	void ChangeDirectionLegLow(void);
	void ChangeDirectionEnemyOnly(void);
	void ChangeDirectionEnemyRoar(void);
	void ChangeDirectionPlayerOnly(void);
	void ChangeEndDirection(void);
};