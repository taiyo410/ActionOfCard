#include <DxLib.h>
#include "Application.h"
#include "Utility/UtilityCommon.h"
#include "Resource.h"
#include "ResourceManager.h"

namespace {
	using TYPE = ResourceData::TYPE;
}
ResourceManager::ResourceManager(void):
	dummy_(TYPE::NONE,L"")
{
	//素材の登録文字列とSRCの結び付け
	resStr_ = {
		//モデル
		{"STAGE",SRC::STAGE },
		{"STAGE_WALL",SRC::STAGE_WALL},
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
		{"P_FIRE_MAGIC",SRC::P_FIRE_MAGIC },
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
		{"E_REVOLUTION" ,SRC::E_REVOLUTION},
		//画像
		{"TITLE_BACK_IMG",SRC::TITLE_BACK_IMG },
		{"TITLE_LOGO" ,SRC::TITLE_LOGO},
		{"GAME_OVER_IMG" ,SRC::GAME_OVER_IMG},
		{"GAME_CLEAR_IMG" ,SRC::GAME_CLEAR_IMG},
		{"PLAYER_ATK_CARD_IMG" ,SRC::PLAYER_ATK_CARD_IMG},
		{"PLAYER_FIRE_CARD_IMG" ,SRC::PLAYER_FIRE_CARD_IMG},
		{"PLAYER_THUNDER_CARD_IMG" ,SRC::PLAYER_THUNDER_CARD_IMG},
		{"ENEMY_ATK_CARD_IMG" ,SRC::ENEMY_ATK_CARD_IMG},
		{"RELOAD_CARD_IMG" ,SRC::RELOAD_CARD_IMG},
		{"RELOAD_GAGE" ,SRC::RELOAD_GAUGE},
		{"P_CARD_NUM_GAUGE" ,SRC::P_CARD_NUM_GAUGE},
		{"P_CARD_NUM_GAUGE_BACK" ,SRC::P_CARD_NUM_GAUGE_BACK},
		{"E_HP_BAR" ,SRC::E_HP_BAR},
		{"SKIP_BUTTOM_MASK" ,SRC::SKIP_BUTTOM_MASK },
		{"CARD_REVOLVER_L_ARROW" ,SRC::CARD_REVOLVER_L_ARROW},
		{"INTENSIVE_LINE_1" ,SRC::INTENSIVE_LINE_1},
		{"INTENSIVE_LINE_2" ,SRC::INTENSIVE_LINE_2},
		{"REVERSE_FADE_MASK" ,SRC::REVERSE_FADE_MASK },
		{"WIN_ARROW_IMG" ,SRC::WIN_ARROW_IMG },
		{"LOSE_ARROW_IMG" ,SRC::LOSE_ARROW_IMG},
		{"HIGHER_IMG" ,SRC::HIGHER_IMG},
		{"LOWER_IMG" ,SRC::LOWER_IMG},
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
		{"FIRE_BALL_EFF" ,SRC::FIRE_BALL_EFF},
		{"THUNDER_EFF" ,SRC::THUNDER_EFF},
		{"REVOLUTION_EFF" ,SRC::REVOLUTION_EFF},
		//BGM
		{"TITLE_BGM",SRC::TITLE_BGM},
		{"GAME_BGM",SRC::GAME_BGM},
		{"GAME_CLEAR_BGM",SRC::GAME_CLEAR_BGM},
		{"GAME_OVER_BGM",SRC::GAME_OVER_BGM},
		//SE
		{"P_FOOT_SE",SRC::PLAYER_FOOT_SE},
		{"E_FOOT_SE",SRC::ENEMY_FOOT_SE},
		{"E_STOMP_SE",SRC::ENEMY_STOMP_SE},
		{"E_CHARGE_SE",SRC::ENEMY_CHARGE_SE},
		{"E_JUMP_LAND_SE",SRC::ENEMY_JUMP_LAND_SE},
		{"E_HIT_SE",SRC::ENEMY_HIT_SE},
		{"E_ROAR_SE",SRC::ENEMY_ROAR_SE},
		{"P_ATTACK_SE",SRC::PLAYER_ATTACK_SE},
		{"P_DODGE_SE",SRC::PLAYER_DODGE_SE},
		{"P_HIT_SE",SRC::PLAYER_HIT_SE},
		{"CARD_PUT_SE",SRC::CARD_PUT_SE},
		{"CARD_MOVE_SE",SRC::CARD_MOVE_SE},
		{"CARD_BE_REFLECTED_SE",SRC::CARD_BE_REFLECTED_SE},
		{"CARD_BREAK_SE",SRC::CARD_BREAK_SE},
		{"CARD_RELOAD_SE",SRC::CARD_RELOAD_SE},
		{"CARD_RELOAD_FINISH_SE",SRC::CARD_RELOAD_FINISH_SE},
		{"MOVE_BTN_SE",SRC::MOVE_BTN_SE},
		{"DESIDE_BTN_SE",SRC::DESIDE_BTN_SE},
		{"GAME_START_SE",SRC::GAME_START_SE},
		{"FIRE_SPAWN_SE",SRC::FIRE_SPAWN_SE},
		{"FIRE_HIT_SE",SRC::FIRE_HIT_SE},
		{"THUNDER_SE",SRC::THUNDER_SE},
		{"REVOLUTION_SE",SRC::ENEMY_REVOLUTION_SE},
		//Json
		{"CHARA_DATA" ,SRC::CHARA_DATA},
		{"DECK_DATA" ,SRC::DECK_DATA},
		{"ACTION_DATA" ,SRC::ACTION_DATA},
		{"COLLISION_DATA" ,SRC::COLLISION_DATA},
		{"UI_DATA" ,SRC::UI_DATA},
		//ピクセルシェーダ
		{"STAGE_PS",SRC::STAGE_PS},
		{"SKYDOME_PS",SRC::SKYDOME_PS},
		{"HPBAR_PS",SRC::HPBAR_PS},
		{"ARCBAR_PS",SRC::ARCBAR_PS},
		{"CARD_NORMAL_PS",SRC::CARD_NORMAL_PS},
		{"CARD_RELOAD_PS",SRC::CARD_RELOAD_PS},
		{"CARD_SELECT_PS",SRC::CARD_SELECT_PS},
		{"REVOLUTION_POSTEFF_PS",SRC::REVOLUTION_POSTEFF_PS},
		//頂点シェーダ
		{"STAGE_VS",SRC::STAGE_VS}
	};

	//リソース種類と文字列の結び付け
	resTypeStr_ = {
		{"model",{TYPE::MODEL,Application::PATH_MODEL}},
		{"image",{TYPE::IMG,Application::PATH_IMAGE}},
		{"images",{TYPE::IMGS,Application::PATH_IMAGE}},
		{"effect",{TYPE::EFFEKSEER,Application::PATH_EFFECT}},
		{"sound",{TYPE::SOUND,Application::PATH_SOUND}},
		{"json",{TYPE::JSON,Application::PATH_JSON}},
		{"pixelShader",{TYPE::PIXEL_SHADER,Application::PATH_SHADER }},
		{"vertexShader",{TYPE::VERTEX_SHADER,Application::PATH_SHADER }},
	};

	loadDataFunc_ = {
		{TYPE::IMG,[this](const RES_INFO _info,const nlohmann::json _data) {LoadResourceCommon(_info,_data); }},
		{TYPE::IMGS,[this](const RES_INFO _info,const nlohmann::json _data) {LoadResourceImages(_info,_data); }},
		{TYPE::MODEL,[this](const RES_INFO _info,const nlohmann::json _data) {LoadResourceCommon(_info,_data); }},
		{TYPE::SOUND,[this](const RES_INFO _info,const nlohmann::json _data) {LoadResourceSound(_info,_data); }},
		{TYPE::FONT,[this](const RES_INFO _info,const nlohmann::json _data) {LoadResourceCommon(_info,_data); }},
		{TYPE::EFFEKSEER,[this](const RES_INFO _info,const nlohmann::json _data) {LoadResourceCommon(_info,_data); }},
		{TYPE::VERTEX_SHADER,[this](const RES_INFO _info,const nlohmann::json _data) {LoadResourceShader(_info,_data); }},
		{TYPE::PIXEL_SHADER,[this](const RES_INFO _info,const nlohmann::json _data) {LoadResourceShader(_info,_data); }},
		{TYPE::JSON,[this](const RES_INFO _info,const nlohmann::json _data) {LoadResourceCommon(_info,_data); }},
	};
}

void ResourceManager::Init(void)
{
	//リソースをJsonファイルから読み込む
	nlohmann::json json;
	json = UtilityCommon::LoadJsonData(UtilityCommon::GetStringFromWString(Application::PATH_JSON + L"ResourceData.json"));

	for (const auto& [key, value] : json["ResourceData"].items())
	{
		for (const auto& str : resTypeStr_)
		{
			//一致していなければ飛ばす
			if (key != str.first)continue;

			for (const auto& resData : value)
			{
				//タイプを読み込み、対応したロード関数を呼ぶ
				TYPE type = str.second.resType;
				loadDataFunc_[type](str.second,resData);
			}
		}
	}
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
	if (res.type_ == TYPE::NONE)
	{
		return dummy_;
	}
	return res;
}

int ResourceManager::LoadModelDuplicate(SRC src)
{
	ResourceData& res = _Load(src);
	if (res.type_ == TYPE::NONE)
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
		if (load.second.type_ == TYPE::SOUND)
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

const ResourceManager::SRC ResourceManager::GetSrcFromString(const std::string& _str) const
{
	auto it = resStr_.find(_str);

	if (it != resStr_.end())
	{
		return it->second;
	}

	//見つからなかった場合はNONEを返す
	return SRC::NONE;
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

	return loadedMap_.at(src);
}

const ResourceManager::RESOURCE_COMMON_PARAM ResourceManager::GetResourceParameter(const RES_INFO _info, const nlohmann::json& _data )
{
	RESOURCE_COMMON_PARAM resParam;
	resParam.type = _info.resType;
	resParam.src = resStr_[_data["name"]];
	resParam.path = _info.typePath + UtilityCommon::GetWStringFromString(_data["handle"]);

	//サウンドだった場合のパス
	if (resParam.type == TYPE::SOUND)
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

void ResourceManager::LoadResourceCommon(const RES_INFO _info, const nlohmann::json& _data )
{
	std::unique_ptr<ResourceData> res;

	//共通パラメータの取得
	RESOURCE_COMMON_PARAM parameter = GetResourceParameter(_info, _data);

	//リソースオブジェクト生成
	res = std::make_unique<ResourceData>(parameter.type, parameter.path);

	//配列に挿入
	resourcesMap_.emplace(parameter.src, std::move(res));
}

void ResourceManager::LoadResourceImages(const RES_INFO _info, const nlohmann::json& _data )
{
	std::unique_ptr<ResourceData> res;

	//共通パラメータの取得
	RESOURCE_COMMON_PARAM parameter = GetResourceParameter(_info, _data);

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

void ResourceManager::LoadResourceSound(const RES_INFO _info, const nlohmann::json& _data )
{
	std::unique_ptr<ResourceData> res;

	//共通パラメータの取得
	RESOURCE_COMMON_PARAM parameter = GetResourceParameter(_info, _data);

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
	pitch = _data.value("pitch", 0.0f);
	timeStretch = _data.value("timeStretch", 1.0f);
	volume = _data.value("volume", 1.0f);
	loopStartTime = _data.value("loopStartTime", 0.0f);
	loopEndTime = _data.value("loopEndTime", 0.0f);

	//サウンドの情報を渡す
	res = std::make_unique<ResourceData>(parameter.type, parameter.path, soundType, pitch, timeStretch, volume, loopStartTime, loopEndTime);

	//配列に挿入
	resourcesMap_.emplace(parameter.src, std::move(res));
}

void ResourceManager::LoadResourceShader(const RES_INFO _info, const nlohmann::json& _data )
{
	std::unique_ptr<ResourceData> res;

	//共通パラメータの取得
	RESOURCE_COMMON_PARAM parameter = GetResourceParameter(_info,_data);

	//定数バッファサイズ
	int constBufNum = 0;

	//定数バッファサイズを取得
	constBufNum = _data.value("constBufNum", 0);
	//シェーダのリソース情報を渡す
	res = std::make_unique<ResourceData>(parameter.type, parameter.path, constBufNum);

	//配列に挿入
	resourcesMap_.emplace(parameter.src, std::move(res));
}
