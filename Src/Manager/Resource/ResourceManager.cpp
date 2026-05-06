#include <DxLib.h>
#include "../../Application.h"
#include "../Utility/UtilityCommon.h"
#include "Resource.h"
#include "ResourceManager.h"

ResourceManager::ResourceManager(void):
	dummy_(ResourceData::TYPE::NONE,L"")
{
	//素材の登録文字列とSRCの結び付け
	resStr_ = {
		//モデル
		{"STAGE",SRC::STAGE },
		{"STAGE_WALL",SRC::STAGE_WALL  },
		{"PLAYER" ,SRC::PLAYER},
		{"ENEMY_1",SRC::ENEMY},
		{"SKY_DOME",SRC::SKY_DOME },
		{"SPHERE_ROCK" ,SRC::SPHERE_ROCK},
		{"KEY_BLADE" ,SRC::KEY_BLADE},
		{"P_IDLE", SRC::P_IDLE},
		{"P_RUN" ,SRC::P_RUN},
		{"P_ATTACK_1_SHORT" ,SRC::P_ATTACK_1_SHORT },
		{"P_ATTACK_1_MIDDLE" ,SRC::P_ATTACK_1_MIDDLE},
		{"P_ATTACK_2",SRC::P_ATTACK_2 },
		{"P_ATTACK_3",SRC::P_ATTACK_3 },
		{"P_DODGE",SRC::P_DODGE },
		{"P_RELOAD",SRC::P_RELOAD },
		{"P_DEATH" ,SRC::P_DEATH},
		{"E_IDLE" ,SRC::E_IDLE},
		{"E_RUN" ,SRC::E_RUN},
		{"E_STOMP_ATK",SRC::E_STOMP_ATK},
		{"E_JUMP_ATK" ,SRC::E_JUMP_ATK},
		{"E_ROAR_ATK",SRC::E_ROAR_ATK },
		{"E_KNOCK_DOWN",SRC::E_KNOCK_DOWN },
		{"REACT" ,SRC::REACT},
		{"E_DEATH" ,SRC::E_DEATH},
		//g画像
		{"TITLE_BACK_IMG",SRC::TITLE_BACK_IMG },
		{"TITLE_LOGO" ,SRC::TITLE_LOGO},
		{"GAME_OVER_IMG" ,SRC::GAME_OVER_IMG},
		{"GAME_CLEAR_IMG" ,SRC::GAME_CLEAR_IMG},
		{"PLAYER_ATK_CARD_IMG" ,SRC::PLAYER_ATK_CARD_IMG},
		{"ENEMY_ATK_CARD_IMG" ,SRC::ENEMY_ATK_CARD_IMG},
		{"RELOAD_CARD_IMG" ,SRC::RELOAD_CARD_IMG},
		{"RELOAD_GAGE" ,SRC::RELOAD_GAUGE},
		{"P_HP_ARC_OUTLINE" ,SRC::P_HP_ARC_OUTLINE },
		{"P_HP_LINE_OUT_LINE" ,SRC::P_HP_LINE_OUT_LINE},
		{"P_CARD_NUM_GAUGE_MASK" ,SRC::P_CARD_NUM_GAUGE_MASK},
		{"P_CARD_NUM_GAUGE_FRAME" ,SRC::P_CARD_NUM_GAUGE_FRAME},
		{"P_CARD_NUM_GAUGE_BACK" ,SRC::P_CARD_NUM_GAUGE_BACK},
		{"E_HP_BAR_MASK" ,SRC::E_HP_BAR_MASK},
		{"E_HP_BAR_FRAME" ,SRC::E_HP_BAR_FRAME},
		{"E_HP_COVER" ,SRC::E_HP_COVER},
		{"SKIP_BUTTOM_MASK" ,SRC::SKIP_BUTTOM_MASK },
		{"CARD_REVOLVER_L_ARROW" ,SRC::CARD_REVOLVER_L_ARROW},
		{"INTENSIVE_LINE_1" ,SRC::INTENSIVE_LINE_1},
		{"INTENSIVE_LINE_2" ,SRC::INTENSIVE_LINE_2},
		//複数画像
		{"NUMBERS_IMGS" ,SRC::NUMBERS_IMGS},
		{"CONTROLLER_UI_IMGS" ,SRC::CONTROLLER_UI_IMGS},
		//エフェクト
		{"BLAST" ,SRC::BLAST},
		{"KEY_BLADE_HIT_EFF" ,SRC::KEY_BLADE_HIT_EFF},
		{"E_JUMP_CHARGE_EFF" ,SRC::E_JUMP_CHARGE_EFF},
		{"E_DEATH_EFF" ,SRC::E_DEATH_EFF},
		{"RELOAD_EFF" ,SRC::RELOAD_EFF},
		{"RELOAD_END_EFF" ,SRC::RELOAD_END_EFF},
		//BGM
		{"TITLE_BGM",SRC::TITLE_BGM},
		{"GAME_BGM",SRC::GAME_BGM},
		{"GAME_CLEAR_BGM",SRC::GAME_CLEAR_BGM},
		{"GAME_OVER_BGM",SRC::GAME_OVER_BGM},
		//SE
		{"PLAYER_FOOT_SE",SRC::PLAYER_FOOT_SE},
		{"ENEMY_FOOT_SE",SRC::ENEMY_FOOT_SE},
		{"ENEMY_STOMP_SE",SRC::ENEMY_STOMP_SE},
		{"ENEMY_CHARGE_SE",SRC::ENEMY_CHARGE_SE},
		{"ENEMY_JUMP_LAND_SE",SRC::ENEMY_JUMP_LAND_SE},
		{"ENEMY_HIT_SE",SRC::ENEMY_HIT_SE},
		{"PLAYER_ATTACK_SE",SRC::PLAYER_ATTACK_SE},
		{"PLAYER_DODGE_SE",SRC::PLAYER_DODGE_SE},
		{"PLAYER_HIT_SE",SRC::PLAYER_HIT_SE},
		{"CARD_PUT_SE",SRC::CARD_PUT_SE},
		{"CARD_MOVE_SE",SRC::CARD_MOVE_SE},
		{"CARD_BE_REFLECTED_SE",SRC::CARD_BE_REFLECTED_SE},
		{"CARD_BREAK_SE",SRC::CARD_BREAK_SE},
		{"CARD_RELOAD_SE",SRC::CARD_RELOAD_SE},
		{"CARD_RELOAD_FINISH_SE",SRC::CARD_RELOAD_FINISH_SE},
		{"MOVE_BTN_SE",SRC::MOVE_BTN_SE},
		{"DESIDE_BTN_SE",SRC::DESIDE_BTN_SE},
		{"GAME_START_SE",SRC::GAME_START_SE},
		//Json
		{"CHARA_DATA" ,SRC::CHARA_DATA},
		//ピクセルシェーダ
		{"STAGE_PS",SRC::STAGE_PS},
		{"SKYDOME_PS",SRC::SKYDOME_PS},
		{"HPBAR_PS",SRC::HPBAR_PS},
		{"ARCBAR_PS",SRC::ARCBAR_PS},
		{"CARD_NORMAL_PS",SRC::CARD_NORMAL_PS},
		{"CARD_RELOAD_PS",SRC::CARD_RELOAD_PS},
		{"CARD_SELECT_PS",SRC::CARD_SELECT_PS},
		//頂点シェーダ
		{"STAGE_VS",SRC::STAGE_VS}
	};

	//リソース種類と文字列の結び付け
	resTypeStr_ = {
		{"model",{ResourceData::TYPE::MODEL,Application::PATH_MODEL}},
		{"image",{ResourceData::TYPE::IMG,Application::PATH_IMAGE}},
		{"images",{ResourceData::TYPE::IMGS,Application::PATH_IMAGE}},
		{"effect",{ResourceData::TYPE::EFFEKSEER,Application::PATH_EFFECT}},
		{"sound",{ResourceData::TYPE::SOUND,Application::PATH_SOUND}},
		{"json",{ResourceData::TYPE::JSON,Application::PATH_JSON}},
		{"pixelShader",{ResourceData::TYPE::PIXEL_SHADER,Application::PATH_SHADER }},
		{"vertexShader",{ResourceData::TYPE::VERTEX_SHADER,Application::PATH_SHADER }},
	};

	loadDataFunc_ = {
		{ResourceData::TYPE::IMG,[this](const nlohmann::json _data) {LoadResourceCommon(_data); }},
		{ResourceData::TYPE::IMGS,[this](const nlohmann::json _data) {LoadResourceImages(_data); }},
		{ResourceData::TYPE::MODEL,[this](const nlohmann::json _data) {LoadResourceCommon(_data); }},
		{ResourceData::TYPE::SOUND,[this](const nlohmann::json _data) {LoadResourceSound(_data); }},
		{ResourceData::TYPE::FONT,[this](const nlohmann::json _data) {LoadResourceCommon(_data); }},
		{ResourceData::TYPE::EFFEKSEER,[this](const nlohmann::json _data) {LoadResourceCommon(_data); }},
		{ResourceData::TYPE::VERTEX_SHADER,[this](const nlohmann::json _data) {LoadResourceShader(_data); }},
		{ResourceData::TYPE::PIXEL_SHADER,[this](const nlohmann::json _data) {LoadResourceShader(_data); }},
		{ResourceData::TYPE::JSON,[this](const nlohmann::json _data) {LoadResourceCommon(_data); }},
	};
}

void ResourceManager::Init(void)
{
	//リソースをJsonファイルから読み込む
	nlohmann::json json;
	json = UtilityCommon::LoadJsonData(UtilityCommon::GetStringFromWString(Application::PATH_JSON+ L"ResourceData.json"));

	for (const auto& data : json["ResourceData"])
	{
		//タイプを読み込み、対応したロード関数を呼ぶ
		ResourceData::TYPE type = resTypeStr_[data["type"]].resType;
		loadDataFunc_[type](data);
	}

	//Jsonを読み込み終わったので、関連の情報は解放する
	resTypeStr_.clear();
	resStr_.clear();
}

void ResourceManager::SceneChangeRelease(void)
{
	for (auto& p : loadedMap_)
	{
		p.second.Release();
	}

	loadedMap_.clear();
}

void ResourceManager::Release(void)
{
	SceneChangeRelease();
	resourcesMap_.clear();
}

const ResourceData& ResourceManager::Load(SRC src)
{
	ResourceData& res = _Load(src);
	if (res.type_ == ResourceData::TYPE::NONE)
	{
		return dummy_;
	}
	return res;
}

int ResourceManager::LoadModelDuplicate(SRC src)
{
	ResourceData& res = _Load(src);
	if (res.type_ == ResourceData::TYPE::NONE)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res.handleId_);
	res.duplicateModelIds_.push_back(duId);

	return duId;
}

const ResourceData ResourceManager::GetResource(const SRC src) const
{
	const auto it = loadedMap_.find(src);
	if (it == loadedMap_.end())
	{
		return dummy_;
	}
	return it->second;
}

std::vector<const ResourceData*> ResourceManager::GetSoundResources(ResourceData::SOUND_TYPE _soundType) const
{
	std::vector<const ResourceData*>retArray;
	for (auto& load : loadedMap_)
	{
		if (load.second.type_ == ResourceData::TYPE::SOUND)
		{
			//サウンドの種類に指定がなければサウンドをすべて渡す
			if (_soundType == ResourceData::SOUND_TYPE::MAX)
			{
				retArray.push_back(&load.second);
			}
			//BGMかSEが指定されていれば、そのサウンドの種類のみを渡す
			else if (load.second.soundType_ == _soundType)
			{
				retArray.push_back(&load.second);
			}
		}
	}

	return retArray;
}

ResourceManager::~ResourceManager(void)
{
	loadedMap_.clear();
	resourcesMap_.clear();
}

ResourceData& ResourceManager::_Load(SRC src)
{

	// ロード済みチェック
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return lPair->second;
	}

	// リソース登録チェック
	const auto& rPair = resourcesMap_.find(src);
	if (rPair == resourcesMap_.end())
	{
		// 登録されていない
		return dummy_;
	}

	// ロード処理
	rPair->second->Load();

	// 念のためコピーコンストラクタ
	loadedMap_.emplace(src, *rPair->second);

	return *rPair->second;

}

const ResourceManager::RESOURCE_COMMON_PARAM ResourceManager::GetResourceParameter(const nlohmann::json& _data)
{
	RESOURCE_COMMON_PARAM resParam;
	resParam.type = resTypeStr_[_data["type"]].resType;
	resParam.src = resStr_[_data["name"]];
	resParam.path = resTypeStr_[_data["type"]].typePath + UtilityCommon::GetWStringFromString(_data["handle"]);

	//サウンドだった場合のパス
	if (resParam.type == ResourceData::TYPE::SOUND)
	{
		if (_data["soundtype"] == "BGM")
		{
			resParam.path = Application::PATH_SOUND_BGM + UtilityCommon::GetWStringFromString(_data["handle"]);
		}
		else if (_data["soundtype"] == "SE")
		{
			resParam.path = Application::PATH_SOUND_SE + UtilityCommon::GetWStringFromString(_data["handle"]);
		}
	}

	return resParam;
}

void ResourceManager::LoadResourceCommon(const nlohmann::json& _data)
{
	std::unique_ptr<ResourceData> res;

	//共通パラメータの取得
	RESOURCE_COMMON_PARAM parameter = GetResourceParameter(_data);

	//リソースオブジェクト生成
	res = std::make_unique<ResourceData>(parameter.type, parameter.path);

	//配列に挿入
	resourcesMap_.emplace(parameter.src, std::move(res));
}

void ResourceManager::LoadResourceImages(const nlohmann::json& _data)
{
	std::unique_ptr<ResourceData> res;

	//共通パラメータの取得
	RESOURCE_COMMON_PARAM parameter = GetResourceParameter(_data);

	//読み込み
	int numX = _data.value("numX",0);
	int numY = _data.value("numY", 0);
	int sizeX = _data.value("sizeX", 0);
	int sizeY = _data.value("sizeY", 0);

	//リソースオブジェクト生成
	res = std::make_unique<ResourceData>(parameter.type, parameter.path,
		numX, numY, sizeX, sizeY);

	//配列に挿入
	resourcesMap_.emplace(parameter.src, std::move(res));
}

void ResourceManager::LoadResourceSound(const nlohmann::json& _data)
{
	std::unique_ptr<ResourceData> res;

	//共通パラメータの取得
	RESOURCE_COMMON_PARAM parameter = GetResourceParameter(_data);

	//サウンドの種類によってパスを変える
	ResourceData::SOUND_TYPE soundType = ResourceData::SOUND_TYPE::MAX;

	//サウンド種類によって異なるパスを指定
	std::wstring soundTypePath = L"";

	float pitch = 0.0f;
	float timeStretch = 1.0f;
	float volume = 1.0f;
	float loopStartTime = 0.0f;
	float loopEndTime = 0.0f;

	//サウンドの情報を読み込む。存在しない場合はデフォルト値を入れる
	_data.contains("pitch") ? pitch = _data.value("pitch",0.0f) : pitch = 0.0f;
	_data.contains("timeStretch") ? timeStretch = _data.value("timeStretch", 0.0f) : timeStretch = 1.0f;
	_data.contains("volume") ? volume = _data.value("volume", 0.0f) : volume = 1.0f;
	_data.contains("loopStartTime") ? loopStartTime = _data.value("loopStartTime", 0.0f) : loopStartTime = 0.0f;
	_data.contains("loopEndTime") ? loopEndTime = _data.value("loopEndTime", 0.0f) : loopEndTime = 0.0f;

	//サウンドの情報を渡す
	res = std::make_unique<ResourceData>(parameter.type, parameter.path, soundType, pitch, timeStretch, volume, loopStartTime, loopEndTime);

	//配列に挿入
	resourcesMap_.emplace(parameter.src, std::move(res));
}

void ResourceManager::LoadResourceShader(const nlohmann::json& _data)
{
	std::unique_ptr<ResourceData> res;

	//共通パラメータの取得
	RESOURCE_COMMON_PARAM parameter = GetResourceParameter(_data);

	//定数バッファサイズ
	int constBufNum = 0;

	//定数バッファサイズを取得
	if (_data.contains("constBufNum")) { constBufNum = _data.value("constBufNum", 0); }

	//シェーダのリソース情報を渡す
	res = std::make_unique<ResourceData>(parameter.type, parameter.path, constBufNum);

	//配列に挿入
	resourcesMap_.emplace(parameter.src, std::move(res));
}
