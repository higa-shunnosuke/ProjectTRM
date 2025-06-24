#include "SceneManager.h"
#include "SceneFactory.h"
#include "../Utility/LightMapManager.h"

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
	ChangeScene(eSceneType::loading);

#ifdef _DEBUG
	ProjectConfig::DEBUG = true;
#endif // _DEBUG
}

//  更新処理
void SceneManager::Update(float delta_second)
{
	// 入力情報を取得
	InputManager* input = InputManager::GetInstance();
	// デバックモードの切り替え
	if (input->GetKeyState(KEY_INPUT_TAB) == eInputState::Pressed)
	{
		ProjectConfig::DEBUG = !ProjectConfig::DEBUG;
	}

	// シーンの更新
	eSceneType next_scene_type = current_scene->Update(delta_second);

	// オブジェクトマネージャーの情報取得
	GameObjectManager* object = GameObjectManager::GetInstance();
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

	// 攻撃判定確認処理
	for (GameObject* target : objects_list)
	{
		// 攻撃性がないオブジェクトの場合攻撃判定確認処理はしない
		if (target->GetAggressive() == false)
		{
			continue;
		}
		else
		{
			// 範囲攻撃
			if (target->GetAoE() == true)
			{
				// 攻撃対象がいるか
				bool is_hit = false;

				for (GameObject* partner : objects_list)
				{
					if (target == partner)
					{
						continue;
					}

					// 当たり判定情報を取得
					Collision tc = target->GetCollision();
					Collision pc = partner->GetCollision();

					// 攻撃判定が有効か
					if (tc.IsCheckHitTarget(pc.object_type))
					{
						// 攻撃判定確認処理
						if (CheckHitBox(target, partner) == true)
						{
							is_hit = true;
						}
					}
				}

				if (is_hit == false)
				{
					// 攻撃対象がいないことを通知する
					target->NoHit();
				}
			}
			// 単体攻撃
			else
			{
				// 最短距離をステージサイズで初期化
				float shortest_distance = (float)ProjectConfig::STAGE_WIDTH;
				// 攻撃対象
				GameObject* partner = nullptr;

				// 候補から攻撃対象を確定する
				for (GameObject* potential_partner : objects_list)
				{
					// 自分同士の当たり判定確認処理はしない
					if (target == potential_partner)
					{
						continue;
					}

					// 当たり判定情報を取得
					Collision tc = target->GetCollision();
					Collision pc = potential_partner->GetCollision();

					// 攻撃判定が有効か
					if (tc.IsCheckHitTarget(pc.object_type))
					{
						// ユニット同士の距離を計算
						float distance = fabsf(target->GetLocation().x - potential_partner->GetLocation().x);

						// 最長距離を更新
						if (distance < shortest_distance)
						{
							shortest_distance = distance;
							partner = potential_partner;
						}
					}
					else
					{
						// 判定が有効でなければ無視する
						continue;
					}
				}

				// 攻撃判定確認処理
				if (CheckHitBox(target, partner) == false)
				{
					// 攻撃対象がいないことを通知する
					target->NoHit();
				}
			}
		}
	}

	// 明暗判定確認処理
	LightMapManager* light = LightMapManager::GetInstance();
	std::vector<GameObject*> light_list = light->GetLightsList();
	bool in_light = false;	// ライトフラグ

	for (int i = 0; i < objects_list.size(); i++)
	{
		// 動かないオブジェクトの場合当たり判定確認処理をしない
		if (objects_list[i]->GetMobility() == false)
		{
			continue;
		}
		else
		{
			// ライトリストの中身がなかったら
			if (light_list.empty())
			{
				// 当たっていないことを通知する
				objects_list[i]->OutLightRange();
			}
			else
			{
				for (int j = 0; j < light_list.size(); j++)
				{
					// 明暗判定確認処理
					if (CheckLightRange(objects_list[i], light_list[j]) == true)
					{
						in_light = true;
						break;
					}
					else
					{
						in_light = false;
					}
				}

				// ライトに当たっていないことを通知する
				if (in_light != true)
				{
					objects_list[i]->OutLightRange();
				}
			}
		}
	}
	
	//削除予定
	////オブジェクト削除判定処理
	//for (int i = 0; i < objects_list.size(); i++)
	//{
	//	if (objects_list[i]->GetHP() <= 0)
	//	{
	//		object->DestroyObject(objects_list[i]);
	//	}
	//}

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

	// 描画範囲の初期化
	SetDrawArea(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y);

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

	/*--------------------別関数を作ってほしい--------------------*/
	// リザルトへの値渡し
	if (next_scene != nullptr && current_scene != nullptr)
	{
		next_scene->win_flg = current_scene->win_flg;
		next_scene->StageNumber = current_scene->StageNumber;
		if (Result* res = dynamic_cast<Result*>(next_scene))
		{
			if (current_scene->max_unit != -1)
			{
				next_scene->make_unit = current_scene->make_unit;
				next_scene->dead_unit = current_scene->dead_unit;
				next_scene->max_unit = current_scene->max_unit;
				next_scene->make_enemy = current_scene->make_enemy;
				next_scene->dead_enemy = current_scene->dead_enemy;
				next_scene->max_enemy = current_scene->max_enemy;
			}
		}
	}
	/*---------------------------ここまで---------------------------*/

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

	// 通り抜けられないオブジェクト同士は当たり判定確認処理はしない
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
		Vector2D box_size = (tc.collision_size + pc.collision_size) / 2.0f;
		
		// 矩形同士の当たり判定
		if ((fabsf(diff.x) < box_size.x) && (fabsf(diff.y) < box_size.y))
		{
			// 当たっていることを通知する
			target->OnHitCollision(partner);
			partner->OnHitCollision(target);
		}
	}
}

// 攻撃判定確認処理
bool SceneManager::CheckHitBox(GameObject* target, GameObject* partner)
{
	// ヌルポチェック
	if (target == nullptr || partner == nullptr)
	{
		return false;
	}

	// 当たり判定情報を取得
	Collision tc = target->GetCollision();
	Collision pc = partner->GetCollision();

	//２つのオブジェクトの距離を取得
	Vector2D diff = target->GetLocation() - partner->GetLocation();

	//２つのオブジェクトの当たり判定の大きさを取得
	Vector2D box_size;
	box_size.x = tc.hitbox_size.x;
	box_size.y = (tc.hitbox_size.y + pc.collision_size.y) / 2.0f;

	// 矩形同士の当たり判定
	if ((fabsf(diff.x) <= box_size.x) && (fabsf(diff.y) <= box_size.y))
	{
		// 当たっていることを通知する
		target->OnAreaDetection(partner);
		return true;
	}
	else
	{
		// 当たっていない
		return false;
	}
}

// 明暗検知処理
bool SceneManager::CheckLightRange(GameObject* target, GameObject* partner)
{
	// ヌルポチェック
	if (partner == nullptr || target == nullptr)
	{
		return false;
	}

	// 当たり判定情報を取得
	Collision tc = target->GetCollision();
	Collision pc = partner->GetCollision();

	Vector2D circlePos = partner->GetLocation();
	float radius = pc.light_size * 100;
	Vector2D boxPos = target->GetLocation();
	Vector2D boxSize = pc.collision_size;

	if (tc.CheckCircleRectCollision(circlePos, radius, boxPos))
	{
		// 当たっていることを通知する
		target->InLightRange();
		return true;
	}
	else
	{
		return false;
	}
}
