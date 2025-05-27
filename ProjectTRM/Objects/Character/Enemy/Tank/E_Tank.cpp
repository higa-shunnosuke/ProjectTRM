#include "E_Tank.h"
#include "../../../GameObjectManager.h"

// 敵近接のカウンタを初期化
size_t E_Tank::count = 0;

// 敵近接の数取得処理
size_t E_Tank::GetCount()
{
	return count;
}

// コンストラクタ
E_Tank::E_Tank() :
	anim_max_count(),
	recovery_time(),
	damage_rate(),
	anim_rate(),
	speed(),
	Damage()
{
	count++;
}

// デストラクタ
E_Tank::~E_Tank()
{
	count--;
}

// 初期化処理
void E_Tank::Initialize()
{
	is_mobility = true;
	is_aggressive = true;

	collision.is_blocking = true;
	collision.object_type = eObjectType::Enemy;
	collision.hit_object_type.push_back(eObjectType::Player);
	collision.collision_size = Vector2D(50.0f, 100.0f);
	collision.hitbox_size = Vector2D(50.0f, 120.0f);
	z_layer = 2;

	flip_flag = false;

	// 最初の状態を移動にする
	now_state = State::Move;

	//攻撃力
	Damage = 2;

	// HP初期化
	HP = 100;

	// スピードの初期化
	speed = 50.0f;

	alpha = MAX_ALPHA;
	add = -ALPHA_ADD;
}

// 更新処理
void E_Tank::Update(float delta_second)
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
	else if (now_state == State::Idle)
	{
		recovery_time += delta_second;
	}
	
	// アニメーション管理処理
	AnimationControl(delta_second);
}

// 描画処理
void E_Tank::Draw(const Vector2D camera_pos) const
{
	// 画像のずれ
	Vector2D offset;
	offset.x = 30.0f;
	offset.y = -30.0f;

	// カメラ座標をもとに描画位置を計算
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// 敵近接の描画
	DrawRotaGraphF(position.x + offset.x, position.y + offset.y,
		2.0, 0.0, image, TRUE, flip_flag);

	if (ProjectConfig::DEBUG)
	{	
		int color;
		if (in_light == true){
			color = 0xffffff;
		}
		else{
			color = 0xff0000;
		}
		//残りHPの表示
		DrawFormatString((int)position.x, (int)(position.y - 40.0f), color, "%d", HP);
		// 中心を表示
		DrawCircle((int)position.x, (int)position.y, 2, 0x0000ff, TRUE);
		// 当たり判定表示
		DrawBox((int)(position.x - collision.collision_size.x / 2), (int)(position.y - collision.collision_size.y / 2),
			(int)(position.x + collision.collision_size.x / 2), (int)(position.y + collision.collision_size.y / 2), 0x0000ff, FALSE);
		// 攻撃範囲を表示
		DrawBox((int)position.x, (int)(position.y - collision.hitbox_size.y / 2),
			(int)(position.x + collision.hitbox_size.x), (int)(position.y + collision.hitbox_size.y / 2), 0x0000ff, FALSE);
	}
}

// 終了時処理
void E_Tank::Finalize()
{
	GameObjectManager* object = GameObjectManager::GetInstance();
	object->DestroyObject(this);
}

// 当たり判定通知処理
void E_Tank::OnHitCollision(GameObject* hit_object)
{

}

// 攻撃範囲通知処理
void E_Tank::OnAreaDetection(GameObject* hit_object)
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
		// 待機状態なら待機する
		else if (now_state == State::Idle)
		{
			// 待機時間が終わったら攻撃状態にする
			if (recovery_time >= 4.0f)
			{
				now_state = State::Attack;
			}
		}
		// 攻撃状態なら攻撃する
		else if (now_state == State::Attack)
		{
			if (Anim_count == anim_max_count)
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
void E_Tank::NoHit()
{
	// 移動状態にする
	if (now_state != State::Death)
	{
		now_state = State::Move;
	}
}

// ライト範囲通知処理
void E_Tank::InLightRange()
{
	in_light = true;
	Damage = 1;
	speed = 30.0f;
}

// ライト範囲通知処理
void E_Tank::OutLightRange()
{
	in_light = false;
	Damage = 2;
	speed = 50.0f;
}

// HP管理処理
void E_Tank::HPControl(int Damage)
{
	// ダメージ軽減
	if (!in_light)
	{
		Damage = 0;
	}

	// ダメージ反映
	this->HP -= Damage;
	if (this->HP < 0)
	{
		this->HP = 0;
	}
}

// 攻撃処理
void E_Tank::Attack(GameObject* hit_object)
{
	// 攻撃対象にダメージを与える
	hit_object->HPControl(Damage);
}

// 移動処理
void E_Tank::Movement(float delta_second)
{
	// 右向きに移動させる
	velocity.x = speed;

	// 移動の実行
	location += velocity * delta_second;
}

// アニメーション制御処理
void E_Tank::AnimationControl(float delta_second)
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
			animation = rm->GetImages("Resource/Images/Enemy/Tank/E_Tank_Idle.png", 4, 4, 1, 100, 75);
			image = animation[Anim_count];
			anim_max_count = 3;
			anim_rate = 0.3f;
			break;
		case State::Move:
			animation = rm->GetImages("Resource/Images/Enemy/Tank/E_Tank_Walk.png", 6, 6, 1, 100, 75);
			image = animation[Anim_count];
			anim_max_count = 5;
			anim_rate = 0.1f;
			break;
		case State::Attack:
			animation = rm->GetImages("Resource/Images/Enemy/Tank/E_Tank_Attack.png", 8, 8, 1, 100, 75);
			image = animation[Anim_count];
			anim_max_count = 7;
			anim_rate = 0.1f;
			break;
		case State::Damage:
			break;
		case State::Death:
			animation = rm->GetImages("Resource/Images/Enemy/Tank/E_Tank_Dead.png", 4, 4, 1, 100, 75);
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
		break;
	case State::Death:
		image = animation[Anim_count];
		// 硬直開始
		if (Anim_count == anim_max_count)
		{
			Finalize();
		}
		break;
	}

	// アニメーションの更新
	Anim_flame += delta_second;

	// 光に入っていたらアニメーションを遅くする
	float delay = 1.0f;
	if (in_light == true && now_state != State::Death)
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
void E_Tank::EffectControl(float delta_second)
{

}
