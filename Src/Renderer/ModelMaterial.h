#pragma once
#include <string>
#include <vector>
#include <map>
#include <DxLib.h>
#include "../Manager/Resource/ResourceManager.h"

class ResourceManager;

class ModelMaterial
{

public:

	// 設定できる最大のテクスチャの数
	static constexpr int MAX_TEXTURES = 4;

	// テクスチャアドレスタイプ
	enum class TEXADDRESS
	{
		NONE = 0,
		WRAP,
		MIRROR,
		CLAMP,
		BORDER,
		MAX
	};

	// コンストラクタ
	
	/// @brief コンストラクタ
	/// @param shaderFileNameVS 頂点シェーダファイル名
	/// @param constBufFloat4SizeVS 頂点シェーダ定数バッファサイズ
	/// @param shaderFileNamePS ピクセルシェーダファイル名
	/// @param constBufFloat4SizePS ピクセルシェーダ定数バッファサイズ
	ModelMaterial(const ResourceManager::SRC _vsSrc,const ResourceManager::SRC _psSrc);

	/// @brief // デストラクタ
	/// @param  
	~ModelMaterial(void);


	/// @brief 頂点シェーダ定数バッファを追加
	/// @param contBuf 追加したい定数バッファ
	void AddConstBufVS(const FLOAT4& contBuf);

	/// @brief ピクセルシェーダ定数バッファを追加
	/// @param contBuf 追加したい定数バッファ
	void AddConstBufPS(const FLOAT4& contBuf);
	
	/// @brief 頂点シェーダ定数バッファを更新
	/// @param idx 配列番号
	/// @param contBuf 定数バッファ
	void SetConstBufVS(int idx, const FLOAT4& contBuf);

	/// @brief ピクセルシェーダ定数バッファを更新
	/// @param idx 配列番号
	/// @param contBuf 定数バッファ
	void SetConstBufPS(int idx, const FLOAT4& contBuf);

	/// @brief テクスチャを更新
	/// @param slot スロット番号
	/// @param texDiffuse テクスチャバッファ
	void SetTextureBuf(int slot, int texDiffuse);
	
	/// @brief 頂点シェーダの取得
	/// @param  
	/// @return 頂点シェーダ
	int GetShaderVS(void) const { return shaderVS_; }

	/// @brief ピクセルシェーダの取得
	/// @param  
	/// @return ピクセルシェーダ
	int GetShaderPS(void) const { return shaderPS_; }

	/// @brief 頂点シェーダ定数バッファハンドルの取得
	/// @param  
	/// @return 頂点シェーダ定数バッファハンドル
	int GetConstBufVS(void) const { return constBufVS_; }

	/// @brief ピクセルシェーダ定数バッファハンドルの取得
	/// @param  
	/// @return ピクセルシェーダ定数バッファハンドル
	int GetConstBufPS(void) const { return constBufPS_; }

	/// @brief 頂点シェーダ定数バッファの取得
	/// @param  
	/// @return 頂点シェーダ定数バッファ
	const std::vector<FLOAT4>& GetConstBufsVS(void) const { return constBufsVS_; }

	/// @brief ピクセルシェーダ定数バッファの取得
	/// @param  
	/// @return ピクセルシェーダ定数バッファ
	const std::vector<FLOAT4>& GetConstBufsPS(void) const { return constBufsPS_; }

	/// @brief テクスチャの取得
	/// @param  
	/// @return テクスチャ
	const std::map<int, int>& GetTextures(void) const { return textures_; }

	/// @brief テクスチャアドレスを取得
	/// @param  
	/// @return テクスチャアドレス
	TEXADDRESS GetTextureAddress(void) const { return texAddress_; }
	
	/// @brief テクスチャアドレスを設定
	/// @param texA テクスチャアドレス
	void SetTextureAddress(TEXADDRESS texA) { texAddress_ = texA; }

private:

	// 頂点シェーダ
	int shaderVS_;

	// ピクセルシェーダ
	int shaderPS_;

	//リソースマネージャ
	ResourceManager& resMng_;

	// 頂点定数バッファの確保サイズ(FLOAT4をいくつ作るか)
	int constBufFloat4SizeVS_;

	// 頂点定数バッファハンドル
	int constBufVS_;

	// ピクセル定数バッファの確保サイズ(FLOAT4をいくつ作るか)
	int constBufFloat4SizePS_;

	// ピクセル定数バッファハンドル
	int constBufPS_;


	// テクスチャアドレス
	TEXADDRESS texAddress_;

	// 定数定数バッファ
	std::vector<FLOAT4> constBufsVS_;

	// ピクセル定数バッファ
	std::vector<FLOAT4> constBufsPS_;

	// 画像
	std::map<int, int> textures_;

};
