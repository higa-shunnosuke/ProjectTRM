#pragma once

#include "SceneBase.h"
#include "Title/Title.h"
#include "StageSelect/StageSelect.h"
#include "InGame/InGame.h"
#include "Result/Result.h"
#include "Loading/Loading.h"
#include "End/End.h"

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
			// ローディング
		case eSceneType::loading:
			return dynamic_cast<SceneBase*>(new Loading());
			// タイトル
		case eSceneType::title:
			return dynamic_cast<SceneBase*>(new Title());
			// ステージセレクト
		case eSceneType::stage_select:
			return dynamic_cast<SceneBase*>(new StageSelect);
			// インゲーム、リスタート
		case eSceneType::in_game:
		case eSceneType::re_start:
			return dynamic_cast<SceneBase*>(new InGame());
			// リザルト
		case eSceneType::result:
			return dynamic_cast<SceneBase*>(new Result());
			// エンド
		case eSceneType::end:
			return dynamic_cast<SceneBase*>(new End());
		default:
			return nullptr;
		}
	}
};