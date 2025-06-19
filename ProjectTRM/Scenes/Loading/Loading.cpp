#include "Loading.h"

// コンストラクタ
Loading::Loading():
	count(),
	size(),
	is_complete(false),
	next_scene(nullptr)
{

}

// デストラクタ
Loading::~Loading()
{

}

// 初期化処理
void Loading::Initialize()
{
	count = 0;
	size = load_list.size();
	is_complete = false;
}

// 更新処理
eSceneType Loading::Update(const float& delta_second)
{
	// 1フレームごとに1つずつロード
	if (count < size)
	{
		// リソースマネージャーのポインタ
		ResourceManager* rm = ResourceManager::GetInstance();
		// 読み込むファイル
		LoadItem item = load_list[count];

		// 画像の読み込み
		if (item.type == "image")
		{
			// ファイルパス
			std::string Path = "Resource/Images/" + item.path + ".png";

			if (!rm->GetImages(Path.c_str())[0])
			{
				// エラー処理
				MessageBoxA(NULL, "画像の読込みに失敗しました", "エラー", MB_OK);
			}
			
		}
		// 音源の読み込み
		else if (item.type == "sound")
		{
			// ファイルパス
			std::string Path = "Resource/Sounds/" + item.path + ".mp3";

			if (!rm->GetSounds(Path.c_str()))
			{
				// エラー処理
				MessageBoxA(NULL, "音源の読込みに失敗しました", "エラー", MB_OK);
			}
		}

		count++;
	}
	else if (!is_complete)
	{
		// ロード完了
		is_complete = true;

		// タイトルシーンへ遷移
		return eSceneType::title;
	}

	// 現在のシーン情報を返却する
	return GetNowSceneType();
}

// 描画処理
void Loading::Draw() const
{
	// UI描画
	DrawString(300, 200, "Now Loading...", GetColor(255, 255, 255));

	// ゲージバーの描画
	float rate = (float)count / size;
	int barWidth = 400;
	int barHeight = 20;
	int x = 200;
	int y = 300;
	DrawBox(x, y, x + barWidth, y + barHeight, GetColor(100, 100, 100), TRUE);         // 背景
	DrawBox(x, y, x + (int)(barWidth * rate), y + barHeight, GetColor(0, 255, 0), TRUE); // 緑の進捗
	DrawBox(x, y, x + barWidth, y + barHeight, GetColor(255, 255, 255), FALSE);         // 枠線
}

// 終了処理
void Loading::Finalize()
{
	// 親クラスの終了時処理を呼び出す
	__super::Finalize();
}

// 現在のシーンタイプ取得処理
const eSceneType Loading::GetNowSceneType() const
{
	return eSceneType::loading;
}

// 次のシーンタイプ設定処理
void Loading::SetNextSceneType(SceneBase* next_scene)
{
	this->next_scene = next_scene;
}