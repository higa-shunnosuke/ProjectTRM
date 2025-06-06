#pragma once

#include "../SceneBase.h"

#include "../../Objects/Character/Player/Oracle/Oracle.h"
#include "../../Objects/Character/Enemy/Hertic/Heretic.h"

#include <chrono>

#define Cost_UpNum 5
#define Tank_Cost 20
#define Melee_Cost 50
#define Range_Cost 100


enum class GameState
{
	PLAYING,
	BOSS_DEAD,
	CLEAR
};


class InGame : public SceneBase
{
private:

	GameState state = GameState::PLAYING;
	std::chrono::steady_clock::time_point	summon_time[3];		// 召喚開始時間
	std::chrono::seconds					cooldown[3];		// 召喚クールダウン
	std::chrono::steady_clock::time_point	prev_time;			// コスト加算用変数
	int SunImages[10];
	int BackGroundImage[3];
	int sound;
	int Click;
	int ClickUp;
	int Cost_Max;
	int Sun_Level;
	float move_camera;
	Vector2D old_camerapos;
	int			cost;				// コスト
	bool		summon_flag[3];		// 召喚フラグ
	Oracle*		player;				// 巫女のポインタ
	Heretic*	enemy;				// 異端者のポインタ
	int			unit_ui[4];			// ユニット選択UIの画像
	int bgmHandle[4];				// BGM

public:
	// コンストラクタ
	InGame();
	// デストラクタ
	virtual ~InGame();

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
	/// 敵生成処理
	/// </summary>
	/// <param name="e_enem">敵の種類</param>
	void CreateEnemy(E_enemy e_enem);

	/// <summary>
	/// 太陽のレベル取得処理
	/// </summary>
	/// <returns></returns>
	const int GetSunLevel() const;

private:
	/// <summary>
	/// ステージの生成処理
	/// </summary>
	void LoadStage();

	/// <summary>
	///	ユニット追召喚
	/// </summary>
	void UnitSelection();

	/// <summary>
	///	コスト管理処理
	/// </summary>
	void RegenerateCost();

	/// <summary>
	/// 画像読込み処理
	/// </summary>
	/// <returns></returns>
	const void LoadImages() const;
};
