#pragma once
#include <map>
#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include "Template/Singleton.h"
#include "Resource.h"

class ResourceManager : 
	public Singleton<ResourceManager>
{
	//シングルトン
	friend class Singleton<ResourceManager>;

public:

	//リソース
	enum class SRC
	{
		NONE,
		//モデル
		STAGE_FLOOR,				//ステージ床
		STAGE_WALL,					//ステージ壁
		PLAYER,						//プレイヤー
		ENEMY,						//敵
		SKY_DOME,					//スカイドーム
		SPHERE_ROCK,				//岩
		KEY_BLADE,					//鍵剣
		//アニメーション
		//プレイヤー
		P_IDLE,						//待機
		P_RUN,						//走る
		P_ATTACK_1_SHORT,			//攻撃1(近距離)
		P_ATTACK_1_MIDDLE,			//攻撃1(中距離)
		P_ATTACK_2,					//攻撃2
		P_ATTACK_3,					//攻撃3
		P_FIRE_MAGIC,				//炎魔法
		P_DODGE,					//回避
		P_RELOAD,					//リロード
		P_DEATH,					//プレイヤー倒れ
		//敵
		E_IDLE,						//待機
		E_RUN,						//走る
		E_STOMP_ATK,				//横薙ぎ攻撃
		E_JUMP_ATK,					//ジャンプ攻撃
		E_ROAR_ATK,					//咆哮攻撃
		E_KNOCK_DOWN,				//大ダウン
		REACT,
		E_DEATH,					//敵倒れる
		E_REVOLUTION,				//革命アクション

		//画像
		TITLE_BACK_IMG,				//タイトル背景
		TITLE_LOGO,					//タイトルロゴ
		GAME_OVER_IMG,				//ゲームオーバーイメージ
		GAME_CLEAR_IMG,
		PLAYER_ATK_CARD_IMG,		//番号なしアタックカード
		PLAYER_FIRE_CARD_IMG,		//番号なしファイアカード
		PLAYER_THUNDER_CARD_IMG,	//番号なしサンダーカード
		ENEMY_ATK_CARD_IMG,			//敵番号なしアタックカード
		RELOAD_CARD_IMG,			//リロードカード
		RELOAD_GAUGE,				//リロードゲージ
		P_CARD_NUM_GAUGE,			//カード枚数ゲージマスク
		P_CARD_NUM_GAUGE_BACK,		//カード枚数ゲージ背景
		E_HP_BAR,					//敵体力体力ゲージマスク
		SKIP_BUTTOM_MASK,			//スキップボタン画像
		CARD_REVOLVER_L_ARROW,		//カードリボルバー回転方向の左矢印
		INTENSIVE_LINE_1,			//集中線画像1(複数画像でアニメーション)
		INTENSIVE_LINE_2,			//集中線画像2(複数画像でアニメーション)
		REVERSE_FADE_MASK,			//色反転のマスク
		HIGHER_IMG,					//数字勝ち負けの大きい数を示す画像
		LOWER_IMG,					//数字勝ち負けの小さい数を示す画像
		WIN_IMG,					//数字勝ち負けの小さい数を示す画像

		//スプライト画像
		NUMBERS_IMGS,				//カード番号
		CONTROLLER_UI_IMGS,			//コントローラーのボタンUI

		//エフェクト
		BLAST,						//爆発エフェクト
		KEY_BLADE_HIT_EFF,			//鍵剣ヒットエフェクト
		E_JUMP_CHARGE_EFF,			//敵のジャンプチャージエフェクト
		E_DEATH_EFF,				//敵の死亡エフェクト
		RELOAD_EFF,					//リロードエフェクト
		RELOAD_END_EFF,				//リロード終了エフェクト
		FIRE_BALL_EFF,				//炎のエフェクト
		THUNDER_EFF,				//雷のエフェクト
		REVOLUTION_EFF,				//革命エフェクト
		P_HIT_EFF,				//革命エフェクト

		//サウンド
		//BGM
		TITLE_BGM,					//タイトルBGM
		GAME_BGM,					//ゲームBGM
		GAME_CLEAR_BGM,				//ゲームクリアBGM
		GAME_OVER_BGM,				//ゲームオーバーBGM
		//SE
		PLAYER_FOOT_SE,				//プレイヤー足音
		ENEMY_FOOT_SE,				//エネミー足音
		ENEMY_STOMP_SE,				//エネミー爆発音
		ENEMY_CHARGE_SE,			//エネミーチャージ音
		ENEMY_JUMP_LAND_SE,			//エネミージャンプ着地音
		ENEMY_ROAR_SE,				//エネミー咆哮
		ENEMY_HIT_SE,				//エネミーヒット着地音
		ENEMY_REVOLUTION_SE,		//革命発生時の音
		PLAYER_ATTACK_SE,			//プレイヤー攻撃音
		PLAYER_DODGE_SE,			//プレイヤー回避音
		PLAYER_HIT_SE,				//プレイヤーヒット音
		FIRE_SPAWN_SE,				//ファイア出現時の音
		FIRE_HIT_SE,				//ファイアヒット音
		THUNDER_SE,					//サンダーの音
		//カード関連
		CARD_PUT_SE	,					//カードを引く(アクション開始時)
		CARD_MOVE_SE,					//カード移動
		CARD_BE_REFLECTED_SE,			//カード弾かれ音
		CARD_BREAK_SE,					//カード弾き音(カードに勝った)
		CARD_RELOAD_SE,					//カードリロード音
		CARD_RELOAD_FINISH_SE,			//カードリロード終了音
		//ボタン
		MOVE_BTN_SE,					//移動ボタン
		DESIDE_BTN_SE,					//決定ボタン
		GAME_START_SE,					//ゲームスタート音

		//Json
		CHARA_DATA,						//キャラクターデータ
		DECK_DATA,						//デッキデータ
		ACTION_DATA,					//アクションデータ
		COLLISION_DATA,					//当たり判定データ
		UI_DATA,						//UIのパラメータデータ
		MODEL_DATA,						//モデルのパラメータデータ

		//ピクセルシェーダ
		STANDARD_PS,				//通常描画シェーダー
		TILEMAP_PS,					//タイルマップ
		CHARACTER_MODEL_PS,			//キャラクター用のピクセルシェーダ
		SKYDOME_PS,					//スカイドーム
		HPBAR_PS,					//HPバー
		ARCBAR_PS,					//円形ゲージ
		CARD_NORMAL_PS,				//通常カード
		CARD_RELOAD_PS,				//リロードカード
		CARD_SELECT_PS,				//カード選択枠
		REVOLUTION_POSTEFF_PS,		//革命を起こした時のポストエフェクト
		CHARACTER_SHADOW_PS,		//キャラクター用のシャドウシェーダ
		SHADOW_PS,					//シャドウ用のピクセルシェーダ

		//頂点シェーダ
		STANDARD_VS,				//通常描画シェーダー
		TILEMAP_VS,					//タイルマップ
		CHARACTER_MODEL_VS,			//キャラクターモデル
		SHADOW_MESH_VS,				//シャドウメッシュ
		SHADOW_SKINNED_MESH_VS,		//シャドウスキンメッシュ
	};

	struct RES_INFO
	{
		ResourceData::TYPE resType;
		std::wstring typePath;
	};

	struct RESOURCE_COMMON_PARAM
	{
		ResourceData::TYPE type = ResourceData::TYPE::NONE;		//タイプ	
		SRC src = SRC::NONE;									//素材名
		std::wstring path = L"";								//素材のパス
	};

	struct IMGS_PARAMETA
	{
		int numX = 0;	//横枚数
		int numY = 0;	//縦枚数
		int sizeX = 0;	//横サイズ
		int sizeY = 0;	//縦サイズ
	};

	struct SOUND_PARAMETA
	{
		ResourceData::SOUND_TYPE soundType=ResourceData::SOUND_TYPE::MAX;
		float pitch = 0.0f;
		float timeStretch = 1.0f;
		float volume = 1.0f;
		float loopStartTime = 0.0f;
		float loopEndTime = 0.0f;
	};

	/// @brief 初期化
	/// @param  
	void Init(void);
	
	/// @brief 解放(シーン切替時に一旦解放)
	/// @param  
	void SceneChangeRelease(void);
	
	/// @brief リソースの完全解放
	/// @param  
	void Release(void);
	
	/// @brief リソースのロード
	/// @param src 読み込むリソース
	/// @return リソース
	const ResourceData& Load(SRC src);
	
	/// @brief リソースの複製ロード(モデル用)
	/// @param src 複製したいリソース
	/// @return 複製したリソース
	int LoadModelDuplicate(SRC src);

	/// @brief ロード済みリソースの取得
	/// @param src 
	/// @return 
	const ResourceData GetResource(const SRC src) const;

	/// @brief サウンドリソースのイテレータの取得
	/// @param _soundType BGM、SEなどのサウンドの種類(指定なしでサウンドの種類をすべて渡す)
	/// @return サウンドの配列
	std::vector<const ResourceData*> GetSoundResources(ResourceData::SOUND_TYPE _soundType=ResourceData::SOUND_TYPE::MAX)const;

	/// @brief 文字列からリソース名を取得
	/// @param _str 取得したい文字列
	/// @return 
	const SRC GetSrcFromString(const std::string& _str) const;
private:

	//JSONファイルで素材が見つからなかった場合のエラー文
	const std::wstring NONE_SRC_INJSON_STR = L"JSONファイルで名前が見つかりませんでした。";

	// リソース管理の対象
	std::map<SRC, std::unique_ptr<ResourceData>> resourcesMap_;

	// 読み込み済みリソース
	std::map<SRC, ResourceData&> loadedMap_;

	// ダミーリソース
	ResourceData dummy_;

	//リソースごとの文字列
	std::unordered_map<std::string,SRC > resStr_;
	//リソースのタイプごとの文字列
	std::unordered_map<std::string, RES_INFO> resTypeStr_;

	//データロード関数
	std::unordered_map<ResourceData::TYPE, std::function<void(const RES_INFO _info,const nlohmann::json _data)>> loadDataFunc_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	ResourceManager(void);
	ResourceManager(const ResourceManager& _copy) = delete;

	// コピー代入演算子は使用不可
	ResourceManager& operator=(const ResourceManager _copy) = delete;
	~ResourceManager(void)override;

	// 内部ロード
	ResourceData& _Load(SRC src);

	//外部ファイルからのロード	
	const RESOURCE_COMMON_PARAM GetResourceParameter(const RES_INFO _info, const nlohmann::json& _data );		//共通パラメータの取得

	//handleIDしか使用しない種類のロード
	void LoadResourceCommon(const RES_INFO _info, const nlohmann::json& _data);

	//複数画像のロード
	void LoadResourceImages(const RES_INFO _info, const nlohmann::json& _data );

	//サウンドのロード
	void LoadResourceSound(const RES_INFO _info, const nlohmann::json& _data );

	//シェーダのロード
	void LoadResourceShader(const RES_INFO _info, const nlohmann::json& _data );
};	