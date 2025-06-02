#pragma once

#include "../../Character.h"

// ゲームオブジェクト基底クラス
class Boss :public Character
{
private:
	static size_t count;
	int anim_max_count;		// アニメーション画像の枚数
	float recovery_time;	// 硬直時間
	float damage_rate;		// 持続ダメージの頻度
	float anim_rate;		// アニメーションの早さ
	float speed;			// 歩く速さ
	int Damage;				// ダメージ
	bool old_light;			// 前回のライトフラグ

	// エフェクト
	int effect;
	std::vector<int> effect_image;
	float effect_flame;		// エフェクトアニメーションの早さ
	int effect_count;		// エフェクトアニメーションのカウント

public:
	//コンストラクタ
	Boss();
	//デストラクタ
	virtual ~Boss();

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

public:
	/// <summary>
	/// HP管理処理
	/// </summary>
	/// <param name="hit_object">ダメージ</param>
	virtual void HPControl(int Damage);

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