#pragma once

#include "..//SceneBase.h"

enum TitleState :int
{
	SELECT_WAIT,
	SELECTED,
	USEING,
	FINAL
};

#define MAX_SELECT 1

// タイトルシーン
class Title : public SceneBase
{
private:
	TitleState State = TitleState::SELECT_WAIT;
	int		Anim_count;
	float	Anim_flame;
	int Selected = 0;
	int Y;

	int MovieHandle;

	int BackGroundImage;
	int SelectBackGroundImage;
	int BackBackGroundImage;
	int Select_End;
	int Default_End;
	int Select_Start;
	int Default_Start;
	std::vector<int> SellectImage;
	int BGM;
	int DecisionSE;
public:
	// コンストラクタ
	Title();
	// デストラクタ
	virtual ~Title();

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

