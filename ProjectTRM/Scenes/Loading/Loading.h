#pragma once

#include "../SceneBase.h"

struct LoadItem {
	std::string type;	// "image" or "sound"
	std::string path;	// ファイルパス
};

// ローディングシーン
class Loading :public SceneBase
{
private:
	SceneBase* next_scene;	// 次のシーン情報
	int count;				// 読み込んだアイテムの数
	int size;				// 読み込むアイテムの数
	bool is_complete;		// 読み込み完了フラグ

	// ロードリスト
	std::vector<LoadItem> load_list = {
		// タイトル
		{"image","BackGround/Title"},
		// ステージセレクト
		{"image","BackGround/BlueMoonUI"},
		{"image","BackGround/YelloMoonUI"},
		{"image","BackGround/RedMoonUI"},
		{"image","BackGround/StageSelect"},
		// インゲーム
		{"image","Unit/Tank/Tank_Cost_New"},
		{"image","Unit/Melee/Melee_Cost_New"},
		{"image","Unit/Ranged/Ranged_Cost"},
		{"image","Unit/Guardian/Guardian_Cost"},
		{"image","BackGround/Sun"},
		{"image","BackGround/BlueMoon"},
		{"image","BackGround/YelloMoon"},
		{"image","BackGround/RedMoon"},
		{"image","BackGround/text_fail"},
		{"image","BackGround/text_clear"},
		{"image","BackGround/Text"},
		{"image","BackGround/TextLeft"},
		{"image","BackGround/cost_base"},
		// リザルト
		{"image","BackGround/Result"},
		// タイトル
		{"sound","Decision"},
		{"sound","Title/BGM/OP"},
		// ステージセレクト
		{"sound","StageSelect/CursorMove"},
		{"sound","StageSelect/BGM/StageSelect"},
		// インゲーム
		{"sound","InGame/Click"},
		{"sound","InGame/ClickUp"},
		{"sound","Ingame/BGM/Stage1"},
		{"sound","Ingame/BGM/Stage2"},
		{"sound","Ingame/BGM/Stage3"},
		{"sound","Ingame/SummonAllies"},
		{"sound","Ingame/SummonEnemy"},
		// リザルト
		{"sound","Result/Win_BGM"},
		{"sound","Result/Loose_BGM"},
		{"sound","Result/CursorMove"},
		{"sound","Result/Lost"},
		{"sound","Result/WINSE"},
		{"sound","Result/CountDown"},
		// エンド
		{"sound","Ending/ED"},

	};

public:
	// コンストラクタ
	Loading();
	// デストラクタ
	virtual ~Loading();

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Initialize() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="delta_second">1フレーム当たりの時間</param>
	/// <returns>次のシーンタイプ情報</returns>
	virtual eSceneType Update(const float& delta_second) override;

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() const override;

	/// <summary>
	/// 終了時処理
	/// </summary>
	virtual void Finalize() override;

	/// <summary>
	/// 現在のシーンタイプ情報を取得する
	/// </summary>
	/// <returns>現在のシーンタイプ</returns>
	virtual const eSceneType GetNowSceneType() const override;

	/// <summary>
	/// 次のシーンタイプ設定処理
	/// </summary>
	void SetNextSceneType(SceneBase* next_scene);
};

