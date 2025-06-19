#include "Title.h"

// コンストラクタ
Title::Title()
{

}

// デストラクタ
Title::~Title()
{

}

// 初期化処理
void Title::Initialize()
{
	// 親クラスの初期化処理を呼び出す
	__super::Initialize();

	// 画像の読み込み
	ResourceManager* rm = ResourceManager::GetInstance();


	BackGroundImage = rm->GetImages("Resource/Images/BackGround/Title.png")[0];

	BGM = rm->GetSounds("Resource/Sounds/Title/BGM/OP.mp3");
	DecisionSE = rm->GetSounds("Resource/Sounds/Decision.mp3");

	Anim_flame = 0;
	Anim_count = 0;

	ChangeVolumeSoundMem(100, BGM);
	ChangeVolumeSoundMem(190, DecisionSE);
	if (PlaySoundMem(BGM, DX_PLAYTYPE_LOOP) == -1)
	{
		MessageBoxA(NULL, "BGM1の再生に失敗しました", "エラー", MB_OK);
	}
}

// 更新処理
eSceneType Title::Update(const float& delta_second)
{
	// 入力情報を取得
	InputManager* input = InputManager::GetInstance();

	// インゲームシーンに遷移する
	if (input->GetKeyState(KEY_INPUT_RETURN)==eInputState::Pressed)
	{
		PlaySoundMem(DecisionSE, DX_PLAYTYPE_BACK);
		return eSceneType::stage_select;
	}
	if (input->GetButtonState(XINPUT_BUTTON_A) == eInputState::Pressed)
	{
		PlaySoundMem(DecisionSE, DX_PLAYTYPE_BACK);
		return eSceneType::stage_select;
	}
	if (input->GetKeyState(KEY_INPUT_SPACE) == eInputState::Pressed)
	{
		PlaySoundMem(DecisionSE, DX_PLAYTYPE_BACK);
		return eSceneType::end;
	}
	if (input->GetButtonState(XINPUT_BUTTON_B) == eInputState::Pressed)
	{
		PlaySoundMem(DecisionSE, DX_PLAYTYPE_BACK);
		return eSceneType::end;
	}

	Anim_flame += delta_second;
	if (Anim_flame >= 1.0f)
	{
		Anim_count ++;
		Anim_flame = 0.0f;
	}
	if (Anim_count > 1)
	{
		Anim_count = 0;
		Anim_flame = 0.0f;
	}

	// 現在のシーン情報を返却する
	return GetNowSceneType();
}

// 描画処理
void Title::Draw() const
{

	DrawGraph(0, 0, BackGroundImage, 0);

	/*
	SetFontSize(120);
	DrawFormatString(500, 150, 0xff5500, "Project\n -TRM-");
	SetFontSize(60);

	if (Anim_count == 1)
	{
	DrawFormatString(500, 450, 0xff5500, "Press A Button");
	}
	DrawFormatString(1080, 620, 0xff5500, "B:End");

	if (ProjectConfig::DEBUG)
	{
		SetFontSize(60);
		DrawFormatString(100, 140, 0xffffff, "Title");
		SetFontSize(32);
		DrawFormatString(100, 300, 0xffffff, "Enterを押してね!");
	}
	*/

	// 親クラスの描画処理を呼び出す
	__super::Draw();

}

// 終了処理
void Title::Finalize()
{

	StopSoundMem(BGM);

	// 親クラスの終了時処理を呼び出す
	__super::Finalize();
}

// 現在のシーンタイプ取得処理
const eSceneType Title::GetNowSceneType() const
{
	return eSceneType::title;
}