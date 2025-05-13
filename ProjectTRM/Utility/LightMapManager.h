#pragma once

#include "../Utility/Singleton.h"
#include "../Utility/ProjectConfig.h"
#include "../Objects/GameObject.h"
#include "../Utility/ResourceManager.h"

struct LightDetail {
	GameObject* object;
	float size;

	bool operator==(const LightDetail detail) const
	{
		return object == detail.object;
	}
};

/// <summary>
/// ライトマップ管理クラス
/// </summary>
class LightMapManager :public Singleton<LightMapManager>
{
private:
	std::vector<LightDetail> light_list;	// 追従リスト
	int light_graph;						// 光の画像
	int light_screen;						// ライトマップ
	int screen_brightness;					// 画面の明るさ（0～255）

public:

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize()
	{
		// 画像の読み込み
		ResourceManager* rm = ResourceManager::GetInstance();
		light_graph = rm->GetImages("Resource/Images/Light/light.png", 1, 1, 1, 1024, 1024)[0];
		
		// ライトマップを生成
		light_screen = MakeScreen(D_WIN_MAX_X, D_WIN_MAX_Y, TRUE);

		screen_brightness = 100;
	}

	/// <summary>
	/// ライト追加処理
	/// </summary>
	/// <param name="obj">追従するオブジェクト</param>
	void AddLight(LightDetail light)
	{
		// 追従リストに追加
		light_list.push_back(light);
	}

	/// <summary>
	/// ライト削除処理
	/// </summary>
	/// <param name="obj">追従しているオブジェクト</param>
	void DeleteLight(GameObject* obj)
	{
		// 追従リストのから削除
		light_list.erase(
			std::remove_if(light_list.begin(), light_list.end(),
				[obj](const LightDetail& light)
				{
					return light.object == obj;
				}),
			light_list.end());
	}

	/// <summary>
	/// 光の加算合成処理
	/// </summary>
	void DrawLights(const Vector2D camera_pos) const
	{
		// 描画先をライトマップに反映する
		SetDrawScreen(light_screen);
		// ライトマップの初期化（暗いグレーで塗る）
		DrawBox(0, 0, 1280, 720,
			GetColor(screen_brightness, screen_brightness, screen_brightness),
			TRUE);
		
		// 追跡リスト内の座標に光の画像を加算合成
		SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
		for (LightDetail light :light_list)
		{
			if (light.object != nullptr)
			{
				// 座標を取得
				Vector2D light_pos = light.object->GetLocation();

				// カメラ座標をもとに描画位置を計算
				light_pos.x -= camera_pos.x - D_WIN_MAX_X / 2;

				// ライトマップ上に光を描画
				DrawRotaGraphF(light_pos.x, light_pos.y,
					light.size, 0.0,light_graph, TRUE,0);
			}
		}
		// ブレンドモードを初期化
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		// 描画先を表画面に戻す
		SetDrawScreen(DX_SCREEN_BACK);
	}

	/// <summary>
	/// ライトマップ描画処理
	/// </summary>
	void DrawLightMap() const
	{
		// ライトマップを乗算合成して描画
		SetDrawBlendMode(DX_BLENDMODE_MUL, 255);
		DrawGraph(0, 0, light_screen, TRUE);
		
		// ブレンドモードを初期化
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	/// <summary>
	/// 画面の明るさ調整処理
	/// </summary>
	/// <param name="value">明るさ（0～255）</param>
	void SetBrightness(int value)
	{
		// 画面の明るさを調整
		screen_brightness = value;
	}
};