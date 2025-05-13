#pragma once

#include "../../Character.h"

#define GRAVITY (9.8f)
#define INITIAL_SPEED (100.0f)

// ゲームオブジェクト基底クラス
class P_Projectile :public Character
{
private:
	int Damage;	//ダメージ
	Vector2D target_location;
	Vector2D old_location;
	float angle;

public:
	//コンストラクタ
	P_Projectile();
	//デストラクタ
	virtual ~P_Projectile();

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

private:
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

public:
	void SetTargetLocation(Vector2D location);

};