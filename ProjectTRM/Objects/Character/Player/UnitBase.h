#pragma once

#include "../Character.h"
#include "../../GameObjectManager.h"
#include "../../../Utility/LightMapManager.h"
#include "../../../Scenes/InGame/InGame.h"

class UnitBase : public Character
{
protected:
	int Damage;	//ダメージ
	int sounds;	//音
	float basic_power;	//基礎攻撃力
	float basic_speed;	//基礎移動速度
	LightMapManager* light; //明り
	int anim_max_count;
	GameObjectManager* object;

	//エフェクト類
	int effect_image;
	int effect_alpha;
	std::vector<int> Effect;
	int effect_max_count;	//エフェクト画像の枚数

	//ダメージ表記関連
	std::vector<int> text;
	std::vector<int> reduction_amount;	//HP減少量
	std::vector<float> damage_time; //ダメージの表記時間
	int minus;

public:
	//コンストラクタ
	UnitBase();
	//デストラクタ
	virtual ~UnitBase();

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

	/// <summary>
	/// SE制御処理
	/// </summary>
	virtual void SoundControl();
};

