#include "Oracle.h"
#include "../Guardian/P_Guardian.h"
#include "../../../../Scenes/InGame/InGame.h"


// コンストラクタ
Oracle::Oracle() :
	summon_flag(),
	player(),
	old_sun_level(),
	now_sun_level(),
	power_up(false),
	effect_image(),
	anime_max_count(),
	JustDead(false),
	move_location(0.0f),
	sounds(),
	HPColor(0x00ff00)
{

}

// デストラクタ
Oracle::~Oracle()
{

}

// 初期化処理
void Oracle::Initialize()
{
	// 画像の読み込み
	ResourceManager* rm = ResourceManager::GetInstance();
	EffectImage = rm->GetImages("Resource/Images/Effect/Unit/sprite-sheet.png", 15, 5, 3, 128, 128);
	animation = rm->GetImages("Resource/Images/Unit/Oracle/Oracle_Idle.png",7,7,1,128,128);
	sounds = rm->GetSounds("Resource/Sounds/Ingame/SummonAllies.mp3");
	anime_max_count = 7;

	is_mobility = false;
	is_aggressive = false;

	collision.is_blocking = true;
	collision.object_type = eObjectType::Player;
	collision.hit_object_type.push_back(eObjectType::Enemy);
	collision.collision_size = Vector2D(60.0f, 120.0f);
	collision.hitbox_size = Vector2D(200.0f, 200.0f);
	z_layer = 1;

	now_state = State::Summon;

	flip_flag = true;

	summon_flag = false;

	old_sun_level = 1;

	velocity.x = -5.0f;

	// HP初期化
	HP = 100;
}

// 更新処理
void Oracle::Update(float delta_second)
{
	if (ProjectConfig::DEBUG)
	{
		if (CheckHitKey(KEY_INPUT_6))
		{
			HPControl(1);
		}
	}

	if (HP < 25)
	{
		HPColor = 0xff0000;
	}
	else if (HP < 50)
	{
		HPColor = 0xfff000;
	}

	if (Ingame->GetSunLevel() != old_sun_level)
	{
		power_up = !power_up;
		old_sun_level = Ingame->GetSunLevel();
	}
	if (now_state == State::Summon)
	{
		Movement(delta_second);
	}

	AnimationControl(delta_second);

	EffectControl(delta_second);
	/*if (summon_flag == true)
	{
		attack_flame += delta_second;
	}*/
}

// 描画処理
void Oracle::Draw() const
{
	Vector2D position;
	if (now_state == State::Summon)
	{
		position = Vector2D(move_location,this->GetLocation().y);
		position.x -= velocity.x;
	}
	else
	{
		position = this->GetLocation();
	}

	if (ProjectConfig::DEBUG)
	{
		// 巫女の描画
		DrawBox((int)(position.x - collision.collision_size.x / 2), (int)(position.y - collision.collision_size.y / 2),//数字を変える
			(int)(position.x + collision.collision_size.x / 2), (int)(position.y + collision.collision_size.y / 2), 0xff0000, TRUE);
		// 中心を表示
		DrawCircle((int)position.x, (int)position.y, 2, 0x0000ff, TRUE);
		// 当たり判定表示
		DrawBox((int)(position.x - collision.collision_size.x / 2), (int)(position.y - collision.collision_size.y / 2),
			(int)(position.x + collision.collision_size.x / 2), (int)(position.y + collision.collision_size.y / 2), 0x0000ff, FALSE);
		// 攻撃範囲を表示
		DrawBox((int)position.x, (int)(position.y - collision.hitbox_size.y / 2),
			(int)(position.x - collision.hitbox_size.x), (int)(position.y + collision.hitbox_size.y / 2), 0x0000ff, FALSE);
		//残りHPの表示
		DrawFormatString(position.x, position.y - 40.0f, 0xffffff, "%d", HP);
	}


	if (now_state != State::Death && now_state != State::Summon)
	{
		DrawBoxAA(position.x - 50.0f, position.y - 100.0f, position.x + 50.0f, position.y - 85.0f, 0xFFFFFF, false);
		DrawBoxAA(position.x - 50.0f, position.y - 100.0f, position.x + (50.0f - (100 - HP)), position.y - 85.0f, HPColor, true);
	}


	DrawRotaGraphF(position.x - 15.0f, position.y - 30.0f,
		1.5, 0.0, image, TRUE, flip_flag);
	

	//太陽のレベルアップ時エフェクト描画
	if (power_up)
	{
		DrawRotaGraph(position.x, position.y + 15.0f, 1.0, 0.0, effect_image, TRUE);
	}

}

// 終了時処理
void Oracle::Finalize()
{

}

// 当たり判定通知処理
void Oracle::OnHitCollision(GameObject* hit_object)
{

}

// 攻撃範囲通知処理
void Oracle::OnAreaDetection(GameObject* hit_object)
{
	//Collision hit_col = hit_object->GetCollision();
	//velocity.x = -1.0f;
	//if (hit_col.object_type == eObjectType::Enemy)
	//{
	//	summon_flag = true;
	//	velocity.x = 0.0f;
	//	if (attack_flame >= 2.0f)
	//	{
	//		//ここでユニットの召喚をする？

	//	}
	//}
}

// 攻撃処理
void Oracle::Attack()
{
	
}


// HP管理処理
void Oracle::HPControl(float Damage)
{
	HP -= Damage;
	
	GameObjectManager* object = GameObjectManager::GetInstance();

	if (!summon_flag)
	{

			summon_flag = true;
			// 召喚SE再生
			PlaySoundMem(sounds, DX_PLAYTYPE_BACK);
			object->CreateObject<P_Guardian>(Vector2D(location.x, location.y + 15.0f))->SetInGamePoint(Ingame);
	}

	if (this->HP <= 0)
	{
		now_state = State::Death;
		this->HP = 0.0f;
		z_layer = 10;
	}

}

// 移動処理
void Oracle::Movement(float delta_second)
{
	// 移動の実行
	move_location += velocity.x * 10 * delta_second;
	if (move_location <= location.x + 15.0f)
	{
		now_state = State::Idle;
	}
}

bool Oracle::GetDead()
{
	return JustDead;
}

void Oracle::SetLocation(const Vector2D& location)
{
	__super::SetLocation(location);
	move_location = location.x + 100.0f;
}

State Oracle::GetNowState() const
{
	return now_state;
}

// アニメーション制御処理
void Oracle::AnimationControl(float delta_second)
{
	ResourceManager* rm = ResourceManager::GetInstance();
	if (now_state != old_state)
	{
		Anim_count = 0;		
		switch (now_state)
		{
		case State::Summon:
			animation = rm->GetImages("Resource/Images/Unit/Oracle/Oracle_Walk.png", 7, 7, 1, 128, 128);
			anime_max_count = 7;
			break;
		case State::Idle:
			animation = rm->GetImages("Resource/Images/Unit/Oracle/Oracle_Idle.png", 7, 7, 1, 128, 128);
			anime_max_count = 7;
			break;
		case State::Death:
			animation = rm->GetImages("Resource/Images/Unit/Oracle/Oracle_Dead.png", 5, 5, 1, 128, 128);
			anime_max_count = 7;
		}
		old_state = now_state;
	}

	auto now_time = std::chrono::steady_clock::now();

	switch (now_state)
	{
	case Idle:
	case Summon:
		if (now_time - anime_time > std::chrono::milliseconds(100))
		{
			Anim_count++;
			anime_time = std::chrono::steady_clock::now();

			if (Anim_count >= anime_max_count)
			{
				Anim_count = 0;
			}
		}
		break;
	case Death:
		if (now_time - anime_time > std::chrono::milliseconds(250))
		{
			Anim_count++;
			anime_time = std::chrono::steady_clock::now();

			if (Anim_count >= anime_max_count)
			{
				Anim_count = 0;
			}
		}
		break;
	}


	switch (now_state)
	{
	case Idle:
	case Summon:
		image = animation[Anim_count];
		break;
	case Death:
		if (Anim_count < anime_max_count - 2)
		{
			image = animation[Anim_count];
		}
		else
		{
			image = animation[4];
		}
		if (Anim_count == anime_max_count - 1)
		{
			JustDead = true;
		}
		break;
	default:
		break;
	}
}

// エフェクト制御処理
void Oracle::EffectControl(float delta_second)
{
	if (power_up)
	{
		Effect_flame += delta_second;
		if (Effect_flame >= 0.1f)
		{
			Effect_count++;
			if (Effect_count > 11)
			{
				Effect_count = 0;
				power_up = !power_up;
			}
			Effect_flame = 0;
		}
		effect_image = EffectImage[Effect_count];
	}
}