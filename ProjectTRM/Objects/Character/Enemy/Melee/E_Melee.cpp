#include "E_Melee.h"
size_t E_Melee::count = 0;
size_t E_Melee::GetCount()
{
	return count;
}

// コンストラクタ
E_Melee::E_Melee() :
	Damage()
{

}

// デストラクタ
E_Melee::~E_Melee()
{

}

// 初期化処理
void E_Melee::Initialize()
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
	Damage = 2;

	// HP初期化
	HP = 20;
}

// 更新処理
void E_Melee::Update(float delta_second)
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
void E_Melee::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// 近接ユニットの描画
	// オフセット値を基に画像の描画を行う
	DrawRotaGraphF(position.x, position.y, 2.0, 0.0, image, TRUE, flip_flag);
	/*DrawBox((int)(position.x - collision.box_size.x / 2), (int)(position.y - collision.box_size.y / 2),
		(int)(position.x + collision.box_size.x / 2), (int)(position.y + collision.box_size.y / 2), 0xff00a0, TRUE);*/

#ifdef DEBUG
	//残りHPの表示
	DrawFormatString(position.x, position.y - 40.0f, 0xffffff, "%d", HP);

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
void E_Melee::Finalize()
{

}

// 当たり判定通知処理
void E_Melee::OnHitCollision(GameObject* hit_object)
{

}

// 攻撃範囲通知処理
void E_Melee::OnAreaDetection(GameObject* hit_object)
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
void E_Melee::Attack(GameObject* hit_object)
{
	hit_object->HPControl(Damage);
	attack_flame = 2.0f;
}

// 移動処理
void E_Melee::Movement(float delta_second)
{

	// 移動の実行
	location.x += velocity.x * 10 * delta_second;
}

// アニメーション制御処理
void E_Melee::AnimationControl(float delta_second)
{
	// 画像の読み込み
	ResourceManager* rm = ResourceManager::GetInstance();

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
		animation = rm->GetImages("Resource/Images/Enemy/Melee/Melee_Walk.png", 4, 4, 1, 32, 32);
		image = animation[1 + Anim_count];
		break;
	case State::Attack:
		animation = rm->GetImages("Resource/Images/Enemy/Melee/Melee_Attack.png", 4, 4, 1, 32, 32);
		image = animation[1 + Anim_count];
		if (Anim_count >= 2)
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
void E_Melee::EffectControl(float delta_second)
{

}
