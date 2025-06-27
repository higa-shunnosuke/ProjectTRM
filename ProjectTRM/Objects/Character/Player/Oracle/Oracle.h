#pragma once

#include "../../Character.h"
#include <chrono>

// ゲームオブジェクト基底クラス
class Oracle :public Character
{
private:
	bool summon_flag;	//召喚フラグ
	Oracle* player;			// 巫女のポインタ
	int max_Hp = 100;
	std::vector<int>EffectImage;
	int old_sun_level;
	int now_sun_level;
	bool power_up;
	int effect_image;
	bool	JustDead = false;	//死亡時アニメーションも終わって丁度死んだ
	int anime_max_count;
	float move_location;
	std::chrono::steady_clock::time_point	anime_time;			// アニメーション加算用変数
	int sounds;


public:
	//コンストラクタ
	Oracle();
	//デストラクタ
	virtual ~Oracle();

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
	virtual void OnAreaDetection(GameObject* hit_object);

public:
	/// <summary>
	/// HP管理処理
	/// </summary>
	/// <param name="hit_object">ダメージ</param>
	virtual void HPControl(float Damage);

	bool GetDead();

	/// <summary>
	/// 位置情報変更処理
	/// </summary>
	/// <param name="location">変更したい位置情報</param>
	void SetLocation(const Vector2D& location) override;

	State GetNowState() const;

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