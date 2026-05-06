#include "../pch.h"
#include "../Utility/UtilityCommon.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Resource/FontManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Renderer/PixelMaterial.h"
#include "../Renderer/PixelRenderer.h"
#include "../Common/Easing.h"
#include "CardUIDraw.h"

CardUIDraw::CardUIDraw(int& _typeImg,Vector2F& _centerPos, float& _scl):
	typeImg_(_typeImg),
	centerPos_(_centerPos),
	scl_(_scl),
	selectEaseCnt_()
{
	//通常カードシェーダ
	normalCardPSMaterial_ = std::make_unique<PixelMaterial>(ResourceManager::SRC::CARD_NORMAL_PS);
}

CardUIDraw::~CardUIDraw(void)
{
}
void CardUIDraw::Load(void)
{
}

void CardUIDraw::Init(void)
{
	//画像サイズ取得
	GetGraphSizeF(typeImg_, &size_.x, &size_.y);
	halfSize_ = size_ / 2.0f;

	//左上の座標
	rightTopPos_ = centerPos_ - halfSize_ * scl_;

	//右下の座標
	leftDownPos_ = centerPos_ + halfSize_ * scl_;

	Vector2 rightTopPos = {static_cast<int>(rightTopPos_.x),static_cast<int>(rightTopPos_.y)};

	easing_ = std::make_unique<Easing>();

	trans_.pos = CARD_INIT_POS;
	trans_.quaRot = Quaternion();
	trans_.scl = { CARD_SCL,CARD_SCL,CARD_SCL };
	trans_.quaRotLocal =
		Quaternion::Euler({ 0.0f,0.0f,0.0f });

	//通常カードシェーダ
	normalCardPSMaterial_ = std::make_unique<PixelMaterial>(ResourceManager::SRC::CARD_NORMAL_PS);
	normalCardPSMaterial_->AddTextureBuf(typeImg_);
	normalCardPSMaterial_->AddConstBuf({ 0.0f,0.0f, 0.0f,1.0f });		//カードの色
	normalCardPSMaterial_->AddConstBuf({ 1.0f,0.0f, size_.x,size_.y });		//サイズ
	normalCardPSRenderer_ = std::make_unique<PixelRenderer>(*normalCardPSMaterial_);
	normalCardPSRenderer_->MakeSquareVertex(rightTopPos_, size_);

	//リロードカード
	reloadCardPSMaterial_= std::make_unique<PixelMaterial>(ResourceManager::SRC::CARD_RELOAD_PS);
	reloadCardPSMaterial_->AddTextureBuf(ResourceManager::GetInstance().Load(ResourceManager::SRC::RELOAD_GAUGE).handleId_);
	reloadCardPSMaterial_->AddConstBuf({ 0.0f,0.0f, 0.0f,1.0f });		//カードの色
	reloadCardPSMaterial_->AddConstBuf({ 1.0f,0.0f, 0.0f,1.0f });		//アウトラインの色
	reloadCardPSRenderer_ = std::make_unique<PixelRenderer>(*reloadCardPSMaterial_);
	reloadCardPSRenderer_->MakeSquareVertex(rightTopPos_, size_);

	//選択枠
	selectFramePSMaterial_ = std::make_unique<PixelMaterial>(ResourceManager::SRC::CARD_SELECT_PS);
	selectFramePSMaterial_->AddTextureBuf(typeImg_);
	selectFramePSMaterial_->AddConstBuf({ 0.0f,0.0f, 0.0f,0.0f });		//カードの色
	selectFramePSRenderer_ = std::make_unique<PixelRenderer>(*selectFramePSMaterial_);
	selectFramePSRenderer_->MakeSquareVertexFromCenter(centerPos_, size_);

}

void CardUIDraw::Update(void)
{
	selectFramePSMaterial_->SetConstBuf(NORMAL_CARD_CONST_BUF, { SceneManager::GetInstance().GetTotalTime(),1.0f, 1.0f,0.0f});
	SelectFrameEasing();
}

void CardUIDraw::Draw(void)
{
	normalCardPSMaterial_->SetConstBuf(NORMAL_CARD_CONST_BUN_NUM, { 0.0f,0.0f, 0.0f,1.0f });		//カードの色
	DrawCard();
}

void CardUIDraw::DrawSelectedFrame(void)
{
	//選択枠描画
	selectFramePSRenderer_->DrawFromCenter(centerPos_.x, centerPos_.y);
}

void CardUIDraw::DrawSelectCard(void)
{
	//白いモヤを描画する
	float totalTime = SceneManager::GetInstance().GetTotalTime();
	normalCardPSMaterial_->SetConstBuf(NORMAL_CARD_CONST_BUN_NUM, { SELECT_FOG_STRENGTH,totalTime, 0.0f,1.0f });		//カードの色

	//カード描画
	DrawCard();
}

void CardUIDraw::DrawCard(void)
{
	//画像サイズ取得
	GetGraphSizeF(typeImg_, &size_.x, &size_.y);

	//ハーフサイズ計算
	halfSize_ = size_ / 2.0f;

	//左上の座標
	Vector2F rightTopPos = centerPos_ - halfSize_ * scl_;

	//レンダラーにセット
	normalCardPSRenderer_->SetSize(size_ * scl_);
	normalCardPSRenderer_->Draw(rightTopPos.x, rightTopPos.y);
}

void CardUIDraw::SelectFrameEasing(void)
{
	//サイズ
	Vector2F size = easing_->EaseFunc(size_ - SELECT_CARD_FRAME_MOVE_AMOUNT,
		size_, selectEaseCnt_ / SELECT_CARD_FRAME_EASING_TIME,
		Easing::EASING_TYPE::QUAD_BACK);

	//イージングカウント更新
	selectEaseCnt_ += SceneManager::GetInstance().GetDeltaTime();
	selectEaseCnt_ > SELECT_CARD_FRAME_EASING_TIME ? selectEaseCnt_ = 0.0f : selectEaseCnt_;

	//シェーダーに値セット
	selectFramePSRenderer_->SetSize(size);
}

void CardUIDraw::DrawReloadGauge(const float& _reloadPer)
{
	//画像サイズ取得
	GetGraphSizeF(typeImg_, &size_.x, &size_.y);

	halfSize_ = size_ / 2.0f;

	//左上の座標
	Vector2F rightTopPos = centerPos_ - halfSize_ * scl_;

	reloadCardPSMaterial_->SetConstBuf(RELOAD_PER_CONST_BUF_SIZE, { 0.0f,0.0f,_reloadPer,0.0f });
	reloadCardPSRenderer_->SetSize(size_ * scl_);
	reloadCardPSRenderer_->Draw(rightTopPos.x, rightTopPos.y);
}
