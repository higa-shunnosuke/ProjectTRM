#include "InGame.h"
#include "DxLib.h"
#include <fstream>

#include "../../Utility/StageData.h"
#include "../../Utility/Camera/Camera.h"

#define PLAYER_INITIAL_LOCATION 3 // プレイヤー初期位置(何ブロック目）

// コンストラクタ
InGame::InGame()
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

	//カメラの情報を取得
	Camera* camera = Camera::GetInstance();
	//カメラ座標の初期化
	camera->SetCameraPos(Vector2D(D_WIN_MAX_X / 2, D_WIN_MAX_Y / 2));

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
	
	// 親クラスの更新処理を呼び出す
	return __super::Update(delta_second);
}

// 描画処理
void InGame::Draw() const
{
	SetFontSize(60);
	DrawFormatString(120, 140, 0xffffff, "InGame");
	SetFontSize(32);
	DrawFormatString(100, 300, 0xffffff, "Enterを押してください");

	// 親クラスの描画処理を呼び出す
	__super::Draw();
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
	std::string file_name = "Resource/Map/Stage.csv";

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

		// ファイルから1文字抽出する
		int c = fgetc(fp);

		// 抽出した文字がEOFならループ終了
		if (c == EOF)
		{
			break;
		}
		// 抽出した文字が改行文字なら、次の行を見に行く
		else if (c == '\n')
		{
			x = 0;
			y++;
		}
		// 抽出した文字が0なら、生成しないで次の文字を見に行く
		else if (c == '0')
		{
			x++;
		}
	}
	// 開いたファイルを閉じる
	fclose(fp);
}