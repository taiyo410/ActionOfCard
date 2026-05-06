#pragma once
#include <DxLib.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>
#include "../Common/Vector2.h"
#include "../Common/Vector2F.h"

class UtilityCommon
{
public:

	// ラジアン(rad)・度(deg)変換用
	static constexpr float RAD2DEG = (180.0f / DX_PI_F);
	static constexpr float DEG2RAD = (DX_PI_F / 180.0f);

	//カラーコード
	static constexpr unsigned int BLACK = 0x000000;	//黒
	static constexpr unsigned int RED = 0xFF0000;	//赤
	static constexpr unsigned int BLUE = 0x0000FF;	//青
	static constexpr unsigned int YELLOW = 0xFFFF00;	//黄色
	static constexpr unsigned int GREEN = 0x008000;	//緑
	static constexpr unsigned int CYAN = 0x00FFFF;	//水色
	static constexpr unsigned int PINK = 0xFFC0CB;	//桃色
	static constexpr unsigned int ORANGE = 0xFFA500;	//オレンジ
	static constexpr unsigned int LIME = 0xADFF2F;	//黄緑
	static constexpr unsigned int PURPLE = 0x800080;	//紫
	static constexpr unsigned int WHITE = 0xFFFFFF;	//白
	static constexpr unsigned int GRAY = 0xBBBBBB;	//灰色
	static constexpr unsigned int BROWN = 0x8B4513;	//茶色

	//固定デルタタイム
	static constexpr float FIXED_DELTA_TIME = 1.0f / 60.0f;
	//パーセント
	static constexpr float RATIO_MAX = 1.0f;
	static constexpr float RATIO_MIN = 0.0f;
	static constexpr int PERCENT_MAX = 100;

	//アルファ値
	static constexpr int ALPHA_MAX = 255;
	static constexpr int ALPHA_MIN = 0;

	//画像ハンドルの初期化値
	static constexpr int INITIAL_HANDLE = -1;


	/// @brief 四捨五入
	/// @param v ベクトル
	/// @return 
	static int Round(float v);

	/// @brief 文字列の分割
	/// @param line 文字列
	/// @param delimiter 
	/// @return 
	static std::vector <std::string> Split(std::string& line, char delimiter);

	/// @brief ラジアン(rad)から度(deg)
	/// @param rad ラジアン
	/// @return 
	static double Rad2DegD(double rad);
	static float Rad2DegF(float rad);
	static int Rad2DegI(int rad);

	/// @brief 度(deg)からラジアン(rad)
	/// @param deg 度
	/// @return 
	static double Deg2RadD(double deg);
	static float Deg2RadF(float deg);
	static int Deg2RadI(int deg);

	/// @brief 0～360度の範囲に収める
	/// @param deg デグリー角
	/// @return 
	static double DegIn360(double deg);

	// 0(0)～2π(360度)の範囲に収める
	
	/// @brief 0(0)～2π(360度)の範囲に収める
	/// @param rad ラジアン
	/// @return 
	static double RadIn2PI(double rad);

	/// @brief 回転が少ない方の回転向きを取得する(時計回り:1、反時計回り:-1)
	/// @param from 始端
	/// @param to 終端
	/// @return 
	static int DirNearAroundRad(float from, float to);

	/// @brief 回転が少ない方の回転向きを取得する(時計回り:1、反時計回り:-1)
	/// @param from 始端
	/// @param to 終端
	/// @return 
	static int DirNearAroundDeg(float from, float to);

	/// @brief 待機時間
	/// @param totalTime 合計時間
	/// @param waitTime 現在の時間
	/// @return 
	static bool IsTimeOver(float& totalTime, const float& waitTime);
	
	/// @brief 範囲外の値を指定した範囲に収める
	/// @param index 収めたい値
	/// @param max 範囲の最大値
	/// @return 0からN-1の範囲に補正した値
	static int WrapIndex(int index, int max);
	
	/// @brief CSVファイルの読み込み
	/// @param filePath ファイルパス
	/// @return 読み込んだデータ
	static std::vector<std::vector<int>> LoadCSVData(const std::wstring& filePath);
	
	/// @brief stringオブジェクトから(SJIS)からwstringオブジェクトに変換
	/// @param str stringオブジェクト
	/// @return wstringオブジェクト
	static std::wstring GetWStringFromString(const std::string& str);

	/// @brief wstringオブジェクトから(SJIS)からstringオブジェクトに変換
	/// @param str stringオブジェクト
	/// @return wstringオブジェクト
	static std::string GetStringFromWString(const std::wstring& wstr);

	/// @brief JSONファイルの読み込み
	/// @param filePath ファイルパス
	/// @return 読み込んだJSONデータ
	static nlohmann::json LoadJsonData(const std::string& filePath);



};