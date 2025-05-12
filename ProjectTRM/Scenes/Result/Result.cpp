#include "Result.h"

// コンストラクタ
Result::Result()
{
	
}

// デストラクタ
Result::~Result()
{

}

// 初期化処理
void Result::Initialize()
{
	// 親クラスの初期化処理を呼び出す
	__super::Initialize();

	// 画像の読み込み
	ResourceManager* rm = ResourceManager::GetInstance();


	DeadImage[0] = rm->GetImages("Resource/Images/Enemy/Heretic/NotDead.png")[0];
	DeadImage[1] = rm->GetImages("Resource/Images/Enemy/Heretic/ImDead.png")[0];
}

// 更新処理
eSceneType Result::Update(const float& delta_second)
{
	// 入力情報を取得
	InputManager* input = InputManager::GetInstance();

	if (win_flg)
	{
		// ステージ選択シーンに遷移する
		if (input->GetKeyState(KEY_INPUT_SPACE) == eInputState::Pressed)
		{
			return eSceneType::stage_select;
		}
		else if (input->GetButtonState(XINPUT_BUTTON_START) == eInputState::Pressed)
		{
			return eSceneType::stage_select;
		}

		// タイトルシーンに遷移する
		if (input->GetKeyState(KEY_INPUT_X) == eInputState::Pressed)
		{
			return eSceneType::title;
		}
		else if (input->GetButtonState(XINPUT_BUTTON_X) == eInputState::Pressed)
		{
			return eSceneType::title;
		}
	}
	else
	{
		// インゲームシーンに遷移する
		if (input->GetKeyState(KEY_INPUT_SPACE) == eInputState::Pressed)
		{
			return eSceneType::in_game;
		}
		else if (input->GetButtonState(XINPUT_BUTTON_START) == eInputState::Pressed)
		{
			return eSceneType::in_game;
		}
		// タイトルシーンに遷移する
		else if (input->GetKeyState(KEY_INPUT_X) == eInputState::Pressed)
		{
			return eSceneType::title;
		}
		else if (input->GetButtonState(XINPUT_BUTTON_X) == eInputState::Pressed)
		{
			return eSceneType::title;
		}

		auto now_time = std::chrono::steady_clock::now();

		if (now_time - prev_time > std::chrono::milliseconds(1000))
		{
			count--;
			prev_time = std::chrono::steady_clock::now();
		}
		else if (count == 0)
		{
			return eSceneType::title;
		}
	}


	// インゲームシーンに遷移する
	if (input->GetKeyState(KEY_INPUT_RETURN) == eInputState::Pressed)
	{
		return eSceneType::end;
	}
	if (input->GetButtonState(XINPUT_BUTTON_START) == eInputState::Pressed)
	{
		return eSceneType::end;
	}



	
	// 親クラスの更新処理を呼び出す
	return __super::Update(delta_second);

}

// 描画処理
void Result::Draw() const
{
	SetFontSize(60);
	DrawFormatString(120, 140, 0xffffff, "Result");
	SetFontSize(32);
	if (win_flg)
	{
#ifdef DEBUG
		DrawString(500, 500, "Space : Stage Select", 0xffffff);
		DrawString(570, 550, "X : Title", 0xffffff);
#endif // DEBUG

	}
	else
	{
#ifdef DEBUG
		SetFontSize(60);
		DrawFormatString(500, 350, 0xffffff, "%d", count);
		SetFontSize(32);
		DrawString(500, 500, "Space : Retry", 0xffffff);
		DrawString(570, 550, "X : Title", 0xffffff);
#endif // DEBUG

	}

	// 親クラスの描画処理を呼び出す
 	__super::Draw();
}

// 終了処理
void Result::Finalize()
{
	// 親クラスの終了時処理を呼び出す
	__super::Finalize();
}

// 現在のシーンタイプ情報を取得する
const eSceneType Result::GetNowSceneType() const
{
	return eSceneType::result;
}
