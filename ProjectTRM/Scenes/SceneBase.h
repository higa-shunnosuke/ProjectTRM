#pragma once

#include <vector>
#include <string>
#include "DxLib.h"

#include "../Utility/ProjectConfig.h"
#include "../Utility/ResourceManager.h"
#include "../Utility/Input/InputManager.h"
#include "../Utility/Camera/Camera.h"
#include "../Objects/GameObjectManager.h"


/// <summary>
/// シーンタイプ
/// </summary>
enum class eSceneType
{
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
	GameObjectManager* object;	// オブジェクトマネージャーのポインタ
	int cursor;					// カーソル

public:
	// コンストラクタ
	SceneBase():
		object(nullptr),
		cursor()
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
		// オブジェクトマネージャーの情報を取得
		object = GameObjectManager::GetInstance();
	}

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="delta_second">1フレーム当たりの時間</param>
	/// <returns>次のシーンタイプ情報</returns>
	virtual eSceneType Update(const float& delta_second)
	{
		// 生成するオブジェクトがあれば、オブジェクトリスト内に挿入する
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
		// カメラのポインタ
		Camera* camera = Camera::GetInstance();

		// オブジェクトリスト内のオブジェクトを描画する
		for (GameObject* obj : object->GetObjectsList())
		{
			obj->Draw(camera->GetCameraPos());
		}
	}

	/// <summary>
	/// 終了時処理
	/// </summary>
	virtual void Finalize()
	{
		// オブジェクトリスト内のオブジェクトを破棄
		object->DestroyAllObject();
	}

	/// <summary>
	/// 現在のシーンタイプ取得処理
	/// </summary>
	/// <returns>現在のシーンタイプ情報</returns>
	virtual const eSceneType GetNowSceneType() const = 0;

};

