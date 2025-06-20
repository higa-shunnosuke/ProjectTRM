#pragma once

#include "../SceneBase.h"
#include <chrono>

// リザルトシーン
class Result : public SceneBase
{
private:
	int count = 10;
	int cursor = -1;
	bool show = false;
	std::chrono::steady_clock::time_point	prev_time;				// 加算用変数
	std::chrono::steady_clock::time_point	select_time;			// 選択用変数
	std::chrono::steady_clock::time_point	choice_time = std::chrono::steady_clock::now();			// 選択用変数
	int bgmHandle[4];				// BGM
	int CursorMoveSE;
	int DecisionSE;
	bool isDecision = false;
	eSceneType nextScene=eSceneType::result;
	int decisionWaitFrame = 0;
	int CountdownSE;
	int BackGroued_Image;
public:
	// コンストラクタ
	Result();
	// デストラクタ
	virtual ~Result();

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
};

