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

	ResourceManager* rm = ResourceManager::GetInstance();

	bgmHandle[0] = rm->GetSounds("Resource/Sounds/Result/Lost.mp3");
	bgmHandle[1] = rm->GetSounds("Resource/Sounds/Result/Loose_BGM.mp3");
	bgmHandle[2] = rm->GetSounds("Resource/Sounds/Result/Win_BGM.mp3");
	bgmHandle[3] = rm->GetSounds("Resource/Sounds/Result/Wining.mp3");

	if (win_flg)
	{
		if (bgmHandle[3] == -1)
		{
			MessageBoxA(NULL, "BGM1の読み込みに失敗しました", "エラー", MB_OK);
		}
		else
		{
			// 音量設定
			ChangeVolumeSoundMem(150, bgmHandle[3]);
			// BGM再生
			if (PlaySoundMem(bgmHandle[3], DX_PLAYTYPE_BACK) == -1)
			{
				MessageBoxA(NULL, "BGM1の再生に失敗しました", "エラー", MB_OK);
			}
		}
	}
	else
	{
		if (bgmHandle[1] == -1)
		{
			MessageBoxA(NULL, "BGM2の読み込みに失敗しました", "エラー", MB_OK);
		}
		else
		{
			// 音量設定
			ChangeVolumeSoundMem(150, bgmHandle[1]);
			// BGM再生
			if (PlaySoundMem(bgmHandle[1], DX_PLAYTYPE_BACK) == -1)
			{
				MessageBoxA(NULL, "BGM2の再生に失敗しました", "エラー", MB_OK);
			}
		}
	}


}

// 更新処理
eSceneType Result::Update(const float& delta_second)
{
	// 入力情報を取得
	InputManager* input = InputManager::GetInstance();

	if (win_flg)
	{
		switch (cursor)
		{
		case 1:
		{
			if (input->GetKeyState(KEY_INPUT_UP) == eInputState::Pressed)
			{
				cursor++;
			}
			if (input->GetButtonState(XINPUT_BUTTON_DPAD_UP) == eInputState::Pressed)
			{
				cursor++;
			}
			// ステージ選択シーンに遷移する
 			 if (input->GetKeyState(KEY_INPUT_SPACE) == eInputState::Pressed)
			{
				return eSceneType::title;
			}
			else if (input->GetButtonState(XINPUT_BUTTON_A) == eInputState::Pressed)
			{
				 return eSceneType::title;
			}
		}
		break;
		case 2:
		{

			if (input->GetKeyState(KEY_INPUT_DOWN) == eInputState::Pressed)
			{
				cursor--;
			}
			else if (input->GetButtonState(XINPUT_BUTTON_DPAD_DOWN) == eInputState::Pressed)
			{
				cursor--;
			}
			// タイトルシーンに遷移する
			if (input->GetKeyState(KEY_INPUT_SPACE) == eInputState::Pressed)
			{
				return eSceneType::stage_select;
			}
			else if (input->GetButtonState(XINPUT_BUTTON_A) == eInputState::Pressed)
			{
				return eSceneType::stage_select;
			}
		}
		break;
		default:
			if (input->GetKeyState(KEY_INPUT_UP) == eInputState::Pressed)
			{
				cursor = 2;
			}
			else if (input->GetButtonState(XINPUT_BUTTON_DPAD_UP) == eInputState::Released)
			{
				cursor = 2;
			}
			if (input->GetKeyState(KEY_INPUT_DOWN) == eInputState::Pressed)
			{
				cursor = 1;
			}
			else if (input->GetButtonState(XINPUT_BUTTON_DPAD_DOWN) == eInputState::Pressed)
			{
				cursor = 1;
			}
			break;
		}
	}
	else
	{
		switch (cursor)
		{
		case 1:
		{
			if (input->GetKeyState(KEY_INPUT_UP) == eInputState::Pressed)
			{
				cursor++;
			}
			else if (input->GetButtonState(XINPUT_BUTTON_DPAD_UP) == eInputState::Pressed)
			{
				cursor++;
			}
			// ステージ選択シーンに遷移する
			if (input->GetKeyState(KEY_INPUT_SPACE) == eInputState::Pressed)
			{
				return eSceneType::title;
			}
			else if (input->GetButtonState(XINPUT_BUTTON_A) == eInputState::Pressed)
			{
				return eSceneType::title;
			}
		}
		break;
		case 2:
		{

			if (input->GetKeyState(KEY_INPUT_DOWN) == eInputState::Pressed)
			{
				cursor--;
			}
			else if (input->GetButtonState(XINPUT_BUTTON_DPAD_DOWN) == eInputState::Pressed)
			{
				cursor--;
			}
			// タイトルシーンに遷移する
			if (input->GetKeyState(KEY_INPUT_SPACE) == eInputState::Pressed)
			{
				return eSceneType::in_game;
			}
			else if (input->GetButtonState(XINPUT_BUTTON_A) == eInputState::Pressed)
			{
				return eSceneType::in_game;
			}
		}
		break;
		default:
			if (input->GetKeyState(KEY_INPUT_UP) == eInputState::Pressed)
			{
				cursor = 2;
			}
			else if (input->GetButtonState(XINPUT_BUTTON_DPAD_UP) == eInputState::Released)
			{
				cursor = 2;
			}
			else if (input->GetKeyState(KEY_INPUT_DOWN) == eInputState::Pressed)
			{
				cursor = 1;
			}
			else if (input->GetButtonState(XINPUT_BUTTON_DPAD_DOWN) == eInputState::Released)
			{
				cursor = 1;
			}
			break;
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
	auto  now_time = std::chrono::steady_clock::now();

	if (now_time - select_time > std::chrono::milliseconds(500))
	{
		if (show)
		{
			show = false;
		}
		else
		{
			show = true;
		}
		select_time = std::chrono::steady_clock::now();
	}

	if (cursor == -1)
	{
		if (now_time - choice_time > std::chrono::milliseconds(1000))
		{
			cursor = 2;
		}
	}
	

	// 親クラスの更新処理を呼び出す
	return __super::Update(delta_second);
}

// 描画処理
void Result::Draw() const
{

	DrawFormatString(500, 150, 0xffffff, "死亡したユニット:%d\n生産したユニット:%d", -dead_unit, make_unit - 1);

	SetFontSize(60);
	DrawFormatString(120, 140, 0xffffff, "Result");
	SetFontSize(32);
	if (win_flg)
	{
		switch (cursor)
		{
			case 1:
			{
				DrawCircle(480, 566, 10, 0xffffff);
				if (show != true)
				{
					DrawString(500, 550, ": Title", 0xffffff);
				}
				DrawString(500, 500, ": Stage Select", 0xffffff);
			}
			break;
			case 2:
			{
				DrawCircle(480, 516, 10, 0xffffff);
				if (show != true)
				{
					DrawString(500, 500, ": Stage Select", 0xffffff);
				}
				DrawString(500, 550, ": Title", 0xffffff);
			}
			break;
		default:
		{
			DrawString(500, 500, ": Stage Select", 0xffffff);
			DrawString(500, 550, ": Title", 0xffffff);
		}
			break;
		}

	}
	else
	{
		SetFontSize(80);
		DrawFormatString(500, 350, 0xffffff, "%d", count);
		SetFontSize(32);		
		switch (cursor)
		{
		case 1:
		{
			DrawCircle(480, 566, 10, 0xffffff);
			if (show != true)
			{
				DrawString(500, 550, ": Title", 0xffffff);
			}
			DrawString(500, 500, ": ReStart", 0xffffff);
		}
		break;
		case 2:
		{
			DrawCircle(480, 516, 10, 0xffffff);
			if (show != true)
			{
				DrawString(500, 500, ": ReStart", 0xffffff);
			}
			DrawString(500, 550, ": Title", 0xffffff);
		}
		break;
		default:
		{
			DrawString(500, 500, ": ReStart", 0xffffff);
			DrawString(500, 550, ": Title", 0xffffff);
		}
		break;
		}

	}

	// 親クラスの描画処理を呼び出す
 	__super::Draw();
}

// 終了処理
void Result::Finalize()
{
	
	if (win_flg)
	{
		StopSoundMem(bgmHandle[3]);
	}
	else
	{
		StopSoundMem(bgmHandle[1]);
	}
	
	// 親クラスの終了時処理を呼び出す
	__super::Finalize();
}

// 現在のシーンタイプ情報を取得する
const eSceneType Result::GetNowSceneType() const
{
	return eSceneType::result;
}
