#include<DxLib.h>
#include<EffekseerForDXLib.h>
#include<Effekseer.h>
#include "../../Manager/Generic/SceneManager.h"
#include "EffectController.h"

EffectController::EffectController(void)
{
}

EffectController::~EffectController(void)
{
	//全停止
	AllDelete();
}

void EffectController::Update(void)
{
	for (auto& effect : effects_)
	{
		for (auto& playData : effect.second.playData)
		{
			//再生が終わったら
			if (IsEffekseer3DEffectPlaying(playData.playId) == -1)
			{
				//ループするなら
				if (playData.isLoop)
				{
					//もう一回
 					RePlay(effect.first, playData.playId);
				}
				//ループしない
				else 
				{
					//消去
					//Delete(effect.first, playData.playId);
					playData.isDelete = true;
				}
			}
		}
	}

	//消去フラグが立っているものを消去
	for (auto& effect : effects_)
	{
		auto it=effect.second.playData.remove_if([](const PlayData& playData) { return playData.isDelete; });
	}
}

void EffectController::Add(const int _effHandle, const EFF_TYPE _effType)
{
	//エフェクトの要素が存在するか
	if (effects_.count(_effType))
	{
		//既に存在したので何もしない
		return;
	}

	//エフェクト
	EffectData effect;

	//リソースID
	effect.resId = _effHandle;

	//個数初期化
	//effect.playNum = 0;

	//追加
	effects_.emplace(_effType, effect);
}

const int EffectController::Play(const EFF_TYPE _effType, const VECTOR _pos, const Quaternion _quaRot, const VECTOR _scl, const bool _isLoop, const float _speedMultiplier)
{
	//エフェクトの要素が存在するか
	if (!effects_.count(_effType))
	{
		//その要素がなかったので失敗
		return -1;
	}

	//プレイハンドルを設定
	PlayData playData;
	playData.playId = PlayEffekseer3DEffect(effects_[_effType].resId);

	//以降、再生するエフェクトの制御は必ずプレイハンドルIDを使用すること
	//エフェクトの大きさ設定(XYZ)
	SetScalePlayingEffekseer3DEffect(playData.playId, _scl.x, _scl.y, _scl.z);
	playData.scl = _scl;

	//エフェクトの角度を設定
	SetRotationPlayingEffekseer3DEffect(playData.playId, _quaRot.ToEuler().x, _quaRot.ToEuler().y, _quaRot.ToEuler().z);
	playData.quaRot = _quaRot;

	//エフェクトの位置を設定
	SetPosPlayingEffekseer3DEffect(playData.playId, _pos.x, _pos.y, _pos.z);
	playData.pos = _pos;

	//エフェクトの速度を設定
	SetSpeedPlayingEffekseer3DEffect(playData.playId, _speedMultiplier);
	playData.speedMulti = _speedMultiplier;

	//ループの有無
	playData.isLoop = _isLoop;

	//プレイエフェクトの保存
	effects_[_effType].playData.push_back(playData);

	////配列保存
	int ret = playData.playId;

	////カウンタ増加
	//effects_[_effType].playNum++;

	//プレイID返す
	return ret;
}


void EffectController::SetPos(const EFF_TYPE _effType, const int _playId, const VECTOR _pos)
{
	if (!IsExistPlayData(_effType, _playId))
	{
		//その要素がなかった
		return;
	}

	//座標の再設定
	PlayData& it = FindPlayData(_effType, _playId).value().get();
	SetPosPlayingEffekseer3DEffect(it.playId, _pos.x, _pos.y, _pos.z);
	it.pos = _pos;
}

void EffectController::SetQuaRot(const EFF_TYPE _effType, const int _playId, const Quaternion _quaRot)
{
	if (!IsExistPlayData(_effType, _playId))
	{
		//その要素がなかった
		return;
	}

	//playDataの参照を取得
	PlayData& it = FindPlayData(_effType, _playId).value().get();
	//回転の再設定
	SetRotationPlayingEffekseer3DEffect(it.playId, _quaRot.ToEuler().x, _quaRot.ToEuler().y, _quaRot.ToEuler().z);
	it.quaRot = _quaRot;
}

void EffectController::SetScale(const EFF_TYPE _effType, const int _playId, const VECTOR _scl)
{
	if (!IsExistPlayData(_effType, _playId))
	{
		//その要素がなかった
		return;
	}

	//playDataの参照を取得
	PlayData& it = FindPlayData(_effType, _playId).value().get();
	//大きさの再設定
	SetScalePlayingEffekseer3DEffect(it.playId, _scl.x, _scl.y, _scl.z);
	it.scl = _scl;
}

void EffectController::SetSpeed(const EFF_TYPE _effType, const int _playId, const float _speedMultiplier)
{
	if (!IsExistPlayData(_effType, _playId))
	{
		//その要素がなかった
		return;
	}

	//playDataの参照を取得
	PlayData& it = FindPlayData(_effType, _playId).value().get();
	//速度の再設定
	SetSpeedPlayingEffekseer3DEffect(it.playId, _speedMultiplier);
	it.speedMulti = _speedMultiplier;
}

void EffectController::Stop(const EFF_TYPE _effType, const int _playId)
{
	if (!IsExistPlayData(_effType, _playId))
	{
		//その要素がなかった
		return;
	}
	
	//playDataの参照を取得
	PlayData& it = FindPlayData(_effType, _playId).value().get();
	//エフェクトストップ
	StopEffekseer3DEffect(it.playId);
}

void EffectController::Delete(const EFF_TYPE _effType, const int _playId)
{
	//エフェクトの要素が存在するか また 再生中のエフェクトが存在するか また その配列のエフェクトが存在するか
	if (!IsExistPlayData(_effType, _playId))
	{
		//その要素がなかった
		return;
	}

	//指定されたエフェクトを削除
	Stop(_effType, _playId);

	auto it=std::find_if(effects_[_effType].playData.begin(), effects_[_effType].playData.end(),
		[_playId](const PlayData& data)
		{
			return data.playId == _playId;
		});

	effects_[_effType].playData.erase(it);
}

void EffectController::AllStop(void)
{
	//全停止
	for (const auto& effect : effects_)
	{
		for(const auto& playData : effect.second.playData)
		{
			Stop(effect.first, playData.playId);
		}
	}
}

void EffectController::AllDelete(void)
{
	//全停止
	AllStop();
	
	//エフェクトのクリア
	effects_.clear();
}

const bool EffectController::IsEnd(const EFF_TYPE _effType, const int _playId)
{
	//再生が終わっているか
	PlayData& it = FindPlayData(_effType, _playId).value().get();
	if (IsEffekseer3DEffectPlaying(it.playId) == -1)
	{
		//終わっていた
		return true;
	}

	//終わっていない
	return false;
}

//const int EffectController::GetPlayNum(const EFF_TYPE _effType)
//{
//	//エフェクトの要素が存在するか また 再生中のエフェクトが存在するか
//	if (!effects_.count(_effType))
//	{
//		//そもそもその要素がなかった
//		return -1;
//	}
//
//	return effects_[_effType].playNum;
//}

void EffectController::RePlay(const EFF_TYPE _effType, const int _playId)
{
	//再生中のエフェクト参照の取得
	PlayData& it = FindPlayData(_effType, _playId).value().get();

	//各要素
	VECTOR scl = it.scl;
	VECTOR rot = it.quaRot.ToEuler();
	VECTOR pos = it.pos;
	float speed = it.speedMulti;

	//削除
	//Delete(_effType, _playId);

	//プレイハンドルを設定
	it.playId = PlayEffekseer3DEffect(effects_[_effType].resId);

	//以降、再生するエフェクトの制御は必ずプレイハンドルIDを使用すること
	//エフェクトの大きさ設定(XYZ)
	SetScalePlayingEffekseer3DEffect(it.playId, scl.x, scl.y, scl.z);
	//エフェクトの角度を設定
	SetRotationPlayingEffekseer3DEffect(it.playId, rot.x, rot.y, rot.z);

	//エフェクトの位置を設定
	SetPosPlayingEffekseer3DEffect(it.playId, pos.x, pos.y, pos.z);

	//エフェクトの速度を設定
	SetSpeedPlayingEffekseer3DEffect(it.playId, speed);
}

const std::optional<std::reference_wrapper<EffectController::PlayData>>  EffectController::FindPlayData(const EFF_TYPE _effType, const int _playId)
{
	//エフェクトの要素が存在するか
	if(!effects_.count(_effType))
	{
		//存在がなければ末尾を返す
		return std::nullopt;
	}

	auto& playDataList = effects_[_effType].playData;

	//エフェクト配列からプレイIDが一致するものを探す
	auto it = std::find_if(playDataList.begin(), playDataList.end(),
		[_playId](const PlayData& data) 
		{
			return data.playId == _playId;
		});
	if (it == playDataList.end()) return std::nullopt;
	return *it;
}

bool EffectController::IsExistPlayData(const EFF_TYPE _effType, const int _playId)
{
	//エフェクトの要素が存在するか または 再生中のエフェクトが存在するか または　その配列のエフェクトが存在するか
	auto it = FindPlayData(_effType, _playId);
	if (!effects_.count(_effType) || !it.has_value())
	{
		//その要素がなかった
		return false;
	}
	return true;
}
