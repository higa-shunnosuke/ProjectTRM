#include "E_Melee.h"

// 敵近接のカウンタを初期化
size_t E_Melee::count = 0;

// 敵近接の数取得処理
size_t E_Melee::GetCount()
{
	return count;
}

// コンストラクタ
E_Melee::E_Melee() :
	Damage()
{
	count++;
}

// デストラクタ
E_Melee::~E_Melee()
{
	count--;
}

// 初期化処理
void E_Melee::Initialize()
{
	// 現在時刻を取得
	anime_time = std::chrono::steady_clock::now();

	is_mobility = true;
	is_aggressive = true;

	collision.is_blocking = true;
	collision.object_type = eObjectType::Enemy;
	collision.hit_object_type.push_back(eObjectType::Player);
	collision.box_size = Vector2D(60.0f, 60.0f);
	collision.attack_size = Vector2D(100.0f, 100.0f);
	z_layer = 2;

	flip_flag = false;

	// 最初の状態を移動にする
	now_state = State::Move;

	//攻撃力
	Damage = 2;

	// HP初期化
	HP = 20;

	alpha = MAX_ALPHA;
	add = -ALPHA_ADD;
}

// 更新処理
void E_Melee::Update(float delta_second)
{
	// 移動処理
	if (now_state == State::Move)
	{
		Movement(delta_second);
	}

	// 硬直処理
	if (now_state == State::Damage)
	{
		// 現在時刻を取得
		auto now_time = std::chrono::steady_clock::now();

		// 硬直時間
		if (now_time - recovery_time > std::chrono::milliseconds(1000))
		{
			now_state = State::Move;
		}
	}

	// アニメーション管理処理
	AnimationControl(delta_second);

	// 状態更新処理
	old_state = now_state;
}

// 描画処理
void E_Melee::Draw(const Vector2D camera_pos) const
{
	// カメラ座標をもとに描画位置を計算
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// 敵近接の描画
	DrawRotaGraphF(position.x, position.y, 2.0, 0.0, image, TRUE, flip_flag);

#ifdef DEBUG
	//残りHPの表示
	DrawFormatString((int)position.x, (int)(position.y - 40.0f), 0xffffff, "%d", HP);

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
void E_Melee::Finalize()
{

}

// 当たり判定通知処理
void E_Melee::OnHitCollision(GameObject* hit_object)
{

}

// 攻撃範囲通知処理
void E_Melee::OnAreaDetection(GameObject* hit_object)
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
			// 攻撃処理
			Attack(hit_object);
		}
		// 待機状態なら攻撃状態にする
		else if (now_state == State::Idle)
		{
			// 現在時刻を取得
			auto now_time = std::chrono::steady_clock::now();

			// 待機時間
			if (now_time - recovery_time > std::chrono::milliseconds(2000))
			{
				now_state = State::Attack;
				// 攻撃処理
				Attack(hit_object);
			}
		}
	}
}

// HP管理処理
void E_Melee::HPControl(int Damage)
{
	// 攻撃状態でなければダメージ状態にする
	if (now_state != State::Attack)
	{
		now_state = State::Damage;
	}

	__super::HPControl(Damage);
}

// 攻撃処理
void E_Melee::Attack(GameObject* hit_object)
{
	// 攻撃対象にダメージを与える
	hit_object->HPControl(Damage);
}

// 移動処理
void E_Melee::Movement(float delta_second)
{
	// 右向きに移動させる
	velocity.x = 5.0f;

	// 移動の実行
	location += velocity * 10 * delta_second;
}

// アニメーション制御処理
void E_Melee::AnimationControl(float delta_second)
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
			animation = rm->GetImages("Resource/Images/Enemy/Melee/Melee_Walk.png", 4, 4, 1, 32, 32);
			break;
		case State::Move:
			animation = rm->GetImages("Resource/Images/Enemy/Melee/Melee_Walk.png", 4, 4, 1, 32, 32);
			break;
		case State::Attack:
			animation = rm->GetImages("Resource/Images/Enemy/Melee/Melee_Attack.png", 4, 4, 1, 32, 32);
			image = animation[Anim_count];
			// 硬直開始
			if (Anim_count == 3)
			{
				now_state = State::Idle;
				recovery_time = std::chrono::steady_clock::now();
			}
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
			recovery_time = std::chrono::steady_clock::now();
		}
		break;
	case State::Damage:
		// ダメージ状態開始
		recovery_time = std::chrono::steady_clock::now();
		break;
	case State::Death:
		break;
	}

	// 現在時刻を取得
	auto now_time = std::chrono::steady_clock::now();

	// アニメーション間隔
	if (now_time - anime_time > std::chrono::milliseconds(500))
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

		// アニメーション開始時間の更新
		anime_time = std::chrono::steady_clock::now();
	}
}
// エフェクト制御処理
void E_Melee::EffectControl(float delta_second)
{

}
