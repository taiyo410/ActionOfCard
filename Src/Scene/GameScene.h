#pragma once
#include <memory>
#include "SceneBase.h"

class PauseScene;
class PixelMaterial;
class PixelRenderer;
class CharacterBase;
class SkyDome;
class Player;
class Enemy;
class Stage;
class CardSystem;

class DrawUI3D;

class GameScene : public SceneBase
{

public:
	
	//更新フェーズ
	enum class UPDATE_PHASE   
	{
		NONE,
		FADE,
		NORMAL,
		DIRECTION,
		CLEAR_DIRECTION,
		OVER_DIRECTION,
		SLOW
	};

	/// @brief コンストラクタ
	/// @param  
	GameScene(void);

	/// @brief デストラクタ
	/// @param  
	~GameScene(void)override;

	/// @brief 読み込み
	/// @param  
	void Load(void) override;

	/// @brief 初期化
	/// @param  
	void Init(void) override;

	/// @brief 解放
	/// @param  
	void Release(void)override;
private:

	//BGM
	static constexpr float BGM_GAME_VOL = 0.5f;

	//集中線シェーダー関連
	//定数バッファの個数
	static constexpr int INTENSIVE_CBUFFER_NUM = 1;

	//集中線アニメーション速度
	static constexpr int INTENSIVE_LINE_ANIM_SPEED = 5;

	//スローの時、何フレームに1回更新するか
	static constexpr int FRAME_PER_UPDATE = 2;

	//スキップボタン画像の位置
	static constexpr Vector2F SKIP_BTN_POS = { 30.0f,30.0f };

	//スキップボタン長押し時間
	static constexpr float SKIP_BTN_TIME = 1.0f;

	//ボタン長押し文字のY座標オフセット
	static constexpr float SKIP_BTN_STR_OFFSET_Y = 16.0f;

	//ポストエフェクト用スクリーン
	int postEffectScreen_;

	//スローカウンタ(フレーム)
	int slowFrame_;

	//更新フェーズ
	UPDATE_PHASE updatePhase_;											//更新
	std::map<UPDATE_PHASE, std::function<void(void)>>changeUpdate_;		//遷移

	//スカイドーム
	std::unique_ptr<SkyDome> skyDome_;

	//ステージ
	std::unique_ptr<Stage>stage_;

	//ポーズ画面
	std::shared_ptr<PauseScene> pauseScene_;

	//プレイヤー
	std::unique_ptr<Player>player_;

	//敵
	std::unique_ptr<Enemy>enemy_;

	//スキップ中
	bool isSkippingDirection_;
	//長押しカウンタ
	float skipKeepCnt_;

	//演出更新のスキップ
	void CheckSkip(void);

	//更新関数
	void NoneUpdate(void);				//何もしない
	void FadeUpdate(void);				//フェード
	void NormalUpdate(void) override;	//通常
	void DirectionUpdate(void);			//演出時
	void ClearDirectionUpdate(void);	//クリア演出
	void OverDirectionUpdate(void);		//ゲームオーバー
	void SlowUpdate(void);				//スロー

	//描画関数
	void NormalDraw(void) override;		//通常
	void DirectionDraw(void);			//演出時の描画

	/// @brief 更新フェーズの変更
	/// @param _phase 
	void ChangeUpdatePhase(const UPDATE_PHASE _phase);

	//各状態の遷移処理
	void ChangeNone(void);
	void ChangeFade(void);
	void ChangeDirection(void);
	void ChangeClearDirection(void);
	void ChangeOverDirection(void);
	void ChangeNormal(void);
	void ChangeSlow(void);

	//処理の変更
	void OnSceneEnter(void) override;

	//演出スキップ
	void Skip(void);
};