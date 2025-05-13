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

}

// 更新処理
eSceneType StageSelect::Update(const float& delta_second)
{
	// 入力情報を取得
	InputManager* input = InputManager::GetInstance();

	// ステージ選択シーンに遷移する
	if (input->GetKeyState(KEY_INPUT_RIGHT) == eInputState::Pressed)
	{
		SerectStage += 1;
		if (SerectStage > 3)
		{
			SerectStage = 3;
		}
	}
	// ステージ選択シーンに遷移する
	else if (input->GetKeyState(KEY_INPUT_LEFT) == eInputState::Pressed)
	{
		SerectStage -= 1;
		if (SerectStage < 1)
		{
			SerectStage = 1;
		}
	}

	// インゲームシーンに遷移する
	if (input->GetKeyState(KEY_INPUT_RETURN) == eInputState::Pressed)
	{
		SetStageNumber(SerectStage);
		return eSceneType::in_game;
	}
	if (input->GetButtonState(XINPUT_BUTTON_START) == eInputState::Pressed)
	{
		SetStageNumber(SerectStage);
		return eSceneType::in_game;
	}

	// 親クラスの更新処理を呼び出す
	return __super::Update(delta_second);
}

// 描画処理
void StageSelect::Draw() const
{

	int upnum1 = 0 , upnum2 = 0 , upnum3 = 0;


	SetFontSize(60);
	DrawFormatString(20, 40, 0xffffff, "StageSelect");
	SetFontSize(32);
	DrawFormatString(100, 300, 0xffffff, "Enterを押してください");
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


	// 親クラスの描画処理を呼び出す
	__super::Draw();
}

// 終了処理
void StageSelect::Finalize()
{
	// 親クラスの終了時処理を呼び出す
	__super::Finalize();
}

// 現在のシーンタイプ取得処理
const eSceneType StageSelect::GetNowSceneType() const
{
	return eSceneType::stage_select;
}
