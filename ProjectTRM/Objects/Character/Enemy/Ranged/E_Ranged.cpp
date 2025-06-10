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

	// フラグ設定
	is_mobility = true;
	is_aggressive = true;

	// コリジョン設定
	collision.is_blocking = true;
	collision.object_type = eObjectType::Enemy;
	collision.hit_object_type.push_back(eObjectType::Player);
	collision.collision_size = Vector2D(50.0f, 100.0f);
	collision.hitbox_size = Vector2D(300.0f, 120.0f);
	z_layer = 2;

	// 画像反転なし
	flip_flag = false;

	// 最初の状態を移動にする
	now_state = State::Move;

	//攻撃力初期化
	Damage = 5;

	// HP初期化
	HP = 20;

	// スピードの初期化
	speed = 70.0f;
}

// 更新処理
void E_Ranged::Update(float delta_second)
{
	// 親クラスの更新
	__super::Update(delta_second);
}

// 描画処理
void E_Ranged::Draw(const Vector2D camera_pos) const
{
	// 画像のずれ
	Vector2D offset;
	offset.x = 10.0f;
	offset.y = -30.0f;

	// カメラ座標をもとに描画位置を計算
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// 遠距離の描画
	DrawRotaGraphF(position.x + offset.x, position.y + offset.y,
		2.0, 0.0, image, TRUE, flip_flag);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawRotaGraphF(position.x, position.y - 75.0f,
		3.0, 0.0, effect, TRUE, flip_flag);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 親クラスの描画
	__super::Draw(camera_pos);
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
	Damage = 2;
	speed = 30.0f;
}

// ライト範囲通知処理
void E_Ranged::OutLightRange()
{
	in_light = false;
	Damage = 5;
	speed = 70.0f;
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