#pragma once

#include "..//SceneBase.h"


#define Set_StageX  D_WIN_MAX_X + 200
#define FirstStage 1
#define SecondStage 2
#define ThardStage 3
//最後のステージ
#define FinalStage ThardStage

#define Centher D_WIN_MAX_X/2

enum Stage : int
{
	DEFAULT,
	LMOVE,
	RMOVE,
	END
};

// タイトルシーン
class StageSelect : public SceneBase
{
private:
	Stage State = Stage::DEFAULT;
	int Stage_Image[3];
	int BackGroued_Image;
	int SerectStage = 1;

	int x = 200;
	int y = 100;
	const TCHAR* StageText[3] = { "Stage 1 : 始まりの森","Stage 2 :　未開の遺跡","Stage 3 :　灯りの橋", };
	const TCHAR* StageFlabarText[3] = { "始まりの森\n闇の軍勢の斥候が襲い掛かる・・・" , "前人未到の遺跡\n古代より闇の本隊が迫る!" , "最終決戦の橋\n奴らは全力を出してくるぞ！備えろ！" };
	const TCHAR* StageSelectText[3] = {"1  ->" , "<-  2  ->" , "<-  3"};
	
	int w = x + 100;
	int h = y + 100;


	int ChangeX = Set_StageX;
	int BGM;
	int DecisionSE;
	int CursorMoveSE;
public:
	// コンストラクタ
	StageSelect();
	// デストラクタ
	virtual ~StageSelect();

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

