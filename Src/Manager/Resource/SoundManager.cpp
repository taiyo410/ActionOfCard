#include <DxLib.h>
#include <cassert>
#include "../Resource/ResourceManager.h"
#include "../../Application.h"
#include "SoundManager.h"
SoundManager::SoundManager(void):
	resMng_(ResourceManager::GetInstance()),
    dummy_()
{  
	// 音量の初期化
	for (int i = 0; i < TYPE_MAX; ++i)
	{
		volume_[i] = DEFAULT_VOLUME;
	}
}

SoundManager::~SoundManager(void)
{
}

void SoundManager::Init(void)
{
}

void SoundManager::Play(const SRC _src, const PLAYTYPE _playType)
{
	//音源が読み込まれていない場合はエラー
	int handleId = resMng_.GetResource(_src).handleId_;
	assert(handleId == -1 && NOT_LOAD_ERROR_MESSAGE);

    //音源が再生済みか調べる
	if (CheckSoundMem(handleId) == 1 &&
        _playType != PLAYTYPE::BACK)
	{
		Stop(_src);  // 再生済みなら停止
	}

    //音源の再生
    int i=PlaySoundMem(handleId, GetPlayType(_playType));
}

void SoundManager::Stop(const SRC _src)
{
    //音源の停止
    StopSoundMem(resMng_.GetResource(_src).handleId_);
}

bool SoundManager::IsPlay(const SRC _src) const
{
    const auto res = resMng_.GetResource(_src);

    if (res.handleId_ == -1)
    {
        return false; // 見つからない場合は未再生とする
    }
    return CheckSoundMem(res.handleId_) == 1;
}

const void SoundManager::SetLoadedSoundsVolume(void)
{
    for (int i = 0; i < TYPE_MAX; i++) 
    {
        SetSystemVolume(volume_[i], static_cast<TYPE>(i)); 
    }
}

void SoundManager::SetSoundVolumeSRC(const SRC _src, const float _volumePercent)
{
    //リソースの取得
	const ResourceData res = resMng_.GetResource(_src);
    if (res.type_ == ResourceData::TYPE::NONE)
    {
        return; // 見つからない場合は処理しない
    }

	int volume = static_cast<int>(VOLUME_MAX * _volumePercent);
    //音量設定
	ChangeVolumeSoundMem(volume, res.handleId_);
}

void SoundManager::SetSystemVolume(const float _volumePercent, const TYPE _type)
{    
    //型変換
	const int type = static_cast<int>(_type);
    //音量設定
    volume_[type] = _volumePercent;
   
	const auto sounds = resMng_.GetSoundResources(_type);
    //音量調整
	for (const auto& pair : sounds)
	{
        int volume = static_cast<int>(VOLUME_MAX * volume_[type]);
        ChangeVolumeSoundMem(volume, pair->handleId_);
	}
}

int SoundManager::GetPlayType(const PLAYTYPE _playType)
{
    switch (_playType)
    {
    case PLAYTYPE::NORMAL:
        return DX_PLAYTYPE_NORMAL;
        break;

    case PLAYTYPE::LOOP:
        return DX_PLAYTYPE_LOOP;
        break;

    case PLAYTYPE::BACK:
        return DX_PLAYTYPE_BACK;
        break;

    default:
        return DX_PLAYTYPE_NORMAL;
        break;
    }
}

void SoundManager::AllStop(void)
{
    std::vector sounds = resMng_.GetSoundResources();
    for (const auto& sound : sounds)
    {
        //再生されていない、もしくはエラー発生したら飛ばす
        if (CheckSoundMem(sound->handleId_) <= 0)continue;
        StopSoundMem(sound->handleId_);
    }
}
