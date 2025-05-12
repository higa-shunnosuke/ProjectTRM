#include "InGame.h"
#include "DxLib.h"
#include <fstream>

#include "../../Utility/StageData.h"
#include "../../Utility/Camera/Camera.h"
#include "../../Utility/LightMapManager.h"

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
	unit_ui()
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
	unit_ui[2]= rm->GetImages("Resource/Images/Unit/Ranged/Ranged_Walk.png", 4, 4, 1, 32, 32)[0];

	// ライトマップの初期化
	LightMapManager* light_map = LightMapManager::GetInstance();
	light_map->Initialize();

	// カメラ座標の初期化
	Camera* camera = Camera::GetInstance();
	camera->Initialize();

	// ステージ読み込み
	LoadStage();

	// 画像の読み込み
	LoadImages();

	// カーソルの初期化
	cursor = 0;

	// コストの初期化 
	cost = 100;
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
	if (enemy->GetDead())
	{

	}

	// 入力情報を取得
	InputManager* input = InputManager::GetInstance();

	//カメラの情報を取得
	Camera* camera = Camera::GetInstance();
	//カメラの更新
	camera->Update();

	// リザルトシーンに遷移する
	if (input->GetKeyState(KEY_INPUT_RETURN) == eInputState::Pressed)
	{
		return eSceneType::result;
	}
	if (input->GetButtonState(XINPUT_BUTTON_START) == eInputState::Pressed)
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

	// 親クラスの更新処理を呼び出す
	return __super::Update(delta_second);
}

// 描画処理
void InGame::Draw() const
{
	// 光を加算合成
	LightMapManager* light_map = LightMapManager::GetInstance();
	light_map->DrawLights();
	
	// 親クラスの描画処理を呼び出す
	__super::Draw();

	// ライトマップを描画
	light_map->DrawLightMap();

	// ボタンサイズ
	const int button_width = 200;
	const int button_height = 200;

	// ボタンの数
	const int button_count = 3;

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

	// コスト表示
	DrawFormatString(1200, 10, 0xffffff, "%d", cost);

#if _DEBUG
	Camera* camera = Camera::GetInstance();
	// カメラ座標描画
	DrawFormatString(500, 300, 0xffffff, "%f", camera->GetCameraPos().x);

	// シーン情報の描画
	SetFontSize(60);
	DrawFormatString(0, 0, 0xffffff, "InGame");
	SetFontSize(32);
	DrawFormatString(100, 300, 0xffffff, "Spaceを押してください");
#endif
}

// 終了処理
void InGame::Finalize()
{
	// 親クラスの終了時処理を呼び出す
	__super::Finalize();

	SceneBase::win_flg = this->win_flg;

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

// ステージ生成処理
void InGame::LoadStage()
{
	// オブジェクトマネージャーのポインタ
	GameObjectManager* object = GameObjectManager::GetInstance();
	
	FILE* fp = NULL;
	std::string file_name = "Resource/Map/Stage1.csv";

	// 指定されたファイルを開く
	errno_t result = fopen_s(&fp, file_name.c_str(), "r");

	// エラーチェック
	if (result != 0)
	{
		throw (file_name + "が開けません");
	}

	// カウント用変数
	int x = 0;			// 列
	int y = 0;			// 行

	// ファイル内の文字を確認していく
	while (true)
	{
		//座標計算
		Vector2D location;
		location.x = (float)(x * BOX_SIZE + BOX_SIZE / 2);
		location.y = (float)(y * BOX_SIZE + BOX_SIZE / 2);

		// ファイルから1文字抽出
		int c = fgetc(fp);

		// EOFならループ終了
		if (c == EOF)
		{
			break;
		}
		// 改行文字なら、次の行へ
		else if (c == '\n')
		{
			x = 0;
			y++;
		}
		// 0なら、次の文字へ
		else if (c == '0')
		{
			x++;
		}
		// Gなら、地面を生成
		else if (c == 'G')
		{
			object->CreateObject<Ground>(Vector2D(location.x, location.y));
			x++;
		}
		// Oなら、巫女を生成
		else if (c == 'O')
		{
			player = object->CreateObject<Oracle>(Vector2D(location.x, location.y - 30));
			x++;
		}
		// Hなら、異端者
		else if (c == 'H')
		{
			enemy = object->CreateObject<Heretic>(Vector2D(location.x, location.y - 30));
			enemy->SetInGamePoint(this);
			x++;
		}
	}
	// 開いたファイルを閉じる
	fclose(fp);
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
		if (cursor < 2)
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
		// ライトマップマネージャーのポインタ
		LightMapManager* light = LightMapManager::GetInstance();

		switch (cursor)
		{
			// タンク召喚
		case 0:
			if (summon_flag[cursor] == false)
			{
				if (cost - 10 >= 0)
				{
					// 光を追加 & タンクを生成
					light->AddLight(object->CreateObject<P_Tank>(Vector2D(player->GetLocation().x, player->GetLocation().y + 30.0f)));
					object->CreateObject<P_Tank>(Vector2D(player->GetLocation().x, player->GetLocation().y + 30.0f));
					cost -= 10;
					
					//summon_flag[cursor] = true;
					summon_time[cursor] = std::chrono::steady_clock::now();
				}
			}
			break;
			// 近接召喚
		case 1:
			if (summon_flag[cursor] == false)
			{
				if (cost - 20 >= 0)
				{
					// 光を追加 & 近接を生成
					light->AddLight(object->CreateObject<P_Melee>(Vector2D(player->GetLocation().x, player->GetLocation().y + 30.0f)));
					cost -= 20;
					//summon_flag[cursor] = true;
					summon_time[cursor] = std::chrono::steady_clock::now();
				}
			}
			break;
			// 遠距離召喚
		case 2:
			if (summon_flag[cursor] == false)
			{
				if (cost - 30 >= 0)
				{
					object->CreateObject<P_Ranged>(Vector2D(player->GetLocation().x, player->GetLocation().y + 30.0f));
					cost -= 30;
					//summon_flag[cursor] = true;
					summon_time[cursor] = std::chrono::steady_clock::now();
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