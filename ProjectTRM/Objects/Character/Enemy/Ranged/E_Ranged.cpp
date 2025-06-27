#include "E_Ranged.h"
#include "../../../GameObjectManager.h"
#include "E_Projectile.h"

// 敵遠距離のカウンタを初期化
size_t E_Ranged::count = 0;

// 敵遠距離の数取得処理
size_t E_Ranged::GetCount()
{
	return count;
}

// コンストラクタ
E_Ranged::E_Ranged()
{
	count++;
}

// デストラクタ
E_Ranged::~E_Ranged()
{
	count--;

}

// 初期化処理
void E_Ranged::Initialize()
{
	// 親クラスの初期化
	__super::Initialize();

	// コリジョン設定
	collision.collision_size = Vector2D(50.0f, 100.0f);
	collision.hitbox_size = Vector2D(300.0f, 120.0f);
	z_layer = 2;

	// 画像反転なし
	flip_flag = false;

	// 最初の状態を移動にする
	now_state = State::Move;

	//攻撃力初期化
	Damage = 5.0f;

	// HP初期化
	HP = 50;

	// スピードの初期化
	speed = 70.0f;

	// リカバリーの初期化
	recovery_time = 5.0f;
}

// 更新処理
void E_Ranged::Update(float delta_second)
{
	// 親クラスの更新
	__super::Update(delta_second);
}

// 描画処理
void E_Ranged::Draw() const
{
	// 画像のずれ
	Vector2D offset;
	offset.x = 10.0f;
	offset.y = -30.0f;

	// 遠距離の描画
	DrawRotaGraphF(location.x + offset.x, location.y + offset.y,
		2.0, 0.0, image, TRUE, flip_flag);

	if (now_state != State::Death)
	{
		// エフェクト１の描画
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawRotaGraphF(location.x, location.y - 75.0f,
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
void E_Ranged::Finalize()
{
	// 親クラスの終了
	__super::Finalize();
}

// 当たり判定通知処理
void E_Ranged::OnHitCollision(GameObject* hit_object)
{

}

// 攻撃範囲通知処理
void E_Ranged::OnAreaDetection(GameObject* hit_object)
{
	// 親クラスの攻撃範囲通知処理
	__super::OnAreaDetection(hit_object);
}

// 攻撃範囲通知処理
void E_Ranged::NoHit()
{
	
}

// ライト範囲通知処理
void E_Ranged::InLightRange()
{
	in_light = true;
	Damage = 2.0f;
	speed = 30.0f;
}

// ライト範囲通知処理
void E_Ranged::OutLightRange()
{
	in_light = false;
	Damage = 5.0f;
	speed = 70.0f;
}

// 攻撃処理
void E_Ranged::Attack(GameObject* hit_object)
{
	// 攻撃SE再生
	PlaySoundMem(SE[4], DX_PLAYTYPE_BACK);

	E_Projectile* arrow;
	GameObjectManager* object = GameObjectManager::GetInstance();
	arrow = object->CreateObject<E_Projectile>(Vector2D(location.x + 20.0f, location.y - 5.0f));
	arrow->SetTargetLocation(hit_object->GetLocation());
	arrow->SetDamage(Damage);
}

// 移動処理
void E_Ranged::Movement(float delta_second)
{
	// 親クラスの移動処理
	__super::Movement(delta_second);
}

// アニメーション制御処理
void E_Ranged::AnimationControl(float delta_second)
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
			animation = rm->GetImages("Resource/Images/Enemy/Ranged/E_Ranged_Idle.png", 2, 2, 1, 100, 75);
			image = animation[Anim_count];
			anim_max_count = 1;
			anim_rate = 0.3f;
			break;
		case State::Move:
			animation = rm->GetImages("Resource/Images/Enemy/Ranged/E_Ranged_Walk.png", 6, 6, 1, 100, 75);
			image = animation[Anim_count];
			anim_max_count = 5;
			anim_rate = 0.1f;
			break;
		case State::Attack:
			animation = rm->GetImages("Resource/Images/Enemy/Ranged/E_Ranged_Attack.png", 6, 6, 1, 100, 75);
			image = animation[Anim_count];
			anim_max_count = 5;
			anim_rate = 0.1f;
			on_hit = 5;
			break;
		case State::Damage:
			break;
		case State::Death:
			animation = rm->GetImages("Resource/Images/Enemy/Ranged/E_Ranged_Dead.png", 4, 4, 1, 100, 75);
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
void E_Ranged::EffectControl(float delta_second)
{
	// 親クラスのエフェクト
	__super::EffectControl(delta_second);
}