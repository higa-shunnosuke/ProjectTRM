#include "Title.h"

// コンストラクタ
Title::Title()
{
	Anim_count = 0;
	Anim_flame = 0;
	BGM = 0;
	BackBackGroundImage = 0;
	BackGroundImage = 0;
	DecisionSE = 0;
	Default_End = 0;
	Default_Start = 0;
	Select_Start = 0;
	Select_End = 0;
	MovieHandle = 0;
	SelectBackGroundImage = 0;
	Y = 0;
}

// デストラクタ
Title::~Title()
{
	ChangeFontType(DX_FONTTYPE_NORMAL);

}

// 初期化処理
void Title::Initialize()
{
	// 親クラスの初期化処理を呼び出す
	__super::Initialize();

	ChangeFontType(DX_FONTTYPE_ANTIALIASING_EDGE_8X8);

	MovieHandle = LoadGraph("Resource/Movie/TitleBack.mp4");

	// ムービーの再生位置を0秒目に変更します
	SeekMovieToGraph(MovieHandle, 0000);
	PlayMovieToGraph(MovieHandle);           // 再再生

	// 画像の読み込み
	ResourceManager* rm = ResourceManager::GetInstance();

	SelectBackGroundImage = rm->GetImages		("Resource/Images/BackGround/Select_Title.png")[0];
	BackGroundImage		  = rm->GetImages		("Resource/Images/BackGround/Title.png")[0];
	BackBackGroundImage   = rm->GetImages		("Resource/Images/BackGround/TitleBack.png")[0];
	Select_Start		  = rm->GetImages		("Resource/Images/Title/Select_Start.png")[0];
	Select_End			  = rm->GetImages		("Resource/Images/Title/Select_End.png")[0];
	Default_Start		  = rm->GetImages		("Resource/Images/Title/Default_Start.png")[0];
	Default_End			  = rm->GetImages		("Resource/Images/Title/Default_End.png")[0];
	SellectImage		  = rm->GetImages		("Resource/Images/Effect/Flamethrower.png",10,3,4,96,48);

	BGM = rm->GetSounds("Resource/Sounds/Title/BGM/OP.mp3");
	DecisionSE = rm->GetSounds("Resource/Sounds/Decision.mp3");
	Y = D_WIN_MAX_Y;
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
	int state = GetMovieStateToGraph(MovieHandle);
	if (state == 0) { // 停止状態
		SeekMovieToGraph(MovieHandle, 7000);        // 先頭に戻す
		PlayMovieToGraph(MovieHandle);           // 再生
	}

	switch (State)
	{
	case SELECT_WAIT:
	{

		// インゲームシーンに遷移する
		if (input->GetKeyState(KEY_INPUT_RETURN) == eInputState::Pressed)
		{
			PlaySoundMem(DecisionSE, DX_PLAYTYPE_BACK);
			State = TitleState::USEING;
		}
		if (input->GetButtonState(XINPUT_BUTTON_A) == eInputState::Pressed)
		{
			PlaySoundMem(DecisionSE, DX_PLAYTYPE_BACK);
			State = TitleState::USEING;
		}
		if (input->GetKeyState(KEY_INPUT_SPACE) == eInputState::Pressed)
		{
			PlaySoundMem(DecisionSE, DX_PLAYTYPE_BACK);
			return eSceneType::end;
		}

	}
	break;
	case USEING:
		Y--;
		if (Y == 450)
		{
			State = TitleState::SELECTED;
		}
		break;
	case SELECTED:
		if (input->GetButtonState(XINPUT_BUTTON_DPAD_UP) == eInputState::Pressed)
		{
			if (Selected != 0)
			{
				Selected--;
			}
		}
		else if (input->GetButtonState(XINPUT_BUTTON_DPAD_DOWN) == eInputState::Pressed)
		{
			if (Selected != MAX_SELECT)
			{
				Selected++;
			}
		}

		if (input->GetKeyState(KEY_INPUT_UP) == eInputState::Pressed)
		{
			if (Selected != 0)
			{
				Selected--;
			}
		}
		else if (input->GetKeyState(KEY_INPUT_DOWN) == eInputState::Pressed)
		{
			if (Selected != MAX_SELECT)
			{
				Selected++;
			}
		}
		else if ((input->GetButtonState(XINPUT_BUTTON_A) == eInputState::Pressed) || (input->GetKeyState(KEY_INPUT_RETURN) == eInputState::Pressed))
		{
			PlaySoundMem(DecisionSE, DX_PLAYTYPE_BACK);
			switch (Selected)
			{
			case 0:
				return eSceneType::stage_select;
			case MAX_SELECT:
				return eSceneType::end;
			default:
				break;
			}
		}
		break;
	case FINAL:
		break;
	default:
		break;
	}

	Anim_flame += delta_second*10.0f;
	if (Anim_flame >= 1.0f)
	{
		Anim_count++;
		Anim_flame = 0.0f;
	}
	if (Anim_count > 9)
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
	// ムービー映像を画面いっぱいに描画します
	DrawExtendGraph(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, MovieHandle, FALSE);

	switch (State)
	{
	case SELECT_WAIT:

		DrawGraph(0, 0, BackGroundImage, 1);
		break;
	case SELECTED:
	{

		DrawGraph(0, 0, SelectBackGroundImage, 1);
		

		SetFontSize(64);
		float SelectColor = 0xffff00, Color = 0xff0000;



		if (Selected == 0)
		{
			DrawExtendGraphF(350, 390, 870, 650, Select_Start, true);
			DrawExtendGraphF(400, 500, 900, 800, Default_End, true);

		}
		else
		{
			DrawExtendGraphF(400, 390, 900, 650, Default_Start, true);			
			DrawExtendGraphF(350, 500, 870, 800, Select_End, true);

		}

		SetFontSize(32);
	}
		break;
	case FINAL:

		break;
	default:
		break;
	}

	// 親クラスの描画処理を呼び出す
	__super::Draw();

}

// 終了処理
void Title::Finalize()
{
	ChangeFontType(DX_FONTTYPE_NORMAL);
	// 読み込んだムービーファイルのグラフィックハンドルの削除
	DeleteGraph(MovieHandle);
	StopSoundMem(BGM);

	// 親クラスの終了時処理を呼び出す
	__super::Finalize();
}

// 現在のシーンタイプ取得処理
const eSceneType Title::GetNowSceneType() const
{
	return eSceneType::title;
}