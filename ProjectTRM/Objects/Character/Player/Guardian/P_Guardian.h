#pragma once
#include "../../Character.h"
#include "../../../GameObjectManager.h"

#define BASIC_Guardian_POWER (20)
#define BASIC_Guardian_SPEED (-4.0f)

class P_Guardian : public Character
{
private:
	static size_t count;

public:
	static size_t GetCount();

private:
	std::vector<int> Effect;
	int effect_image;
	int Damage;	//ダメージ
	GameObjectManager* object;
	int anim_max_count;		// アニメーション画像の枚数
	int sounds;
	int effect_alpha;	//エフェクトの透明度調整用
	int effect_max_count;
	std::vector<int> text;
	std::vector<int> reduction_amount;	//HP減少量
	std::vector<float> damage_time; //ダメージの表記時間
	int minus;

public:
	//コンストラクタ
	P_Guardian();
	//デストラクタ
	virtual ~P_Guardian();

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
	/// 攻撃範囲通知処理
	/// </summary>
	virtual void NoHit() override;

	/// <summary>
	/// HP管理処理
	/// </summary>
	/// <param name="hit_object">ダメージ</param>
	virtual void HPControl(float Damage) override;

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

	/// <summary>
	/// SE制御処理
	/// </summary>
	virtual void SoundControl();
};

