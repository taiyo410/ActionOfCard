#pragma once
#include <DxLib.h>
#include"../Common/Quaternion.h"
#include"../UIData/CharacterUIData.h"	

class ResourceManager;
class SceneManager;
class SoundManager;
class ActionController;
class AnimationController;
class CharacterBase;
class PlayerLogic;

class ActionBase
{

public:

	//攻撃のステータス(どちらも使用するステータス)
	struct ATK_STATUS
	{
		float colStartCnt = 0.0f;	//当たり判定発生の時間
		float colEndCnt = 0.0f;		//当たり判定終了の時間
		float bufferFrame = 0.0f;	//コンボ入力のバッファ
		float atkRadius = 0.0f;		//攻撃範囲
		float atkPoint = 0.0f;		//攻撃力
		VECTOR pos = {};			//当たり判定座標
		bool isDamage = true;		//ダメージ判定
	};

	/// @brief コンストラクタ
	/// @param _actCntl アクションコントローラー
	ActionBase(ActionController& _actCntl,CharacterBase& _character);

	/// @brief デストラクタ
	/// @param  
	virtual ~ActionBase(void);

	/// @brief ロード
	/// @param  
	virtual void Load(void) = 0;

	/// @brief 初期化
	/// @param  
	virtual void Init(void) = 0;

	/// @brief 更新
	/// @param  
	virtual void Update(void) = 0;
	
	/// @brief 解放
	/// @param  
	virtual void Release(void);

	/// @brief 1方向に動く移動量
	/// @param  
	/// @return 1方向に動く移動量
	const float& GetSpeed(void) { return speed_; }

	/// @brief 方向転換可能かどうかの取得
	/// @param  
	/// @return 方向転換可能かどうか
	const bool& GetIsTurnable(void) { return isTurnable_; }
	
	/// @brief 攻撃当たり判定中かの取得
	/// @param  
	/// @return 
	const bool& GetIsAliveAtkCol(void) { return isAliveAtkCol_; }

	/// @brief のけぞりカウントのセット
	/// @param _flinchTime のけぞりカウント
	void SetFlinchCnt(float _flinchTime) { flinchCnt_ = _flinchTime; }

	/// @brief 技攻撃力の取得
	/// @param  
	/// @return 技攻撃力
	const float& GetAtkPoint(void)const { return atk_.atkPoint; }

	////カードアクションの取得
	//const CARD_ACT_TYPE& GetCardAction(void)const { return actType_; }

	/// @brief リロード中かどうかの取得
	/// @param  
	/// @return  リロード中かどうか
	virtual const bool IsReloading(void)const { return false; }

	/// @brief ジャンプ攻撃かどうかの取得
	/// @param  
	/// @return ジャンプ攻撃かどうか
	virtual const bool IsJumpAtk(void)const { return false; }

	/// @brief ジャンプ攻撃チャージ
	/// @param  
	/// @return 
	virtual const bool IsJumpAtkCharge(void)const { return false; }

	/// @brief ジャンプチャージのカード勝利数
	/// @param  
	/// @return 
	virtual const int GetJumpCardNum(void)const { return -1; }

	/// @brief 攻撃がダメージを与えたかを返す
	/// @param  
	const bool GetIsDamage(void)const { return atk_.isDamage; }

	/// @brief ダメージを与えたことをセット
	/// @param  
	void SetIsDamage(void) { atk_.isDamage = true; }

	/// @brief アニメーション情報のロード
	/// @param name アニメーション名
	/// @param animVar アニメーション変数
	virtual void LoadAnimVar(const ACTION_LOAD_DATA& animVar) {}

protected:

	//移動スピード
	static constexpr float MOVE_SPEED = 6.0f;

	//ブレンド時間
	static constexpr float BLEND_TIME = 0.5f;

	//速度
	float speed_;			

	//怯み時間
	float flinchCnt_;

	//シーンマネージャ
	SceneManager& scnMng_;

	//サウンドマネージャ
	SoundManager& soundMng_;

	//リソースマネージャ
	ResourceManager& resMng_;

	//アクションコントローラー
	ActionController& actionCntl_;

	//アニメーション
	AnimationController& anim_;

	//キャラクター
	CharacterBase& character_;

	//方向転換可能かどうか
	bool isTurnable_;

	//敵への方向転換
	bool isTargetTurn_;

	//攻撃の当たり判定中か
	bool isAliveAtkCol_;

	//現在の攻撃
	ATK_STATUS atk_;

	////使用アニメーション
	AnimationController::ANIMATION_VARIABLE animVar_;

private:

	// 回転完了までの時間
	static constexpr float TIME_ROT = 0.1f;
};

