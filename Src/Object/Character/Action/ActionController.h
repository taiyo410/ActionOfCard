#pragma once
#include <DxLib.h>
#include <memory>
#include <functional>
#include <map>
#include <queue>
#include"../Utility/UtilityCommon.h"
#include"../Utility/Utility3D.h"
#include"../Manager/Generic/InputManager.h"
#include"../Base/ActionBase.h"
#include"../Object/Common/Transform.h"
#include"../UIData/CharacterUIData.h"

class CharacterBase;
class Player;
class PlayerInput;
class AnimationController;
class SceneManager;
class CardDeck;
class CardPresenter;
class ActionBase;
class LogicBase;
class Idle;
class Run;
class Jump;
class React;
class PlayerCardAttackOneMiddle;
class PlayerCardAttackOneShort;
class PlayerCardAttackTwo;
class PlayerCardAttackThree;
class PlayerCardMagicFire;
class EnemyCardAttackJump;
class EnemyCardAttackStomp;


class ActionController
{

public:

	//パンチ有効時間
	static constexpr float PUNCH_TIME_MAX = 0.5f;
	
	//プレイヤーの入力される種類
	enum class ACTION_TYPE
	{
		IDLE,			//何もしてない
		MOVE,			//移動
		REACT,			//パンチされた状態
		JUMP,			//ジャンプ
		CARD_ATTACK_ONE_MIDDLE,	//カードアクション
		CARD_ATTACK_ONE_SHORT,	//カードアクション
		CARD_ATTACK_TWO,	//カードアクション
		CARD_ATTACK_THREE,	//カードアクション
		CARD_RELOAD,	//カードリロード
		CARD_MAGIC_FIRE,//カードによる炎魔法
		CARD_ATTACK_ENEMY_JUMP,
		CARD_ATTACK_ENEMY_STOMP,
		DODGE,
	};

	/// @brief コンストラクタ
	/// @param _charaObj キャラクターオブジェクト
	/// @param _input 入力情報
	/// @param _trans モデル情報
	/// @param _deck デッキ
	/// @param _anim アニメーション情報
	/// @param _padNum パッド情報
	ActionController(CharacterBase& _charaObj,LogicBase& _input, Transform& _trans,
		CardPresenter& _deck, AnimationController& _anim, InputManager::JOYPAD_NO _padNum);

	/// @brief デストラクタ
	/// @param  
	~ActionController(void);
	
	/// @brief 初期化
	/// @param  
	void Init(void);
	
	/// @brief 読み込み
	/// @param  
	void Load(void);
	
	/// @brief 更新処理
	/// @param  
	void Update(void);
	
	/// @brief 状態遷移
	/// @param _act 次に遷移したい状態
	void ChangeAction(ACTION_TYPE _act);

	/// @brief 移動量の取得
	/// @param  
	/// @return 移動量
	const VECTOR& GetMovePow(void)const { return movePow_; }
	
	/// @brief 入力クラスの取得
	/// @param  
	/// @return 入力クラス
	LogicBase& GetInput(void)const { return logic_; }
	
	/// @brief アニメーションの再生
	/// @param  
	/// @return 
	AnimationController& GetAnimation(void)const { return anim_; }
	
	/// @brief 角度Yを取得
	/// @param  
	/// @return 
	const Quaternion GetPlayerRotY(void);
	
	/// @brief スピードの取得
	/// @param  
	/// @return スピード
	const float& GetSpd(void)const;

	/// @brief 再生しているリソースをすべて止める
	/// @param  
	void StopResource(void);

	/// @brief カード選択条件
	/// @param  
	/// @return true:カード選択できる
	const bool IsCardDecisionControl(void);

	/// @brief のけぞりカウントのセット
	/// @param _flinchTime のけぞらせたい時間
	void SetFlinchCnt(const float _flinchTime);

	/// @brief アクションの追加
	/// @param _types 追加したいアクションタイプ
	void AddAction(std::vector<ACTION_TYPE> _types);

	/// @brief メインアクションの取得
	/// @param  
	/// @return 
	ActionBase& GetMainAction(void);

	/// @brief 
	/// @param  
	/// @return 
	const bool GetIsJumpAtk(void)const;

	//アクションごとのアニメーションのロードの通知
	void AnimLoadNotify(const ACTION_LOAD_DATA& animVar);

	/// @brief カードアクションを決める
	/// @param  
	void DesideCardAction(void);

	/// @brief 通常攻撃のコンボ
	/// @param  
	void ChangeComboCardAttack(void);

	/// @brief コンボ入力の受付
	/// @param  
	void ComboInput(void);

	/// @brief 回避によるカードアクション中断
	/// @param  
	void CancelCardActionByDodge(void);

#ifdef _DEBUG
	//デバッグb
	void DrawDebug(void);
	//カードアクション時間(デバッグ用)
	static constexpr float CARD_ACT_TIME_MAX = 5.0f; 

#endif // _DEBUG

private:

	//プレイヤーナンバー(カードデッキで判定する用)
	static constexpr int PLAYER_NUM = 0;

	// 回転完了までの時間
	static constexpr float TIME_ROT = 0.0f;

	//CPUの思考ウェイト(高いほど出す頻度が高い)
	static constexpr int STOMP_WEIGHT = 50;	//通常攻撃
	static constexpr int JUMP_WEIGHT = 50;		//ジャンプ攻撃

	//プレイヤーに攻撃を仕掛けるプレイヤーとの距離
	static constexpr float ATK_DISTANCE = 250.0f;

	// シーンマネージャ参照
	SceneManager& scnMng_;

	//アニメーションコントローラー
	AnimationController& anim_;

	//状態遷移
	std::map<ACTION_TYPE, std::function<void(void)>>changeAction_;

	//状態更新
	std::function<void(void)>actionUpdate_;

	//アクション関数ポインタ
	std::unordered_map<ACTION_TYPE, std::function<void(void)>>actionTable_;

	//先行入力を受けつける溜めの攻撃配列
	std::queue<ACTION_TYPE>atkCombos_;

	//カードデッキ
	CardPresenter& cardPresent_;

	//オブジェクト(当たり判定用)
	CharacterBase& charaObj_;

	//モデル情報
	Transform& trans_;

	//各キャラクターの入力情報
	LogicBase& logic_;

	//状態
	ACTION_TYPE act_;

	//パッド番号
	InputManager::JOYPAD_NO padNum_;

	//カード関連
	bool isCardAct_;	//カードアクション中かどうか

	//移動関連
	float speed_;			// 移動スピード
	VECTOR moveDir_;		// 移動方向
	VECTOR movePow_;		// 移動量

	//回転
	Quaternion playerRotY_;		//プレイヤーY角度
	Quaternion goalQuaRot_;		//目的の回転
	float stepRotTime_;			//補完時間
	VECTOR dir_;				//方向

	//プレイヤーのメインとなるアクション(移動やジャンプなど)
	std::map<ACTION_TYPE, std::unique_ptr<ActionBase>>mainAction_;

	//サブアクション(カードセレクトなど同時並行となるもの)
	std::map<ACTION_TYPE, std::unique_ptr<ActionBase>>subAction_;

	//カード選択
	void CardMove(void);

	//入力方向に応じて方向を決める
	void MoveDirFromInput(void);

	//方向と移動量の更新
	void DirAndMovePowUpdate(void);

	//カードを右に動かせるか判定
	const bool IsCardLeftMoveable(void);

	//カードを左に動かせるか判定
	const bool IsCardRightMoveable(void);

	//敵との距離で1段階目の攻撃を決める
	void DesideAttackOne(void);

	//敵のアクションをランダムで決める
	void DesideEnemyCardAction(void);

	/// @brief 攻撃可能かどうかの判定
	/// @param  
	/// @return 
	const bool IsAttacable(void);
};