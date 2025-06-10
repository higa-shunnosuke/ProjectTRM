#include "InGame.h"
#include "DxLib.h"
#include <fstream>

#include "../../Utility/Camera/Camera.h"

#include "../../Objects/Block/Ground.h"
#include "../../Objects/Character/Player/Melee/P_Melee.h"
#include "../../Objects/Character/Player/Ranged/P_Ranged.h"
#include "../../Objects/Character/Player/Tank/P_Tank.h"
#include "../../Objects/Character/Player/Bonfire/Bonfire.h"

#include "../../Objects/Character/Enemy/Melee/E_Melee.h"
#include "../../Objects/Character/Enemy/Tank/E_Tank.h"
#include "../../Objects/Character/Enemy/Ranged/E_Ranged.h"
#include "../../Objects/Character/Enemy/Boss/Boss.h"

// コンストラクタ
InGame::InGame():
	cost(),
	cooldown(),
	summon_flag(),
	player(nullptr),
	enemy(nullptr),
	unit_ui(),
	Sun_Level(1),
	BackGroundImage(),
	Click(),
	ClickUp(),
	Cost_Max(),
	SunImages(),
	move_camera(),
	sound(),
	bgmHandle()
{
	
}

// デストラクタ
InGame::~InGame()
{
	// 解放忘れ防止
	Finalize();
}

// 初期化処理
void InGame::Initialize()
{
	// 親クラスの初期化処理を呼び出す
	__super::Initialize();

	// 画像の読み込み
	ResourceManager* rm = ResourceManager::GetInstance();
	// コストUI
	unit_ui[0] = rm->GetImages("Resource/Images/Unit/Tank/Tank_Cost.png")[0];
	unit_ui[1] = rm->GetImages("Resource/Images/Unit/Melee/Melee_Cost.png")[0];
	unit_ui[2] = rm->GetImages("Resource/Images/Unit/Ranged/Ranged_Cost.png")[0];
	unit_ui[3] = rm->GetImages("Resource/Images/BackGround/Sun.png")[0];
	// 背景
	BackGroundImage[0] = rm->GetImages("Resource/Images/BackGround/BlueMoon.png")[0];
	BackGroundImage[1] = rm->GetImages("Resource/Images/BackGround/YelloMoon.png")[0];
	BackGroundImage[2] = rm->GetImages("Resource/Images/BackGround/RedMoon.png")[0];
	//太陽
	std::string Imagestring, Imagepng, Number;;
	Imagestring = "Resource/Images/BackGround/Sun";
	Imagepng = ".png";

	for (int i = 0; i < 10; i++)
	{
		Number = std::to_string(i);
		SunImages[i] = rm->GetImages(Imagestring + Number + Imagepng)[0];
	}

	// 音源の読み込み
	// 決定
	Click = rm->GetSounds("Resource/Sounds/InGame/Click.mp3");
	ClickUp = rm->GetSounds("Resource/Sounds/InGame/ClickUp.mp3");
	// BGM
	bgmHandle[0] = rm->GetSounds("Resource/Sounds/Ingame/BGM/Stage1.mp3");
	bgmHandle[1] = rm->GetSounds("Resource/Sounds/Ingame/BGM/Stage2.mp3");
	bgmHandle[2] = rm->GetSounds("Resource/Sounds/Ingame/BGM/Stage3.mp3");
	// 勝利
	bgmHandle[3] = rm->GetSounds("Resource/Sounds/Result/Win_BGM.mp3");

	// ライトマップの初期化
	LightMapManager* light_map = LightMapManager::GetInstance();
	light_map->Initialize();

	// カメラ座標の初期化
	Camera* camera = Camera::GetInstance();
	camera->Initialize();

	// ステージ読み込み
	//LoadStage();

	// 画像の読み込み
	LoadImages();

	// オブジェクトマネージャーのポインタ
	GameObjectManager* object = GameObjectManager::GetInstance();

	// 巫女生成
	player = object->CreateObject<Oracle>(Vector2D(STAGE_WIDTH - 200, 630));
	player->SetInGamePoint(this);
	// 異端者生成
	enemy = object->CreateObject<Heretic>(Vector2D(200, 630));
	enemy->SetInGamePoint(this);
	// 篝火生成
	for (int i = 1; i < 3; i++)
	{
		object->CreateObject<Bonfire>(Vector2D(player->GetLocation().x - (250 * i), 630));
	}

	// BGM再生
	switch (StageNumber)
	{
	case 1:
		// 音量設定
		ChangeVolumeSoundMem(100, bgmHandle[0]);
		// BGM再生
		if (PlaySoundMem(bgmHandle[0], DX_PLAYTYPE_LOOP) == -1)
		{
			MessageBoxA(NULL, "BGM1の再生に失敗しました", "エラー", MB_OK);
		}
			break;
	case 2:
		// 音量設定
		ChangeVolumeSoundMem(100, bgmHandle[1]);
		// BGM再生
		if (PlaySoundMem(bgmHandle[1], DX_PLAYTYPE_LOOP) == -1)
		{
			MessageBoxA(NULL, "BGM2の再生に失敗しました", "エラー", MB_OK);
		}
			break;
	default:
		// 音量設定
		ChangeVolumeSoundMem(100, bgmHandle[2]);
		// BGM再生
		if (PlaySoundMem(bgmHandle[2], DX_PLAYTYPE_LOOP) == -1)
		{
			MessageBoxA(NULL, "BGM3の再生に失敗しました", "エラー", MB_OK);
		}
		break;
	}

	// カーソルの初期化
	cursor = 0;

	// コストの初期化 
	cost = 0;
	prev_time = std::chrono::steady_clock::now();

}

// 更新処理
eSceneType InGame::Update(const float& delta_second)
{
	int Pcount_sum = 0;
	int Ecount_sum = 0;

	Pcount_sum = (int)P_Tank::GetCount() + (int)P_Melee::GetCount() + (int)P_Ranged::GetCount() ;
	Ecount_sum = (int)E_Tank::GetCount() + (int)E_Melee::GetCount() + (int)E_Ranged::GetCount()	;

	if (max_unit < Pcount_sum)
	{
		make_unit +=  Pcount_sum - max_unit ;
	}

	if(Pcount_sum < max_unit)
	{
		dead_unit -= max_unit - Pcount_sum;
	}
	max_unit = Pcount_sum;

	if (Ecount_sum < max_enemy)
	{
		dead_enemy = max_enemy - Ecount_sum;
	}
	max_enemy = Ecount_sum;

	if (CheckHitKey(KEY_INPUT_Z))
	{
		IsPlayerWin(false);
		return eSceneType::result;
	}

	if (enemy->GetHP() <= 0 || player->GetHP() <= 0)
	{
		if (player->GetHP() <= 0)
		{
			IsPlayerWin(false);
			return eSceneType::result;
		}
		else
		{
			if (!CheckSoundMem(bgmHandle[3]))
			{
				ChangeVolumeSoundMem(100, bgmHandle[3]);
				PlaySoundMem(bgmHandle[3], DX_PLAYTYPE_BACK);
			}
			IsPlayerWin(true);
			state = GameState::BOSS_DEAD;
		}
	}

	// 入力情報を取得
	InputManager* input = InputManager::GetInstance();

	//カメラの情報を取得
	Camera* camera = Camera::GetInstance();

	//カメラの更新
	camera->Update();

	switch (state)
	{
	case GameState::PLAYING:

		move_camera = 0.0f;

		// リザルトシーンに遷移する
		if (input->GetKeyState(KEY_INPUT_RETURN) == eInputState::Pressed)
		{
			return eSceneType::result;
		}

		// ユニット選択処理
		UnitSelection();

		// コスト管理処理
		RegenerateCost();

		if (old_camerapos.x != camera->GetCameraPos().x)
		{
			move_camera = old_camerapos.x - camera->GetCameraPos().x;
			old_camerapos = camera->GetCameraPos();

		}

		// 親クラスの更新処理を呼び出す
		return __super::Update(delta_second);
		break;
	case GameState::BOSS_DEAD:

		camera->SetCameraPos(Vector2D(0, 0));
		if (enemy->GetDead())
		{
			return eSceneType::result;
		}

		__super::Update(delta_second);
		break;
	case GameState::CLEAR:
		break;
	default:
		break;
	}
}

// 描画処理
void InGame::Draw() const
{
	// カメラの情報取得
	Camera* camera = Camera::GetInstance();
	float ShowBackGround_Y = 0;
	switch (StageNumber)
	{
	case 1:
		ShowBackGround_Y = -120.0f;
		break;
	case 2:
		ShowBackGround_Y = -100.0f;
		break;
	case 3:
		 ShowBackGround_Y = -125.0f;
		break;
	default:
		break;
	}

	switch (state)
	{
	case GameState::PLAYING:
	{
		DrawGraph(camera->GetCameraPos().x - 700.0f, ShowBackGround_Y, BackGroundImage[StageNumber - 1], 0);

		LightMapManager* light_map = LightMapManager::GetInstance();

		// 光を加算合成
		light_map->DrawLights(camera->GetCameraPos());

		// ライトマップを描画
		light_map->DrawLightMap();

		// 親クラスの描画処理を呼び出す
		__super::Draw();

		// ボタンサイズ
		const int button_width = 200;
		const int button_height = 200;

		// ボタンの数
		const int button_count = 4;

		// ボタンの総合幅を計算
		int total_buttons_width = button_count * button_width;
		// 間隔の総合幅を計算
		int total_spacing = D_WIN_MAX_X - total_buttons_width;
		// ボタンの間隔の計算
		int spacing = total_spacing / (button_count + 1);

		for (int i = 0; i < button_count; ++i)
		{
			//	選択肢の位置
			int x = spacing + i * (button_width + spacing);
			int y = 60;

			// 拡大用変数
			int w = button_width;
			int h = button_height;

			if (i == cursor)
			{
				// 選択中のものだけ拡大（1.2倍）
				int w = (int)(button_width * 1.2);
				int h = (int)(button_height * 1.2);

				// 枠（背景）を描画
				DrawBox(x - (w - button_width) / 2, y - (h - button_height) / 2, x + w, y + h, GetColor(255, 255, 255), TRUE);
				//キャラの描画範囲を制限
				SetDrawArea(x - (w - button_width) / 2, y - (h - button_height) / 2, x + w, y + h);

				if (i == 3)
				{
					if (cost < Sun_Level * 100)
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);

					for (int i = 0; i < 10; i++)
					{
						if (Sun_Level - 1 >= i)
						{
							// キャラ画像を中心に描画
							DrawExtendGraph(
								(int)(x + (button_width - w * 1.5) / 2), (int)(y + (button_height - h * 1.5) / 2),
								(int)(x + (button_width + w * 1.7) / 2), (int)(y + (button_height + h * 1.7) / 2),
								SunImages[i], TRUE);
						}
						else
						{
							break;
						}
					}

					SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

				}
				else
				{
					// キャラ画像を中心に描画
					DrawExtendGraph(
						(int)(x + (button_width - w * 1.5) / 2), (int)(y + (button_height - h * 1.5) / 2),
						(int)(x + (button_width + w * 1.7) / 2), (int)(y + (button_height + h * 1.7) / 2),
						unit_ui[i], TRUE);
				}
			}
			else
			{

				if (i == 3)
				{
					if (cost < Sun_Level * 100 && Sun_Level < 9)
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);

					// 枠（背景）を描画
					DrawBox(x, y, x + button_width, y + button_height, GetColor(100, 100, 100), TRUE);
					//キャラの描画範囲を制限
					SetDrawArea(x, y, x + button_width, y + button_height);


					for (int i = 0; i < 10; i++)
					{
						if (Sun_Level - 1 >= i)
						{
							// キャラ画像を中心に描画
							DrawExtendGraph(
								(int)(x + (button_width - w * 1.5) / 2), (int)(y + (button_height - h * 1.5) / 2),
								(int)(x + (button_width + w * 1.7) / 2), (int)(y + (button_height + h * 1.7) / 2),
								SunImages[i], TRUE);

						}
						else
						{
							break;
						}
					}
		

					SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

				}
				else
				{

					// 枠（背景）を描画
					DrawBox(x, y, x + button_width, y + button_height, GetColor(100, 100, 100), TRUE);
					//キャラの描画範囲を制限
					SetDrawArea(x, y, x + button_width, y + button_height);

					// キャラ画像を中心に描画
					DrawExtendGraph(
						(int)(x + (button_width - w * 1.5) / 2), (int)(y + (button_height - h * 1.5) / 2),
						(int)(x + (button_width + w * 1.7) / 2), (int)(y + (button_height + h * 1.7) / 2),
						unit_ui[i], TRUE);
				}
			}

			// 描画範囲を元に戻す
			SetDrawArea(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y);
		}

		DrawFormatString(1000, 30, 0x00ffff, "Level:%d/10", Sun_Level);
		// コスト表示
		if (cost < Sun_Level * 100)
		{
			DrawFormatString(1100, 0, 0xffffff, "%d/%d", cost, Sun_Level * 100);
		}
		else
		{
			DrawFormatString(1100, 0, 0xffff00, "%d/%d", cost, Sun_Level * 100);
		}


		if (ProjectConfig::DEBUG)
		{
			// カメラ座標描画
			DrawFormatString(500, 300, 0xffffff, "%f", camera->GetCameraPos().x);

			// シーン情報の描画
			SetFontSize(60);
			DrawFormatString(0, 0, 0xffffff, "InGame");
			SetFontSize(32);
			DrawFormatString(100, 300, 0xffffff, "Spaceを押してください");
			DrawFormatString(1000, 0, 0xffffff, "Stage:%d", StageNumber);
		}

	}
	break;
	case GameState::BOSS_DEAD:
	{

		DrawGraph(enemy->GetLocation().x - 100.0f, ShowBackGround_Y, BackGroundImage[StageNumber - 1], 0);

		LightMapManager* light_map = LightMapManager::GetInstance();

		// 光を加算合成
		light_map->DrawLights(camera->GetCameraPos());

		// 親クラスの描画処理を呼び出す
		__super::Draw();

		// ライトマップを描画
		light_map->DrawLightMap();

		break;
	}
	}
	Vector2D P_position = player->GetLocation();
	P_position.x -= camera->GetCameraPos().x - D_WIN_MAX_X / 2;
	Vector2D E_position = enemy->GetLocation();
	E_position.x -= camera->GetCameraPos().x - D_WIN_MAX_X / 2;
	// 巫女HP表示
	DrawBoxAA(P_position.x - 50.0f, P_position.y - 150.0f, P_position.x + (50.0f - (100 - player->GetHP())), P_position.y - 135.0f, 0xFFFFFF, true);
	// 異端者HP表示
	DrawBoxAA(E_position.x - 50.0f, E_position.y - 150.0f, E_position.x + (50.0f - (100 - ((double)enemy->GetHP() / 500) * 100)), E_position.y - 135.0f, 0xFFFFFF, true);

}

// 終了処理
void InGame::Finalize()
{
	if (CheckSoundMem(bgmHandle[StageNumber - 1]) == 1)
	{
		StopSoundMem(bgmHandle[StageNumber - 1]);
	}
	
	// 親クラスの終了時処理を呼び出す
	__super::Finalize();


}

// 現在のシーンタイプ情報を取得する
const eSceneType InGame::GetNowSceneType() const
{
	return eSceneType::in_game;
}


// 敵生成処理
void InGame::CreateEnemy(E_enemy e_enem)
{
	// オブジェクトマネージャーの情報を取得
	GameObjectManager* object = GameObjectManager::GetInstance();

	switch (e_enem)
	{
	case Tank:
		object->CreateObject<E_Tank>(Vector2D(enemy->GetLocation().x, enemy->GetLocation().y + 30.0f));
		break;
	case Melee:
		object->CreateObject<E_Melee>(Vector2D(enemy->GetLocation().x, enemy->GetLocation().y + 30.0f));
		break;
	case Range:
		object->CreateObject<E_Ranged>(Vector2D(enemy->GetLocation().x, enemy->GetLocation().y + 30.0f));
		break;
	case Boss:
		object->CreateObject<class Boss>(Vector2D(enemy->GetLocation().x, enemy->GetLocation().y - 30.0f));
		break;
	default:
		break;
	}
}

const int InGame::GetSunLevel() const
{
	return Sun_Level;
}

//	ユニット召喚
void InGame::UnitSelection()
{
	// 入力情報を取得
	InputManager* input = InputManager::GetInstance();

	// カーソル操作
	if (input->GetKeyState(KEY_INPUT_RIGHT) == eInputState::Pressed ||
		input->GetButtonState(XINPUT_BUTTON_DPAD_RIGHT) == eInputState::Pressed)
	{
		if (cursor < 3)
		{
			cursor++;
		}
	}
	if (input->GetKeyState(KEY_INPUT_LEFT) == eInputState::Pressed ||
		input->GetButtonState(XINPUT_BUTTON_DPAD_LEFT) == eInputState::Pressed)
	{
		if (cursor > 0)
		{
			cursor--;
		}
	}

	// 決定
	if (input->GetKeyState(KEY_INPUT_SPACE) == eInputState::Pressed ||
		input->GetButtonState(XINPUT_BUTTON_A) == eInputState::Pressed)
	{
		// オブジェクトマネージャーのポインタ
		GameObjectManager* object = GameObjectManager::GetInstance();

		switch (cursor)
		{
			// タンク召喚
		case 0:
			if (summon_flag[cursor] == false)
			{
				if (cost - Tank_Cost >= 0)
				{
					// タンクを生成
					GameObject* obj = object->CreateObject<P_Tank>(Vector2D(player->GetLocation().x, player->GetLocation().y + 30.0f));
					obj->SetInGamePoint(this);
					cost -= Tank_Cost;

					//summon_flag[cursor] = true;
					summon_time[cursor] = std::chrono::steady_clock::now();
				}
			}
			break;
			// 近接召喚
		case 1:
			if (summon_flag[cursor] == false)
			{
				if (cost - Melee_Cost >= 0)
				{
					// 近接を生成
					GameObject* obj = object->CreateObject<P_Melee>(Vector2D(player->GetLocation().x, player->GetLocation().y + 30.0f));
					obj->SetInGamePoint(this);
					cost -= Melee_Cost;
					//summon_flag[cursor] = true;
					summon_time[cursor] = std::chrono::steady_clock::now();
				}
			}
			break;
			// 遠距離召喚
		case 2:
			if (summon_flag[cursor] == false)
			{
				if (cost - Range_Cost >= 0)
				{
					object->CreateObject<P_Ranged>(Vector2D(player->GetLocation().x, player->GetLocation().y + 30.0f))->SetInGamePoint(this);
					cost -= Range_Cost;
					//summon_flag[cursor] = true;
					summon_time[cursor] = std::chrono::steady_clock::now();
				}
			}
			break;
		case 3:
			if (Sun_Level != 10)
			{
				if (cost >= Sun_Level * 100)
				{
					cost = 0;
					Sun_Level++;
					PlaySoundMem(ClickUp, DX_PLAYTYPE_BACK);
				}
			}
			break;
		}
	}
}

//	コスト管理処理
void InGame::RegenerateCost()
{
	auto now_time = std::chrono::steady_clock::now();

	if (cost < Sun_Level * 100)
	{
		if (now_time - prev_time > std::chrono::milliseconds(500))
		{
			cost += Cost_UpNum + ((Sun_Level) * 5);

			if (cost >= Sun_Level * 100)
			{
				PlaySoundMem(Click, DX_PLAYTYPE_BACK);
				cost = Sun_Level * 100;
			}
			prev_time = std::chrono::steady_clock::now();
		}
	}
	else
	{

		prev_time = std::chrono::steady_clock::now();
	}
}

const void InGame::LoadImages() const
{
	//リソースマネージャーのインスタンス取得
	ResourceManager* rm = ResourceManager::GetInstance();

	//画像読込み
	// 単品
	rm->GetImages("Resource/Images/Unit/Ranged/Arrow.png", 1, 1, 1, 32, 32);

	//歩行アニメーション
	rm->GetImages("Resource/Images/Unit/Melee/Melee_Walk.png", 4, 4, 1, 32, 32);
	rm->GetImages("Resource/Images/Unit/Tank/Tank_Walk.png", 4, 4, 1, 32, 32);
	rm->GetImages("Resource/Images/Unit/Ranged/Ranged_Walk.png", 4, 4, 1, 32, 32);
	rm->GetImages("Resource/Images/Unit/Guardian/Guardian_Walk.png", 3, 3, 1, 1024, 1024);
	rm->GetImages("Resource/Images/Enemy/Melee/Melee_Walk.png", 4, 4, 1, 32, 32);
	rm->GetImages("Resource/Images/Enemy/Tank/Tank_Walk.png", 4, 4, 1, 32, 32);
	rm->GetImages("Resource/Images/Enemy/Ranged/Ranged_Walk.png", 4, 4, 1, 32, 32);

	//攻撃アニメーション
	rm->GetImages("Resource/Images/Unit/Melee/Melee_Attack.png", 4, 4, 1, 32, 32);
	rm->GetImages("Resource/Images/Unit/Tank/Tank_Attack.png", 4, 4, 1, 32, 32);
	rm->GetImages("Resource/Images/Unit/Ranged/Ranged_Attack.png", 4, 4, 1, 32, 32);
	rm->GetImages("Resource/Images/Unit/Guardian/Guardian_Attack.png", 3, 3, 1, 1024, 1024);
	rm->GetImages("Resource/Images/Enemy/Melee/Melee_Attack.png", 4, 4, 1, 32, 32);
	rm->GetImages("Resource/Images/Enemy/Tank/Tank_Attack.png", 4, 4, 1, 32, 32);
	rm->GetImages("Resource/Images/Enemy/Ranged/Ranged_Attack.png", 4, 4, 1, 32, 32);

	//死亡アニメーション
	rm->GetImages("Resource/Images/Unit/Melee/Melee_Down.png", 3, 3, 1, 32, 32);
	rm->GetImages("Resource/Images/Unit/Tank/Tank_Down.png", 3, 3, 1, 32, 32);
	rm->GetImages("Resource/Images/Unit/Ranged/Ranged_Down.png", 3, 3, 1, 32, 32);
	rm->GetImages("Resource/Images/Unit/Guardian/Guardian_Down.png", 3, 3, 1, 1024, 1024);

}