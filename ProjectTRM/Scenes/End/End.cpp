#include "End.h"
#include "../../Application.h"


// コンストラクタ
End::End()
{

}

// デストラクタ
End::~End()
{

}

// 初期化処理
void End::Initialize()
{
	// 親クラスの初期化処理を呼び出す
	__super::Initialize();

	// 画像の読み込み
	ResourceManager* rm = ResourceManager::GetInstance();

}

// 更新処理
eSceneType End::Update(const float& delta_second)
{
	// 入力情報を取得
	InputManager* input = InputManager::GetInstance();



	auto now_time = std::chrono::steady_clock::now();
	if (now_time - prev_time > std::chrono::milliseconds(10))
	{
		x -= 10;
		if (x <= 0)
		{
			//ゲームを終了させる
			Application* app = Application::GetInstance();
			app->QuitGame();
		}
		prev_time = std::chrono::steady_clock::now();
	}



	//// ENTERキー、STARTボタンを押されたら
	//if (input->GetKeyState(KEY_INPUT_RETURN) == eInputState::Pressed ||
	//	input->GetButtonState(XINPUT_BUTTON_A) == eInputState::Pressed)
	//{
	//	//ゲームを終了させる
	//	Application* app = Application::GetInstance();
	//	app->QuitGame();
	//}

	// 親クラスの更新処理を呼び出す
	return __super::Update(delta_second);
}

// 描画処理
void End::Draw() const
{
	if (ProjectConfig::DEBUG)
	{
		SetFontSize(60);
		DrawFormatString(120, 140, 0xffffff, "End");
		SetFontSize(32);
		DrawFormatString(100, 300, 0xffffff, "Aボタンを押してください");
	}

	DrawBox(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, 0xffffff, TRUE);
	DrawBox(0, 0, x, D_WIN_MAX_Y, 0x000000, 1);

	// 親クラスの描画処理を呼び出す
	__super::Draw();

}

// 終了処理
void End::Finalize()
{
	// 親クラスの終了時処理を呼び出す
	__super::Finalize();
}

// 現在のシーンタイプ取得処理
const eSceneType End::GetNowSceneType() const
{
	return eSceneType::end;
}
