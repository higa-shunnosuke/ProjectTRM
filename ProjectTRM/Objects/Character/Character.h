#pragma once

#include "../GameObject.h"
#include "string"
#include "vector"



enum State {
	Idle,
	Move,
	Attack,
	Damage,
	Death
};

#define MAX_ALPHA (255);
#define ALPHA_ADD (15);

// ゲームオブジェクト基底クラス
class Character:public GameObject
{
protected:
	Vector2D velocity;
	bool flip_flag;
	State now_state;
	State old_state;
	std::vector<int> effect;
	std::vector<int> animation;

	int		Anim_count = 0;
	float	Anim_flame = 0.0f;
	int     count;
	float   attack_flame = 0.0f;
	bool	attack_flag;
	float   dmage_flame = 0.0f;

	int alpha;
	int add;

public:
	//コンストラクタ
	Character();
	//デストラクタ
	virtual ~Character();

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

protected:
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
	/// 攻撃処理
	/// </summary>
	virtual void Attack(GameObject* hit_object);
};