#pragma once

#include "../UnitBase.h"
#include "../../../../Utility/LightMapManager.h"
#include "../../../GameObjectManager.h"

#define BASIC_Tank_POWER (1.0f)
#define BASIC_Tank_SPEED (-5.0f)

// ゲームオブジェクト基底クラス
class P_Tank :public UnitBase
{
private:
	static size_t count;

public:
	static size_t GetCount();

private:
	LightMapManager* light; //明り

public:
	//コンストラクタ
	P_Tank();
	//デストラクタ
	virtual ~P_Tank();

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
	/// <summary>
	/// SE制御処理
	/// </summary>
	virtual void SoundControl();
};