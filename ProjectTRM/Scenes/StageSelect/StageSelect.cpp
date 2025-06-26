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

	if (StageNumber != NULL)
	{
		SerectStage = StageNumber;
	}
	else
	{
		SerectStage = 1;
	}

	// 画像の読み込み
	ResourceManager* rm = ResourceManager::GetInstance();

	DecisionSE = rm->GetSounds("Resource/Sounds/Decision.mp3");
	CursorMoveSE= rm->GetSounds("Resource/Sounds/StageSelect/CursorMove.mp3");
	Stage_Image[0] = rm->GetImages("Resource/Images/BackGround/BlueMoonUI.png")[0];
	Stage_Image[1] = rm->GetImages("Resource/Images/BackGround/YelloMoonUI.png")[0];
	Stage_Image[2] = rm->GetImages("Resource/Images/BackGround/RedMoonUI.png")[0];
	BackGroued_Image = rm->GetImages("Resource/Images/BackGround/StageSelect.png")[0];

		ChangeFontType(DX_FONTTYPE_ANTIALIASING_EDGE);

	BGM = rm->GetSounds("Resource/Sounds/StageSelect/BGM/StageSelect.mp3");

	ChangeVolumeSoundMem(100, BGM);
	ChangeVolumeSoundMem(190, DecisionSE);
	if (PlaySoundMem(BGM, DX_PLAYTYPE_LOOP) == -1)
	{
		MessageBoxA(NULL, "BGM1の読み込みに失敗しました", "エラー", MB_OK);
	}
}

// 更新処理
eSceneType StageSelect::Update(const float& delta_second)
{
	// 入力情報を取得
	InputManager* input = InputManager::GetInstance();

	// →を押したらステージを1つ右に移動させて画像を切り替える処理
	if (input->GetKeyState(KEY_INPUT_RIGHT) == eInputState::Pressed ||
		input->GetButtonState(XINPUT_BUTTON_DPAD_RIGHT) == eInputState::Pressed)
	{
		SerectStage += 1;
		if (SerectStage > ThardStage)
		{
			SerectStage = ThardStage;
		}
		else
		{
			PlaySoundMem(CursorMoveSE, DX_PLAYTYPE_BACK);
			x = 200;
			ChangeX = Set_StageX;
			State = Stage::LMOVE;
		}
	}
	// ←を押したらステージを1つ右に移動させて画像を切り替える処理
	else if (input->GetKeyState(KEY_INPUT_LEFT) == eInputState::Pressed ||
		input->GetButtonState(XINPUT_BUTTON_DPAD_LEFT) == eInputState::Pressed)
	{
		SerectStage -= 1;
		if (SerectStage < FirstStage)
		{
			SerectStage = FirstStage;
		}
		else
		{
			PlaySoundMem(CursorMoveSE, DX_PLAYTYPE_BACK);
			ChangeX = 200;
			x = -Set_StageX;
			State = Stage::RMOVE;
		}
	}

	switch (State)
	{
	case Stage::DEFAULT:

	// インゲームに遷移する
	if (input->GetKeyState(KEY_INPUT_RETURN) == eInputState::Pressed)
	{
		PlaySoundMem(DecisionSE, DX_PLAYTYPE_BACK);
		SetStageNumber(SerectStage);
		return eSceneType::in_game;
	}
	if (input->GetButtonState(XINPUT_BUTTON_A) == eInputState::Pressed)
	{
		PlaySoundMem(DecisionSE, DX_PLAYTYPE_BACK);
		SetStageNumber(SerectStage);
		return eSceneType::in_game;
	}
	if (input->GetButtonState(XINPUT_BUTTON_B) == eInputState::Pressed)
	{
		return eSceneType::title;
	}
	break;
	case Stage::LMOVE:

		x -= 5;
		ChangeX -= 5;

		if (ChangeX == 200)
		{
		x = 200;
		ChangeX = Set_StageX;
		State = Stage::DEFAULT;
		}

		break;
	case Stage::RMOVE:

		x += 5;
		ChangeX += 5;

		if (x == 200)
		{
			x = 200;
			ChangeX = Set_StageX;
			State = Stage::DEFAULT;
		}
		break;
	case Stage::END:
		break;
	default:
		break;
	}

	// 現在のシーン情報を返却する
	return GetNowSceneType();
}

// 描画処理
void StageSelect::Draw() const
{

	int Cw = ChangeX + 100;

	switch (State)
	{
	case DEFAULT:
	{
		DrawExtendGraph((int)(0), (int)(0), (int)(1280), (int)(y + 620), BackGroued_Image, TRUE);

		DrawFormatString(x - 50, 50, 0xffffff, StageText[SerectStage - 1]);

		// ステージ選択画面で選んだステージ画像の範囲を拡大して表示する処理
		DrawExtendGraph((int)(x), (int)(y), (int)(x + w), (int)(y + h), Stage_Image[SerectStage - 1], TRUE);

		
		DrawString(x, 350, StageFlabarText[SerectStage - 1], 0xffffff);
		if (SerectStage != FirstStage)
		{
			DrawString(Centher - 80, 650, StageSelectText[SerectStage - 1], 0xffffff);
		}
		else
		{
			DrawString(Centher, 650, StageSelectText[SerectStage - 1], 0xffffff);
		}
		break;

	}
	//ステージセレクト画面で右キー（→）を押して選択ステージが左にスライドして切り替わる
	case LMOVE:
	{

		DrawExtendGraph((int)(x-200), (int)(0), (int)(x + 1080), (int)(y + 620), BackGroued_Image, TRUE);
		if (SerectStage != FinalStage)
		{
			DrawExtendGraph((int)(ChangeX - 200), (int)(0), (int)(ChangeX + 1080), (int)(y + 620), BackGroued_Image, TRUE);
		}
		else
		{
			DrawExtendGraph((int)(ChangeX - 200), (int)(0), (int)(ChangeX + 1080), (int)(y + 620), BackGroued_Image, TRUE);
		}
		// ステージ選択画面で選んだステージ画像の範囲を拡大して表示する処理
		DrawExtendGraph((int)(x), (int)(y), (int)(x + w), (int)(y + h), Stage_Image[SerectStage - 2], TRUE);
		DrawExtendGraph((int)(ChangeX), (int)(y), (int)(ChangeX + w), (int)(y + h), Stage_Image[SerectStage-1], TRUE);

		DrawFormatString(x - 50, 50, 0xffffff, StageText[SerectStage - 2]);
		DrawFormatString(ChangeX - 50, 50, 0xffffff, StageText[SerectStage - 1]);

		DrawString(x, 350, StageFlabarText[SerectStage - 2], 0xffffff);
		DrawString(ChangeX, 350, StageFlabarText[SerectStage - 1], 0xffffff);
		if (SerectStage != FirstStage)
		{

			DrawString(Centher + (x - 200), 650, StageSelectText[SerectStage - 2], 0xffffff);
			DrawString(Centher + (ChangeX - 280), 650, StageSelectText[SerectStage - 1], 0xffffff);
		}
		else
		{
			//ステージセレクト画面に対応する説明文を表示する
			DrawString(Centher + (x - 200), 650, StageSelectText[SerectStage - 2], 0xffffff);
			DrawString(Centher + (ChangeX - 200), 650, StageSelectText[SerectStage - 1], 0xffffff);
		}

	}
	break;
	//ステージセレクト画面で左キー（←）を押して選択ステージが右にスライドして切り替わる
	case Stage::RMOVE:
	{
		DrawExtendGraph((int)(x-200), (int)(0), (int)(x + 1080), (int)(y + 620), BackGroued_Image, TRUE);
		DrawExtendGraph((int)(ChangeX-200), (int)(0), (int)(ChangeX + 1080), (int)(y + 620), BackGroued_Image, TRUE);

		// ステージ選択画面で選んだステージ画像の範囲を拡大して表示する処理
		DrawExtendGraph((int)(ChangeX), (int)(y), (int)(ChangeX + w), (int)(y + h), Stage_Image[SerectStage], TRUE);
		DrawExtendGraph((int)(x), (int)(y), (int)(x + w), (int)(y + h), Stage_Image[SerectStage - 1], TRUE);

		DrawFormatString(ChangeX - 50, 50, 0xffffff, StageText[SerectStage]);
		DrawFormatString(x - 50, 50, 0xffffff, StageText[SerectStage - 1]);

		DrawString(ChangeX, 350, StageFlabarText[SerectStage], 0xffffff);
		DrawString(x, 350, StageFlabarText[SerectStage - 1], 0xffffff);

		if (SerectStage != FirstStage)
		{

			DrawString(Centher + (x - 200), 650, StageSelectText[SerectStage-1], 0xffffff);
			DrawString(Centher + (ChangeX - 200), 650, StageSelectText[SerectStage], 0xffffff);
		}
		else
		{
			DrawString(Centher + (x - 200), 650, StageSelectText[SerectStage-1], 0xffffff);
			DrawString(Centher + (ChangeX - 200), 650, StageSelectText[SerectStage], 0xffffff);
		}


		break;
	}
	case END:
		break;
	default:
		break;
	}
}
// 終了処理
void StageSelect::Finalize()
{
	ChangeFontType(DX_FONTTYPE_NORMAL);

	StopSoundMem(BGM);

	// 親クラスの終了時処理を呼び出す
	__super::Finalize();
}

// 現在のシーンタイプ情報を取得する
const eSceneType StageSelect::GetNowSceneType() const
{
	return eSceneType::stage_select;
}
