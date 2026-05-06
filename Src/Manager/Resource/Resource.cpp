#include <DxLib.h>
#include <wingdi.h>
#include <EffekseerForDXLib.h>
#include <cassert>
#include "../Utility/UtilityCommon.h"
#include "Resource.h"

ResourceData::ResourceData(void):
	type_(TYPE::NONE),
	path_(L""),
	numX_(UtilityCommon::INITIAL_HANDLE),
	numY_(UtilityCommon::INITIAL_HANDLE),
	sizeX_(UtilityCommon::INITIAL_HANDLE),
	sizeY_(UtilityCommon::INITIAL_HANDLE),
	handleId_(UtilityCommon::INITIAL_HANDLE),
	handleIds_(nullptr),
	soundType_(SOUND_TYPE::MAX),
	pitch_(0.0f),
	timeStretch_(0.0f),
	volume_(0.0f),
	loopStartTime_(0),
	loopEndTime_(0)
{

}

ResourceData::ResourceData(TYPE type, const std::wstring& path):
	type_(type),
	path_(path),
	numX_(UtilityCommon::INITIAL_HANDLE),
	numY_(UtilityCommon::INITIAL_HANDLE),
	sizeX_(UtilityCommon::INITIAL_HANDLE),
	sizeY_(UtilityCommon::INITIAL_HANDLE),
	handleId_(UtilityCommon::INITIAL_HANDLE),
	handleIds_(nullptr),
	soundType_(SOUND_TYPE::MAX),
	pitch_(0.0f),
	timeStretch_(0.0f),
	volume_(0.0f),
	loopStartTime_(0),
	loopEndTime_(0)
{
	AddFunc();
}

ResourceData::ResourceData(TYPE type, const std::wstring& path, int numX, int numY, int sizeX, int sizeY):
	type_(type),
	path_(path),
	numX_(numX),
	numY_(numY),
	sizeX_(sizeX),
	sizeY_(sizeY),
	handleId_(UtilityCommon::INITIAL_HANDLE),
	handleIds_(nullptr),
	soundType_(SOUND_TYPE::MAX),
	pitch_(0.0f),
	timeStretch_(0.0f),
	volume_(0.0f),
	loopStartTime_(0),
	loopEndTime_(0)
{
	AddFunc();
}

ResourceData::ResourceData(TYPE type, const std::wstring& path
	, SOUND_TYPE soundType
	, const float pitch
	, const float timeStretch
	, const float volume
	, const int loopStartTime, const int loopEndTime):
	type_(type),
	path_(path),
	soundType_(soundType),
	pitch_(pitch),
	timeStretch_(timeStretch),
	volume_(volume),
	loopStartTime_(loopStartTime),
	loopEndTime_(loopEndTime),
	handleId_(-1),
	handleIds_(nullptr)
{
	AddFunc();

	//サウンドの状態設定関数の表に、状態に応じた関数を追加する
	if(pitch_!=0.0f)
	{
		setCreateFunc_[SET_SOUND_STATUS::PITCH] = [this]() { SetCreateSoundPitchRate(pitch_); };
		setReturnStatusFunc_[SET_SOUND_STATUS::PITCH] = [this]() { SetCreateSoundPitchRate(0.0f);  };
	}
	if(timeStretch_!=1.0f)
	{
		setCreateFunc_[SET_SOUND_STATUS::TIME_STRETCH] = [this]() {SetCreateSoundTimeStretchRate(timeStretch_); }; 
		setReturnStatusFunc_[SET_SOUND_STATUS::TIME_STRETCH] = [this]() { SetCreateSoundTimeStretchRate(1.0f); };
	}
	if(loopEndTime_!=0|| loopStartTime_<loopEndTime_)
	{ 
		setCreateFunc_[SET_SOUND_STATUS::LOOP_START] = [this]() { SetCreateSoundLoopAreaTimePos(loopStartTime_, loopEndTime_); };
		setReturnStatusFunc_[SET_SOUND_STATUS::LOOP_START] = [this]() { SetCreateSoundLoopAreaTimePos(0, 0); };
	}
}

ResourceData::ResourceData(TYPE type, const std::wstring& path, int constBufNum):
	type_(type),
	path_(path),
	constBufNum(constBufNum),
	numX_(UtilityCommon::INITIAL_HANDLE),
	numY_(UtilityCommon::INITIAL_HANDLE),
	sizeX_(UtilityCommon::INITIAL_HANDLE),
	sizeY_(UtilityCommon::INITIAL_HANDLE),
	handleId_(UtilityCommon::INITIAL_HANDLE),
	handleIds_(nullptr),
	soundType_(SOUND_TYPE::MAX),
	pitch_(0.0f),
	timeStretch_(0.0f),
	volume_(0.0f),
	loopStartTime_(0),
	loopEndTime_(0)
{
	AddFunc();
}

ResourceData::~ResourceData(void)
{
	duplicateModelIds_.clear();
	loadFunc_.clear();
	releaseFunc_.clear();
}

void ResourceData::Load(void)
{
	loadFunc_[type_]();

	//読み込みできたか確認
	assert(handleId_ != -1); // 読み込みに失敗してたら即終了
}

void ResourceData::Release(void)
{
	releaseFunc_[type_]();
}

void ResourceData::CopyHandle(int* imgs)
{
	if (handleIds_ == nullptr)
	{
		return;
	}

	int num = numX_ * numY_;
	for (int i = 0; i < num; i++)
	{
		imgs[i] = handleIds_[i];
	}
}

void ResourceData::AddFunc(void)
{
	loadFunc_ = {
		{ TYPE::IMG, [this]() { LoadImg(); } },
		{ TYPE::IMGS, [this]() { LoadImgs(); } },
		{ TYPE::MODEL, [this]() { LoadModel(); } },
		{ TYPE::SOUND, [this]() { LoadSound(); } },
		{ TYPE::FONT, [this]() { LoadFont(); } },
		{ TYPE::EFFEKSEER, [this]() { LoadEffekseer(); } },
		{ TYPE::VERTEX_SHADER, [this]() { LoadVS(); } },
		{ TYPE::PIXEL_SHADER, [this]() { LoadPS(); } },
		{ TYPE::JSON, [this]() { LoadJson(); } },
	};

	releaseFunc_ = {
		{ TYPE::IMG, [this]() { ReleaseImg(); } },
		{ TYPE::IMGS, [this]() { ReleaseImgs(); } },
		{ TYPE::MODEL, [this]() { ReleaseModel(); } },
		{ TYPE::SOUND, [this]() { ReleaseSound(); } },
		{ TYPE::FONT, [this]() { ReleaseFont(); } },
		{ TYPE::EFFEKSEER, [this]() { ReleaseEffekseer(); } },
		{ TYPE::VERTEX_SHADER, [this]() { ReleaseVS(); } },
		{ TYPE::PIXEL_SHADER, [this]() { ReleasePS(); } },
		{ TYPE::JSON, [this]() { ReleaseJson(); } }
	};
}

void ResourceData::LoadImg(void)
{
	handleId_ = LoadGraph(path_.c_str());
}

void ResourceData::LoadImgs(void)
{
	handleIds_ = new int[numX_ * numY_];
	LoadDivGraph(
		path_.c_str(),
		numX_ * numY_,
		numX_, numY_,
		sizeX_, sizeY_,
		&handleIds_[0]);
}

void ResourceData::LoadModel(void)
{
	handleId_ = MV1LoadModel(path_.c_str());
}

void ResourceData::LoadSound(void)
{
	constexpr float VOLUME_MAX = 255.0f;  //最大音量
	//サウンドの状態設定関数の表から、状態に応じた関数を呼び出す
	for (const auto& func : setCreateFunc_)
	{
		func.second();
	}
	handleId_ = LoadSoundMem(path_.c_str());

	for (const auto& func : setReturnStatusFunc_)
	{
		func.second();
	}

	//読み込んだサウンドの音量を、設定されている音量にする
	if(volume_!=1.0f)
	{
		ChangeVolumeSoundMem(static_cast<int>(VOLUME_MAX * volume_), handleId_);
	}
}

void ResourceData::LoadFont(void)
{
	handleId_ = AddFontResourceEx(path_.c_str(), FR_PRIVATE, NULL);
}

void ResourceData::LoadEffekseer(void)
{
	handleId_ = LoadEffekseerEffect(path_.c_str());
}

void ResourceData::LoadVS(void)
{
	handleId_ = LoadVertexShader(path_.c_str());
}

void ResourceData::LoadPS(void)
{
	handleId_ = LoadPixelShader(path_.c_str());
}

void ResourceData::LoadJson(void)
{
	using json = nlohmann::json;
	std::ifstream ifs(UtilityCommon::GetStringFromWString(path_));
	if (!ifs.is_open())
	{
		std::cerr << "ファイルが開けません" << std::endl;
	}
	ifs >> jsonData;
}

void ResourceData::ReleaseImg(void)
{
	DeleteGraph(handleId_);
}

void ResourceData::ReleaseImgs(void)
{
	int num = numX_ * numY_;
	for (int i = 0; i < num; i++)
	{
		DeleteGraph(handleIds_[i]);
	}
	delete[] handleIds_;
}

void ResourceData::ReleaseModel(void)
{
	MV1DeleteModel(handleId_);
	auto ids = duplicateModelIds_;
	for (auto id : ids)
	{
		MV1DeleteModel(id);
	}
}

void ResourceData::ReleaseSound(void)
{
	DeleteSoundMem(handleId_);
}

void ResourceData::ReleaseFont(void)
{
	RemoveFontResourceEx(path_.c_str(), FR_PRIVATE, NULL);
}

void ResourceData::ReleaseEffekseer(void)
{
	DeleteEffekseerEffect(handleId_);
}

void ResourceData::ReleaseVS(void)
{
	DeleteShader(handleId_);
}

void ResourceData::ReleasePS(void)
{
	DeleteShader(handleId_);
}

void ResourceData::ReleaseJson(void)
{

}
