#pragma once

#include "SceneBase.h"
#include "Title/Title.h"
#include "InGame/InGame.h"
#include "Result/Result.h"

class SceneFactory
{
public:

	/// <summary>
	/// シーン生成処理
	/// </summary>
	/// <param name="next_type">次のシーンタイプ</param>
	/// <returns>生成したシーン情報のポインタ</returns>
	static SceneBase* CreateScene(eSceneType next_type)
	{
		// シーンタイプによって、生成するシーンを切り替える
		switch (next_type)
		{
		case eSceneType::title:
			return dynamic_cast<SceneBase*>(new Title());
		case eSceneType::in_game:
		case eSceneType::re_start:
			return dynamic_cast<SceneBase*>(new InGame());
		case eSceneType::result:
			return dynamic_cast<SceneBase*>(new Result());
		default:
			return nullptr;
		}
	}

};