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
	for (auto effect : effects_)
	{
		for (int i = 0; i < effect.second.playNum; i++)
		{
			//再生が終わったら
			if (IsEffekseer3DEffectPlaying(effect.second.playData[i].playId) == -1)
			{
				//ループするなら
				if (effect.second.playData[i].isLoop)
				{
					//もう一回
 					RePlay(effect.first, i);
				}
				//ループしない
				else 
				{
					//消去
					Delete(effect.first, i);
				}
			}
		}
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
	effect.playNum = 0;

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
	effects_[_effType].playData.emplace(effects_[_effType].playNum, playData);

	//配列保存
	int ret = effects_[_effType].playNum;

	//カウンタ増加
	effects_[_effType].playNum++;


	//配列番号を返す
	return ret;
}


void EffectController::SetPos(const EFF_TYPE _effType, const int _arrayNum, const VECTOR _pos)
{
	//エフェクトの要素が存在するか または 再生中のエフェクトが存在するか または　その配列のエフェクトが存在するか
	if (!effects_.count(_effType) || effects_[_effType].playNum <= 0 || effects_[_effType].playNum < _arrayNum)
	{
		//その要素がなかった
		return;
	}

	//座標の再設定
	SetPosPlayingEffekseer3DEffect(effects_[_effType].playData[_arrayNum].playId, _pos.x, _pos.y, _pos.z);
	effects_[_effType].playData[_arrayNum].pos = _pos;
}

void EffectController::SetQuaRot(const EFF_TYPE _effType, const int _arrayNum, const Quaternion _quaRot)
{
	//エフェクトの要素が存在するか また 再生中のエフェクトが存在するか
	if (!effects_.count(_effType) || effects_[_effType].playNum <= 0 || effects_[_effType].playNum < _arrayNum)
	{
		//その要素がなかった
		return;
	}
	
	//回転の再設定
	SetRotationPlayingEffekseer3DEffect(effects_[_effType].playData[_arrayNum].playId, _quaRot.ToEuler().x, _quaRot.ToEuler().y, _quaRot.ToEuler().z);
	effects_[_effType].playData[_arrayNum].quaRot = _quaRot;
}

void EffectController::SetScale(const EFF_TYPE _effType, const int _arrayNum, const VECTOR _scl)
{
	//エフェクトの要素が存在するか また 再生中のエフェクトが存在するか
	if (!effects_.count(_effType) || effects_[_effType].playNum <= 0 || effects_[_effType].playNum < _arrayNum)
	{
		//その要素がなかった
		return;
	}
	
	//大きさの再設定
	SetScalePlayingEffekseer3DEffect(effects_[_effType].playData[_arrayNum].playId, _scl.x, _scl.y, _scl.z);
	effects_[_effType].playData[_arrayNum].scl = _scl;
}

void EffectController::SetSpeed(const EFF_TYPE _effType, const int _arrayNum, const float _speedMultiplier)
{
	
	//エフェクトの要素が存在するか また 再生中のエフェクトが存在するか
	if (!effects_.count(_effType) || effects_[_effType].playNum <= 0 || effects_[_effType].playNum < _arrayNum)
	{
		//その要素がなかった
		return;
	}
	
	//速度の再設定
	SetSpeedPlayingEffekseer3DEffect(effects_[_effType].playData[_arrayNum].playId, _speedMultiplier);
	effects_[_effType].playData[_arrayNum].speedMulti = _speedMultiplier;
}

void EffectController::Stop(const EFF_TYPE _effType, const int _arrayNum)
{
	//エフェクトの要素が存在するか また 再生中のエフェクトが存在するか
	if (!effects_.count(_effType) || effects_[_effType].playNum <= 0 || effects_[_effType].playNum < _arrayNum)
	{
		//その要素がなかった
		return;
	}
	
	//エフェクトストップ
	StopEffekseer3DEffect(effects_[_effType].playData[_arrayNum].playId);
}

void EffectController::Delete(const EFF_TYPE _effType, const int _arrayNum)
{
	//指定されたエフェクトを削除
	Stop(_effType, _arrayNum);
	effects_[_effType].playData.erase(_arrayNum);
	effects_[_effType].playNum--;
}

void EffectController::AllStop(void)
{
	//全停止
	for (const auto& effect : effects_)
	{
		for (int i = 0; i < effect.second.playNum; i++)
		{
			Stop(effect.first, i);
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

const bool EffectController::IsEnd(const EFF_TYPE _effType, const int _arrayNum)
{
	//エフェクトの要素が存在するか また 再生中のエフェクトが存在するか
	if (!effects_.count(_effType) || effects_[_effType].playNum <= 0 || effects_[_effType].playNum < _arrayNum)
	{
		//そもそもその要素がなかった
		return true;
	}

	//再生が終わっているか
	if (IsEffekseer3DEffectPlaying(effects_[_effType].playData[_arrayNum].playId) == -1)
	{
		//終わっていた
		return true;
	}

	//終わっていない
	return false;
}

const int EffectController::GetPlayNum(const EFF_TYPE _effType)
{
	//エフェクトの要素が存在するか また 再生中のエフェクトが存在するか
	if (!effects_.count(_effType))
	{
		//そもそもその要素がなかった
		return -1;
	}

	return effects_[_effType].playNum;
}

void EffectController::RePlay(const EFF_TYPE _effType, const int _arrayNum)
{
	//各要素
	VECTOR scl = effects_[_effType].playData[_arrayNum].scl;
	VECTOR rot = effects_[_effType].playData[_arrayNum].quaRot.ToEuler();
	VECTOR pos = effects_[_effType].playData[_arrayNum].pos;
	float speed = effects_[_effType].playData[_arrayNum].speedMulti;

	//削除
	Delete(_effType, _arrayNum);

	//プレイハンドルを設定
	effects_[_effType].playData[_arrayNum].playId = PlayEffekseer3DEffect(effects_[_effType].resId);

	//以降、再生するエフェクトの制御は必ずプレイハンドルIDを使用すること
	//エフェクトの大きさ設定(XYZ)
	SetScalePlayingEffekseer3DEffect(effects_[_effType].playData[_arrayNum].playId, scl.x, scl.y, scl.z);

	//エフェクトの角度を設定
	SetRotationPlayingEffekseer3DEffect(effects_[_effType].playData[_arrayNum].playId, rot.x, rot.y, rot.z);

	//エフェクトの位置を設定
	SetPosPlayingEffekseer3DEffect(effects_[_effType].playData[_arrayNum].playId, pos.x, pos.y, pos.z);

	//エフェクトの速度を設定
	SetSpeedPlayingEffekseer3DEffect(effects_[_effType].playData[_arrayNum].playId, speed);
}
