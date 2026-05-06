#pragma once
#include <unordered_map>
#include <string>
#include "../Resource/ResourceManager.h"
#include "../Resource/Resource.h"
#include "../Template/Singleton.h"

class ResourceManager;

class SoundManager :
	public Singleton<SoundManager>
{
	//シングルトン
	friend class Singleton<SoundManager>;

public:

	using SRC = ResourceManager::SRC;
	using TYPE = ResourceData::SOUND_TYPE;

	/// 再生種類
	enum class PLAYTYPE
	{
		NORMAL,	//ノーマル再生
		LOOP,	//ループ再生
		BACK	//バックグラウンド再生
	};

	//デフォルトの音量
	static constexpr int DEFAULT_VOLUME = 70;

	//音源種類最大数
	static constexpr int TYPE_MAX = static_cast<int>(TYPE::MAX);

	/// @brief デストラクタ
	/// @param  
	~SoundManager(void)override;

	/// @brief 初期化
	/// @param  
	void Init(void);

	/// @brief 音源の再生
	/// @param _src リソース種類
	/// @param _playType 再生種類
	void Play(const SRC _src, const PLAYTYPE _playType);
	
	/// @brief 音源の停止
	/// @param _src リソース種類
	void Stop(const SRC _src);

	/// @brief 再生中かを返す
	/// @param _src リソース種類
	/// @return trueの:再生中　false;場合再生していない
	bool IsPlay(const SRC _src) const;

	/// @brief 読み込んだ音量を設定する
	/// @param  
	/// @return 
	const void SetLoadedSoundsVolume(void);

	/// @brief リソース別の音量をセット
	/// @param _src 音量調整したいリソース
	/// @param _volumePercent 設定したい音量
	void SetSoundVolumeSRC(const SRC _src, const float _volumePercent);

	/// @brief 音量の設定
	/// @param _volumePercent 音量パーセント
	/// @param _type サウンド種類
	void SetSystemVolume(const float _volumePercent, const TYPE _type);

	/// @brief 音量を返す
	/// @param _type サウンド種類
	/// @return 指定したサウンド種類の音量を返す
	const float GetSoundTypeVolume(const int _type) const { return volume_[_type]; }

	/// @brief 再生中のサウンドをすべて止める
	/// @param  
	void AllStop(void);

private:	
	
	//音源が読み込まれていないときのエラーメッセージ
	const std::string NOT_LOAD_ERROR_MESSAGE = "音源が読み込まれてないです";

	//音量マックス値
	static constexpr float VOLUME_MAX = 255.0f;

	//静的インスタンス
	static SoundManager* instance_;
	
	//ボリューム
	float volume_[TYPE_MAX];

	//リソースマネージャー
	ResourceManager& resMng_;

	// ダミーリソース
	ResourceData dummy_;

	// コンストラクタ
	SoundManager(void);

	// デフォルトコンストラクタは使用不可
	SoundManager(const SoundManager&) = delete;

	// コピー代入演算子は使用不可
	SoundManager& operator=(const SoundManager&) = delete;

	//再生種類を取得
	int GetPlayType(const PLAYTYPE _playType);
};