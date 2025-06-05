#pragma once

#include "../../Character.h"

#include <chrono>

#define Enemy_Think_Standard
#define ENEMY_BOTTOM_COST 50

#ifdef Enemy_Think_Standard//基本的な教本

#define TANK_eva	 10
#define MELEE_eva	 30
#define RANGE_eva	 20
#define BOSS_eva	 50

#define TANK_cost	 10
#define MELEE_cost	 30
#define RANGE_cost	 50
#define BOSS_cost	 150


#endif // Enemy_Think_Standard



enum E_enemy :int
{
	Tank,
	Melee,
	Range,
	Boss
};

// ゲームオブジェクト基底クラス
class Heretic :public Character
{
private:
	State nowsta = State::Idle;
	std::vector<int> EffectImage;
	int DeadImage[2];
	int SoptLight;
	int LightStrong = 0 ;
	std::chrono::steady_clock::time_point	prev_time;			// コスト加算用変数
	std::chrono::steady_clock::time_point	rush_time;			// コスト加算用変数
	std::chrono::steady_clock::time_point	efect_time;			// エフェクト加算用変数
	int Player_evaluation[4];
	int Enemy_evaluation[3];
	int Cost;
	bool Summon_Boss = false;	//ボス1度でも生成したかどうか
	bool	JustDead = false;	//死亡時アニメーションも終わって丁度死んだ
	bool	Iam_Dead = false;
	bool	Time_rush = false;;
	bool	summon_flag;	//召喚フラグ
	bool	summon_effect;	//召喚フラグ
	std::vector<GameObject*> ObjectList;

	short int old_EnemySum;

	class	InGame* Ingame;	//InGameのポインター
	float	CountFlame;		//flameの管理
	int		CountTime;		//整数型での管理

public:



	//コンストラクタ
	Heretic();
	//デストラクタ
	virtual ~Heretic();

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Initialize();
	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="delta_second">1フレームあたりの時間</param>
	virtual void Update(float delta_second);
	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw(const Vector2D camera_pos) const;
	/// <summary>
	/// 終了時処理
	/// </summary>
	virtual void Finalize();
	/// <summary>
	/// エネミー生成処理
	/// </summary>
	/// <param name = "point">: InGameSceneのPointer</param>
	void SetInGamePoint(class InGame* point);
	/// <summary>
	/// エネミーの思案処理
	/// </summary>
	void ThinkingEnemy();
	void FirstStageEnemy();


public:
	/// <summary>
	/// 当たり判定通知処理
	/// </summary>
	/// <param name="hit_object">当たったゲームオブジェクトのポインタ</param>
	virtual void OnHitCollision(GameObject* hit_object);

public:
	/// <summary>
	/// HP管理処理
	/// </summary>
	/// <param name="hit_object">ダメージ</param>
	virtual void HPControl(int Damage);

	bool GetDead();


	void SamonEnemy(int e_enem);

private:
	/// <summary>
	/// 攻撃処理
	/// </summary>
	virtual void Attack();
	/// <summary>
	/// 移動処理
	/// </summary>
	/// <param name="hit_object">1フレームあたりの時間</param>
	virtual void Movement(float delta_second);
	/// <summary>
	/// アニメーション制御処理
	/// </summary>
	/// <param name="hit_object">1フレームあたりの時間</param>
	virtual void AnimationControl(float delta_second);
	/// <summary>
	/// エフェクト制御処理
	/// </summary>
	/// <param name="hit_object">1フレームあたりの時間</param>
	virtual void EffectControl(float delta_second);
};