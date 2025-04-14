#pragma once
#include <vector>
#include <map>

#include "Vector2D.h"
#include "Singleton.h"

#define BLOCK_SIZE 32
#define STAGE_WIDTH 20
#define STAGE_HEIGHT 16

/// <summary>
/// パネル情報
/// </summary>
enum ePanelID : unsigned char
{
	SORA,
	FLOOR,
	NONE
};

/// <summary>
/// ステージのパネル情報
/// </summary>
class StageData :public Singleton<StageData>
{
private:
	std::vector<std::vector<ePanelID>> data;

public:
	/// <summary>
	/// 全てのパネル情報を取得する
	/// </summary>
	/// <returns>全てのパネル情報</returns>
	static const std::vector<std::vector<ePanelID>> GetAll();

	/// <summary>
	/// 現在座標のパネル情報を取得する
	/// </summary>
	/// <param name="location">現在座標</param>
	/// <returns>パネル情報</returns>
	static const ePanelID GetPanelData(const Vector2D& location);

	/// <summary>
	/// 座標情報を添字に変換する
	/// </summary>
	/// <param name="location">座標情報</param>
	/// <param name="i">変換後の添字</param>
	/// <param name="j">変換後の添字</param>
	static void ConvertToIndex(const Vector2D& location, int& i, int& j);

private:
	/// <summary>
	/// ステージデータを読み込む
	/// </summary>
	void Load();
};