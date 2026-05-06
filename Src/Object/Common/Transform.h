#pragma once
#include <memory>
#include <DxLib.h>
#include "../../Common/Quaternion.h"
#include "../Common/Collider.h"

class Transform
{

public:

	/// @brief コンストラクタ
	/// @param  
	Transform(void);

	/// @brief モデルIDを設定するコンストラクタ
	/// @param model 
	Transform(int model);

	/// @brief デストラクタ
	/// @param  
	~Transform(void);

	//モデルのハンドルID
	int modelId;

	//大きさ
	VECTOR scl;

	//クォータニオン回転
	VECTOR rot;

	//位置
	VECTOR pos;			//基本位置
	VECTOR localPos;	//相対位置
	VECTOR overAllPos;	//総合位置

	//マトリックス
	MATRIX matScl;		//大きさ(行列)
	MATRIX matRot;		//回転(行列)
	MATRIX matPos;		//位置(行列)

	//回転
	Quaternion quaRot;

	//ローカル回転
	Quaternion quaRotLocal;

	//コライダ
	std::shared_ptr<Collider> collider;

	/// @brief モデル制御の基本情報更新
	/// @param  
	void Update(void);

	/// @brief モデルセット
	/// @param modelHId 
	void SetModel(int modelHId);
	
	/// @brief 
	/// @param  前方方向を取得
	/// @return 
	VECTOR GetForward(void) const;

	/// @brief 後方方向を取得
	/// @param  
	/// @return 
	VECTOR GetBack(void) const;

	/// @brief 右方向を取得
	/// @param  
	/// @return 
	VECTOR GetRight(void) const;

	/// @brief 左方向を取得
	/// @param  
	/// @return 
	VECTOR GetLeft(void) const;

	/// @brief 上方向を取得
	/// @param  
	/// @return 
	VECTOR GetUp(void) const;

	/// @brief 下方向を取得
	/// @param  
	/// @return 
	VECTOR GetDown(void) const;

	/// @brief 対象方向を取得
	/// @param vec 
	/// @return 
	VECTOR GetDir(const VECTOR& vec) const;

};
