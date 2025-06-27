#pragma once

#include "../EnemyBase.h"

// ゲームオブジェクト基底クラス
class Boss :public EnemyBase
{
private:
	static size_t count;
	
public:
	/// <summary>
	/// 敵ボスのカウント取得処理
	/// </summary>
	/// <returns>敵近接の数</returns>
	static size_t GetCount();
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
	virtual void Draw() const;
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
};