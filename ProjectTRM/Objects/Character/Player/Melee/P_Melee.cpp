#include "P_Melee.h"
#include "../../../../Utility/LightMapManager.h"
#include "../../../../Scenes/InGame/InGame.h"
#include <iostream >

size_t P_Melee:: count = 0;
size_t P_Melee::GetCount()
{
	return count;
}

// コンストラクタ
P_Melee::P_Melee()
{
	count++;
}

// デストラクタ
P_Melee::~P_Melee()
{
	count--;
}

// 初期化処理
void P_Melee::Initialize()
{
	// 画像の読み込み
	ResourceManager* rm = ResourceManager::GetInstance();
	animation = rm->GetImages("Resource/Images/Unit/Melee/Unit_Melee_Walk.png", 10, 10, 1, 100, 55);
	Effect = rm->GetImages("Resource/Images/Effect/Magic_Remove.png", 10, 5, 2, 192, 192);
	sounds = rm->GetSounds("Resource/Sounds/UnitSE/damage02.wav");

	collision.collision_size = Vector2D(60.0f, 60.0f);
	collision.hitbox_size = Vector2D(70.0f, 100.0f);
	z_layer = 3;

	attack_flag = false;
	flip_flag = true;

	now_state = State::Summon;

	//攻撃力
	basic_power = BASIC_Melee_POWER;

	basic_speed = BASIC_Melee_SPEED;

	// HP初期化
	HP = 30;

	__super::Initialize();

}

// 更新処理
void P_Melee::Update(float delta_second)
{	
	// 親クラスの更新
	__super::Update(delta_second);
}

// 描画処理
void P_Melee::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;
	position.y += z_layer * 2;

	//召喚陣描画
	if (now_state == State::Summon)
	{
		DrawRotaGraphF(position.x, position.y + collision.collision_size.y / 3, 0.5, 0.0, effect_image, TRUE, flip_flag);
	}

	// 近接ユニットの描画
	// オフセット値を基に画像の描画を行う
	if (Anim_count <= anim_max_count)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawRotaGraphF(position.x, position.y, 1.5, 0.0, image, TRUE, flip_flag);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	if (!reduction_amount.empty() && now_state != State::Death)
	{
		for (int i = reduction_amount.size() - 1; i >= 0; --i) {
			DrawRotaGraphF(position.x - 20.0f, (position.y - 100.0f) + damage_time[i] * 100, 0.7, 0.0, minus, TRUE);
			DrawRotaGraphF(position.x, (position.y - 100.0f) + damage_time[i] * 100, 1.0, 0.0, text[reduction_amount[i]], TRUE);
		}
	}

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
	__super::Draw(camera_pos);
}

// 終了時処理
void P_Melee::Finalize()
{
	__super::Finalize();
}

// 当たり判定通知処理
void P_Melee::OnHitCollision(GameObject* hit_object)
{

}

// 攻撃範囲通知処理
void P_Melee::OnAreaDetection(GameObject* hit_object)
{
	// 親クラスの攻撃範囲通知処理
	__super::OnAreaDetection(hit_object);

}

// 移動処理
void P_Melee::Movement(float delta_second)
{
	//親クラスの移動処理
	__super::Movement(delta_second);
}

// アニメーション制御処理
void P_Melee::AnimationControl(float delta_second)
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
			animation = rm->GetImages("Resource/Images/Unit/Melee/Unit_Melee_Idle.png", 8, 8, 1, 100, 55);
			anim_max_count = 8;
			break;
		case State::Move:
			animation = rm->GetImages("Resource/Images/Unit/Melee/Unit_Melee_Walk.png", 10, 10, 1, 100, 55);
			anim_max_count = 10;
			break;
		case State::Attack:
			animation = rm->GetImages("Resource/Images/Unit/Melee/Unit_Melee_Attack.png", 9, 9, 1, 100, 55);
			anim_max_count = 9;
			if (Anim_count == anim_max_count)
			{				
				attack_flag = true;
				now_state = State::Idle;
			}
			break;
		case State::Death:
			animation = rm->GetImages("Resource/Images/Unit/Melee/Unit_Melee_Death.png", 9, 9, 1, 100, 55);
			anim_max_count = 9;
			break;
		default:
			break;
		}
		old_state = now_state;
	}

	// 親クラスのアニメーション]
	__super::AnimationControl(delta_second);

}
// エフェクト制御処理
void P_Melee::EffectControl(float delta_second)
{
	//親クラスのエフェクト制御処理
	__super::EffectControl(delta_second);
}

//SEの制御処理
void P_Melee::SoundControl()
{
	if (now_state != old_state)
	{
		ResourceManager* rm = ResourceManager::GetInstance();
		switch (now_state)
		{

		case State::Attack:
			sounds = rm->GetSounds("Resource/Sounds/UnitSE/Tank/Tank_Hit.mp3");
			break;
		default:
			break;
		}
		//親クラスのSE制御処理
		__super::SoundControl();
	}
}