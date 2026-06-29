#pragma once

#include<memory>
#include "Object/Character/Base/CharacterBase.h"
#include "Object/Card/CardBase.h"
#include "Common/Vector2.h"

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

#ifdef _DEBUG
	void DrawDebug(void);
#endif // _DEBUG

#pragma region メンバー定数
	//敵番号(デッキで判定する用)
	static constexpr int ENEMY_NUM = 1;

	//Enemyの文字列
	const std::string ENEMY_STR = "Enemy";
#pragma endregion

#pragma region 外部ファイル読み込み
	int chestFrameNum_ = 0;							//胸のフレーム番号
	float deathEffectStartAnimStep_ = 0.0f;			//死亡エフェクトの発生アニメステップ
	VECTOR deathEffectScale_ = {};					//死亡エフェクトサイズの倍率
	float roarStartAnimStep_ = 0.0f;				//咆哮開始アニメーションステップ
	float roarEndAnimStep_ = 0.0f;					//咆哮終了アニメーションステップ
	float roarCameraShakeLimit_ = 0.0f;				//咆哮時のカメラシェイク
	float deathSclDownDiff_=0.0f;							//死亡時のスケールダウン量
#pragma endregion

#pragma region メンバー変数
	//咆哮アニメーション
	AnimationController::ANIMATION_VARIABLE roarAnim_;
	bool isRoar_;		//咆哮状態かどうか
	int deathEffPlayId_;	//死亡エフェクトプレイID
#pragma endregion

#pragma region メンバー関数
	//コライダ作成
	void MakeColliderGeometry(void) override;

	//キャラクター別のアクションデータの呼び出し時のコールバック
	void LoadCharacterActionDataCallBack(const ACTION_LOAD_DATA& _animVar) override;

	//キャラクター別のモデル情報
	void LoadModelDataCharacter(const nlohmann::json& _data) override;

	//キャラクター別の処理
	void LoadCharacter(void) override;					//キャラクター単体でのロード
	void InitCharacter(void) override;					//キャラクター別の初期化
	void UpdateNormalCharacter(void) override;			//戦闘(ゲーム時)の更新
	void UpdateDirectionCharacter(void) override;		//ゲーム前のスタート時の演出	
	void UpdateClearDirectionCharacter(void) override;	//クリア演出		
	void UpdateOverDirectionCharacter(void) override;	//ゲームオーバー演出
	void DrawCharacter(void) override;					//描画

	//クリア演出に変更
	void ChangeUpdateClearDirection(void)override;
#pragma endregion
};