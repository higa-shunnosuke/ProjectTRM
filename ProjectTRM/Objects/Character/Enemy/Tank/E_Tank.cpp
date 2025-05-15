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
	Damage(),
	anime_time(),
	recovery_time()
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
	collision.box_size = Vector2D(60.0f, 60.0f);
	collision.attack_size = Vector2D(100.0f, 100.0f);
	z_layer = 2;

	attack_flag = false;
	flip_flag = false;
	
	in_light = false;

	// 最初の状態を移動にする
	now_state = State::Move;

	// 右へ移動
	velocity.x = 5.0f;

	//攻撃力
	Damage = 1;

	// HP初期化
	HP = 100;
}

// 更新処理
void E_Tank::Update(float delta_second)
{
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

	// 状態更新処理
	old_state = now_state;

	// 持続ダメージ
	if (in_light == true && anime_time >= 0.1f)
	{
		HP -= 1;
		// アニメーション開始時間の初期化
		anime_time = 0;
	}

	// HPが０になると終了処理
	if (HP <= 0)
	{
		Finalize();
	}
}

// 描画処理
void E_Tank::Draw(const Vector2D camera_pos) const
{
	// カメラ座標をもとに描画位置を計算
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// 敵近接の描画
	DrawRotaGraphF(position.x, position.y, 2.0, 0.0, image, TRUE, flip_flag);

#ifdef DEBUG
	//残りHPの表示
	if (in_light == true)
	{
		DrawFormatString((int)position.x, (int)(position.y - 40.0f), 0xffffff, "%d", HP);
	}
	else
	{
		DrawFormatString((int)position.x, (int)(position.y - 40.0f), 0xff0000, "%d", HP);
	}

	// 中心を表示
	DrawCircle((int)position.x, (int)position.y, 2, 0x0000ff, TRUE);
	// 当たり判定表示
	/*DrawBox((int)(position.x - collision.box_size.x / 2), (int)(position.y - collision.box_size.y / 2),
		(int)(position.x + collision.box_size.x / 2), (int)(position.y + collision.box_size.y / 2), 0xff00a0, TRUE);*/
	DrawBox((int)(position.x - collision.box_size.x / 2), (int)(position.y - collision.box_size.y / 2),
		(int)(position.x + collision.box_size.x / 2), (int)(position.y + collision.box_size.y / 2), 0x0000ff, FALSE);
	// 攻撃範囲を表示
	DrawBox((int)(position.x - collision.attack_size.x / 2), (int)(position.y - collision.attack_size.y / 2),
		(int)(position.x + collision.attack_size.x / 2), (int)(position.y + collision.attack_size.y / 2), 0x0000ff, FALSE);
#endif
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
			if (recovery_time >= 1.0f)
			{
				now_state = State::Attack;
			}
		}
		// 攻撃状態なら攻撃する
		else if (now_state == State::Attack)
		{
			if (Anim_count == 3)
			{
				// 攻撃処理
				Attack(hit_object);
			}
		}
	}
}

// 攻撃範囲通知処理
void E_Tank::NoHit()
{
	// 待機状態なら待機する
	if (now_state == State::Idle)
	{
		// 待機時間が終わったら移動状態にする
		if (recovery_time >= 1.0f)
		{
			now_state = State::Move;
		}
	}
	// 攻撃状態でなければ移動状態にする
	else if (now_state != State::Attack)
	{
		now_state = State::Move;
	}
}

// ライト範囲通知処理
void E_Tank::InLightRange()
{
	in_light = true;
}

// ライト範囲通知処理
void E_Tank::OutLightRange()
{
	in_light = false;
}

// HP管理処理
void E_Tank::HPControl(int Damage)
{
	// ダメージ軽減
	if (!in_light)
	{
		Damage *= 0.5;
	}

	this->HP -= (int)Damage;
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
	velocity.x = 5.0f;

	// 移動の実行
	location += velocity * 10 * delta_second;
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
			animation = rm->GetImages("Resource/Images/Enemy/Tank/Tank_Walk.png", 4, 4, 1, 32, 32);
			break;
		case State::Move:
			animation = rm->GetImages("Resource/Images/Enemy/Tank/Tank_Walk.png", 4, 4, 1, 32, 32);
			break;
		case State::Attack:
			animation = rm->GetImages("Resource/Images/Enemy/Tank/Tank_Attack.png", 4, 4, 1, 32, 32);
			image = animation[Anim_count];
			break;
		case State::Damage:
			break;
		case State::Death:
			break;
		}
	}

	// アニメーションの実行
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
		// 硬直開始
		if (Anim_count == 3)
		{
			now_state = State::Idle;
			recovery_time = 0;
		}
		break;
	case State::Damage:
		break;
	case State::Death:
		break;
	}

	// アニメーションの更新
	anime_time += delta_second;

	// アニメーション間隔
	if (anime_time >= 0.1f)
	{
		// 次のアニメーションに進める
		if (Anim_count < 3)
		{
			Anim_count++;
		}
		else
		{
			Anim_count = 0;
		}

		if (in_light == false)
		{
			// アニメーション開始時間の初期化
			anime_time = 0;
		}
	}
}
// エフェクト制御処理
void E_Tank::EffectControl(float delta_second)
{

}