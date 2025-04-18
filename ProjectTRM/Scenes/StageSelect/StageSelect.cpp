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

	// インゲームシーンに遷移する
	if (input->GetKeyState(KEY_INPUT_RETURN) == eInputState::Pressed)
	{
		return eSceneType::in_game;
	}
	if (input->GetButtonState(XINPUT_BUTTON_START) == eInputState::Pressed)
	{
		return eSceneType::in_game;
	}

	// 親クラスの更新処理を呼び出す
	return __super::Update(delta_second);
}

// 描画処理
void StageSelect::Draw() const
{
	SetFontSize(60);
	DrawFormatString(120, 140, 0xffffff, "StageSelect");
	SetFontSize(32);
	DrawFormatString(100, 300, 0xffffff, "Enterを押してください");

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
