#include "UnitBase.h"

// コンストラクタ
UnitBase::UnitBase() :
	Damage(),
	sounds(),
	basic_power(),
	basic_speed(),
	effect_image(),
	effect_alpha(),
	light(nullptr)
{

}

// デストラクタ
UnitBase::~UnitBase()
{

}

// 初期化処理
void UnitBase::Initialize()
{
	light = LightMapManager::GetInstance();
	light->AddLight(this);

	is_mobility = true;
	is_aggressive = true;

	collision.is_blocking = true;
	collision.object_type = eObjectType::Player;
	collision.hit_object_type.push_back(eObjectType::Enemy);


	//移動速度
	velocity.x = basic_speed;

	//攻撃力
	Damage = basic_power;

	//反転フラグ
	flip_flag = true;

	//攻撃フラグ
	attack_flag = false;

	//初期化
	now_state = State::Move;

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


		if (HP <= 0)
		{
			now_state = State::Death;
			add = -1;
		}
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

	//レイヤーをもとに座標をずらす
	position.y += z_layer * 5;

	// 灯守の描画
	// オフセット値を基に画像の描画を行う
	if (Anim_count <= anim_max_count)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawRotaGraphF(position.x, position.y, 2.0, 0.0, image, TRUE, flip_flag);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	//ステートによってエフェクトの描画を行う
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
		(int)(position.x + collision.box_size.x / 2), (int)(position.y + collision.box_size.y / 2), 0xff00ff, TRUE);*/

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
		// HPを表示
		DrawFormatString(position.x, position.y - 20.0f, 0x00ffff, "%d", HP);
	}
}

// 終了時処理
void UnitBase::Finalize()
{
	GameObjectManager* object = GameObjectManager::GetInstance();
	LightMapManager* light = LightMapManager::GetInstance();
	light->DeleteLight(this);
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
	if (now_state != State::Death)
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
	}
}

// 攻撃範囲通知処理
void UnitBase::NoHit()
{
	// 移動状態にする
	if (now_state != State::Death)
	{
		velocity.x = basic_speed + ((basic_speed / 100) * (Ingame->GetSunLevel()));
	}
}

// HP管理処理
void UnitBase::HPControl(int Damage)
{
	// 攻撃状態でなければダメージ状態にする
	if (now_state != State::Attack)
	{
		now_state = State::Damage;
	}

	__super::HPControl(Damage);
}

// 攻撃処理
void UnitBase::Attack(GameObject* hit_object)
{

	PlaySoundMem(sounds, DX_PLAYTYPE_BACK, TRUE);
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
		image = animation[Anim_count];
		break;
	case State::Move:
		velocity.x = basic_speed + ((basic_speed / 100) * (Ingame->GetSunLevel()));
		image = animation[Anim_count];
		break;
	case State::Attack:
		image = animation[Anim_count];
		if (Anim_count == anim_max_count / 2)
		{
			PlaySoundMem(sounds, DX_PLAYTYPE_BACK);
		}
		if (Anim_count == anim_max_count - 1)
		{
			attack_flag = true;
			now_state = State::Move;
			velocity.x = basic_speed + ((basic_speed / 100) * (Ingame->GetSunLevel()));
		}
		break;
	case State::Damage:
		alpha += add;
		if (alpha <= 0 || alpha >= 255)
		{
			add = -add;
		}
		if (velocity.x < 0.0f)
		{
			image = animation[Anim_count];
		}
		else
		{
			image = animation[0];
		}
		break;
	case State::Death:
		image = animation[Anim_count];
		if (Anim_count == anim_max_count - 1)
		{

			location.y -= Anim_count * 10 + Anim_flame * 10;
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

	Effect_flame += delta_second;
	if (Effect_flame >= 0.1f)
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
}

//SEの制御処理
void UnitBase::SoundControl()
{

}