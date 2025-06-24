#include "P_Tank.h"
#include "Torch.h"
#include "../../../GameObjectManager.h"
#include "../../../../Scenes/InGame/InGame.h"

size_t P_Tank::count =0;
size_t P_Tank::GetCount()
{
	return count;
}

// コンストラクタ
P_Tank::P_Tank() :
	light(nullptr)
{
	count++;
}

// デストラクタ
P_Tank::~P_Tank()
{
	count--;
}

// 初期化処理
void P_Tank::Initialize()
{
	// 画像等の読み込み
	ResourceManager* rm = ResourceManager::GetInstance();
	animation = rm->GetImages("Resource/Images/Unit/Tank/Tank_Unit_Walk.png", 9, 9, 1, 48, 32);
	effect_image = rm->GetImages("Resource/Images/Effect/Unit/Ghost.png", 1, 1, 1, 50, 50)[0];
	sounds = rm->GetSounds("Resource/Sounds/UnitSE/Tank/Tank_Attack.mp3");


	light = LightMapManager::GetInstance();
	light->AddLight(this);
	collision.light_size = 2.0;

	collision.collision_size = Vector2D(60.0f, 60.0f);
	collision.hitbox_size = Vector2D(100.0f, 100.0f);
	z_layer = 1;


	// HP初期化
	HP = 40;

	__super::Initialize();
}

// 更新処理
void P_Tank::Update(float delta_second)
{

	__super::Update(delta_second);
}

// 描画処理
void P_Tank::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;
	position.y += z_layer * 2;

	//召喚陣描画
	if (now_state == State::Summon)
	{
		DrawRotaGraphF(position.x + collision.collision_size.y / 3, position.y + collision.collision_size.y / 2, 0.5, 0.0, effect_image, TRUE, flip_flag);
	}

	// 灯守の描画
	// オフセット値を基に画像の描画を行う
	if (Anim_count <= anim_max_count - 1)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawRotaGraphF(position.x, position.y, 2.5, 0.0, image, TRUE, flip_flag);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	if (!reduction_amount.empty() && now_state != State::Death)
	{
		for (int i = reduction_amount.size() - 1; i >= 0; --i) {
			DrawRotaGraphF(position.x - 20.0f, (position.y - 100.0f) + damage_time[i] * 100, 0.7, 0.0, minus, TRUE);
			DrawRotaGraphF(position.x, (position.y - 100.0f) + damage_time[i] * 100, 1.0, 0.0, text[reduction_amount[i]], TRUE);
		}
	}

	//ステートによってエフェクトの描画を行う
	switch (now_state)
	{
	case State::Death:
		position.y -= Effect_count * 10 + Effect_flame * 100;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, effect_alpha);
		DrawRotaGraphF(position.x, position.y, 2.0, 0.0, effect_image, TRUE, flip_flag);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		break;
	default:
		break;
	}
	/*DrawBox((int)(position.x - collision.box_size.x / 2), (int)(position.y - collision.box_size.y / 2),
		(int)(position.x + collision.box_size.x / 2), (int)(position.y + collision.box_size.y / 2), 0xff00ff, TRUE);*/

	__super::Draw(camera_pos);
}

// 終了時処理
void P_Tank::Finalize()
{
	light->DeleteLight(this);
	__super::Finalize();
}

// 当たり判定通知処理
void P_Tank::OnHitCollision(GameObject* hit_object)
{

}

// 攻撃範囲通知処理
void P_Tank::OnAreaDetection(GameObject* hit_object)
{
	//現在のステータスが死亡状態かどうか
	if (now_state != State::Death && Ingame->GetNowState() == GameState::PLAYING)
	{
		Collision hit_col = hit_object->GetCollision();

		if (hit_col.object_type == eObjectType::Enemy)
		{
			velocity.x = 0.0f;
			if (now_state != State::Summon)
			{
				now_state = State::Idle;
			}
		}
	}
}

// 移動処理
void P_Tank::Movement(float delta_second)
{
	__super::Movement(delta_second);
}

// アニメーション制御処理
void P_Tank::AnimationControl(float delta_second)
{
	// 画像の読み込み
	ResourceManager* rm = ResourceManager::GetInstance();

	//ステートが変わった時の初期化
	if (old_state != now_state)
	{
		Anim_flame = 0;
		Anim_count = 0;
		switch (now_state)
		{
		case State::Idle:
		case State::Summon:
			animation = rm->GetImages("Resource/Images/Unit/Tank/Tank_Unit_Idle.png", 10, 10, 1, 48, 32);
			anim_max_count = 10;
			break;
		case State::Move:
			animation = rm->GetImages("Resource/Images/Unit/Tank/Tank_Unit_Walk.png", 9, 9, 1, 48, 32);
			anim_max_count = 9;
			break;
		case State::Damage:
			PlaySoundMem(sounds, DX_PLAYTYPE_BACK);
			break;
		case State::Death:
			animation = rm->GetImages("Resource/Images/Unit/Tank/Tank_Unit_Down.png", 8, 8, 1, 48, 32);
			anim_max_count = 8;
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
	case State::Summon:
		image = animation[Anim_count];
		break;
	case State::Move:
		if (Ingame->GetNowState() == GameState::BOSS_DEAD)
		{
			velocity.x = -BASIC_Tank_SPEED - ((BASIC_Tank_SPEED / 100) * (Ingame->GetSunLevel()));
		}
		else
		{
			velocity.x = BASIC_Tank_SPEED + ((BASIC_Tank_SPEED / 100) * (Ingame->GetSunLevel()));
		}
		image = animation[Anim_count];
		break;
	case State::Death:
		image = animation[Anim_count];
		if (Anim_count == anim_max_count - 1)
		{
			object->CreateObject<Torch>(this->location);
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
void P_Tank::EffectControl(float delta_second)
{
	__super::EffectControl(delta_second);
}

//SEの制御処理
void P_Tank::SoundControl()
{
	if (now_state != old_state)
	{
		ResourceManager* rm = ResourceManager::GetInstance();
		switch (now_state)
		{

		case State::Attack:
			sounds = rm->GetSounds("Resource/Sounds/UnitSE/Tank/Tank_Attack.mp3");
			break;
		default:
			break;
		}
		__super::SoundControl();
	}
}