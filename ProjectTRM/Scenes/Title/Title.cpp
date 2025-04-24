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
	
}

// 更新処理
eSceneType Title::Update(const float& delta_second)
{
	// 入力情報を取得
	InputManager* input = InputManager::GetInstance();

	// インゲームシーンに遷移する
	if (input->GetKeyState(KEY_INPUT_RETURN)==eInputState::Pressed)
	{
		return eSceneType::stage_select;
	}
	if (input->GetButtonState(XINPUT_BUTTON_START)== eInputState::Pressed)
	{
		return eSceneType::stage_select;
	}
	if (input->GetKeyState(KEY_INPUT_SPACE) == eInputState::Pressed)
	{
		return eSceneType::end;
	}


	Anim_flame += 1.0f;
	if (Anim_flame == 60.0f)
	{
		Anim_count ++;
		Anim_flame = 0.0f;
	}
	if (Anim_count == 4)
	{
		Anim_count = 0;
		Anim_flame = 0.0f;
	}


	// 親クラスの更新処理を呼び出す
	return __super::Update(delta_second);
}

// 描画処理
void Title::Draw() const
{
	SetFontSize(120);
	DrawFormatString(500, 150, 0xff5500, "Project\n -TRM-");
	SetFontSize(60);

	if (Anim_count <= 1)
	{
	DrawFormatString(500, 450, 0xff5500, "Press_AnyButton");
	}

#ifdef _DEBUG
	SetFontSize(60);
	DrawFormatString(100, 140, 0xffffff, "Title");
	SetFontSize(32);
	DrawFormatString(100, 300, 0xffffff, "Enterを押してね!");
#endif

	// 親クラスの描画処理を呼び出す
	__super::Draw();

}

// 終了処理
void Title::Finalize()
{
	// 親クラスの終了時処理を呼び出す
	__super::Finalize();
}

// 現在のシーンタイプ取得処理
const eSceneType Title::GetNowSceneType() const
{
	return eSceneType::title;
}