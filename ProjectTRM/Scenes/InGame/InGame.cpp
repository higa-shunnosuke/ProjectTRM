#include "InGame.h"
#include "DxLib.h"
#include <fstream>

#include "../../Utility/StageData.h"
#include "../../Utility/Camera/Camera.h"

#include "../../Objects/Block/Ground.h"
#include "../../Objects/Character/Player/Melee/P_Melee.h"
#include "../../Objects/Character/Player/Ranged/P_Ranged.h"
#include "../../Objects/Character/Player/Tank/P_Tank.h"

#include "../../Objects/Character/Enemy/Melee/E_Melee.h"
#include "../../Objects/Character/Enemy/Tank/E_Tank.h"
#include "../../Objects/Character/Enemy/Ranged/E_Ranged.h"

// コンストラクタ
InGame::InGame():
	cost(),
	cooldown(),
	summon_flag(),
	player(nullptr),
	enemy(nullptr),
	unit_ui(),
	Cost_Click_Count(),
	Cost_value(1)
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
	unit_ui[0]= rm->GetImages("Resource/Images/Unit/Tank/Tank_Walk.png", 4, 4, 1, 32, 32)[0]	;
	unit_ui[1]= rm->GetImages("Resource/Images/Unit/Melee/Melee_Walk.png", 4, 4, 1, 32, 32)[0]	;
	unit_ui[2] = rm->GetImages("Resource/Images/Unit/Ranged/Ranged_Walk.png", 4, 4, 1, 32, 32)[0];
	unit_ui[3] = rm->GetImages("Resource/Images/BackGround/Sun.png")[0];
	BackGroundImage[0] = rm->GetImages("Resource/Images/BackGround/BlueMoon.png")[0];
	BackGroundImage[1] = rm->GetImages("Resource/Images/BackGround/YelloMoon.png")[0];
	BackGroundImage[2]= rm->GetImages("Resource/Images/BackGround/RedMoon.png")[0];

	Click = rm->GetSounds("Resource/Sounds/InGame/Click.mp3");
	ClickUp = rm->GetSounds("Resource/Sounds/InGame/ClickUp.mp3");

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


	int locationy = 750;
	int locationx = 1350;
	for (int count = 0; count < 3; count++)
	{
		for (int locationx = 1350; locationx > 0; locationx -= 60)
		{
			object->CreateObject<Ground>(Vector2D(locationx, locationy));
		}
		locationy -= 60;
	}

	switch (StageNumber)
	{
	case 1:
		player = object->CreateObject<Oracle>(Vector2D(1000, 630));

		enemy = object->CreateObject<Heretic>(Vector2D(100, 630));
		enemy->SetInGamePoint(this);
		break;

	case 2:
		player = object->CreateObject<Oracle>(Vector2D(1170, 630));

		enemy = object->CreateObject<Heretic>(Vector2D(30, 630));
		enemy->SetInGamePoint(this);
		break;
	default:
		player = object->CreateObject<Oracle>(Vector2D(1170, 630));

		enemy = object->CreateObject<Heretic>(Vector2D(30, 630));
		enemy->SetInGamePoint(this);
		break;
	}

	// カーソルの初期化
	cursor = 0;

	// コストの初期化 
	cost = 0;	
	prev_time = std::chrono::steady_clock::now();

	//// クールダウン / 召喚フラグの初期化
	//for (int i = 0; i < 3; i++)
	//{
	//	cooldown[i] = std::chrono::seconds((i+1) * 2);
	//	summon_flag[i] = false;
	//}
}

// 更新処理
eSceneType InGame::Update(const float& delta_second)
{
	move_camera = 0.0f;

	if (enemy->GetHP() <= 0 || player->GetHP() <= 0)
	{
		if(player->GetHP() <= 0)
		{
			IsPlayerWin(false);
		return eSceneType::result;
		}
		else	
		{
			IsPlayerWin(true);
			return eSceneType::result;
		}
	}

	// 入力情報を取得
	InputManager* input = InputManager::GetInstance();

	//カメラの情報を取得
	Camera* camera = Camera::GetInstance();

	//カメラの更新
	camera->Update();

	//　ポイント加算やぁぁぁぁ
	if ((input->GetKeyState(KEY_INPUT_Y) == eInputState::Pressed) || (input->GetButtonState(XINPUT_BUTTON_Y) == eInputState::Pressed))
	{
		cost += 1;
	}

	// リザルトシーンに遷移する
	if (input->GetKeyState(KEY_INPUT_RETURN) == eInputState::Pressed)
	{
		return eSceneType::result;
	}
	else if (input->GetButtonState(XINPUT_BUTTON_START) == eInputState::Pressed)
	{
		return eSceneType::result;
	}
	
	// ユニット選択処理
	UnitSelection();

	// コスト管理処理
	RegenerateCost();

	//// クールダウン管理処理
	//CooldownManagement(delta_second);

#ifdef DEBUG
	
#endif // DEBUG

	if (old_camerapos.x != camera->GetCameraPos().x)
	{
		move_camera = old_camerapos.x - camera->GetCameraPos().x;
	old_camerapos = camera->GetCameraPos();

	}


	// 親クラスの更新処理を呼び出す
	return __super::Update(delta_second);
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
		DrawGraph(camera->GetCameraPos().x - 700.0f, ShowBackGround_Y, BackGroundImage[StageNumber-1], 0);

	

	// 光を加算合成
	LightMapManager* light_map = LightMapManager::GetInstance();
	light_map->DrawLights(camera->GetCameraPos());
	
	// 親クラスの描画処理を呼び出す
	__super::Draw();

	// ライトマップを描画
	light_map->DrawLightMap();

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

			// キャラ画像を中心に描画
			DrawExtendGraph(
				(int)(x + (button_width - w * 1.5) / 2), (int)(y + (button_height - h * 1.5) / 2),
				(int)(x + (button_width + w * 1.7) / 2), (int)(y + (button_height + h * 1.7) / 2),
				unit_ui[i], TRUE);
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
				(int)(x + (button_width + w * 1.5) / 2), (int)(y + (button_height + h * 1.5) / 2),
				unit_ui[i], TRUE);
		}

		// 描画範囲を元に戻す
		SetDrawArea(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y);
	}

	DrawFormatString(1000, 40, 0x00ffff, "Level:%d", Cost_value);

	// コスト表示
	DrawFormatString(1200, 10, 0xffffff, "%d", cost);


#if _DEBUG
	// カメラ座標描画
	DrawFormatString(500, 300, 0xffffff, "%f", camera->GetCameraPos().x);

	// シーン情報の描画
	SetFontSize(60);
	DrawFormatString(0, 0, 0xffffff, "InGame");
	SetFontSize(32);
	DrawFormatString(100, 300, 0xffffff, "Spaceを押してください");
	DrawFormatString(1000, 0, 0xffffff, "Stage:%d", StageNumber);

#endif
}

// 終了処理
void InGame::Finalize()
{
	// 親クラスの終了時処理を呼び出す
	__super::Finalize();

	//SceneBase::win_flg = this->win_flg;

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
		break;
	default:
		break;
	}
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
					object->CreateObject<P_Ranged>(Vector2D(player->GetLocation().x, player->GetLocation().y + 30.0f));
					cost -= Range_Cost;
					//summon_flag[cursor] = true;
					summon_time[cursor] = std::chrono::steady_clock::now();
				}
			}
			break;
		case 3:
			cost += Cost_value;

			Cost_Click_Count++;
			if (Cost_Click_Count > 10 * Cost_value)
			{
				Cost_Click_Count = 0;
				Cost_value++;
				PlaySoundMem(ClickUp, DX_PLAYTYPE_BACK);
			}
			else
			{
			PlaySoundMem(Click, DX_PLAYTYPE_BACK);
			}

			break;
		}
	}
}

//	コスト管理処理
void InGame::RegenerateCost()
{
	auto now_time = std::chrono::steady_clock::now();

	if (now_time - prev_time > std::chrono::milliseconds(500))
	{
		cost++;
		prev_time = std::chrono::steady_clock::now();
	}
}

// クールダウン処理
void InGame::CooldownManagement()
{
	auto now_time = std::chrono::steady_clock::now();

	for (int i = 0; i < 3; i++)
	{
		if (summon_flag[i] == false)
		{
			continue;
		}
		else
		{
			switch (i)
			{
			case 0:
				if (now_time - summon_time[0] >= cooldown[0])
				{
					summon_flag[0] = false;
				}
				break;
			case 1:
				if (now_time - summon_time[1] >= cooldown[1])
				{
					summon_flag[1] = false;
				}
				break;
			case 2:
				if (now_time - summon_time[2] >= cooldown[2])
				{
					summon_flag[2] = false;
				}
				break;
			}
		}
	}
}

const void InGame::LoadImages() const
{
	//リソースマネージャーのインスタンス取得
	ResourceManager* rm = ResourceManager::GetInstance();

	//画像読込み
	rm->GetImages("Resource/Images/Unit/Melee/Melee_Walk.png", 4, 4, 1, 32, 32);
	rm->GetImages("Resource/Images/Unit/Tank/Tank_Walk.png", 4, 4, 1, 32, 32);
	rm->GetImages("Resource/Images/Unit/Ranged/Ranged_Walk.png", 4, 4, 1, 32, 32);
	rm->GetImages("Resource/Images/Enemy/Melee/Melee_Walk.png", 4, 4, 1, 32, 32);
	rm->GetImages("Resource/Images/Enemy/Tank/Tank_Walk.png", 4, 4, 1, 32, 32);
	rm->GetImages("Resource/Images/Enemy/Ranged/Ranged_Walk.png", 4, 4, 1, 32, 32);
	rm->GetImages("Resource/Images/Unit/Melee/Melee_Attack.png", 4, 4, 1, 32, 32);
	rm->GetImages("Resource/Images/Unit/Tank/Tank_Attack.png", 4, 4, 1, 32, 32);
	rm->GetImages("Resource/Images/Unit/Ranged/Ranged_Attack.png", 4, 4, 1, 32, 32);
	rm->GetImages("Resource/Images/Enemy/Melee/Melee_Attack.png", 4, 4, 1, 32, 32);
	rm->GetImages("Resource/Images/Enemy/Tank/Tank_Attack.png", 4, 4, 1, 32, 32);
}