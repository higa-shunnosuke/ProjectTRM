#include "P_Guardian.h"
#include "../../../../Utility/LightMapManager.h"
#include "../../../../Scenes/InGame/InGame.h"

size_t P_Guardian::count = 0;
size_t P_Guardian::GetCount()
{
	return count;
}

// コンストラクタ
P_Guardian::P_Guardian() :
	Damage(),
	effect_image()
{
	count++;
}

// デストラクタ
P_Guardian::~P_Guardian()
{
	count--;
}

// 初期化処理
void P_Guardian::Initialize()
{
	// 画像の読み込み
	ResourceManager* rm = ResourceManager::GetInstance();
	animation = rm->GetImages("Resource/Images/Unit/Guardian/Guardian_All.png", 121, 11, 11, 96, 96);

	LightMapManager* light = LightMapManager::GetInstance();
	light->AddLight(this);
	collision.light_size = 1.0;

	is_mobility = true;
	is_aggressive = true;

	collision.is_blocking = true;
	collision.object_type = eObjectType::Player;
	collision.hit_object_type.push_back(eObjectType::Enemy);
	collision.collision_size = Vector2D(60.0f, 60.0f);
	collision.hitbox_size = Vector2D(50.0f, 100.0f);
	z_layer = 1;

	attack_flag = false;
	flip_flag = true;

	now_state = State::Move;

	// 右へ移動
	velocity.x = BASIC_SPEED;

	//攻撃力
	Damage = BASIC_POWER;

	// HP初期化
	HP = 60;

	object = GameObjectManager::GetInstance();


	alpha = MAX_ALPHA;
	add = -ALPHA_ADD;
}

// 更新処理
void P_Guardian::Update(float delta_second)
{

	Damage = BASIC_POWER + (Ingame->GetSunLevel() / 5);

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

	if (Anim_count <= anim_max_count)
	{
		AnimationControl(delta_second);
	}
}

// 描画処理
void P_Guardian::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;
	position.y += z_layer * 8;

	// 近接ユニットの描画
	// オフセット値を基に画像の描画を行う
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawRotaGraphF(position.x, position.y, 1.0, 0.0, image, TRUE, flip_flag);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

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
void P_Guardian::Finalize()
{
	LightMapManager* light = LightMapManager::GetInstance();
	light->DeleteLight(this);
	object->DestroyObject(this);
}

// 当たり判定通知処理
void P_Guardian::OnHitCollision(GameObject* hit_object)
{

}

// 攻撃範囲通知処理
void P_Guardian::OnAreaDetection(GameObject* hit_object)
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
void P_Guardian::NoHit()
{
	// 移動状態にする
	if (now_state != State::Death)
	{
		now_state = State::Move;
	}
}

// HP管理処理
void P_Guardian::HPControl(int Damage)
{
	// 攻撃状態でなければダメージ状態にする
	if (now_state != State::Attack)
	{
		now_state = State::Damage;
		dmage_flame = 1.0f;
	}

	__super::HPControl(Damage);
}


// 攻撃処理
void P_Guardian::Attack(GameObject* hit_object)
{
	hit_object->HPControl(Damage);
	attack_flame = 2.0f;
}

// 移動処理
void P_Guardian::Movement(float delta_second)
{
	// 移動の実行
	location.x += velocity.x * 10 * delta_second;
}

// アニメーション制御処理
void P_Guardian::AnimationControl(float delta_second)
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
			anim_max_count = 9;
			break;
		case State::Move:
			anim_max_count = 5;
			break;
		case State::Attack:
			anim_max_count = 5;
			if (Anim_count == anim_max_count)
			{
				attack_flag = true;
				now_state = State::Idle;
			}
			break;
		case State::Damage:
			break;
		case State::Death:
			anim_max_count = 11;
			break;
		default:
			break;
		}
		old_state = now_state;
	}

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
		velocity.x = BASIC_SPEED + ((BASIC_SPEED / 10) * (Ingame->GetSunLevel() - 1));
		image = animation[Anim_count + 10];
		break;
	case State::Attack:
		image = animation[Anim_count + 78];
		if (Anim_count == anim_max_count - 1)
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
		image = animation[Anim_count + 104];
		if (Anim_count == anim_max_count - 1)
		{
			Finalize();
		}
		break;
	default:
		break;
	}

}
// エフェクト制御処理
void P_Guardian::EffectControl(float delta_second)
{
	switch (now_state)
	{
	case State::Idle:
		break;
	case State::Move:
		break;
	case State::Attack:
		break;
	case State::Damage:
		break;
	case State::Death:
		break;
	default:
		break;
	}
}