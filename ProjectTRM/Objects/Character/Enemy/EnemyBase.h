#pragma once

#include "../Character.h"

// 敵ベースクラス
class EnemyBase :public Character
{
protected:
	int anim_max_count;		// アニメーション画像配列の要素数
	float anim_rate;		// アニメーションの早さ
	float damage_rate;		// 持続ダメージの頻度
	float recovery_time;	// 硬直時間
	float recovery_flame;	// 硬直時間の計算
	float speed;			// 歩く速さ
	float Damage;				// ダメージ
	bool old_light;			// 前回のライトフラグ

	// エフェクト
	int effect[2];								// 描画用変数
	std::vector<int> effect_image[2];			// アニメーション画像
	float effect_flame[2];						// アニメーションの早さ
	int effect_count[2];						// アニメーションのカウント
	int effect_max_count[2];					// アニメーション画像配列の要素数

	// サウンド
	int SE[3];		// 再生用変数

public:
	//コンストラクタ
	EnemyBase();
	//デストラクタ
	virtual ~EnemyBase();

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

protected:
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