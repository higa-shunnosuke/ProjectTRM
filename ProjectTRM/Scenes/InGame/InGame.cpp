#include "InGame.h"
#include "DxLib.h"
#include <fstream>

#include "Pause.h"
#include "../../Utility/Camera/Camera.h"

#include "../../Objects/Character/Player/Melee/P_Melee.h"
#include "../../Objects/Character/Player/Ranged/P_Ranged.h"
#include "../../Objects/Character/Player/Tank/P_Tank.h"
#include "../../Objects/Character/Player/Bonfire/Bonfire.h"
#include "../../Objects/Character/Player/Guardian/P_Guardian.h"

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
	sound(),
	bgmHandle(),
	SummonSE(),
	Text_Images(),
	Text_BackGround(0),
	alpha(0),
	digit(0),
	cost_ui(),
	back_buffer(),
	move_flame(0.0f),
	now_zoom(0.0f)
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
	unit_ui[0] = rm->GetImages("Resource/Images/Unit/Tank/Tank_Cost_New.png")[0];
	unit_ui[1] = rm->GetImages("Resource/Images/Unit/Melee/Melee_Cost_New.png")[0];
	unit_ui[2] = rm->GetImages("Resource/Images/Unit/Ranged/Ranged_Cost.png")[0];
	unit_ui[3] = rm->GetImages("Resource/Images/Unit/Guardian/Guardian_Cost.png")[0];
	unit_ui[4] = rm->GetImages("Resource/Images/BackGround/Sun.png")[0];
	// 背景
	BackGroundImage[0] = rm->GetImages("Resource/Images/BackGround/BlueMoon.png")[0];
	BackGroundImage[1] = rm->GetImages("Resource/Images/BackGround/YelloMoon.png")[0];
	BackGroundImage[2] = rm->GetImages("Resource/Images/BackGround/RedMoon.png")[0];
	//文字
	Text_Images[0] = rm->GetImages("Resource/Images/BackGround/text_fail.png")[0];
	Text_Images[1] = rm->GetImages("Resource/Images/BackGround/text_clear.png")[0];
	Text_Images[2] = rm->GetImages("Resource/Images/BackGround/Level.png")[0];
	Text_Images[3] = rm->GetImages("Resource/Images/BackGround/text_max.png")[0];

	Text_BackGround = rm->GetImages("Resource/Images/BackGround/Text.png")[0];
	LText_BackGround = rm->GetImages("Resource/Images/BackGround/TextLeft.png")[0];

	//コスト、レベル描画用
	numbers = rm->GetImages("Resource/Images/BackGround/numbers_gray.png", 10, 5, 2, 32, 32);
	level_num = rm->GetImages("Resource/Images/BackGround/numbers_orange.png", 10, 5, 2, 32, 32);
	numbers.push_back(rm->GetImages("Resource/Images/BackGround/Cost_Over_Gray.png")[0]);

	cost_ui = rm->GetImages("Resource/Images/BackGround/Cost_Out.png")[0];


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
	//敗北
	bgmHandle[4] = rm->GetSounds("Resource/Sounds/Result/Loose_BGM.mp3");
	// 召喚
	SummonSE[0] = rm->GetSounds("Resource/Sounds/Ingame/SummonAllies.mp3");
	SummonSE[1] = rm->GetSounds("Resource/Sounds/Ingame/SummonEnemy.mp3");
	for (int i = 0; i < 2; i++)
	{
		ChangeVolumeSoundMem(100, SummonSE[i]);
	}

	// ライトマップの初期化
	LightMapManager* light_map = LightMapManager::GetInstance();
	light_map->Initialize();

	//// 画像の読み込み
	//LoadImages();

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
		// ステージサイズ設定
		ProjectConfig::STAGE_WIDTH = 1280;
		ProjectConfig::STAGE_HEIGHT = 720;
		break;
	case 2:
		// 音量設定
		ChangeVolumeSoundMem(100, bgmHandle[1]);
		// BGM再生
		if (PlaySoundMem(bgmHandle[1], DX_PLAYTYPE_LOOP) == -1)
		{
			MessageBoxA(NULL, "BGM2の再生に失敗しました", "エラー", MB_OK);
		}
		// ステージサイズ設定
		ProjectConfig::STAGE_WIDTH = 1280;
		ProjectConfig::STAGE_HEIGHT = 720;
		break;
	default:
		// 音量設定
		ChangeVolumeSoundMem(100, bgmHandle[2]);
		// BGM再生
		if (PlaySoundMem(bgmHandle[2], DX_PLAYTYPE_LOOP) == -1)
		{
			MessageBoxA(NULL, "BGM3の再生に失敗しました", "エラー", MB_OK);
		}
		// ステージサイズ設定
		ProjectConfig::STAGE_WIDTH = 1280;
		ProjectConfig::STAGE_HEIGHT = 720;
		break;
	}

	// オブジェクトマネージャーのポインタ
	GameObjectManager* object = GameObjectManager::GetInstance();
	// カメラのポインタ
	Camera* camera = Camera::GetInstance();

	// 巫女生成
	player = object->CreateObject<Oracle>(Vector2D(ProjectConfig::STAGE_WIDTH - 200, 630));
	player->SetInGamePoint(this);
	// 異端者生成
	enemy = object->CreateObject<Heretic>(Vector2D(200, 630));
	enemy->SetInGamePoint(this);
	// 篝火生成
	for (int i = 1; i < 3; i++)
	{
		object->CreateObject<Bonfire>(Vector2D(player->GetLocation().x - (250 * i), 630));
	}
	// カメラ生成
	camera->Initialize();
	camera->SetPlayer(player);

	//ステート設定
	state = GameState::GAMESTART;

	// カーソルの初期化
	cursor = 0;
	now_zoom = 2.0f;

	// コストの初期化 
	cost = 0;
	prev_time = std::chrono::steady_clock::now();

	draw_number.push_back(0);
	digit = 1;

	back_buffer = MakeScreen(1280, 720, TRUE);
}

// 更新処理
eSceneType InGame::Update(const float& delta_second)
{
	// 入力情報を取得
	InputManager* input = InputManager::GetInstance();
	// ポーズクラスのポインタ
	Pause* pause = Pause::GetInstance();

	// ポーズの切り替え
	if (input->GetButtonState(XINPUT_BUTTON_START) == eInputState::Pressed)
	{
		pause->SetPause(!pause->GetPause());
	}

	if (pause->GetPause())
	{
		return GetNowSceneType();
	}
	else
	{

		// ユニットとエネミーのカウント処理
		int Pcount_sum = 0;
		int Ecount_sum = 0;

		Pcount_sum = (int)P_Tank::GetCount() + (int)P_Melee::GetCount() + (int)P_Ranged::GetCount() + (int)P_Guardian::GetCount();
		Ecount_sum = (int)E_Tank::GetCount() + (int)E_Melee::GetCount() + (int)E_Ranged::GetCount();

		if (max_unit < Pcount_sum)
		{
			make_unit += Pcount_sum - max_unit;
		}

		if (Pcount_sum < max_unit)
		{
			dead_unit -= max_unit - Pcount_sum;
		}
		max_unit = Pcount_sum;

		if (Ecount_sum < max_enemy)
		{
			dead_enemy = max_enemy - Ecount_sum;
		}
		max_enemy = Ecount_sum;

		// Zキーでプレイヤーの勝利（デバック用）
		if (CheckHitKey(KEY_INPUT_Z))
		{
			IsPlayerWin(false);
			return eSceneType::result;
		}

		//カメラの情報を取得
		Camera* camera = Camera::GetInstance();

		//カメラの更新
		camera->Update();

		// 勝敗判定処理
		if (enemy->GetHP() <= 0 || player->GetHP() <= 0)
		{
			if (player->GetHP() <= 0)
			{
				if (!CheckSoundMem(bgmHandle[4]))
				{
					move_flame = 0.0f;
					now_zoom = camera->GetZoom();
					ChangeVolumeSoundMem(150, bgmHandle[4]);
					PlaySoundMem(bgmHandle[4], DX_PLAYTYPE_BACK);
				}
				IsPlayerWin(false);
				state = GameState::PLAYER_DEAD;
			}
			else
			{
				if (!CheckSoundMem(bgmHandle[3]))
				{
					move_flame = 0.0f;
					now_zoom = camera->GetZoom();
					ChangeVolumeSoundMem(100, bgmHandle[3]);
					PlaySoundMem(bgmHandle[3], DX_PLAYTYPE_BACK);
				}
				IsPlayerWin(true);
				state = GameState::BOSS_DEAD;
			}
			if (alpha < 510)
			{
				alpha++;
			}
		}



		switch (state)
		{
		case GameState::PLAYING:

			// ユニット選択処理
			UnitSelection();

			if (!draw_number.empty())
			{
				draw_number.clear();
				digit = 0;
			}
			// コスト管理処理
			RegenerateCost();

		case GameState::GAMESTART:
			// リザルトシーンに遷移する
			if (input->GetKeyState(KEY_INPUT_RETURN) == eInputState::Pressed)
			{
				return eSceneType::result;
			}
			if (player->GetNowState() == State::Idle)
			{
				state = GameState::PLAYING;
			}
			move_flame += delta_second;
			if (move_flame >= 0.01f)
			{
				if (now_zoom > 1.1f)
				{
					now_zoom -= 0.01f;
				}
				move_flame = 0;
			}
			camera->SetZoom(now_zoom);
			break;
		case GameState::PLAYER_DEAD:
			if (player->GetDead())
			{
				return eSceneType::result;
			}
			move_flame += delta_second;
			if (move_flame >= 0.01f)
			{
				if (now_zoom <= 2.0f)
				{
					now_zoom += 0.1f;
				}
				move_flame = 0;
			}
			camera->SetCameraPos(Vector2D(ProjectConfig::STAGE_WIDTH - (D_WIN_MAX_X / 2), D_WIN_MAX_Y / 2));
			camera->SetZoom(now_zoom);
			break;

		case GameState::BOSS_DEAD:
			// オブジェクトマネージャーのポインタ
			GameObjectManager* object = GameObjectManager::GetInstance();
			// オブジェクトリストを取得
			std::vector<GameObject*> objects_list = object->GetObjectsList();

			// 敵削除
			if (!objects_list.empty())
			{
				for (GameObject* obj : objects_list)
				{
					// エネミー以外は無視する
					if (obj->GetCollision().object_type == eObjectType::Enemy &&
						// 異端者も無視する
						obj != enemy)
					{
						obj->Finalize();
					}
				}
			}
			move_flame += delta_second;
			if (move_flame >= 0.01f)
			{
				if (now_zoom <= 2.0f)
				{
					now_zoom += 0.1f;
				}
				move_flame = 0;
			}
			camera->SetZoom(now_zoom);
			camera->SetCameraPos(enemy->GetLocation() - 50.0f);
			if (enemy->GetDead())
			{
				return eSceneType::result;
			}
			break;
		}

		// 親クラスの更新処理を呼び出す
		return __super::Update(delta_second);
	}
}

// 描画処理
void InGame::Draw() const
{
	// カメラの情報取得
	Camera* camera = Camera::GetInstance();

	// 背景画像のずれ
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
		//---------------
		// 仮想画面に描画
		//---------------
		SetDrawScreen(back_buffer);
		ClearDrawScreen();

		// 背景
		DrawGraph(D_WIN_MAX_X / 2 - 700, ShowBackGround_Y, BackGroundImage[StageNumber - 1], 0);

		LightMapManager* light_map = LightMapManager::GetInstance();

		// 描画画面設定
		light_map->SetBackBuffer(back_buffer);

		// 光を加算合成
		light_map->DrawLights();

		// ライトマップを描画
		light_map->DrawLightMap();

		// オブジェクト
		__super::Draw();

		//---------------
		// 表画面に描画
		//---------------
		SetDrawScreen(DX_SCREEN_BACK);
		ClearDrawScreen();

		// 仮想画面を描画
		camera->Draw(back_buffer);

		// ボタンサイズ
		const int button_width = 200;
		const int button_height = 200;

		// ボタンの数
		const int button_count = 5;

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


				switch (i)
				{
				case 0:
				{
					if (cost < 20)
					{
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
					}

				}
				break;
				case 1:
				{
					if (cost < 50)
					{
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
					}
					break;
				}
				case 2:
				{
					if (cost < 100)
					{
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
					}
					break;
				}
				case 3:
				{
					if (cost < 500)
					{
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
					}
					break;
				}
				case 4:
					if (cost < Sun_Level * 100)
					{
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
					}

				default:
					break;
				}

				// 枠（背景）を描画
				DrawBox(x - (w - button_width) / 2, y - (h - button_height) / 2, x + w, y + h, GetColor(255, 255, 255), TRUE);
				//キャラの描画範囲を制限
				SetDrawArea(x - (w - button_width) / 2, y - (h - button_height) / 2, x + w, y + h);
				
				
				if (i == 4)
				{
					// キャラ画像を中心に描画
					DrawExtendGraph(
						(int)(x + (button_width - w * 1.5) / 2), (int)(y + (button_height - h * 1.5) / 2),
						(int)(x + (button_width + w * 1.7) / 2), (int)(y + (button_height + h * 1.7) / 2),
						unit_ui[i], TRUE);

					SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

					// 描画範囲を元に戻す
					SetDrawArea(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y);
					if (StageNumber == 1)
					{
						DrawExtendGraph(
							(int)x-440, (int)y + h-20,
							(int)x + 280, (int)y + h + 200,
							LText_BackGround, TRUE);

						DrawFormatString(x - 395, y + h + 80, 0x000000, "信仰度 cost:上限(%d)\nコストの上限と回復速度が上がる\nレベルに応じて味方ユニットを強化する", Sun_Level * 100);
					}

				}
				else
				{
					// キャラ画像を中心に描画
					DrawExtendGraph(
						(int)(x + (button_width - w * 1.5) / 2), (int)(y + (button_height - h * 1.5) / 2),
						(int)(x + (button_width + w * 1.7) / 2), (int)(y + (button_height + h * 1.7) / 2),
						unit_ui[i], TRUE);

					SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

					// 描画範囲を元に戻す
					SetDrawArea(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y);


					if (StageNumber == 1)
					{

						switch (i)
						{
							case 0:
							{
								DrawExtendGraph(
									(int)x, (int)y + h,
									(int)x + 380, (int)y + h + 120,
									Text_BackGround, TRUE);
								DrawFormatString(x + 30, y + h + 40, 0x000000, "灯守 cost:20\n唯一灯りを持つ");
							}
							break;
							case 1:
							{
								DrawExtendGraph(
									(int)x - 50, (int)y + h,
									(int)x + 500, (int)y + h + 120,
									Text_BackGround, TRUE);
								DrawFormatString(x - 10, y + h + 40, 0x000000, "兵士 cost:50\n灯りを持たない近接ユニット");
							}
							break;
							case 2:
							{
								DrawExtendGraph(
									(int)x, (int)y + h,
									(int)x + 380, (int)y + h + 120,
									Text_BackGround, TRUE);
								DrawFormatString(x + 30, y + h + 40, 0x000000, "弓兵 cost:100\n遠距離から攻撃する");
							}
							break;
							case 3:
							{
								DrawExtendGraph(
									(int)x - 120, (int)y + h,
									(int)x + 500, (int)y + h + 120,
									Text_BackGround, TRUE);
								DrawFormatString(x - 70, y + h + 40, 0x000000, "騎士 cost:500\n灯りを持たない上級近接ユニット");
							}
							break;

						}
					}
				}
			}
			else
			{

				switch (i)
				{
				case 0:
				{
					if (cost < 20)
					{
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
					}

				}
				break;
				case 1:
				{
					if (cost < 50)
					{
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
					}
					break;
				}
				case 2:
				{
					if (cost < 100)
					{
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
					}
					break;
				}
				case 3:
				{
					if (cost < 500)
					{
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
					}
					break;
				}
				case 4:
					if (cost < Sun_Level * 100)
					{
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
					}

				default:
					break;
				}

				if (i == 4)
				{
					if (cost < Sun_Level * 100 && Sun_Level < 9)
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);

					// 枠（背景）を描画
					DrawBox(x, y, x + button_width, y + button_height, GetColor(100, 100, 100), TRUE);
					//キャラの描画範囲を制限
					SetDrawArea(x, y, x + button_width, y + button_height);


					// キャラ画像を中心に描画
					DrawExtendGraph(
						(int)(x + (button_width - w * 1.5) / 2), (int)(y + (button_height - h * 1.5) / 2),
						(int)(x + (button_width + w * 1.7) / 2), (int)(y + (button_height + h * 1.7) / 2),
						unit_ui[i], TRUE);

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

					SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);



				}
			}

			// 描画範囲を元に戻す
			SetDrawArea(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y);
		}

		//レベル表記
		//太陽を選択していたらレベルの表記が画像とかぶらないように少し上に移動
		if (cursor == 4)
		{
			DrawRotaGraph(1100, 25, 1.0, 0.0, Text_Images[2], TRUE);
			if (Sun_Level == 10)
			{
				DrawRotaGraph(1200, 25, 1.0, 0.0, Text_Images[3], TRUE);
			}
			else
			{
				DrawRotaGraph(1180, 25, 1.0, 0.0, level_num[Sun_Level], TRUE);
			}
		}
		else
		{
			DrawRotaGraph(1100, 35, 1.0, 0.0, Text_Images[2], TRUE);
			if (Sun_Level == 10)
			{
				DrawRotaGraph(1200, 35, 1.0, 0.0, Text_Images[3], TRUE);
			}
			else
			{
				DrawRotaGraph(1180, 35, 1.0, 0.0, level_num[Sun_Level], TRUE);
			}
		}
		// コスト表示

		DrawRotaGraph(1200, 680, 0.15, 0.0, cost_ui, TRUE);

		for (int i = 0; i < draw_number.size(); i++)
		{
			DrawRotaGraph((1200 - (digit * 15)) + (i * 15), 685, 0.7, 0.0, numbers[draw_number[i]], TRUE);
		}
		if (Sun_Level == 10)
		{
			DrawRotaGraph(1230, 685, 0.7, 0.0, numbers[10], TRUE);
		}
		else
		{


			DrawRotaGraph(1215, 685, 0.7, 0.0, numbers[Sun_Level], TRUE);
			for (int i = 0; i < 2; i++)
			{
				DrawRotaGraph(1230 + (i * 15), 685, 0.7, 0.0, numbers[0], TRUE);
			}
		}


		if (ProjectConfig::DEBUG)
		{
			// シーン情報の描画
			SetFontSize(60);
			DrawFormatString(0, 0, 0xffffff, "InGame");
			SetFontSize(32);
			DrawFormatString(100, 300, 0xffffff, "Spaceを押してください");
			DrawFormatString(1000, 0, 0xffffff, "Stage:%d", StageNumber);
		}

	}
	break;
	case GameState::GAMESTART:
		//---------------
		// 仮想画面に描画
		//---------------
		SetDrawScreen(back_buffer);
		ClearDrawScreen();

		DrawGraph(D_WIN_MAX_X / 2 - 700, ShowBackGround_Y, BackGroundImage[StageNumber - 1], 0);
		__super::Draw();

		//---------------
		// 表画面に描画
		//---------------
		SetDrawScreen(DX_SCREEN_BACK);
		ClearDrawScreen();
		// 仮想画面を描画
		camera->Draw(back_buffer);
		break;

	case GameState::PLAYER_DEAD:
		//---------------
		// 仮想画面に描画
		//---------------
		SetDrawScreen(back_buffer);
		ClearDrawScreen();

		DrawGraph(D_WIN_MAX_X / 2 - 700, ShowBackGround_Y, BackGroundImage[StageNumber - 1], 0);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha / 2);
		DrawRotaGraph(player->GetLocation().x - 120.0f, D_WIN_MAX_Y / 2 + 70.0f, 0.5f, 0.0f, Text_Images[0], true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		__super::Draw();

		//---------------
		// 表画面に描画
		//---------------
		SetDrawScreen(DX_SCREEN_BACK);
		ClearDrawScreen();
		// 仮想画面を描画
		camera->Draw(back_buffer);
		break;
	case GameState::BOSS_DEAD:
	{
		//---------------
		// 仮想画面に描画
		//---------------
		SetDrawScreen(back_buffer);
		ClearDrawScreen();

		DrawGraph(enemy->GetLocation().x - 100.0f, ShowBackGround_Y, BackGroundImage[StageNumber - 1], 0);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha / 2);
		DrawRotaGraph(enemy->GetLocation().x + 180.0f, D_WIN_MAX_Y /2 + 70.0f, 0.5f, 0.0f, Text_Images[1], true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		LightMapManager* light_map = LightMapManager::GetInstance();

		// 光を加算合成
		light_map->DrawLights();

		// 親クラスの描画処理を呼び出す
		__super::Draw();

		// ライトマップを描画
		light_map->DrawLightMap();

		//---------------
		// 表画面に描画
		//---------------
		SetDrawScreen(DX_SCREEN_BACK);
		ClearDrawScreen();
		// 仮想画面を描画
		camera->Draw(back_buffer);
		break;
	}
	}

	// ポーズクラスのポインタ
	Pause* pause = Pause::GetInstance();

	if (pause->GetPause())
	{
		pause->Draw();
	}
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

	// 召喚SE再生
	PlaySoundMem(SummonSE[1], DX_PLAYTYPE_BACK);

	switch (e_enem)
	{
	case Tank:
		object->CreateObject<E_Tank>(Vector2D(enemy->GetLocation().x + 50.0f, enemy->GetLocation().y + 30.0f));
		break;
	case Melee:
		object->CreateObject<E_Melee>(Vector2D(enemy->GetLocation().x + 50.0f, enemy->GetLocation().y + 30.0f));
		break;
	case Range:
		object->CreateObject<E_Ranged>(Vector2D(enemy->GetLocation().x + 50.0f, enemy->GetLocation().y + 30.0f));
		break;
	case Boss:
		object->CreateObject<class Boss>(Vector2D(enemy->GetLocation().x + 50.0f, enemy->GetLocation().y + 0.0f));
		break;
	default:
		break;
	}
}

const int InGame::GetSunLevel() const
{
	return Sun_Level;
}

/// <summary>
/// 現在のステート取得処理
/// </summary>
/// <returns></returns>
const GameState InGame::GetNowState() const
{
	return state;
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
		if (cursor < 4)
		{
			cursor++;
		}
		else
		{
			cursor = 0;
		}
	}
	if (input->GetKeyState(KEY_INPUT_LEFT) == eInputState::Pressed ||
		input->GetButtonState(XINPUT_BUTTON_DPAD_LEFT) == eInputState::Pressed)
	{
		if (cursor > 0)
		{
			cursor--;
		}
		else
		{
			cursor = 4;
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
					// 召喚SE再生
					PlaySoundMem(SummonSE[0], DX_PLAYTYPE_BACK);
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
					// 召喚SE再生
					PlaySoundMem(SummonSE[0], DX_PLAYTYPE_BACK);
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
					// 召喚SE再生
					PlaySoundMem(SummonSE[0], DX_PLAYTYPE_BACK);
					object->CreateObject<P_Ranged>(Vector2D(player->GetLocation().x, player->GetLocation().y + 30.0f))->SetInGamePoint(this);
					cost -= Range_Cost;
					//summon_flag[cursor] = true;
					summon_time[cursor] = std::chrono::steady_clock::now();
				}
			}
			break;
		case 3:
			if (summon_flag[cursor] == false)
			{
				if (cost - Guardian_Cost >= 0)
				{
					// 召喚SE再生
					PlaySoundMem(SummonSE[0], DX_PLAYTYPE_BACK);
					// 近接を生成
					GameObject* obj = object->CreateObject<P_Guardian>(Vector2D(player->GetLocation().x, player->GetLocation().y + 30.0f));
					obj->SetInGamePoint(this);
					cost -= Guardian_Cost;
					//summon_flag[cursor] = true;
					summon_time[cursor] = std::chrono::steady_clock::now();
				}
			}
			break;
		case 4:
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

	if (cost < Sun_Level * 100 || Sun_Level == 10)
	{
		if (now_time - prev_time > std::chrono::milliseconds(500))
		{
			cost += Cost_UpNum + ((Sun_Level) * 5);
			if (Sun_Level == 10)
			{
				cost += 200;
			}
			else
			{
				if (cost >= Sun_Level * 100)
				{
					PlaySoundMem(Click, DX_PLAYTYPE_BACK);
					cost = Sun_Level * 100;
				}
			}
			prev_time = std::chrono::steady_clock::now();
		}
	}
	else
	{

		prev_time = std::chrono::steady_clock::now();
	}
	int cost_calculation = cost;
	if (cost == 0)
	{
		draw_number.push_back(0);
		digit++;
	}
	else
	{
		while (cost_calculation > 0)
		{
			draw_number.insert(draw_number.begin(), cost_calculation % 10);
			cost_calculation /= 10;
			digit++;
		}
	}
}