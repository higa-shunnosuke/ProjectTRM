#pragma once

#include <vector>
#include <string>
#include "DxLib.h"

#include "../Utility/ProjectConfig.h"
#include "../Utility/ResourceManager.h"
#include "../Utility/Input/InputManager.h"
#include "../Utility/Input/InputEventManager.h"
#include "../Objects/GameObjectManager.h"


/// <summary>
/// シーンタイプ
/// </summary>
enum class eSceneType
{
	title,		// タイトル
	in_game,	// インゲーム
	re_start,	// リスタート
	result,		// リザルト
	exit,		// 終了
};

/// <summary>
/// シーン基底クラス
/// </summary>
class SceneBase
{
protected:
	// 各シーンが所有する情報
	


public:
	// コンストラクタ
	SceneBase()
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

		// 現在のシーン情報を返却する
		return GetNowSceneType();
	}

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() const
	{
		
		
	}

	/// <summary>
	/// 終了時処理
	/// </summary>
	virtual void Finalize()
	{
		
	}

	/// <summary>
	/// 現在のシーンタイプ取得処理
	/// </summary>
	/// <returns>現在のシーンタイプ情報</returns>
	virtual const eSceneType GetNowSceneType() const = 0;

};

