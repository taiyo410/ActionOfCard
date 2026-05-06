#pragma once
#include <memory>
#include <chrono>
#include <list>
#include "../../Template/Singleton.h"
#include "../../Common/Fader.h"

class SceneBase;
class Fader;
class Camera;

class SceneManager : 
	public Singleton<SceneManager>
{
	//シングルトン
	friend class Singleton<SceneManager>; 

public:

	// シーン管理用
	enum class SCENE_ID
	{
		NONE,
		TITLE,
		GAME,
		GAME_CLEAR,
		GAME_OVER,
	};
	
	/// @brief 初期化
	/// @param  
	void Init(void);
	
	/// @brief 3D関連の初期化
	/// @param  
	void Init3D(void);

	/// @brief 更新
	/// @param  
	void Update(void);

	/// @brief 描画
	/// @param  
	void Draw(void);

	/// @brief  リソースの破棄
	/// @param  
	void Release(void);
	
	/// @brief 先頭の（Updateが呼ばれる）シーンを切り替える
	/// @param scene 切り替え先のシーン
	void CreateScene(std::shared_ptr<SceneBase> scene);
	
	/// @brief シーンをプッシュする。スタックの数が増える
	/// 一番上のシーンのUpdateしか呼ばれません。
	/// @param scene 
	void PushScene(std::shared_ptr<SceneBase> scene);
	
	/// @brief スタックの頭のシーンを削除する。
	/// ただし、スタック上にシーンが一つしかない場合は、削除しない。
	/// @param  
	void PopScene(void);
	
	/// @brief シーン遷移
	/// @param nextId 変更先のシーン
	void ChangeScene(SCENE_ID nextId);
	
	/// @brief フェードを始める(だんだん明るくなる)
	/// @param  
	void StartFadeIn(void);

	/// @brief フェードアウトを始める(だんだん暗くなる)
	void StartFadeOut(void);
	
	/// @brief シーンチェンジのセット
	/// @param _isSceneChange 
	inline void SetIsSceneChange(const bool _isSceneChange) {isSceneChanging_ = _isSceneChange;}

	/// @brief 現在のシーンIDを返す
	/// @param  
	/// @return 現在のシーンID
	inline SCENE_ID GetSceneID(void) const { return sceneId_; }
	
	/// @brief デルタタイムを返す
	/// @param  
	/// @return デルタタイム
	inline float GetDeltaTime(void) const { return deltaTime_; }
	
	/// @brief 経過時間を返す
	/// @param  
	/// @return 経過時間
	inline float GetTotalTime(void) const { return totalTime_; }
	
	/// @brief カメラを返す
	/// @param  
	/// @return カメラ
	std::weak_ptr<Camera> GetCamera(void) const { return camera_; }

	/// @brief メインスクリーンのハンドルを返す
	/// @param  
	/// @return メインスクリーンのハンドル
	inline int GetMainScreen(void) const { return mainScreen_; }

	/// @brief フェード終了フラグ取得
	/// @param  
	/// @return フェード終了フラグ
	inline bool GetIsEndFade(void)const { return isEndFade_; }

	/// @brief フェードの取得
	/// @param  
	/// @return 
	const Fader& GetFader(void);

	/// @brief  フェード
	/// @param  
	void Fade(void);

private:

	//ライトの方向
	static constexpr VECTOR LIGHT_DIR = { 0.0f, -1.0f, 1.0f };

	//フォグの色
	static constexpr int FOG_COLOR_R = 5;
	static constexpr int FOG_COLOR_G = 5;
	static constexpr int FOG_COLOR_B = 5;

	//フォグの開始距離
	static constexpr float FOG_START = 10000.0f;

	//フォグの終了距離
	static constexpr float FOG_END = 20000.0f;

	//デルタタイム
	static constexpr float DELTA_TIME = 1.0f / 60.0f;

	SCENE_ID sceneId_;
	SCENE_ID waitSceneId_;

	//各種シーン
	std::list<std::shared_ptr<SceneBase>> scenes_;

	//フェード
	std::unique_ptr<Fader> fader_;

	//カメラ
	std::shared_ptr<Camera> camera_;

	//シーン遷移中判定
	bool isSceneChanging_;

	//デルタタイム
	std::chrono::system_clock::time_point preTime_;
	float deltaTime_;

	//経過時間
	float totalTime_;

	//メインスクリーン
	int mainScreen_;

	//フェードが終了したか
	bool isEndFade_;

	// コンストラクタ(シングルトンのためprivate)
	SceneManager(void);

	// デストラクタ
	~SceneManager(void) = default;

	// デルタタイムをリセットする
	void ResetDeltaTime(void);

	// シーン遷移
	void DoChangeScene(SCENE_ID sceneId);

	//シーン遷移用フェード
	void SceneChangeFade(void);
};