#pragma once

#include "../../Character.h"
#include <chrono>

// 敵近接クラス
class E_Melee :public Character
{
private:
	static size_t count;	// 敵近接のカウンタ
	int anim_max_count;		// アニメーション画像の枚数
	float recovery_time;	// 硬直時間
	float damage_rate;		// 持続ダメージの頻度
	float anim_rate;		// アニメーションの早さ
	float speed;			// 歩く速さ

public:
	/// <summary>
	/// 敵近接のカウント取得処理
	/// </summary>
	/// <returns>敵近接の数</returns>
	static size_t GetCount();

private:
	int Damage;	//ダメージ

public:
	//コンストラクタ
	E_Melee();
	//デストラクタ
	virtual ~E_Melee();

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Initialize() override;
	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="delta_second">1フレームあたりの時間</param>
	virtual void Update(float delta_second) override;
	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw(const Vector2D camera_pos) const override;
	/// <summary>
	/// 終了時処理
	/// </summary>
	virtual void Finalize() override;

public:
	/// <summary>
	/// 当たり判定通知処理
	/// </summary>
	/// <param name="hit_object">当たったゲームオブジェクトのポインタ</param>
	virtual void OnHitCollision(GameObject* hit_object) override;
	/// <summary>
	/// 攻撃範囲通知処理
	/// </summary>
	/// <param name="hit_object">当たったゲームオブジェクトのポインタ</param>
	virtual void OnAreaDetection(GameObject* hit_object) override;
	/// <summary>
	/// 攻撃範囲通知処理
	/// </summary>
	virtual void NoHit() override;
	/// <summary>
	/// ライト範囲通知処理
	/// </summary>
	virtual void InLightRange() override;
	/// <summary>
	/// ライト範囲通知処理
	/// </summary>
	virtual void OutLightRange() override;
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