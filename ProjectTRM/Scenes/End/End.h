#pragma once

#include "..//SceneBase.h"
#include <chrono>

// タイトルシーン
class End : public SceneBase
{
private:
	int background;
	int BGM;
	int y = 700;
	int time = 0;
	std::vector<std::string> creditText;
	int titleImage;
	int imageWidth;
	int imageHeight;
public:
	// コンストラクタ
	End();

	int MovieGraphHandle;
	int SoundHandle;
	// デストラクタ
	virtual ~End();

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

