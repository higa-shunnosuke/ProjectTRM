#include "SceneManager.h"
#include "SceneFactory.h"
#include "DxLib.h"

#include "../Utility/Input/InputManager.h"
#include "../Utility/ResourceManager.h"
#include "../Objects/GameObjectManager.h"
#include "../Objects/GameObject.h"


// コンストラクタ
SceneManager::SceneManager() :
	current_scene(nullptr)
{

}

// デストラク
SceneManager::~SceneManager()
{
	// 解放忘れ防止
	Finalize();
}

// 初期化処理
void SceneManager::Initialize()
{
	// 最初のシーンをタイトル画面にする
	ChangeScene(eSceneType::in_game);

}

//  更新処理
void SceneManager::Update(float delta_second)
{
	// シーンの更新
	eSceneType next_scene_type = current_scene->Update(delta_second);

	GameObjectManager* object;			// オブジェクトマネージャーのポインタ
	object = GameObjectManager::GetInstance();

	// オブジェクトリストを取得
	std::vector<GameObject*> objects_list = object->GetObjectsList();

	// 当たり判定確認処理
	for (int i = 0; i < objects_list.size(); i++)
	{
		// 動かないオブジェクトの場合当たり判定確認処理をしない
		if (objects_list[i]->GetMobility() == false)
		{
			continue;
		}
		else
		{
			for (int j = 0; j < objects_list.size(); j++)
			{
				// 自分同士の当たり判定確認処理はしない
				if (i == j)
				{
					continue;
				}

				// 当たり判定確認処理
				CheckCollision(objects_list[i], objects_list[j]);
			}
		}
	}

	// 攻撃範囲検知処理
	for (int i = 0; i < objects_list.size(); i++)
	{
		if (objects_list[i]->GetAggressive() == false)
		{
			continue;
		}
		else
		{
			for (int j = 0; j < objects_list.size(); j++)
			{
				// 自分同士の当たり判定確認処理はしない
				if (i == j)
				{
					continue;
				}

				// 当たり判定確認処理
				CheckAreaDetection(objects_list[i], objects_list[j]);
			}
		}
	}

	// フォントサイズ変更
	SetFontSize(32);

	// 描画処理
	Draw();

	// 現在のシーンタイプが次のシーンタイプと違っているか？
	if (current_scene->GetNowSceneType() != next_scene_type)
	{
		// シーン切り替え処理
		ChangeScene(next_scene_type);
	}
}

// 終了時処理
void SceneManager::Finalize()
{
	// シーン情報が生成されていれば、削除する
	if (current_scene != nullptr)
	{
		current_scene->Finalize();
		delete current_scene;
		current_scene = nullptr;
	}
}

// 描画処理
void SceneManager::Draw() const
{
	// 画面の初期化
	ClearDrawScreen();

	// シーンの描画処理
	current_scene->Draw();

	// 裏画面の内容を表画面に反映する
	ScreenFlip();
}

// シーン切り替え処理
void SceneManager::ChangeScene(eSceneType next_type)
{
	// 次のシーンを生成する
	SceneBase* next_scene = SceneFactory::CreateScene(next_type);

	// エラーチェック
	if (next_scene == nullptr)
	{
		throw ("シーンが生成できませんでした\n");
	}

	// シーン情報が格納されていたら、削除する
	if (current_scene != nullptr)
	{
		current_scene->Finalize();
		delete current_scene;
	}

	// 次のシーンの初期化
	next_scene->Initialize();

	// 現在シーンの上書き
	current_scene = next_scene;
}

// 当たり判定確認処理
void SceneManager::CheckCollision(GameObject* target, GameObject* partner)
{
	// ヌルポチェック
	if (target == nullptr || partner == nullptr)
	{
		return;
	}

	// 当たり判定情報を取得
	Collision tc = target->GetCollision();
	Collision pc = partner->GetCollision();

	if (tc.is_blocking == false || pc.is_blocking == false)
	{
		return;
	}

	// 当たり判定が有効か確認する
	if (tc.IsCheckHitTarget(pc.object_type) || pc.IsCheckHitTarget(tc.object_type))
	{
		//２つのオブジェクトの距離を取得
		Vector2D diff = target->GetLocation() - partner->GetLocation();

		//２つのオブジェクトの当たり判定の大きさを取得
		Vector2D box_size = (tc.box_size + pc.box_size) / 2.0f;
		
		// 矩形同士の当たり判定
		if ((fabsf(diff.x) < box_size.x) && (fabsf(diff.y) < box_size.y))
		{
			// 当たっていることを通知する
			target->OnHitCollision(partner);
			partner->OnHitCollision(target);
		}
	}
}

// 攻撃範囲検知処理
void SceneManager::CheckAreaDetection(GameObject* target, GameObject* partner)
{
	// ヌルポチェック
	if (target == nullptr || partner == nullptr)
	{
		return;
	}

	// 当たり判定情報を取得
	Collision tc = target->GetCollision();
	Collision pc = partner->GetCollision();

	if (tc.is_blocking == false || pc.is_blocking == false)
	{
		return;
	}

	// 当たり判定が有効か確認する
	if (tc.IsCheckHitTarget(pc.object_type) || pc.IsCheckHitTarget(tc.object_type))
	{
		//２つのオブジェクトの距離を取得
		Vector2D diff = target->GetLocation() - partner->GetLocation();

		//２つのオブジェクトの当たり判定の大きさを取得
		Vector2D box_size = (tc.box_size + pc.box_size) / 2.0f;
		
		// 矩形同士の当たり判定
		if ((fabsf(diff.x) < box_size.x) && (fabsf(diff.y) < box_size.y))
		{
			// 当たっていることを通知する
			target->OnAreaDetection(partner);
		}
	}
}