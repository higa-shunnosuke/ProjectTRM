#include "End.h"
#include "../../Application.h"


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
	
	// ムービーファイルをロードします。
	MovieGraphHandle = LoadGraph("Resource/Movie/EndMov.mp4");
	SoundHandle = LoadSoundMem("Resource/Sounds/Ending/ED.mp3");

	// ムービーの再生位置を0秒目に変更します
	SeekMovieToGraph(MovieGraphHandle, 0000);

	ChangeVolumeSoundMem(255 / 2, SoundHandle);
}

// 更新処理
eSceneType End::Update(const float& delta_second)
{
	// 入力情報を取得
	InputManager* input = InputManager::GetInstance();

	PlaySoundMem(SoundHandle, DX_PLAYTYPE_LOOP);


	// ムービーを再生状態にします
	PlayMovieToGraph(MovieGraphHandle);

	if (GetMovieStateToGraph(MovieGraphHandle) == 1)
	{
		//ゲームを終了させる
		Application* app = Application::GetInstance();
		app->QuitGame();
	}

	// 親クラスの更新処理を呼び出す
	return __super::Update(delta_second);
}

// 描画処理
void End::Draw() const
{

	// ループ、GetMovieStateToGraph 関数はムービーの再生状態を得る関数です
   // 戻り値が１の間は再生状態ですのでループを続けます
	while (ProcessMessage() == 0 && GetMovieStateToGraph(MovieGraphHandle) == 1)
	{
		ClearDrawScreen();

		// ムービー映像を画面いっぱいに描画します
		DrawExtendGraph(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, MovieGraphHandle, FALSE);

		// ウエイトをかけます、あまり速く描画すると画面がちらつくからです
		WaitTimer(17);

		ScreenFlip();
	}

	// 親クラスの描画処理を呼び出す
	__super::Draw();

}

// 終了処理
void End::Finalize()
{
	// 親クラスの終了時処理を呼び出す
	__super::Finalize();
}

// 現在のシーンタイプ取得処理
const eSceneType End::GetNowSceneType() const
{
	return eSceneType::end;
}
