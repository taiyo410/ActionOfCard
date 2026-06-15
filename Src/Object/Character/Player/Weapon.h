#pragma once

#include "Object/Character/Base/ItemBase.h"

class EffectController;
class CharacterBase;

class Weapon :
    public ItemBase
{

public:

    /// @brief コンストラクタ
    /// @param _chara 武器を使う親オブジェクト
    Weapon(CharacterBase& _chara);

	/// @brief デストラクタ
	/// @param  
	~Weapon(void) override;

	/// @brief ロード
	/// @param  
	void Load(void) override;

	/// @brief 初期化
	/// @param  
	void Init(void) override;

	/// @brief 更新
	/// @param  
	void Update(void) override;

	/// @brief 描画
	/// @param  
	void Draw(void) override;

	/// @brief 追従対象セット
	/// @param _targetTrans 
	void SetTargetAndFrameNo(Transform* _targetTrans,int _frameNo);

	/// @brief 武器の当たり判定作成
	/// @param  
	void MakeWeaponCollider(void);

	/// @brief 武器の当たり判定削除
	/// @param  
	void DeleteWeaponCollider(void);

	/// @brief 当たった時の処理
	/// @param _hitCol 
	void OnHit(const std::weak_ptr<Collider> _hitCol)override;

private:

	//ローカル角度(デグリー)
	static constexpr VECTOR LOCAL_ANGLE_DEG = { 0.0f,190.0f,-45.0f };

	//武器の大きさ
	static constexpr float WEAPON_SCL = 0.2f;

	//エフェクト再生位置のフレーム番号
	static constexpr int EFFECT_PLAY_FRAME_NO = 1;

	//エフェクトの大きさ
	static constexpr float EFFECT_PLAY_SCL = 20.0f;

	//武器の最大頂点座標
	static constexpr VECTOR MAX_VERTEX_POS = { 91.0f,533.0f,19.7f };

	//武器の最小頂点座標
	static constexpr VECTOR MIN_VERTEX_POS = { 91.0f,-117.0f,19.7f };

	//カプセルコライダーの半径
	static constexpr float CAPSULE_COL_RADIUS = MAX_VERTEX_POS.x* WEAPON_SCL;

	//カプセルコライダーの高さ
	static constexpr float CAPSULE_COL_HEIGHT = (MAX_VERTEX_POS.y - MIN_VERTEX_POS.y) * WEAPON_SCL;


#pragma region 外部ファイル読み込み
	float modelScale_;			//武器のモデルの大きさ
	int hitEffPlayFrameNo_;		//ヒットエフェクト再生するフレーム番号
	float hitEffScl_;			//ヒットエフェクトの大きさ
	VECTOR localAngleDeg_;		//武器のローカル角度
	VECTOR maxVertexPos_;		//武器の最大頂点座標
	VECTOR minVertexPos_;		//武器の最小頂点座標
#pragma endregion

#pragma region メンバー変数
	Transform* targetTrans_;	//武器の追従対象
	CharacterBase& character_;	//武器を使う親キャラクター
	int effectPlayId_;			//エフェクトのプレイID
	int followFrameNo_;			//追従するフレームの番号
	VECTOR localPos_;			//武器の相対位置
	VECTOR localRot_;			//武器の相対回転
#pragma endregion


};