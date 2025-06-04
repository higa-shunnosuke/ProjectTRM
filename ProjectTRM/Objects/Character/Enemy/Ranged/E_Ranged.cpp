#include "E_Ranged.h"
#include "../../../GameObjectManager.h"
#include "E_Projectile.h"

// 敵遠距離のカウンタを初期化
size_t E_Ranged::count = 0;

// 敵遠距離の数取得処理
size_t E_Ranged::GetCount()
{
	return count;
}

// コンストラクタ
E_Ranged::E_Ranged():
	anim_max_count(),
	recovery_time(),
	damage_rate(),
	anim_rate(),
	speed(),
	Damage(),
	effect(),
	old_light(false)
{
	count++;
}

// デストラクタ
E_Ranged::~E_Ranged()
{
	count--;

}

// 初期化処理
void E_Ranged::Initialize()
{
	// 画像の読み込み
	ResourceManager* rm = ResourceManager::GetInstance();
	effect_image = rm->GetImages("Resource/Images/Effect/Smoke-Sheet.png", 19, 4, 5, 80, 80);
	effect = effect_image[0];

	is_mobility = true;
	is_aggressive = true;

	collision.is_blocking = true;
	collision.object_type = eObjectType::Enemy;
	collision.hit_object_type.push_back(eObjectType::Player);
	collision.collision_size = Vector2D(50.0f, 100.0f);
	collision.hitbox_size = Vector2D(300.0f, 120.0f);
	z_layer = 2;

	flip_flag = false;

	// 最初の状態を移動にする
	now_state = State::Move;

	//攻撃力
	Damage = 5;

	// HP初期化
	HP = 20;

	// スピードの初期化
	speed = 70.0f;

	alpha = 0;
	add = -ALPHA_ADD;
}

// 更新処理
void E_Ranged::Update(float delta_second)
{
	// HPが０になると終了処理
	if (HP <= 0)
	{
		now_state = State::Death;
	}

	// 持続ダメージを与える
	if (in_light == true && damage_rate >= 1.0f)
	{
		HPControl(1);
		damage_rate = 0;
	}
	else
	{
		damage_rate += delta_second;
	}

	// 移動処理
	if (now_state == State::Move)
	{
		Movement(delta_second);
	}
	// 待機処理
	else if (now_state==State::Idle)
	{
		recovery_time += delta_second;

		// 待機時間が終わったら攻撃状態にする
		if (recovery_time >= 2.0f)
		{
			now_state = State::Move;
		}
	}

	// アニメーション管理処理
	AnimationControl(delta_second);
}

// 描画処理
void E_Ranged::Draw(const Vector2D camera_pos) const
{
	// 画像のずれ
	Vector2D offset;
	offset.x = 10.0f;
	offset.y = -30.0f;

	// カメラ座標をもとに描画位置を計算
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// 遠距離の描画
	DrawRotaGraphF(position.x + offset.x, position.y + offset.y,
		2.0, 0.0, image, TRUE, flip_flag);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawRotaGraphF(position.x, position.y - 70.0f,
		3.0, 0.0, effect, TRUE, flip_flag);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	if (ProjectConfig::DEBUG)
	{	
		int color;
		if (in_light == true) {
			color = 0xffffff;
		}
		else {
			color = 0xff0000;
		}
		//残りHPの表示
		DrawFormatString((int)position.x, (int)(position.y - 40.0f), color, "%f", HP);
		// 中心を表示
		DrawCircle((int)position.x, (int)position.y, 2, 0x0000ff, TRUE);
		// 当たり判定表示
		DrawBox((int)(position.x - collision.collision_size.x / 2), (int)(position.y - collision.collision_size.y / 2),
			(int)(position.x + collision.collision_size.x / 2), (int)(position.y + collision.collision_size.y / 2), 0xff0000, FALSE);
		// 攻撃範囲を表示
		DrawBox((int)position.x, (int)(position.y - collision.hitbox_size.y / 2),
			(int)(position.x + collision.hitbox_size.x), (int)(position.y + collision.hitbox_size.y / 2), 0xff0000, FALSE);
	}
}

// 終了時処理
void E_Ranged::Finalize()
{
	GameObjectManager* object = GameObjectManager::GetInstance();
	object->DestroyObject(this);
}

// 当たり判定通知処理
void E_Ranged::OnHitCollision(GameObject* hit_object)
{

}

// 攻撃範囲通知処理
void E_Ranged::OnAreaDetection(GameObject* hit_object)
{
	// 検知したオブジェクトのコリジョン情報
	Collision hit_col = hit_object->GetCollision();

	// 検知したオブジェクトがプレイヤーだったら
	if (hit_col.object_type == eObjectType::Player)
	{
		// 移動状態なら攻撃状態にする
		if (now_state == State::Move)
		{
			now_state = State::Attack;
		}
		// 攻撃状態なら攻撃する
		else if (now_state == State::Attack)
		{
			if (Anim_count == anim_max_count - 1)
			{
				// 攻撃処理
				Attack(hit_object);

				// 硬直開始
				now_state = State::Idle;
				recovery_time = 0;
			}
		}
	}
}

// 攻撃範囲通知処理
void E_Ranged::NoHit()
{
	
}

// ライト範囲通知処理
void E_Ranged::InLightRange()
{
	in_light = true;
	Damage = 2;
	speed = 30.0f;
}

// ライト範囲通知処理
void E_Ranged::OutLightRange()
{
	in_light = false;
	Damage = 5;
	speed = 70.0f;
}

// HP管理処理
void E_Ranged::HPControl(int Damage)
{
	// ダメージ軽減
	if (!in_light)
	{
		Damage = 1;
	}

	// ダメージ反映
	this->HP -= Damage;
	if (this->HP < 0)
	{
		this->HP = 0;
	}
}

// 攻撃処理
void E_Ranged::Attack(GameObject* hit_object)
{
	GameObjectManager* object = GameObjectManager::GetInstance();
	object->CreateObject<E_Projectile>(this->location)->SetTargetLocation(hit_object->GetLocation());
}

// 移動処理
void E_Ranged::Movement(float delta_second)
{
	// 右向きに移動させる
	velocity.x = speed;

	// 移動の実行
	location += velocity * delta_second;
}

// アニメーション制御処理
void E_Ranged::AnimationControl(float delta_second)
{
	// 状態が切り替わったらカウントを初期化
	if (old_state != now_state)
	{
		Anim_count = 0;

		// 画像の読み込み
		ResourceManager* rm = ResourceManager::GetInstance();

		// 各状態のアニメーション画像に差し替え
		switch (now_state)
		{
		case State::Idle:
			animation = rm->GetImages("Resource/Images/Enemy/Ranged/E_Ranged_Idle.png", 2, 2, 1, 100, 75);
			image = animation[Anim_count];
			anim_max_count = 1;
			anim_rate = 0.3f;
			break;
		case State::Move:
			animation = rm->GetImages("Resource/Images/Enemy/Ranged/E_Ranged_Walk.png", 6, 6, 1, 100, 75);
			image = animation[Anim_count];
			anim_max_count = 5;
			anim_rate = 0.1f;
			break;
		case State::Attack:
			animation = rm->GetImages("Resource/Images/Enemy/Ranged/E_Ranged_Attack.png", 6, 6, 1, 100, 75);
			image = animation[Anim_count];
			anim_max_count = 5;
			anim_rate = 0.1f;
			break;
		case State::Damage:
			break;
		case State::Death:
			animation = rm->GetImages("Resource/Images/Enemy/Ranged/E_Ranged_Dead.png", 4, 4, 1, 100, 75);
			image = animation[Anim_count];
			anim_max_count = 3;
			anim_rate = 0.2f;
			break;
		}
	}

	// 状態更新処理
	old_state = now_state;

	// アニメーションの実行
	switch (now_state)
	{
	case State::Idle:
		image = animation[Anim_count];
		break;
	case State::Move:
		image = animation[Anim_count];
		break;
	case State::Attack:
		image = animation[Anim_count];
		// 硬直開始
		if (Anim_count == anim_max_count)
		{
			now_state = State::Idle;
			recovery_time = 0;
		}
		break;
	case State::Death:
		image = animation[Anim_count];
		// 死亡
		if (Anim_count == anim_max_count)
		{
			Finalize();
		}
		break;
	}

	// エフェクトの更新
	effect_flame += delta_second;

	if (effect_flame >= 0.2f)
	{
		if (effect_count < 18)
		{
			effect_count++;
		}
		else
		{
			effect_count = 0;
		}

		// エフェクトの透明化処理
		if (old_light == false)
		{
			if (alpha < 200)
			{
				alpha += 40;
			}
			else
			{
				alpha = 200;
			}
		}
		else if (old_light == true)
		{
			if (alpha > 0)
			{
				alpha -= 40;
			}
			else
			{
				alpha = 0;
			}
		}

		effect_flame = 0.0f;
	}

	// ライトフラグの更新
	old_light = in_light;

	// エフェクトのアニメーション
	effect = effect_image[Anim_count];

	// アニメーションの更新
	Anim_flame += delta_second;

	// 光に入っていたらアニメーションを遅くする
	float delay = 1.0f;
	if (in_light == true && (now_state != State::Death && now_state != State::Attack))
	{
		delay = 2.0f;
	}

	// アニメーション間隔
	if (Anim_flame >= anim_rate * delay)
	{
		// 次のアニメーションに進める
		if (Anim_count < anim_max_count)
		{
			Anim_count++;
		}
		else
		{
			Anim_count = 0;
		}

		// アニメーション開始時間の初期化
		Anim_flame = 0;
	}
}
// エフェクト制御処理
void E_Ranged::EffectControl(float delta_second)
{

}