#include "P_Ranged.h"
#include "Projectile.h"
#include "../../../GameObjectManager.h"
size_t P_Ranged::count = 0;
size_t P_Ranged::GetCount()
{
	return count;
}

// コンストラクタ
P_Ranged::P_Ranged()
{

}

// デストラクタ
P_Ranged::~P_Ranged()
{

}

// 初期化処理
void P_Ranged::Initialize()
{
	// 画像の読み込み
	ResourceManager* rm = ResourceManager::GetInstance();
	animation = rm->GetImages("Resource/Images/Unit/Ranged/Ranged_Walk.png", 4, 4, 1, 32, 32);

	is_mobility = true;
	is_aggressive = true;

	collision.is_blocking = true;
	collision.object_type = eObjectType::Player;
	collision.hit_object_type.push_back(eObjectType::Enemy);
	collision.box_size = Vector2D(60.0f, 60.0f);
	collision.attack_size = Vector2D(100.0f, 100.0f);
	z_layer = 2;

	attack_flag = false;
	flip_flag = true;

	now_state = State::Move;

	// HP初期化
	HP = 10;
	old_HP = HP;

	alpha = MAX_ALPHA;
	add = -ALPHA_ADD;
}

// 更新処理
void P_Ranged::Update(float delta_second)
{
	//移動処理
	Movement(delta_second);


	if (attack_flag == true)
	{
		if (velocity.x < 0.0f)
		{
			now_state = State::Move;
			attack_flame = 0.0f;
		}
		else
		{
			attack_flame -= delta_second;
			now_state = State::Idle;
		}
		if (attack_flame <= 0.0f)
		{
			attack_flag = false;
		}
	}
	if (old_HP != HP)
	{
		now_state = State::Damage;
		dmage_flame = 1.0f;
	}

	AnimationControl(delta_second);

	dmage_flame -= delta_second;

	if (dmage_flame <= 0.0f)
	{
		dmage_flame = 0.0f;
		alpha = MAX_ALPHA;
		add = -ALPHA_ADD;
	}

	old_state = now_state;
	old_HP = HP;
}

// 描画処理
void P_Ranged::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// 近接ユニットの描画
		// オフセット値を基に画像の描画を行う
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawRotaGraphF(position.x, position.y, 2.0, 0.0, image, TRUE, flip_flag);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	/*DrawBox((int)(position.x - collision.box_size.x / 2), (int)(position.y - collision.box_size.y / 2),
		(int)(position.x + collision.box_size.x / 2), (int)(position.y + collision.box_size.y / 2), 0xffa000, TRUE);*/

#ifdef DEBUG
	// 中心を表示
	DrawCircle((int)position.x, (int)position.y, 2, 0x0000ff, TRUE);
	// 当たり判定表示
	DrawBox((int)(position.x - collision.box_size.x / 2), (int)(position.y - collision.box_size.y / 2),
		(int)(position.x + collision.box_size.x / 2), (int)(position.y + collision.box_size.y / 2), 0x0000ff, FALSE);
	// 攻撃範囲を表示
	DrawBox((int)(position.x - collision.attack_size.x / 2), (int)(position.y - collision.attack_size.y / 2),
		(int)(position.x + collision.attack_size.x / 2), (int)(position.y + collision.attack_size.y / 2), 0x0000ff, FALSE);
#endif
}

// 終了時処理
void P_Ranged::Finalize()
{

}

// 当たり判定通知処理
void P_Ranged::OnHitCollision(GameObject* hit_object)
{

}

// 攻撃範囲通知処理
void P_Ranged::OnAreaDetection(GameObject* hit_object)
{
	Collision hit_col = hit_object->GetCollision();

	if (hit_col.object_type == eObjectType::Enemy)
	{
		velocity.x = 0.0f;
		if (attack_flag == false)
		{
			now_state = State::Attack;
		}
		else
		{
			if (attack_flame <= 0.0f)
			{
				Attack(hit_object);
			}
		}
	}
	else
	{
		velocity.x = -5.0f;
	}
}

// 移動処理
void P_Ranged::Movement(float delta_second)
{
	// 移動の実行
	location.x += velocity.x * 10 * delta_second;
}

// アニメーション制御処理
void P_Ranged::AnimationControl(float delta_second)
{
	// 画像の読み込み
	ResourceManager* rm = ResourceManager::GetInstance();

	//ステートが変わった時の初期化
	if (old_state != now_state)
	{
		Anim_flame = 0;
		Anim_count = 0;
		count = 1;
	}

	Anim_flame += delta_second;

	if (Anim_flame >= 0.1f)
	{
		Anim_count += count;

		if (Anim_count <= 0 || Anim_count >= 2)
		{
			count *= -1;
		}
		Anim_flame = 0.0f;
	}
	switch (now_state)
	{
	case State::Idle:
		if (Anim_count > 0)
		{
			image = animation[0];
		}
		break;
	case State::Move:
		animation = rm->GetImages("Resource/Images/Unit/Ranged/Ranged_Walk.png", 4, 4, 1, 32, 32);
		image = animation[1 + Anim_count];
		break;
	case State::Attack:
		animation = rm->GetImages("Resource/Images/Unit/Ranged/Ranged_Attack.png", 4, 4, 1, 32, 32);
		image = animation[1 + Anim_count];
		if (Anim_count >= 2)
		{
			attack_flag = true;
		}
		break;
	case State::Damage:
		alpha += add;
		if (alpha <= 0 || alpha >= 255)
		{
			add = -add;
		}
		image = animation[0];
		break;
	case State::Death:
		break;
	default:
		break;
	}

}
// エフェクト制御処理
void P_Ranged::EffectControl(float delta_second)
{

}

// 攻撃処理
void P_Ranged::Attack(GameObject* hit_object)
{
	attack_flame = 2.0f;
}