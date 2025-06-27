#pragma once

#include <vector>
#include <string>
#include "DxLib.h"

#include "../Utility/ProjectConfig.h"
#include "../Utility/ResourceManager.h"
#include "../Utility/Input/InputManager.h"
#include "../Utility/Camera/Camera.h"
#include "../Utility/LightMapManager.h"
#include "../Objects/GameObjectManager.h"

/// <summary>
/// シーンタイプ
/// </summary>
enum class eSceneType
{
	loading,		// ローディング
	title,			// タイトル
	stage_select,	// ステージセレクト
	in_game,		// インゲーム
	re_start,		// リスタート
	result,			// リザルト
	end,			// 終了
};

/// <summary>
/// シーン基底クラス
/// </summary>
class SceneBase
{
protected:
	// 各シーンが所有する情報
	int cursor;					// カーソル
public:
	int make_unit;
	int make_enemy;
	int max_unit = -1;
	int max_enemy;
	int dead_unit;
	int dead_enemy;

	bool win_flg;				//プレイヤーが勝利したかどうか
	int StageNumber;

	// コンストラクタ
	SceneBase() :
		cursor(),
		win_flg(),
		StageNumber()
	{

	}
	// デストラクタ
	virtual ~SceneBase()
	{
		// 解放忘れ防止
		Finalize();
	}

public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Initialize()
	{

	}

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="delta_second">1フレーム当たりの時間</param>
	/// <returns>次のシーンタイプ情報</returns>
	virtual eSceneType Update(const float& delta_second)
	{
		// オブジェクトマネージャーの情報を取得
		GameObjectManager* object = GameObjectManager::GetInstance();
		// オブジェクトリストの更新
		object->CheckCreateObject();

		// リスト内のオブジェクトを更新する
		for (GameObject* obj : object->GetObjectsList())
		{
			obj->Update(delta_second);
		}

		// デストロイリストの更新
		object->CheckDestroyObject();

		// 現在のシーン情報を返却する
		return GetNowSceneType();
	}

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() const
	{
		// カメラの情報取得
		Camera* camera = Camera::GetInstance();

		// オブジェクトマネージャーの情報を取得
		GameObjectManager* object = GameObjectManager::GetInstance();
		// オブジェクトリスト内のオブジェクトを描画する
		for (GameObject* obj : object->GetObjectsList())
		{
			// カメラの座標を引数で渡す
			obj->Draw();
		}
	}

	/// <summary>
	/// 終了時処理
	/// </summary>
	virtual void Finalize()
	{
		// オブジェクトマネージャーの情報を取得
		GameObjectManager* object = GameObjectManager::GetInstance();
		// オブジェクトリスト内のオブジェクトを破棄
		object->DestroyAllObject();

		// ライトマップマネージャーの情報を取得
		LightMapManager* light = LightMapManager::GetInstance();
		// ライトリストを破棄
		light->DeleteLightList();
	}

	/// <summary>
	/// 現在のシーンタイプ取得処理
	/// </summary>
	/// <returns>現在のシーンタイプ情報</returns>
	virtual const eSceneType GetNowSceneType() const = 0;

	/// <summary>
	/// 勝敗設定処理
	/// </summary>
	/// <param name="setflg">勝敗フラグ</param>
	void IsPlayerWin(bool setflg)
	{
		win_flg = setflg;
	}

};

