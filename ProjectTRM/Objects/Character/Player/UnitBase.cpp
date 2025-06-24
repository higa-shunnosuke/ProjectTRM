#include "UnitBase.h"
#include <iostream >

// コンストラクタ
UnitBase::UnitBase() :
	Damage(),
	effect_image(),
	effect_alpha(),
	object(nullptr),
	anim_max_count(),
	sounds(),
	effect_max_count(),
	reduction_amount(),
	text(),
	old_sun_level(),
	power_up(false)
{

}

// デストラクタ
UnitBase::~UnitBase()
{

}

// 初期化処理
void UnitBase::Initialize()
{
	// 画像の読み込み
	ResourceManager* rm = ResourceManager::GetInstance();
	text = rm->GetImages("Resource/Images/BackGround/numbers.png", 10, 5, 2, 32, 32);
	minus = rm->GetImages("Resource/Images/BackGround/minus.png")[0];

	is_mobility = true;
	is_aggressive = true;

	collision.is_blocking = true;
	collision.object_type = eObjectType::Player;
	collision.hit_object_type.push_back(eObjectType::Enemy);


	attack_flag = false;
	flip_flag = true;

	now_state = State::Summon;

	//攻撃力
	Damage = basic_power;

	

	object = GameObjectManager::GetInstance();

	alpha = MAX_ALPHA;
	effect_alpha = MAX_ALPHA;
	add = -ALPHA_ADD;
}

// 更新処理
void UnitBase::Update(float delta_second)
{
	Damage = basic_power + (Ingame->GetSunLevel() / 5);

	if (ProjectConfig::DEBUG)
	{
		InputManager* input = InputManager::GetInstance();
		if (input->GetKeyState(KEY_INPUT_K) == eInputState::Pressed)
		{
			HP = 0;
		}
	}

	if (now_state == State::Summon)
	{
		old_sun_level = Ingame->GetSunLevel();
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
				attack_flame -= delta_second * (1 + (Ingame->GetSunLevel() / 10));
			}
			if (attack_flame <= 0.0f && now_state != State::Summon)
			{
				attack_flag = false;
				now_state = State::Move;
			}
		}

		if (!reduction_amount.empty())
		{
			for (int i = reduction_amount.size() - 1; i >= 0; --i) {
				if (damage_time[i] <= 0.0f) {
					reduction_amount.erase(reduction_amount.begin() + i);
					damage_time.erase(damage_time.begin() + i); // damage_time も合わせて消す
				}
			}
		}

		if (Ingame->GetNowState() == GameState::BOSS_DEAD)
		{
			now_state = State::Move;
			flip_flag = false;
		}


		if (HP <= 0)
		{
			now_state = State::Death;
			add = -1;
		}
	}

	if (old_sun_level != Ingame->GetSunLevel())
	{
		power_up = !power_up;
		old_sun_level = Ingame->GetSunLevel();
	}

	EffectControl(delta_second);

	SoundControl();

	if (Anim_count <= anim_max_count)
	{
		AnimationControl(delta_second);
	}
}

// 描画処理
void UnitBase::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;
	position.y += z_layer * 2;

	if (power_up  && now_state != State::Summon)
	{
		DrawRotaGraph(position.x, position.y + 2.0f, 1.0, 0.0, effect_image, TRUE);
	}

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
		// 攻撃力を表示
		DrawFormatString(position.x, position.y - 20.0f, 0x00ffff, "%f", velocity.x);
	}
}

// 終了時処理
void UnitBase::Finalize()
{
	LightMapManager* light = LightMapManager::GetInstance();
	light->DeleteLight(this);
	//object->CreateObject<Torch>(this->location);
	object->DestroyObject(this);
}

// 当たり判定通知処理
void UnitBase::OnHitCollision(GameObject* hit_object)
{

}

// 攻撃範囲通知処理
void UnitBase::OnAreaDetection(GameObject* hit_object)
{
	//現在のステータスが死亡状態かどうか
	if (now_state != State::Death && Ingame->GetNowState() != GameState::BOSS_DEAD)
	{
		Collision hit_col = hit_object->GetCollision();

		if (hit_col.object_type == eObjectType::Enemy)
		{
			velocity.x = 0.0f;
			if (attack_flag == false && now_state != State::Summon)
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
	}
}

// 攻撃範囲通知処理
void UnitBase::NoHit()
{
	// 移動状態にする
	if (now_state != State::Death && now_state != State::Summon)
	{
		now_state = State::Move;
	}
}

// HP管理処理
void UnitBase::HPControl(float Damage)
{
	// 攻撃状態でなければダメージ状態にする
	if (now_state != State::Attack && now_state != State::Death)
	{
		//PlaySoundMem(sounds, DX_PLAYTYPE_BACK, true);
		reduction_amount.push_back(Damage);
		__super::HPControl(Damage);
		damage_time.push_back(0.5f);
	}
}

// 攻撃処理
void UnitBase::Attack(GameObject* hit_object)
{
	hit_object->HPControl(Damage);
	attack_flame = 2.0f;
}

// 移動処理
void UnitBase::Movement(float delta_second)
{
	// 移動の実行
	location.x += velocity.x * 10 * delta_second;
}

// アニメーション制御処理
void UnitBase::AnimationControl(float delta_second)
{

	Anim_flame += delta_second;

	if (Anim_flame >= 0.1f)
	{
		Anim_count++;

		if (Anim_count == anim_max_count)
		{
			Anim_count = 0;
		}

		Anim_flame = 0.0f;
	}

	switch (now_state)
	{
	case State::Idle:
	case State::Summon:
		image = animation[Anim_count];
		break;
	case State::Move:
		if (Ingame->GetNowState() == GameState::PLAYING)
		{
			velocity.x = basic_speed + ((basic_speed / 100) * (Ingame->GetSunLevel()));
		}
		else if(Ingame->GetNowState() == GameState::BOSS_DEAD)
		{
			velocity.x = -basic_speed - ((basic_speed / 100) * (Ingame->GetSunLevel()));
		}
		image = animation[Anim_count];
		break;
	case State::Attack:
		image = animation[Anim_count];
		if (Anim_count == anim_max_count / 2 && Ingame->GetNowState() == GameState::PLAYING)
		{
			attack_flag = true;
			PlaySoundMem(sounds, DX_PLAYTYPE_BACK);
		}
		if (Anim_count == anim_max_count - 1)
		{
			now_state = State::Idle;
		}
		break;
	case State::Death:
		image = animation[Anim_count];
		if (Anim_count == anim_max_count - 1)
		{

			Anim_count += 2;
			//他のオブジェクトの邪魔をしないようにオブジェクトタイプの消去
			collision.hit_object_type.clear();
			collision.object_type = eObjectType::None;
			LightMapManager* light = LightMapManager::GetInstance();
			light->DeleteLight(this);
		}
		break;
	default:
		break;
	}

}
// エフェクト制御処理
void UnitBase::EffectControl(float delta_second)
{
	ResourceManager* rm = ResourceManager::GetInstance();

	if (now_state != old_state || power_up)
	{
		Effect = rm->GetImages("Resource/Images/Effect/Unit/PowerUp_Unit.png", 12, 5, 3, 128, 128);
		effect_max_count = 12;
		switch (now_state)
		{
		case State::Summon:
			Effect = rm->GetImages("Resource/Images/Effect/magic_circle.png", 10, 5, 2, 192, 65);
			effect_max_count = 10;
			break;
		case State::Death:
			Effect = rm->GetImages("Resource/Images/Effect/Unit/Ghost.png", 1, 1, 1, 50, 50);
			effect_max_count = 25;
			break;
		default:
			break;
		}
	}

	Effect_flame += delta_second;
	if (Effect_flame >= 0.1f)
	{
		Effect_count++;
		if (Effect_count == effect_max_count)
		{
			Effect_count = 0;
			if (power_up)
			{
				power_up = !power_up;
			}
		}
		Effect_flame = 0;
	}
	if (!damage_time.empty())
	{
		for (int i = 0; i < damage_time.size(); i++)
		{
			damage_time[i] -= delta_second;
		}
	}

	switch (now_state)
	{
	case State::Summon:
		if (Effect_count == effect_max_count - 1)
		{
			now_state = State::Move;
			break;
		}
		effect_image = Effect[Effect_count];
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
	if (power_up )
	{
		effect_image = Effect[Effect_count];
	}
}

//SEの制御処理
void UnitBase::SoundControl()
{
	ChangeVolumeSoundMem(90, sounds);
}