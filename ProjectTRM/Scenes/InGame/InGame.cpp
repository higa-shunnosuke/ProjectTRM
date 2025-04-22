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

#define PLAYER_INITIAL_LOCATION 3 // プレイヤー初期位置(何ブロック目）

// コンストラクタ
InGame::InGame():
	cooldown()
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

	// カメラの情報を取得
	Camera* camera = Camera::GetInstance();
	// カメラ座標の初期化
	camera->SetCameraPos(Vector2D(D_WIN_MAX_X / 2, D_WIN_MAX_Y / 2));

	// ステージ読み込み
	LoadStage();

	// カーソルの初期化
	cursor = 1;
}

// 更新処理
eSceneType InGame::Update(const float& delta_second)
{
	// 入力情報を取得
	InputManager* input = InputManager::GetInstance();

	//カメラの情報を取得
	Camera* camera = Camera::GetInstance();

	//カメラ情報の更新
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

	// 親クラスの更新処理を呼び出す
	return __super::Update(delta_second);
}

// 描画処理
void InGame::Draw() const
{
	// 親クラスの描画処理を呼び出す
	__super::Draw();

	//ユニット選択の描画
	int options_size;	// 選択肢の拡大率
	int space_size;		// 隙間の大きさ

	// パネルの描画
	DrawBox(100, 50, 1180, 290, 0xffffff, TRUE);
	
	// 選択肢の描画
	for (int i = 0; i < 3; i++)
	{
		if (i+1 == cursor)
		{
			options_size = 200;
			space_size = 140;

			DrawBox(60 + space_size + i * (options_size + space_size), 50 + 20,
					60 + space_size + i * (options_size + space_size) + options_size, 50 + 20 + options_size, 0x00ff00, TRUE);
		}
		else
		{
			options_size = 160;
			space_size = 170;

			DrawBox(60 + space_size + i * (options_size + space_size), 50 + 40,
					60 + space_size + i * (options_size + space_size) + options_size, 50 + 40 + options_size, 0xff0000, TRUE);
		}
	}

#if _DEBUG	
	// パネルのマス目描画
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			DrawBox(j * 40, 50 + i * 40,
				(j + 1) * 40, 50 + (i + 1) * 40, 0x0000ff, FALSE);
		}
	}

	DrawFormatString(290, 155, 0x000000, "A");
	DrawFormatString(635, 155, 0x000000, "B");
	DrawFormatString(970, 155, 0x000000, "C");

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
}

// 現在のシーンタイプ情報を取得する
const eSceneType InGame::GetNowSceneType() const
{
	return eSceneType::in_game;
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
		location.x = (float)(x * BOX_SIZE + BOX_SIZE / 2 - D_WIN_MAX_X);
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
			x++;
		}
	}
	// 開いたファイルを閉じる
	fclose(fp);
}

//	ユニット追召喚
void InGame::UnitSelection()
{
	// 入力情報を取得
	InputManager* input = InputManager::GetInstance();

	// カーソル操作
	if (input->GetKeyState(KEY_INPUT_RIGHT) == eInputState::Pressed)
	{
		if (cursor < 3)
		{
			cursor++;
		}
	}
	if (input->GetKeyState(KEY_INPUT_LEFT) == eInputState::Pressed)
	{
		if (cursor > 1)
		{
			cursor--;
		}
	}

	// 決定
	if (input->GetKeyState(KEY_INPUT_SPACE) == eInputState::Pressed)
	{
		// オブジェクトマネージャーのポインタ
		GameObjectManager* object = GameObjectManager::GetInstance();

		switch (cursor)
		{
		case 1:
			object->CreateObject<P_Melee>(Vector2D(player->GetLocation().x, player->GetLocation().y + 30.0f));
			break;
		case 2:
			object->CreateObject<P_Ranged>(Vector2D(player->GetLocation().x, player->GetLocation().y + 30.0f));
			break;
		case 3:
			object->CreateObject<P_Tank>(Vector2D(player->GetLocation().x, player->GetLocation().y + 30.0f));
			break;
		}

		object->CreateObject<E_Melee>(Vector2D(enemy->GetLocation().x, enemy->GetLocation().y + 30.0f));

	}
}