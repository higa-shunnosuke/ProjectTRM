#include "StageSelect.h"

// コンストラクタ
StageSelect::StageSelect()
{

}

// デストラクタ
StageSelect::~StageSelect()
{

}

// 初期化処理
void StageSelect::Initialize()
{
	// 親クラスの初期化処理を呼び出す
	__super::Initialize();

	// 画像の読み込み
	ResourceManager* rm = ResourceManager::GetInstance();

	DecisionSE = rm->GetSounds("Resource/Sounds/StageSelect/決定.mp3");
	CursorMoveSE= rm->GetSounds("Resource/Sounds/StageSelect/カーソル移動.mp3");

	Stage_Image[0] = rm->GetImages("Resource/Images/BackGround/BlueMoon.png")[0];
	Stage_Image[1] = rm->GetImages("Resource/Images/BackGround/YelloMoon.png")[0];
	Stage_Image[2] = rm->GetImages("Resource/Images/BackGround/RedMoon.png")[0];

	BGM = rm->GetSounds("Resource/Sounds/StageSelect/StageSelect.mp3");

	ChangeVolumeSoundMem(100, BGM);
	if (PlaySoundMem(BGM, DX_PLAYTYPE_LOOP) == -1)
	{
		MessageBoxA(NULL, "BGM1の再生に失敗しました", "エラー", MB_OK);
	}

}

// 更新処理
eSceneType StageSelect::Update(const float& delta_second)
{
	// 入力情報を取得
	InputManager* input = InputManager::GetInstance();

	// ステージ選択シーンに遷移する
	if (input->GetKeyState(KEY_INPUT_RIGHT)		== eInputState::Pressed ||
		input->GetButtonState(XINPUT_BUTTON_DPAD_RIGHT) == eInputState::Pressed)
	{
		SerectStage += 1;
		if (SerectStage > 4)
		{
			SerectStage = 4;
		}
		else
		{
			PlaySoundMem(CursorMoveSE, DX_PLAYTYPE_BACK);
		}
	}
	// ステージ選択シーンに遷移する
	else if (input->GetKeyState(KEY_INPUT_LEFT) == eInputState::Pressed ||
		input->GetButtonState(XINPUT_BUTTON_DPAD_LEFT) == eInputState::Pressed)
	{
		SerectStage -= 1;
		if (SerectStage < 1)
		{
			SerectStage = 1;
		}
		else
		{
			PlaySoundMem(CursorMoveSE, DX_PLAYTYPE_BACK);
		}
	}

	// インゲームシーンに遷移する
	if (input->GetKeyState(KEY_INPUT_RETURN) == eInputState::Pressed)
	{
		SetStageNumber(SerectStage);
		PlaySoundMem(DecisionSE, DX_PLAYTYPE_BACK);
		return eSceneType::in_game;
	}
	if (input->GetButtonState(XINPUT_BUTTON_A) == eInputState::Pressed)
	{
		SetStageNumber(SerectStage);
		PlaySoundMem(DecisionSE, DX_PLAYTYPE_BACK);
		return eSceneType::in_game;
	}

	// 親クラスの更新処理を呼び出す
	return __super::Update(delta_second);
}

// 描画処理
void StageSelect::Draw() const
{
	/*
	{
		int upnum1 = 0, upnum2 = 0, upnum3 = 0;


		SetFontSize(60);
		DrawFormatString(20, 40, 0xffffff, "Stage Select");
		SetFontSize(32);
		DrawFormatString(100, 400, 0xffffff, "A Button");
		DrawFormatString(100, 450, 0x000000, "Stage:%d", SerectStage);


		switch (SerectStage)
		{
		case 1:
			upnum1 += 50;
			DrawBox(149, 149, 401, 351, 0xffffff, 0);
			break;
		case 2:
			upnum2 += 50;
			DrawBox(399, 149, 651, 351, 0xffffff, 0);
			break;
		case 3:
			upnum3 += 50;
			DrawBox(649, 149, 901, 351, 0xffffff, 0);
			break;
		default:
			break;
		}

		DrawBox(200 - upnum1, 200 - upnum1, 350 + upnum1, 300 + upnum1, 0x00ffff, 1);
		DrawBox(450 - upnum2, 200 - upnum2, 600 + upnum2, 300 + upnum2, 0x00ffff, 1);
		DrawBox(700 - upnum3, 200 - upnum3, 850 + upnum3, 300 + upnum3, 0x00ffff, 1);

	}*/

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
		int y = 120;

		// 拡大用変数
		int w = button_width;
		int h = button_height;

		if (i == SerectStage - 1)
		{
			// 選択中のものだけ拡大（1.2倍）
			int w = (int)(button_width * 1.2);
			int h = (int)(button_height * 1.2);

			// 枠（背景）を描画
			DrawBox(x - (w - button_width) / 2, y - (h - button_height) / 2, x + w, y + h, GetColor(255, 255, 255), TRUE);
			//キャラの描画範囲を制限
			SetDrawArea(x - (w - button_width) / 2, y - (h - button_height) / 2, x + w, y + h);
			{
				// キャラ画像を中心に描画
				DrawExtendGraph(
					(int)(x + (button_width - w * 1.5) / 2), (int)(y + (button_height - h * 1.5) / 2),
					(int)(x + (button_width + w * 1.7) / 2), (int)(y + (button_height + h * 1.7) / 2),
					Stage_Image[i], TRUE);
			}
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
				Stage_Image[i], TRUE);
		}
		// 描画範囲を元に戻す
		SetDrawArea(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y);

	}
		// 親クラスの描画処理を呼び出す
		__super::Draw();
	
}

// 終了処理
void StageSelect::Finalize()
{

	StopSoundMem(BGM);

	// 親クラスの終了時処理を呼び出す
	__super::Finalize();
}

// 現在のシーンタイプ取得処理
const eSceneType StageSelect::GetNowSceneType() const
{
	return eSceneType::stage_select;
}
