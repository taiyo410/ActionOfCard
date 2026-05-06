#pragma once
#include "ObjectBase.h"

class ModelRenderer;
class ModelMaterial;

class Stage :
    public ObjectBase
{

public:

    //ステージのスケール
    static constexpr float STAGE_SCL = 30.0f;
    static constexpr float STAGE_SCL_Y = 0.1f;

    //壁のスケール
    static constexpr float WALL_SCL = 60.0f;

    //UVスケール
    static constexpr float STAGE_UV_SCL = 300.0f;

    //ステージのサイズ
    static constexpr float STAGE_SIZE = 200.0f * STAGE_SCL;

    //ステージの座標
    static constexpr VECTOR STAGE_POS = { 0.0f,0.0f,0.0f };

	/// @brief コンストラクタ
	/// @param  
	Stage(void);

	/// @brief デストラクタ
	/// @param  
	~Stage(void)override;

    /// @brief ロード
    /// @param  
    void Load(void)override;

    /// @brief 初期化
    /// @param  
    void Init(void)override;

    /// @brief 更新
    /// @param  
    void Update(void)override;

    /// @brief 描画
    /// @param  
    void Draw(void)override;

    /// @brief 当たった時の処理
    /// @param _hitCol 相手のコライダ
    void OnHit(const std::weak_ptr<Collider> _hitCol)override;

private:

    //頂点シェーダのパス名
    const std::wstring VERTEX_SHADER_PATH = L"StageVS.cso";

    //ピクセルシェーダのパス名
    const std::wstring PIXEL_SHADER_PATH = L"StagePS.cso";

    //壁の情報
    Transform wallTrans_;

    //シェーダー関連
    std::unique_ptr<ModelMaterial> material_;       //マテリアル
    std::unique_ptr<ModelRenderer> renderer_;       //レンダラー
};

