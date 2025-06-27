#include "Bonfire.h"
#include "../../../../Utility/LightMapManager.h"
#include "../../../../Utility/Input/InputManager.h"

size_t Bonfire::count = 0;
size_t Bonfire::GetCount()
{
	return count;
}

// コンストラクタ
Bonfire::Bonfire() :
	effect_image()
{
	count++;
}

// デストラクタ
Bonfire::~Bonfire()
{
	count--;
}

// 初期化処理
void Bonfire::Initialize()
{
	// 画像の読み込み
	ResourceManager* rm = ResourceManager::GetInstance();
	animation = rm->GetImages("Resource/Images/Unit/Bonfire/bon_fire_new.png", 4, 4, 1, 40, 65);

	LightMapManager* light = LightMapManager::GetInstance();
	light->AddLight(this);
	collision.light_size = 2.0;

	is_mobility = false;
	is_aggressive = false;

	collision.is_blocking = true;
	collision.object_type = eObjectType::Player;
	collision.hit_object_type.push_back(eObjectType::Enemy);
	collision.collision_size = Vector2D(60.0f, 60.0f);
	z_layer = 1;

	attack_flag = false;
	flip_flag = false;

	now_state = State::Idle;


	// HP初期化
	HP = 1;

	object = GameObjectManager::GetInstance();


	alpha = MAX_ALPHA;
	add = -ALPHA_ADD;
}

// 更新処理
void Bonfire::Update(float delta_second)
{

	if (ProjectConfig::DEBUG)
	{
		InputManager* input = InputManager::GetInstance();
		if (input->GetKeyState(KEY_INPUT_K) == eInputState::Pressed)
		{
			HP = 0;
		}
	}

	// 移動処理
	Movement(delta_second);

	if (now_state != State::Death)
	{
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
			}
			if (attack_flame <= 0.0f)
			{
				attack_flag = false;
			}
		}
	}

	if (HP <= 0)
	{
		now_state = State::Death;
	}

	AnimationControl(delta_second);

	EffectControl(delta_second);

	old_state = now_state;
}

// 描画処理
void Bonfire::Draw() const
{
	// 近接ユニットの描画
	// オフセット値を基に画像の描画を行う
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawRotaGraphF(location.x, location.y, 2.0f, 0.0, image, TRUE, flip_flag);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	/*DrawBox((int)(position.x - collision.box_size.x / 2), (int)(position.y - collision.box_size.y / 2),
		(int)(position.x + collision.box_size.x / 2), (int)(position.y + collision.box_size.y / 2), 0xffa000, TRUE);*/

	if (ProjectConfig::DEBUG)
	{
		// 中心を表示
		DrawCircle((int)location.x, (int)location.y, 2, 0x0000ff, TRUE);
		// 当たり判定表示
		DrawBox((int)(location.x - collision.collision_size.x / 2), (int)(location.y - collision.collision_size.y / 2),
			(int)(location.x + collision.collision_size.x / 2), (int)(location.y + collision.collision_size.y / 2), 0x0000ff, FALSE);
		// 攻撃範囲を表示
		DrawBox((int)location.x, (int)(location.y - collision.hitbox_size.y / 2),
			(int)(location.x - collision.hitbox_size.x), (int)(location.y + collision.hitbox_size.y / 2), 0x0000ff, FALSE);
	}
}

// 終了時処理
void Bonfire::Finalize()
{
	LightMapManager* light = LightMapManager::GetInstance();
	light->DeleteLight(this);
	object->DestroyObject(this);
}

// 当たり判定通知処理
void Bonfire::OnHitCollision(GameObject* hit_object)
{

}

// 攻撃範囲通知処理
void Bonfire::OnAreaDetection(GameObject* hit_object)
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
		else
		{
			velocity.x = -5.0f;
		}
	}
}

// HP管理処理
void Bonfire::HPControl(float Damage)
{
	// 攻撃状態でなければダメージ状態にする
	if (now_state != State::Attack)
	{
		now_state = State::Damage;

	}

	__super::HPControl(Damage);
}

// アニメーション制御処理
void Bonfire::AnimationControl(float delta_second)
{
	// 画像の読み込み
	ResourceManager* rm = ResourceManager::GetInstance();

	////ステートが変わった時の初期化
	//if (old_state != now_state)
	//{
	//	Anim_flame = 0;
	//	Anim_count = 0;
	//	switch (now_state)
	//	{
	//	case State::Idle:
	//		animation = rm->GetImages("Resource/Images/Unit/Bonfire/BonFire_Idle.png", 4, 4, 1, 250, 250);
	//		break;
	//	case State::Death:
	//		animation = rm->GetImages("Resource/Images/Unit/Bonfire/Bonfire_Down.png", 4, 4, 1, 250, 250);
	//		break;
	//	default:
	//		break;
	//	}
	//}

	Anim_flame += delta_second;

	if (Anim_flame >= 0.1f)
	{
		Anim_count ++;

		if (Anim_count > 3)
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
	case State::Death:
		image = animation[Anim_count];
		if (Anim_count == 3)
		{
			Finalize();
		}
		break;
	default:
		break;
	}

}
// エフェクト制御処理
void Bonfire::EffectControl(float delta_second)
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
