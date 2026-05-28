#include "../pch.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/Camera.h"
#include "../../Base/CharacterBase.h"
#include "../../../Common/AnimationController.h"
#include "../ActionController.h"
#include "../Object/Common/EffectController.h"
#include "../../Base/LogicBase.h"
#include "PlayerCardReload.h"

PlayerCardReload::PlayerCardReload(ActionController& _actCntl, CharacterBase& _charaObj, CardPresenter& _cardPresenter):
	CardActionBase(_actCntl, _charaObj,_cardPresenter)
{
}

PlayerCardReload::~PlayerCardReload(void)
{
}

void PlayerCardReload::Load(void)
{
}

void PlayerCardReload::Init(void)
{
	//if (!cardFuncs_.empty())
	//{
	//	cardFuncs_.pop();
	//}

	//現在使っているカードを捨てる
	cardPresent_.FinishCard();

	//UIの状態をリロード待機状態へ
	cardPresent_.ChangeUIState(CardUIBase::CARD_SELECT::RELOAD_WAIT);

	//カードリロードのアニメーション再生(溜めるようなモーション)
	anim_.PlayBlend(static_cast<int>(CharacterBase::ANIM_TYPE::CARD_RELOAD), animVar_);
	atkAnim_ = static_cast<int>(CharacterBase::ANIM_TYPE::CARD_RELOAD);

	//リロード音再生
	soundMng_.Play(ResourceManager::SRC::CARD_RELOAD_SE, SoundManager::PLAYTYPE::LOOP);

	//リロードエフェクトの再生
	const Transform& trans = character_.GetTransform();
	effect_->Play(EffectController::EFF_TYPE::RELOAD, trans.pos, trans.quaRot, { RELOD_EFF_SCL,RELOD_EFF_SCL,RELOD_EFF_SCL }, true);
}

void PlayerCardReload::Update(void)
{
	if (actionCntl_.GetInput().GetIsAct().isCardPushKeep)
	{
		//リロードのため時間をカウント
		pushReloadCnt_ += scnMng_.GetDeltaTime();

		//カードUI側にセット
		SetUIReloadCnt();

		//アニメーションループ
		constexpr float LOOP_SPD = 10.0f;
		anim_.SetMidLoop(static_cast<int>(CharacterBase::ANIM_TYPE::CARD_RELOAD), RELOAD_LOOP_START, RELOAD_LOOP_END, LOOP_SPD);
	}
	else
	{
		//アクションをポップし、アイドル状態へ
		cardFuncs_.pop();
		actionCntl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
	}

	//リロードの溜め終了時、カードをリロードする
	if (pushReloadCnt_ >= RELOAD_TIME)
	{
		cardPresent_.DeckReload();
		cardFuncs_.pop();

		//カードUIのリロードカウントの初期化
		pushReloadCnt_ = 0.0f;
		const float EFF_SCL = 100.0f;

		//UIのリロードカウントを初期化
		SetUIReloadCnt();

		//エフェクトの消去
		effect_->Stop(EffectController::EFF_TYPE::RELOAD, 0);
		effect_->Delete(EffectController::EFF_TYPE::RELOAD, 0);

		//リロード終了エフェクトを再生
		const Transform& trans = character_.GetTransform();
		effect_->Play(EffectController::EFF_TYPE::RELOAD_END, trans.pos, trans.quaRot, { EFF_SCL ,EFF_SCL ,EFF_SCL });

		//カードリロード音停止、完了音再生
		soundMng_.Stop(ResourceManager::SRC::CARD_RELOAD_SE);
		soundMng_.Play(ResourceManager::SRC::CARD_RELOAD_FINISH_SE, SoundManager::PLAYTYPE::BACK);

		//アイドル状態へ
		actionCntl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
	}
}

void PlayerCardReload::LoadAnimVar(const ACTION_LOAD_DATA& _data)
{
	if (_data.name != "Reload")return;

	animVar_ = _data.animVariable;
}

void PlayerCardReload::SetUIReloadCnt(void)
{
	float per = pushReloadCnt_ / RELOAD_TIME;
	cardPresent_.SetUIReloadCount(per);
}
