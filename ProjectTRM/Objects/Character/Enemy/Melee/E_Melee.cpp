#include "E_Melee.h"
#include "../../../GameObjectManager.h"

// 敵近接のカウンタを初期化
size_t E_Melee::count = 0;

// 敵近接の数取得処理
size_t E_Melee::GetCount()
{
	return count;
}

// コンストラクタ
E_Melee::E_Melee()
{
	count++;
}

// デストラクタ
E_Melee::~E_Melee()
{
	count--;
}

// 初期化処理
void E_Melee::Initialize()
{
	// 親クラスの初期化
	__super::Initialize();

	// フラグ設定
	is_AoE = true;

	// コリジョン設定
	collision.collision_size = Vector2D(50.0f, 100.0f);
	collision.hitbox_size = Vector2D(90.0f, 120.0f);
	z_layer = 2;

	// 画像反転なし
	flip_flag = false;

	// 最初の状態を移動にする
	now_state = State::Move;

	//攻撃力
	Damage = 4.0f;

	// HP初期化
	HP = 20;

	// スピードの初期化
	speed = 80.0f;

	// リカバリーの初期化
	recovery_time = 1.5f;
}

// 更新処理
void E_Melee::Update(float delta_second)
{
	// 親クラスの更新
	__super::Update(delta_second);
}

// 描画処理
void E_Melee::Draw() const
{
	// 画像のずれ
	Vector2D offset;
	offset.x = 30.0f;
	offset.y = -25.0f;

	// 敵近接の描画
	DrawRotaGraphF(location.x + offset.x, location.y + offset.y,
		2.0, 0.0, image, TRUE, flip_flag);

	if (now_state != State::Death)
	{
		// エフェクト１の描画
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawRotaGraphF(location.x, location.y - 70.0f,
			3.0, 0.0, effect[0], TRUE, flip_flag);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// エフェクト２の描画
		DrawRotaGraphF(location.x, location.y + 5.0f,
			0.7, 0.0, effect[1], TRUE, flip_flag);
	}
	
	// 親クラスの描画
	__super::Draw();
}

// 終了時処理
void E_Melee::Finalize()
{
	// 親クラスの終了
	__super::Finalize();
}

// 当たり判定通知処理
void E_Melee::OnHitCollision(GameObject* hit_object)
{

}

// 攻撃範囲通知処理
void E_Melee::OnAreaDetection(GameObject* hit_object)
{
	// 親クラスの攻撃範囲通知処理
	__super::OnAreaDetection(hit_object);
}

// 攻撃範囲通知処理
void E_Melee::NoHit()
{
	
}

// ライト範囲通知処理
void E_Melee::InLightRange()
{
	in_light = true;
	Damage = 2.0f;
	speed = 40.0f;
}

// ライト範囲通知処理
void E_Melee::OutLightRange()
{
	in_light = false;
	Damage = 4.0f;
	speed = 80.0f;
}

// 移動処理
void E_Melee::Movement(float delta_second)
{
	// 親クラスの移動処理
	__super::Movement(delta_second);
}

// アニメーション制御処理
void E_Melee::AnimationControl(float delta_second)
{
	// 状態が切り替わったらカウントを初期化
	if (old_state != now_state)
	{
		Anim_count = 0;

		// 画像の読み込み
		ResourceManager* rm = ResourceManager::GetInstance();

		// 各状態のアニメーション画像に差し替え
		switch (now_state)
		{
		case State::Idle:
			animation = rm->GetImages("Resource/Images/Enemy/Melee/E_Melee_Idle.png", 4, 4, 1, 100, 75);
			image = animation[Anim_count];
			anim_max_count = 3;
			anim_rate = 0.3f;
			break;
		case State::Move:
			animation = rm->GetImages("Resource/Images/Enemy/Melee/E_Melee_Walk.png", 6, 6, 1, 100, 75);
			image = animation[Anim_count];
			anim_max_count = 5;
			anim_rate = 0.1f;
			break;
		case State::Attack:
			animation = rm->GetImages("Resource/Images/Enemy/Melee/E_Melee_Attack.png", 8, 8, 1, 100, 75);
			image = animation[Anim_count];
			anim_max_count = 7;
			anim_rate = 0.1f;
			on_hit = 4;
			break;
		case State::Damage:
			break;
		case State::Death:
			animation = rm->GetImages("Resource/Images/Enemy/Melee/E_Melee_Dead.png", 4, 4, 1, 100, 75);
			image = animation[Anim_count];
			anim_max_count = 3;
			anim_rate = 0.2f;
			break;
		}
	}

	// 親クラスのアニメーション]
	__super::AnimationControl(delta_second);
}
// エフェクト制御処理
void E_Melee::EffectControl(float delta_second)
{
	// 親クラスのエフェクト
	__super::EffectControl(delta_second);
}
