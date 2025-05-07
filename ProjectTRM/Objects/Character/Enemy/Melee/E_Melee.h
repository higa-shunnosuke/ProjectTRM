#pragma once

#include "../../Character.h"
#include <chrono>

// ゲームオブジェクト基底クラス
class E_Melee :public Character
{
private:
	static size_t count;	// 敵近接のカウンタ
	std::chrono::steady_clock::time_point anime_time;	// アニメーション開始時間
	std::chrono::steady_clock::time_point recovery_time;	// 硬直開始時間

public:
	/// <summary>
	/// 敵近接のカウント取得処理
	/// </summary>
	/// <returns>敵近接の数</returns>
	static size_t GetCount();

private:
	int Damage;	//ダメージ
	int old_HP;

public:
	//コンストラクタ
	E_Melee();
	//デストラクタ
	virtual ~E_Melee();

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

public:
	/// <summary>
	/// 当たり判定通知処理
	/// </summary>
	/// <param name="hit_object">当たったゲームオブジェクトのポインタ</param>
	virtual void OnHitCollision(GameObject* hit_object);

	/// <summary>
	/// 攻撃範囲通知処理
	/// </summary>
	/// <param name="hit_object">当たったゲームオブジェクトのポインタ</param>
	virtual void OnAreaDetection(GameObject* hit_object);

	/// <summary>
	/// HP管理処理
	/// </summary>
	/// <param name="hit_object">ダメージ</param>
	virtual void HPControl(int Damage) override;

private:
	/// <summary>
	/// 攻撃処理
	/// </summary>
	virtual void Attack(GameObject* hit_object);
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