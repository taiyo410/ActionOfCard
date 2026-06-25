#pragma once

#include <memory>
#include "SceneBase.h"

class PauseScene;
class GameEventRevolutionScene;
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

#pragma region メンバー定数
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

	//革命時間の最大、最小時間
	static constexpr float REVOLUTION_TIME_MIN = 8.0f;
	static constexpr float REVOLUTION_TIME_MAX = 15.0f;

	//革命時間関連	
	static constexpr float FADE_TIME = 0.3f;	//フェード時間
	static constexpr float WAIT_TIME = 1.0f;	//フェードイン後の待機時間

	//シャドウマップスケール
	static constexpr int SHADOW_SCALE_X = 256;
	static constexpr int SHADOW_SCALE_Y = 256;

	//シャドウマップのエリア
	static constexpr VECTOR SHADOW_MAP_AREA_MIN = { -4000.0f, -1.0f, -4000.0f };
	static constexpr VECTOR SHADOW_MAP_AREA_MAX = { 4000.0f, -1.0f, 4000.0f };

#pragma endregion

#pragma region メンバー変数
	std::unique_ptr<SkyDome> skyDome_;								//スカイドーム
	std::unique_ptr<Stage> stage_;									//ステージ
	std::shared_ptr<PauseScene> pauseScene_;						//ポーズ画面
	std::shared_ptr<GameEventRevolutionScene> revolutionScene_;		//革命シーン	//レンダラー

	//更新フェーズ
	UPDATE_PHASE updatePhase_;											//更新
	std::map<UPDATE_PHASE, std::function<void(void)>> changeUpdate_;		//遷移

	int slowFrame_;										//スローカウンタ(フレーム)
	bool isSkippingDirection_;							//スキップ中
	float skipKeepCnt_;									//長押しカウンタ
	int shadowMapHandle_;								//シャドウマップハンドル
#pragma endregion

	//処理の変更
	void OnSceneEnter(void) override;

	//各オブジェクトの処理
	void ObjectLoad(void);		//ロード
	void ObjectInit(void);		//初期化
	void ObjectUpdate(void);	//更新
	void ObjectDraw(void);		//描画

	//演出更新のスキップ
	void CheckSkip(void);

	//演出スキップ
	void Skip(void);

	//シーンのフェーズを切り替える
	bool CheckGameStateTransition(void);

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

};