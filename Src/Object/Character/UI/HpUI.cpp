#include "../pch.h"
#include "../Utility/UtilityCommon.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Application.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Common/Easing.h"
#include "../Renderer/PixelRenderer.h"
#include "../Renderer/PixelMaterial.h"
#include "HpUI.h"

HpUI::HpUI(CHARACTER_TYPE _charaType):
	charaType_(_charaType),
	hpData_(),
	waitCnt_(WAIT_TIME),
	resMng_(ResourceManager::GetInstance()),
	barCoverPos_(),
	barPos_(),
	initBarPos_(barPos_),
	initCoverPos_(barCoverPos_),
	hpDis_(),
	barCoverHandle_(UtilityCommon::INITIAL_HANDLE),
	barFrame_(),
	hpMask_(UtilityCommon::INITIAL_HANDLE),
	arcOutLineImg_(UtilityCommon::INITIAL_HANDLE),
	lineOutLineImg_(UtilityCommon::INITIAL_HANDLE),
	lerpCnt_(),
	shakeCnt_()

{
	material_ = std::make_unique<PixelMaterial>(ResourceManager::SRC::HPBAR_PS);
	renderer_ = std::make_unique<PixelRenderer>(*material_);
}

HpUI::~HpUI(void)
{
}

void HpUI::Load(void)
{
	//リソースのロード
	hpMask_ = resMng_.Load(ResourceManager::SRC::E_HP_BAR_MASK).handleId_;
	barCoverHandle_ = resMng_.Load(ResourceManager::SRC::E_HP_COVER).handleId_;
	barFrame_ = resMng_.Load(ResourceManager::SRC::E_HP_BAR_FRAME).handleId_;

	//Jsonからデータを読み込む
	LoadJsonHpUI();

	barPos_ = initBarPos_;
	barCoverPos_ = initCoverPos_;
}

void HpUI::Init(void)
{
	easing_ = std::make_unique<Easing>();
	material_->AddTextureBuf(hpMask_);
	material_->AddConstBuf(barColorL_);	//バーの色(明るい青)
	material_->AddConstBuf(barColorR_);	//バーの色(紫)
	material_->AddConstBuf({ hpData_.hpPer,0.0f,0.0f,0.0f });
	renderer_->MakeSquareVertex(barCoverPos_, BAR_SIZE);
}

void HpUI::Update(void)
{
	//HPの減らし幅ゲージの補完
	if (hpDis_ > 0.0f)
	{
		float lerpStart = hpData_.hpPer + hpDis_;

		//HPが減って一定時間がたったら
		if (waitCnt_ < 0.0f)
		{
			hpData_.preHpPer = easing_->EaseFunc(lerpStart, hpData_.hpPer, (LERP_TIME - lerpCnt_) / LERP_TIME, Easing::EASING_TYPE::LERP);
			hpDis_= hpData_.preHpPer - hpData_.hpPer;
			lerpCnt_ -= SceneManager::GetInstance().GetDeltaTime();
		}
		waitCnt_ -= SceneManager::GetInstance().GetDeltaTime();
	}
	else
	{
		lerpCnt_ = LERP_TIME;
		waitCnt_ = WAIT_TIME;
	}

	//HPバーのシェイク
	if (shakeCnt_ > 0.0f)
	{
		barCoverPos_.y = easing_->EaseFunc(initCoverPos_.y, initCoverPos_.y+COVER_SHAKE_POW_Y,
			(SHAKE_CNT - shakeCnt_) / SHAKE_CNT, Easing::EASING_TYPE::ELASTIC_BACK);

		barPos_.y = easing_->EaseFunc(initBarPos_.y, initBarPos_.y+BAR_SHAKE_POW_Y,
			(SHAKE_CNT - shakeCnt_) / SHAKE_CNT, Easing::EASING_TYPE::ELASTIC_BACK);

		shakeCnt_ -= SceneManager::GetInstance().GetDeltaTime();
	}

	//シェーダの更新
	renderer_->SetPos(barPos_);
	material_->SetConstBuf(PRE_HP_CONST_BUF, { hpData_.hpPer,hpData_.preHpPer,0.0f,0.0f });
}

void HpUI::SetShakeTime(void)
{
	shakeCnt_ = SHAKE_CNT;
}

void HpUI::RefreshHp(const HP_DATA& _hpData)
{
	hpData_.preHpPer = _hpData.preHpPer;
	hpData_.hpPer = _hpData.hpPer;
	hpDis_= hpData_.preHpPer - hpData_.hpPer;
}

void HpUI::LoadJsonHpUI(void)
{
	//Jsonからデータを取得
	nlohmann::json j = resMng_.Load(ResourceManager::SRC::CHARA_DATA).jsonData;
	CHARACTER_TYPE chara = CHARACTER_TYPE::MAX;
	std::string charaStr = "";

	//キャラによって読み込むデータを変える
	charaType_ == CHARACTER_TYPE::PLAYER ? charaStr = "PlayerHPBar" : charaStr = "EnemyHPBar";

	//jsonのデータをそれぞれ格納
	for (auto data : j[charaStr])
	{
		//バー座標
		if (data.contains("barPos"))
		{
			auto& pos = data.at("barPos");
			initBarPos_.x = pos.value("x", 0.0f);
			initBarPos_.y = pos.value("y", 0.0f);
		}

		//バーの左の座標
		if (data.contains("barColor_L")) 
		{
			auto& col = data.at("barColor_L");
			barColorL_.x = col.value("red", 0.0f);
			barColorL_.y = col.value("green", 0.0f);
			barColorL_.z = col.value("blue", 0.0f);
			barColorL_.w = col.value("alpha", 0.0f);
		}

		//バーの右の座標
		if (data.contains("barColor_R"))
		{
			auto& col = data.at("barColor_R");
			barColorR_.x = col.value("red", 0.0f);
			barColorR_.y = col.value("green", 0.0f);
			barColorR_.z = col.value("blue", 0.0f);
			barColorR_.w = col.value("alpha", 0.0f);
		}

		//カバーの座標
		if (data.contains("coverPos"))
		{
			auto& pos = data.at("coverPos");
			initCoverPos_.x = pos.value("x",0.0f);
			initCoverPos_.y = pos.value("y", 0.0f);
		}
	}
}

void HpUI::Draw(void)
{
	DrawExtendGraphF(barCoverPos_.x, barCoverPos_.y, barCoverPos_.x + BAR_COVER_SIZE.x, barCoverPos_.y + BAR_COVER_SIZE.y, barFrame_, true);
	renderer_->Draw(barPos_.x, barPos_.y);
	DrawExtendGraphF(barCoverPos_.x, barCoverPos_.y, barCoverPos_.x + BAR_COVER_SIZE.x, barCoverPos_.y + BAR_COVER_SIZE.y, barCoverHandle_, true);
}
