#pragma once

#include "EnemyCardAttackBase.h"

class EnemyCardAttackStomp :
    public EnemyCardAttackBase
{

public:

	/// @brief コンストラクタ
	/// @param _actCntl アクションコントローラー
	/// @param _charaObj キャラクターオブジェクト
	/// @param _cardPresent カードプレゼンター
	EnemyCardAttackStomp(ActionController& _actCntl, CharacterBase& _charaObj, CardPresenter& _cardPresent);

	/// @brief デストラクタ
	/// @param  
	~EnemyCardAttackStomp(void)override;

	/// @brief ロード
	/// @param  
	void Load(void) override;

	/// @brief アクションで使う変数の初期化
	/// @param  
	void InitAttack(void) override;

	/// @brief 攻撃アクションの内容
	/// @param  
	void AttackUpdate(void) override;

	/// @brief 遷移時の開放
	/// @param  
	void AttackRelease(void) override;

	/// @brief アクションデータのロード
	/// @param _data 読み込むデータ
	void LoadAnimVar(const ACTION_LOAD_DATA& _data) override;

private:

	//スタンプ攻撃
	static constexpr float STOMP_ATK_SHAKE_CNT = 1.0f;              //カメラシェイク時間

#pragma region 外部ファイル読み込み
	float cameraShakeTime_;			//カメラシェイク時間
	int rockNum_;					//岩の数
	float cameraShakeLimit_;		//カメラシェイク制限値
#pragma endregion

#pragma region メンバー変数
	//敵のスタンプ攻撃時に発生する岩
	std::vector<std::shared_ptr<EnemyRock>>rock_;
	//岩生成フラグ
	bool isGenerateRock_;

#pragma endregion





};

