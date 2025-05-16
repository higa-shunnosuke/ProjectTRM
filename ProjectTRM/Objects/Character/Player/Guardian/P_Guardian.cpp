#include "P_Guardian.h"
#include "../../../../Utility/LightMapManager.h"

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
	animation = rm->GetImages("Resource/Images/Unit/Guardian/Guardian_Walk.png", 3, 3, 1, 1024, 1024);

	LightMapManager* light = LightMapManager::GetInstance();
	light->AddLight(this);
	collision.light_size = 1.0;

	is_mobility = true;
	is_aggressive = true;

	collision.is_blocking = true;
	collision.object_type = eObjectType::Player;
	collision.hit_object_type.push_back(eObjectType::Enemy);
	collision.box_size = Vector2D(60.0f, 60.0f);
	collision.attack_size = Vector2D(100.0f, 100.0f);
	z_layer = 1;

	attack_flag = false;
	flip_flag = false;

	now_state = State::Move;

	// 右へ移動
	velocity.x = -5.0f;

	//攻撃力
	Damage = 4;

	// HP初期化
	HP = 25;

	object = GameObjectManager::GetInstance();


	alpha = MAX_ALPHA;
	add = -ALPHA_ADD;
}

// 更新処理
void P_Guardian::Update(float delta_second)
{
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

	dmage_flame -= delta_second;

	if (dmage_flame <= 0.0f)
	{
		dmage_flame = 0.0f;
		alpha = MAX_ALPHA;
		add = -ALPHA_ADD;
	}

	old_state = now_state;
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
	DrawRotaGraphF(position.x, position.y, 0.1, 0.0, image, TRUE, flip_flag);
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
		else
		{
			velocity.x = -5.0f;
		}
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
			animation = rm->GetImages("Resource/Images/Unit/Guardian/Guardian_Idle.png", 1, 1, 1, 1000, 1000);
			break;
		case State::Move:
			animation = rm->GetImages("Resource/Images/Unit/Guardian/Guardian_Walk.png", 3, 3, 1, 1024, 1024);
			break;
		case State::Attack:
			animation = rm->GetImages("Resource/Images/Unit/Guardian/Guardian_Attack.png", 3, 3, 1, 1024, 1024);
			if (Anim_count >= 2)
			{
				attack_flag = true;
				now_state = State::Idle;
			}
			break;
		case State::Damage:
			break;
		case State::Death:
			//animation = rm->GetImages("Resource/Images/Unit/Melee/Melee_Down.png", 3, 3, 1, 32, 32);
			break;
		default:
			break;
		}
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

		image = animation[Anim_count];
		break;
	case State::Attack:
		image = animation[Anim_count];
		if (Anim_count >= 2)
		{
			attack_flag = true;
			now_state = State::Idle;

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
		Finalize();
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