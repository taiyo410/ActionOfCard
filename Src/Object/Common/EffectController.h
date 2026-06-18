#pragma once

#include<vector>
#include<map>
#include"Common/Quaternion.h"

class EffectController
{

public:

	//エフェクトの種類
	enum class EFF_TYPE
	{
		NONE,
		KEY_BLADE_HIT,	//キーブレードヒット
		DASH,			//ダッシュ
		JUMP,			//ジャンプ
		PUNCH_HIT,		//パンチヒット
		BLAST,			//爆発
		RELOAD,			//カードリロード
		RELOAD_END,		//カードリロード
		FIRE_BALL,		//ファイアボール
		THUNDER,		//雷
		E_JUMP_CHARGE,	//ジャンプ溜め
		E_DEATH,		//死んだエフェクト
		E_DEATH_BLAST,	//死んで爆発
	};

	/// @brief コンストラクタ
	/// @param  
	EffectController(void);

	/// @brief デストラクタ
	/// @param  
	~EffectController(void);

	/// @brief 更新関係
	/// @param  
	void Update(void);

	/// @brief エフェクトの追加
	/// @param _effHandle エフェクトハンドル
	/// @param _effType ハンドルを保存したいタイプ
	void Add(const int _effHandle, const EFF_TYPE _effType);

	/// @brief エフェクトの再生
	/// @param _effType エフェクトの種類
	/// @param _pos 座標情報
	/// @param _quaRot 回転情報
	/// @param _scl 大きさ情報
	/// @param _isLoop ループの有無
	/// @param _speedMultiplier 速度の倍率
	/// @return 再生したエフェクトの配列番号(このエフェクトの設定をいじる時に使用), -1:失敗
	const int Play(const EFF_TYPE _effType,
		const VECTOR _pos,
		const Quaternion _quaRot,
		const VECTOR _scl,
		const bool _isLoop = false,
		const float _speedMultiplier = NORMAL_MULTI);

	/// @brief 再生中エフェクトを止める
	/// @param _effType 止めたいエフェクトの種類
	/// @param _arrayNum 止めたいエフェクトのプレイID
	void Stop(const EFF_TYPE _effType, const int _playId);

	/// @brief エフェクトの削除
	/// @param _effType 削除するエフェクトの種類
	/// @param _arrayNum 削除するエフェクトのプレイID
	void Delete(const EFF_TYPE _effType, const int _playId);

	/// @brief全停止	/// @param  
	/// @brief 
	/// @param  
	void AllStop(void);

	/// @brief 全削除
	/// @param  
	void AllDelete(void);

	/// @brief 再生が終わっているか
	/// @param _effType 確認したいエフェクトの種類
	/// @param _playId 確認したいエフェクトのプレイID
	/// @return 
	const bool IsEnd(const EFF_TYPE _effType, const int _playId);
	
	///// @brief 指定したエフェクトがいくつ再生されているか
	///// @param _effType 確認したいエフェクト
	///// @return プレイされている数(-1:そもそもそのエフェクトが存在しない)
	//const int GetPlayNum(const EFF_TYPE _effType);

	/// @brief 座標の再設定
	/// @param _effType エフェクトの種類
	/// @param _playId 変更したいエフェクトのプレイID
	/// @param _pos 変更後の座標情報
	void SetPos(const EFF_TYPE _effType, const int _playId, const VECTOR _pos);
	
	/// @brief 回転の再設定
	/// @param _effType エフェクトの種類
	/// @param _arrayNum 変更したいエフェクトの配列番号
	/// @param _quaRot 変更後の回転情報
	void SetQuaRot(const EFF_TYPE _effType, const int _playId, const Quaternion _quaRot);
	
	/// @brief 大きさの再設定
	/// @param _effType エフェクトの種類
	/// @param _arrayNum 変更したいエフェクトの配列番号
	/// @param _scl 変更後の大きさ
	void SetScale(const EFF_TYPE _effType, const int _playId, const VECTOR _scl);

	/// @brief 速度の再設定
	/// @param _effType エフェクトの種類
	/// @param _arrayNum 変更したいエフェクトの配列番号
	/// @param _speedMultiplier 変更後の速度倍率
	void SetSpeed(const EFF_TYPE _effType, const int _playId, const float _speedMultiplier);

	/// @brief エフェクトのカラー設定
	/// @param _effType エフェクトの種類
	/// @param _playId 再生ID
	/// @param _color 指定したいエフェクトの色
	void SetColor(const EFF_TYPE _effType, const int _playId, const INT4 _color);

private:

	//プレイデータ
	struct PlayData
	{
		int playId = -1;						//再生データ
		bool isLoop = false;					//ループの有無
		VECTOR pos = {};						//座標
		Quaternion quaRot = {};					//回転
		VECTOR scl = {};						//大きさ
		float speedMulti;						//速度倍率
		FLOAT4 col = { 1.0f,1.0f,1.0f,1.0f };	//エフェクトの色
		bool isDelete = false;					//削除フラグ
	};

	//エフェクトデータ
	struct EffectData
	{
		int resId;							//リソースデータ
		//std::map<int, PlayData> playData;	//プレイ中のデータ
		std::list<PlayData> playData;			//プレイ中のデータ
		//int playNum;						//プレイ中のエフェクトの数
	};

	//通常倍率
	static constexpr float NORMAL_MULTI = 1.0f;

	//エフェクト
	std::map<EFF_TYPE, EffectData> effects_;

	/// @brief もう一度再生
	/// @param _effType もう一度再生させるエフェクトの種類
	/// @param _arrayNum もう一度再生させる配列
	void RePlay(const EFF_TYPE _effType, const int _playId);

	/// @brief プレイ中のエフェクトのデータを取得
	/// @param _effType エフェクトの種類
	/// @param _playId プレイ中のエフェクトのID
	/// @param _playData 取得したいプレイデータ
	const std::optional<std::reference_wrapper<PlayData>> FindPlayData(const EFF_TYPE _effType, const int _playId);

	/// @brief プレイ中のエフェクトのデータが存在するか
	/// @param _effType エフェクトの種類
	/// @param _playId プレイ中のエフェクトのID
	/// @return true:存在する、false:存在しない
	bool IsExistPlayData(const EFF_TYPE _effType, const int _playId);
};

