#include "P_Ranged.h"
#include "P_Projectile.h"
#include "../../../GameObjectManager.h"
#include "../../../../Scenes/InGame/InGame.h"

size_t P_Ranged::count = 0;
size_t P_Ranged::GetCount()
{
	return count;
}

// コンストラクタ
P_Ranged::P_Ranged()
{
	count++;
}

// デストラクタ
P_Ranged::~P_Ranged()
{
	count--;
}

// 初期化処理
void P_Ranged::Initialize()
{
	// 画像の読み込み
	ResourceManager* rm = ResourceManager::GetInstance();
	animation = rm->GetImages("Resource/Images/Unit/Ranged/Ranged_Walk.png", 4, 4, 1, 32, 32);
	effect_image = rm->GetImages("Resource/Images/Effect/Unit/Ranged_Ghost.png", 1, 1, 1, 32, 32)[0];

	is_mobility = true;
	is_aggressive = true;

	collision.is_blocking = true;
	collision.object_type = eObjectType::Player;
	collision.hit_object_type.push_back(eObjectType::Enemy);
	collision.collision_size = Vector2D(60.0f, 60.0f);
	collision.hitbox_size = Vector2D(500.0f, 100.0f);
	z_layer = 2;

	attack_flag = false;
	flip_flag = true;

	now_state = State::Move;

	//移動速度
	velocity.x = BASIC_SPEED;

	// HP初期化
	HP = 10;

	object = GameObjectManager::GetInstance();

	alpha = MAX_ALPHA;
	effect_alpha = MAX_ALPHA;
	add = -ALPHA_ADD;
}

// 更新処理
void P_Ranged::Update(float delta_second)
{
	if (ProjectConfig::DEBUG)
	{
		InputManager* input = InputManager::GetInstance();
		if (input->GetKeyState(KEY_INPUT_K) == eInputState::Pressed)
		{
			HP = 0;
		}
	}

	if (now_state != State::Death)
	{
		// 移動処理
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
				now_state = State::Idle;
				attack_flame -= delta_second * (1 + (Ingame->GetSunLevel() / 10));
			}
			if (attack_flame <= 0.0f)
			{
				attack_flag = false;
				now_state = State::Move;
			}
		}

		dmage_flame -= delta_second;

		if (dmage_flame <= 0.0f)
		{
			dmage_flame = 0.0f;
			alpha = MAX_ALPHA;
			add = -ALPHA_ADD;
		}

		if (HP <= 0)
		{
			now_state = State::Death;
			add = -1;
		}
	}

	EffectControl(delta_second);

	SoundControl();

	if (Anim_count <= 2)
	{
		AnimationControl(delta_second);
	}

}

// 描画処理
void P_Ranged::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	position.y += z_layer * 5;

	// 近接ユニットの描画
		// オフセット値を基に画像の描画を行う
	if (Anim_count <= 2)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawRotaGraphF(position.x, position.y, 2.0, 0.0, image, TRUE, flip_flag);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	switch (now_state)
	{
	case State::Damage:
		DrawRotaGraphF(position.x, position.y, 1.0, 0.0, effect_image, TRUE, flip_flag);
		break;
	case State::Death:
		position.y -= Effect_count * 10 + Effect_flame * 10;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, effect_alpha);
		DrawRotaGraphF(position.x, position.y, 2.0, 0.0, effect_image, TRUE, flip_flag);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		break;
	default:
		break;
	}
	/*DrawBox((int)(position.x - collision.box_size.x / 2), (int)(position.y - collision.box_size.y / 2),
		(int)(position.x + collision.box_size.x / 2), (int)(position.y + collision.box_size.y / 2), 0xffa000, TRUE);*/

	if (ProjectConfig::DEBUG)
	{
		// 中心を表示
		DrawCircle((int)position.x, (int)position.y, 2, 0x0000ff, TRUE);
		// 当たり判定表示
		DrawBox((int)(position.x - collision.collision_size.x / 2), (int)(position.y - collision.collision_size.y / 2),
			(int)(position.x + collision.collision_size.x / 2), (int)(position.y + collision.collision_size.y / 2), 0x0000ff, FALSE);
		// 攻撃範囲を表示
		DrawBox((int)position.x, (int)(position.y - collision.hitbox_size.y / 2),
			(int)(position.x - collision.hitbox_size.x), (int)(position.y + collision.hitbox_size.y / 2), 0x0000ff, FALSE);
	}
}

// 終了時処理
void P_Ranged::Finalize()
{
	object->DestroyObject(this);
}

// 当たり判定通知処理
void P_Ranged::OnHitCollision(GameObject* hit_object)
{
	
}

// 攻撃範囲通知処理
void P_Ranged::OnAreaDetection(GameObject* hit_object)
{
	if (target_loc.x == 0.0f || target_loc.x < hit_object->GetLocation().x - this->GetLocation().x)
	{
		target_loc.x = hit_object->GetLocation().x - this->GetLocation().x;
	}
	target_loc.y = hit_object->GetLocation().y;
	//現在のステータスが死亡状態かどうか
	if (now_state != State::Death)
	{
		Collision hit_col = hit_object->GetCollision();

		if (hit_col.object_type == eObjectType::Enemy)
		{
			if (hit_object->GetInLight() || target_loc.y == 630.0f)
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
				if (target_loc.x <= hit_object->GetLocation().x - this->GetLocation().x)
				{

					//２つのオブジェクトの距離を取得
					Vector2D diff = this->GetLocation() - hit_object->GetLocation();

					//２つのオブジェクトの当たり判定の大きさを取得
					Vector2D box_size = (this->collision.collision_size + hit_col.collision_size) / 2.0f;

					// 矩形同士の当たり判定
					if ((fabsf(diff.x) < box_size.x) && (fabsf(diff.y) < box_size.y))
					{
						velocity.x = 0;
						now_state = State::Idle;
						target_loc = 0.0f;
					}
					else
					{
						velocity.x = BASIC_SPEED + ((BASIC_SPEED / 100) * (Ingame->GetSunLevel()));
						now_state = State::Move;
						target_loc = 0.0f;
					}
				}
			}
		}
	}
}

// 攻撃範囲通知処理
void P_Ranged::NoHit()
{
	// 移動状態にする
	if (now_state != State::Death && now_state != State::Damage)
	{
		velocity.x = BASIC_SPEED + ((BASIC_SPEED / 100) * (Ingame->GetSunLevel()));
	}
}

// HP管理処理
void P_Ranged::HPControl(int Damage)
{
	// 攻撃状態でなければダメージ状態にする
	if (now_state != State::Attack)
	{
		now_state = State::Damage;
		dmage_flame = 1.0f;
	}

	__super::HPControl(Damage);
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
		con = 1;
		switch (now_state)
		{
		case State::Idle:
			animation = rm->GetImages("Resource/Images/Unit/Ranged/Ranged_Walk.png", 3, 3, 1, 32, 32);
			break;
		case State::Move:
			animation = rm->GetImages("Resource/Images/Unit/Ranged/Ranged_Walk.png", 3, 3, 1, 32, 32);
			break;
		case State::Attack:
			animation = rm->GetImages("Resource/Images/Unit/Ranged/Ranged_Attack.png", 4, 4, 1, 32, 32);
			if (Anim_count >= 2)
			{
				attack_flag = true;
				now_state = State::Idle;
			}
			break;
		case State::Damage:
			break;
		case State::Death:
			animation = rm->GetImages("Resource/Images/Unit/Ranged/Ranged_Down.png", 3, 3, 1, 32, 32);
			break;
		default:
			break;
		}
		old_state = now_state;
	}

	Anim_flame += delta_second;

	if (Anim_flame >= 0.1f)
	{
		Anim_count += con;

		if (Anim_count <= 0 || Anim_count >= 2)
		{
			con *= -1;
		}

		Anim_flame = 0.0f;
	}

	switch (now_state)
	{
	case State::Idle:
		image = animation[0];
		break;
	case State::Move:
		velocity.x = BASIC_SPEED + ((BASIC_SPEED / 100) * (Ingame->GetSunLevel()));
		image = animation[1 + Anim_count];
		break;
	case State::Attack:
		image = animation[1 + Anim_count];
		if (Anim_count >= 2)
		{
			attack_flag = true;
			now_state = State::Move;
			velocity.x = BASIC_SPEED + ((BASIC_SPEED / 100) * (Ingame->GetSunLevel()));
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
		image = animation[Anim_count];
		if (Anim_count == 2)
		{
			location.y -= Anim_count * 10 + Anim_flame * 10;
			Anim_count = 3;
			//他のオブジェクトの邪魔をしないようにオブジェクトタイプの消去
			collision.hit_object_type.clear();
			collision.object_type = eObjectType::None;
		}
		break;
	default:
		break;
	}

}
// エフェクト制御処理
void P_Ranged::EffectControl(float delta_second)
{
	ResourceManager* rm = ResourceManager::GetInstance();

	if (now_state != old_state)
	{
		Effect_count = 0;
		Effect_flame = 0.0f;
		switch (now_state)
		{
		case State::Damage:
			Effect = rm->GetImages("Resource/Images/Effect/Unit/Unit_Damage.png", 30, 6, 5, 100, 100);
			break;
		case State::Death:
			Effect = rm->GetImages("Resource/Images/Effect/Unit/Ranged_Ghost.png", 1, 1, 1, 32, 32);
			break;
		default:
			break;
		}
	}

	Effect_flame += delta_second;
	if (Effect_flame >= 0.05f)
	{
		if (Effect_count < 29)
		{
			Effect_count++;
		}
		else
		{
			Effect_count = 0;
		}
		Effect_flame = 0;
	}

	switch (now_state)
	{
	case State::Damage:
		effect_image = Effect[Effect_count];
		if (dmage_flame <= 0.0f)
		{
			now_state = State::Move;
		}
		break;
	case State::Death:
		effect_image = Effect[0];
		effect_alpha += add;
		//完全に透明になったらオブジェクト消去
		if (effect_alpha <= 0)
		{
			effect_alpha = 0;
			Finalize();
		}
		break;
	default:
		break;
	}
}

//SEの制御処理
void P_Ranged::SoundControl()
{
	if (now_state != old_state)
	{
		ResourceManager* rm = ResourceManager::GetInstance();
		switch (now_state)
		{

		case State::Attack:
			sounds = rm->GetSounds("Resource/Sounds/UnitSE/Ranged/Ranged_Attack.mp3");
			break;
		default:
			break;
		}
		ChangeVolumeSoundMem(150, sounds);
	}
}

// 攻撃処理
void P_Ranged::Attack(GameObject* hit_object)
{
	PlaySoundMem(sounds, DX_PLAYTYPE_BACK);
	P_Projectile* obj = object->CreateObject<P_Projectile>(this->location);
	obj->SetTargetLocation(hit_object->GetLocation());
	obj->SetInGamePoint(Ingame);
	target_loc = 0.0f;
	attack_flame = 2.0f;
}