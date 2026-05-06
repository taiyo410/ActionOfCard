#pragma once
#include<vector>
#include "../Object/Character/UIData/CharacterUIData.h"
#include "../../Template/Singleton.h"

class CharacterBase;
class Player;
class Enemy;

class CharacterManager :
	public Singleton<CharacterManager>
{
	// シングルトン
	friend class Singleton<CharacterManager>;

public:

	static constexpr int CHARA_MAX = 2;

	/// @brief ロード
	/// @param  
	void Load(void);

	/// @brief 初期化
	/// @param  
	void Init(void);

	/// @brief 更新
	/// @param  
	void Update(void);

	/// @brief 描画
	/// @param  
	void Draw(void);

	/// @brief 2D描画
	/// @param  
	void Draw2D(void);

	/// @brief ゲームクリアシーン遷移条件(敵のHPによるもの)
	/// @param  
	/// @return 
	const bool IsSceneChageClearCondition(void)const;

	/// @brief ゲームオーバーシーン遷移
	/// @param  
	/// @return 
	const bool IsSceneChangeGameOverCondition(void)const;

	/// @brief 演出中の敵の咆哮状態取得
	/// @param  
	/// @return 
	const bool GetIsEnemyRoar(void)const;

	/// @brief クリア演出の終了
	/// @param  
	/// @return 
	const bool GetIsEndClearDirection(void)const;

	/// @brief ゲームオーバー演出の終了
	/// @param  
	/// @return true:終了　false:終了してない
	const bool GetIsEndOverDirection(void)const;

	/// @brief 通常状態へ移行
	/// @param  
	void ChangeCharacterNormalUpdate(void);

	/// @brief 演出状態へ
	/// @param  
	void ChangeCharacterDirectionUpdate(void);

	/// @brief クリア演出へ
	/// @param  
	void ChangeCharacterClearDirection(void);

	/// @brief ゲームオーバー演出へ
	/// @param  
	void ChangeCharacterOverDirection(void);

private:

	//プレイヤー番号
	static constexpr int PLAYER_NO = static_cast<int>(CHARACTER_TYPE::PLAYER);

	//敵番号
	static constexpr int ENEMY_NO = static_cast<int>(CHARACTER_TYPE::ENEMY);

	//シングルトンのためprivate
	CharacterManager(void);
	~CharacterManager(void)override;

	//コピー禁止
	CharacterManager(const CharacterManager& _copy) = delete;
	CharacterManager& operator=(const CharacterManager& _copy) = delete;

	//キャラクター配列
	std::shared_ptr<CharacterBase> characters_[static_cast<int>(CHARACTER_TYPE::MAX)];
};