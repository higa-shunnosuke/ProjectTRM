#pragma once

#include "../../Character.h"

#include <chrono>

#define Enemy_Think_Standard
#define ENEMY_BOTTOM_COST 50

#ifdef Enemy_Think_Standard//基本的な教本

#define Guardian_eva	50
#define TANK_eva		10
#define MELEE_eva		30
#define RANGE_eva		20
#define BOSS_eva		50

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
	State nowsta = State::Idle;									//現在の状態
	std::vector<int> EffectImage;								//召喚エフェクト
	int EffectCount;											//エフェクトの添え字
	float HPColor;												//残HPにより、色を変更する
	std::vector<int> DeadImage;									//死亡時のアニメーション
	std::vector<int> AppearImage;								//登場時のアニメーション
	int SpotLight;												//死亡時に上からの光の画像
	std::chrono::steady_clock::time_point	prev_time;			// コスト加算用変数
	std::chrono::steady_clock::time_point	rush_time;			// コスト加算用変数
	std::chrono::steady_clock::time_point	efect_time;			// エフェクト加算用変数
	std::chrono::steady_clock::time_point	anime_time;			// アニメーション加算用変数
	float Cost;													//現在のコスト
	bool Summon_Boss = false;	//ボス1度でも生成したかどうか
	bool	JustDead = false;	//死亡時アニメーションも終わって丁度死んだ
	bool	Iam_Dead = false;	//死亡アニメーションを再生するかどうか
	bool	Time_rush = false;	//ラッシュする時間
	bool	summon_flag;	//召喚フラグ
	bool	summon_effect;	//召喚フラグ
	std::vector<GameObject*> ObjectList;	//ゲーム上に何がいるのかを保有

	int RushTimer;				//実際に何秒間ラッシュするのか
	short int old_EnemySum;		//古い敵の出現数

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
	virtual void Draw() const;
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
	/// <summary>
	/// 第１ステージの際の簡単な処理
	/// </summary>
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
	virtual void HPControl(float Damage);

	/// <summary>
/// エネミーが死んだかどうか
/// </summary>
	bool GetDead();

	/// <summary>
/// エネミーの召喚処理
/// </summary>
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