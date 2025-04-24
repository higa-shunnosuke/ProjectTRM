#include "E_Tank.h"
size_t E_Tank::count = 0;
size_t E_Tank::GetCount()
{
	return count;
}


// コンストラクタ
E_Tank::E_Tank() :
	Damage()
{

}

// デストラクタ
E_Tank::~E_Tank()
{

}

// 初期化処理
void E_Tank::Initialize()
{
	// 画像の読み込み
	ResourceManager* rm = ResourceManager::GetInstance();
	animation = rm->GetImages("Resource/Images/Enemy/Melee/Melee_Walk.png", 4, 4, 1, 32, 32);

	is_mobility = true;
	is_aggressive = true;

	collision.is_blocking = true;
	collision.object_type = eObjectType::Enemy;
	collision.hit_object_type.push_back(eObjectType::Player);
	collision.box_size = Vector2D(60.0f, 60.0f);
	collision.attack_size = Vector2D(100.0f, 100.0f);
	z_layer = 2;

	attack_flag = false;
	flip_flag = false;

	now_state = State::Move;

	// 右へ移動
	velocity.x = 5.0f;

	//攻撃力
	Damage = 8;

	// HP初期化
	HP = 1;
}

// 更新処理
void E_Tank::Update(float delta_second)
{
	// 移動処理
	Movement(delta_second);

	if (attack_flag == true)
	{
		if (velocity.x > 0.0f)
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

	AnimationControl(delta_second);

	old_state = now_state;
}

// 描画処理
void E_Tank::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// オフセット値を基に画像の描画を行う
	DrawRotaGraphF(camera_pos.x, camera_pos.y, 1.0, 0.0, image, TRUE);
}

// 終了時処理
void E_Tank::Finalize()
{

}

// 当たり判定通知処理
void E_Tank::OnHitCollision(GameObject* hit_object)
{

}

// 攻撃範囲通知処理
void E_Tank::OnAreaDetection(GameObject* hit_object)
{
	Collision hit_col = hit_object->GetCollision();
	if (hit_col.object_type == eObjectType::Player)
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
		velocity.x = 5.0f;
	}
}

// 攻撃処理
void E_Tank::Attack(GameObject* hit_object)
{
	hit_object->HPControl(Damage);
	attack_flame = 2.0f;
}

// 移動処理
void E_Tank::Movement(float delta_second)
{

	// 移動の実行
	location.x += velocity.x * 10 * delta_second;
}

// アニメーション制御処理
void E_Tank::AnimationControl(float delta_second)
{
	// 画像の読み込み
	ResourceManager* rm = ResourceManager::GetInstance();

	if (old_state != now_state)
	{
		Anim_flame = 0;
		Anim_count = 0;
		count_flag = false;
	}

	Anim_flame += delta_second;

	if (Anim_flame >= 0.1f)
	{
		if (count_flag == false)
		{
			Anim_count++;
		}
		else
		{
			Anim_count--;
		}
		if (Anim_count >= 2)
		{
			count_flag = true;
		}
		else if (Anim_count <= 0)
		{
			count_flag = false;
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
		animation = rm->GetImages("Resource/Images/Enemy/Tank/Tank_Walk.png", 4, 4, 1, 32, 32);
		image = animation[1 + Anim_count];
		break;
	case State::Attack:
		animation = rm->GetImages("Resource/Images/Enemy/Tank/Tank_Attack.png", 4, 4, 1, 32, 32);
		image = animation[1 + Anim_count];
		if (count_flag == true)
		{
			attack_flag = true;
		}
		break;
	case State::Damage:
		break;
	case State::Death:
		break;
	default:
		break;
	}

}
// エフェクト制御処理
void E_Tank::EffectControl(float delta_second)
{

}