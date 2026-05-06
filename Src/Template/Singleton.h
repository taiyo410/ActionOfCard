#pragma once
#include <memory>
#include <cassert>

// シングルトンテンプレートクラス
template <typename T>
class Singleton
{

public:

    /// @brief インスタンスの生成
    static void CreateInstance(void)
    {
        if (!instance_)
        {
            instance_ = new T();
        }
    }

    /// @brief インスタンスへのアクセス
    /// @param  
    /// @return インスタンス
    static T& GetInstance(void)
    {
        assert(instance_ != nullptr && "シングルトンクラスが生成されていない");
        return *instance_;
    }
	
    /// @brief 解放処理
    /// @param  
    static void Destroy(void)
	{
		if (instance_)
		{
			delete instance_;
			instance_ = nullptr;
		}
	}   
    /// @brief コピーコンストラクタを削除
    /// @param  
    Singleton(const Singleton&) = delete;
    
    /// @brief 
    /// @param  コピー代入演算子を削除
    /// @return 
    Singleton& operator=(const Singleton&) = delete;

protected:

    /// @brief デフォルトコンストラクタは protected にして外部生成を防止
    /// @param  
    Singleton(void) = default;

    /// @brief デストラクタも protected
    /// @param  
    virtual ~Singleton(void) = default;

private:

    //静的メンバ初期化
    inline static T* instance_ = nullptr; 

};

