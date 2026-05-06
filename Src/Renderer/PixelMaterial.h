#pragma once
#include <string>
#include <vector>
#include <DxLib.h>
#include "../Manager/Resource/ResourceManager.h"

class ResourceManager;

class PixelMaterial
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

	/// @brief コンストラクタ
	/// @param shaderFileName シェーダーファイル名
	/// @param constBufFloat4Size 定数バッファの確保サイズ(FLOAT4をいくつ作るか)
	PixelMaterial(const ResourceManager::SRC _psSrc);

	/// @brief デストラクタ
	/// @param  
	~PixelMaterial(void);

	/// @brief 定数バッファを追加
	/// @param contBuf 定数バッファ
	void AddConstBuf(const FLOAT4& contBuf);

	/// @brief 定数バッファを更新
	/// @param idx 更新する配列番号
	/// @param contBuf 更新後の定数バッファ
	void SetConstBuf(int idx, const FLOAT4& contBuf);

	/// @brief テクスチャバッファを追加
	/// @param texDiffuse テクスチャ
	void AddTextureBuf(int texDiffuse);

	/// @brief テクスチャを更新
	/// @param idx 配列番号
	/// @param texDiffuse テクスチャ
	void SetTextureBuf(int idx, int texDiffuse);

	/// @brief シェーダの取得
	/// @param  
	/// @return シェーダ
	const int GetShader(void) const { return shader_; }

	/// @brief 定数バッファハンドル
	/// @param  
	/// @return 定数バッファ
	const int GetConstBuf(void) const { return constBuf_; }

	/// @brief 定数バッファ配列の取得
	/// @param  
	/// @return 定数バッファ配列
	const std::vector<FLOAT4>& GetConstBufs(void) const { return constBufs_; }

	/// @brief テクスチャの取得
	/// @param  
	/// @return テクスチャ
	const std::vector<int>& GetTextures(void) const { return textures_; }

	/// @brief テクスチャアドレスを取得
	/// @param  
	/// @return テクスチャアドレス
	TEXADDRESS GetTextureAddress(void) const { return texAddress_; }
	
	/// @brief テクスチャアドレスを設定
	/// @param texA テクスチャアドレス
	void SetTextureAddress(TEXADDRESS texA) { texAddress_ = texA; }

	int GetConstBufNum(void) { return constBufFloat4Size_; }

private:

	//リソースマネージャ
	ResourceManager& resMng_;

	//使用するシェーダ
	ResourceManager::SRC psSrc_;

	// シェーダ
	int shader_;

	// 定数バッファの確保サイズ(FLOAT4をいくつ作るか)
	int constBufFloat4Size_;

	// 定数バッファハンドル
	int constBuf_;

	// テクスチャアドレス
	TEXADDRESS texAddress_;

	// 定数バッファ
	std::vector<FLOAT4> constBufs_;

	// 画像
	std::vector<int> textures_;

};
