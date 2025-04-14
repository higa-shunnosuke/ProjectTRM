#include "StageData.h"

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

/// <summary>
/// ステージデータを読み込む
/// </summary>
void StageData::Load()
{
	FILE* fp = NULL;	// ファイルの情報
	int mc = 0;			// マップチップ
	int i = 0;
	int j = 0;

	//読込ファイルを開く
	fopen_s(&fp, "Resource/Map/Stage.csv", "r");

	//エラーチェック
	if (fp == NULL)
	{
		throw("ファイルが読み込めません\n");
	}
	else
	{
		//ファイルの最後までループする
		while (mc != EOF)
		{
			// idの初期値
			ePanelID id = ePanelID::FLOOR;

			//ファイル情報を１つずつ読み込む
			mc = fgetc(fp);
			j++;

			//カンマは無視する
			if (mc == ',')
			{
				j--;
				continue;
			}
			//改行する
			else if (mc == '\n')
			{
				i++;
				j = 0;
				continue;
			}
			else
			{
				switch (mc)
				{
				case '0':
					break;
				case 'F':
					break;
				case 'H':
					break;
				case 'B':
					break;
				case 'S':
					break;
				case 'W':
					break;
				default:
					break;
				}
			}
		}
		//ファイルを閉じる
		fclose(fp);
	}
}

/// <summary>
/// 全てのパネル情報を取得する
/// </summary>
/// <returns>全てのパネル情報</returns>
const std::vector<std::vector<ePanelID>> StageData::GetAll()
{
	StageData* stage = StageData::GetInstance();

	if (stage == nullptr)
	{
		// オブジェクトの作成
		stage = new StageData();
		stage->Load();
	}

	return stage->data;
}

/// <summary>
/// 現在座標のパネル情報を取得する
/// </summary>
/// <param name="location">現在座標</param>
/// <returns>パネル情報</returns>
const ePanelID StageData::GetPanelData(const Vector2D& location)
{
	StageData* stage = StageData::GetInstance();

	if (stage == nullptr)
	{
		// オブジェクトの作成
		stage = new StageData();
		stage->Load();
	}

	// 座標を添字に変換
	int i = 0, j = 0;
	ConvertToIndex(location, i, j);

	// iとjの範囲チェック
	if ((i < 0) || (j < 0) || (stage->data.size() <= i) || (stage->data[0].size() <= j))
	{
		return ePanelID::NONE;
	}

	return stage->data[i][j];
}

/// <summary>
/// 座標情報を添字に変換する
/// </summary>
/// <param name="location">座標情報</param>
/// <param name="i">変換後の添字</param>
/// <param name="j">変換後の添字</param>
void StageData::ConvertToIndex(const Vector2D& location, int& i, int& j)
{
	j = static_cast<int>(location.x / BLOCK_SIZE);
	i = static_cast<int>(location.y / BLOCK_SIZE);
}