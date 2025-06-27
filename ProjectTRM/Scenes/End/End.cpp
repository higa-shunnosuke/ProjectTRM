#include "End.h"
#include "../../Application.h"
#include "../../Utility/Input/InputManager.h"

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

	rm->UnLoadImages();
	rm->UnLoadSounds();

	y = 800;
	time = 0;

	background = LoadGraph("Resource/Images/BackGround/StageSelect.png");
	BGM = rm->GetSounds("Resource/Sounds/Ending/ED3.mp3");

	ChangeVolumeSoundMem(300 / 2, BGM);

	creditText = {
	"Torchguard",
	"Shadows Fall",
	"",
	"",
	"",
	"",
	"",
		"使用素材",
	"",
	"BGM",
	"Audiostock",
	"",
	"効果音",
	"Audiostock",
	"効果音ラボ",
	"",
	"ユニット",
	"Bringer Of Death",
	"Hero and Opponents Animation",
	"PIXEL ART ASSETS",
	"itch.io",
	"Quixote",
	"Cave Explorer",
	"Werewolf Sprite Pixel Characters",
	"EVil Wizard 2",
	"Pixel Wizard 2D Art",
	"",
	"エフェクト",
	"ぴぽや",
	"Pixel Art VFX",
	"",
	"プログラム",
	"比嘉 駿之介",
	"知念 敏輝",
	"大村 彩斗",
	"眞喜志 颯王",
	"",
	"Special Thanks",
	"プレイヤーの皆様",
	"",
	"",
	"",
	"THANK YOU FOR PLAYING"
	};

	if (PlaySoundMem(BGM, DX_PLAYTYPE_LOOP) == -1)
	{
		MessageBoxA(NULL, "BGM1の再生に失敗しました", "エラー", MB_OK);
	}

}

// 更新処理
eSceneType End::Update(const float& delta_second)
{
	// 入力情報を取得
	InputManager* input = InputManager::GetInstance();

	if (input->GetButtonState(XINPUT_BUTTON_START) == eInputState::Released)
	{
		DeleteSoundMem(SoundHandle);
		return eSceneType::title;
	}

	time++;

	const int scrollStartTime = 300;

	if (time > scrollStartTime)
	{
		// Aボタン押しながらで加速する仕様
		if (input->GetButtonState(XINPUT_BUTTON_A) == eInputState::Hold)
		{
			y--;
			time++;
		}

		//通常スクロール(3フレームごとに１ピクセル)
		if (time % 3 == 0)
		{
			y--;
		}
	}

	time++;

	const int lineHeight = 80;
	const int totalScrollLength = static_cast<int>(creditText.size()) * 50 + 720;	//lineHeight=50

	if (y < -((int)creditText.size() * lineHeight + 200))
	{
		Application* app = Application::GetInstance();
		app->QuitGame();
	}

	return GetNowSceneType();
}

// 描画処理
void End::Draw() const
{
	DrawExtendGraph((int)(0), (int)(0), (int)(1280), (int)(720), background, TRUE);

	const int startY = y; //現在のスクロールオフセット
	const int lineHeight = 80;
	const int centerX = 600; //画面中心X（適度調整）

	SetFontThickness(6);

	////フェーズの制御
	//if (time < 300) // 秒程度、タイトル表示のみ
	//{
	//	SetFontSize(80);
	//	const char* titleLine1 = "Torchguard:";
	//	const char* titleLine2 = "Shadows Fall";
	//	// 中央のX位置（画面幅640pxを想定）
	//	int centerX = 640 / 2;
	//	// Y位置調整（縦に並べて中央付近に）
	//	int lineSpacing = 20;
	//	int fontHeight = 80; // フォントサイズと同じ
	//	int totalHeight = fontHeight * 2 + lineSpacing;
	//	int startY = 300 - totalHeight / 2;
	//	// 1行目
	//	DrawString(
	//		centerX - GetDrawStringWidth(titleLine1, strlen(titleLine1)) / 2,
	//		startY,
	//		titleLine1,
	//		GetColor(255, 255, 255));
	//	// 2行目
	//	DrawString(
	//		centerX - GetDrawStringWidth(titleLine2, strlen(titleLine2)) / 2,
	//		startY + fontHeight + lineSpacing,
	//		titleLine2,
	//		GetColor(255, 255, 255));
	//	return;
	//}

	//スクロールクレジット
	SetFontSize(64);

	for (size_t i = 0; i < creditText.size(); ++i)
	{
		int drawY = y + static_cast<int>(i * lineHeight);
		const std::string& text = creditText[i];

		//デフォルトサイズと色
		int fontsize = 64;
		int color = GetColor(255, 255, 255);

		//特定の行だけサイズを大きくする
		if (text == "Torchguard" || text == "Shadows Fall")
		{
			fontsize = 100;
		}

		if (text == "THANK YOU FOR PLAYING")
		{
			fontsize = 100;
		}
		//フォント設定
		SetFontSize(fontsize);
		SetFontThickness(6);

		//表示領域内なら描画
		if (drawY > -lineHeight && drawY < 720 + lineHeight)
		{
			DrawString(centerX - GetDrawStringWidth(text.c_str(), text.length()) / 2,
				drawY,
				text.c_str(),
				color);
		}

		//最後の行（THANK YOU）だけ特別に
		//bool isLastLine = (i == creditText.size() - 1);
		//if (isLastLine && y < -((int)creditText.size() - 5) * lineHeight)
		//{
		//	//最後の行が上に着たら中央に静止表示
		//	SetFontSize(64);
		//	DrawString(centerX - GetDrawStringWidth(creditText[i].c_str(), creditText[i].size()) / 2,
		//		300,
		//		creditText[i].c_str(),
		//		GetColor(255, 255, 255));
		//}
		//else
		//{
		//	int drawY = startY + static_cast<int>(i * lineHeight);
		//	if (drawY > -lineHeight && drawY < 720 + lineHeight)
		//	{
		//		DrawString(centerX - GetDrawStringWidth(creditText[i].c_str(), creditText[i].size()) / 2,
		//			drawY,
		//			creditText[i].c_str(),
		//			GetColor(255, 255, 255));
		//	}
		//}
	}
	__super::Draw();
}

// 終了処理
void End::Finalize()
{
	StopSoundMem(BGM);

	// 親クラスの終了時処理を呼び出す
	__super::Finalize();
}

// 現在のシーンタイプ取得処理
const eSceneType End::GetNowSceneType() const
{
	return eSceneType::end;
}