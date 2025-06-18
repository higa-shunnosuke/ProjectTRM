#include "EnemyBase.h"
#include "../../GameObjectManager.h"

// コンストラクタ
EnemyBase::EnemyBase() :
	anim_max_count(),
	recovery_time(),
	recovery_flame(),
	damage_rate(),
	anim_rate(),
	effect_flame(),
	effect(),
	effect_count(),
	effect_max_count(),
	SE(),
	speed(),
	Damage(),
	on_hit(),
	is_attack(false),
	old_light(false)
{
	
}

// デストラクタ
EnemyBase::~EnemyBase()
{
	
}

// 初期化処理
void EnemyBase::Initialize()
{
	// 画像の読み込み
	ResourceManager* rm = ResourceManager::GetInstance();
	// 闇エフェクト
	effect_image[0] = rm->GetImages("Resource/Images/Effect/Enemy/Smoke.png", 19, 4, 5, 80, 80);
	effect[0] = effect_image[0][0];
	effect_max_count[0] = 18;
	// 持続ダメージエフェクト
	effect_image[1] = rm->GetImages("Resource/Images/Effect/Enemy/Spark.png", 30, 5, 6, 150, 150);
	effect[1] = effect_image[1][0];
	effect_max_count[1] = 29;

	// 音源の読み込み
	SE[0] = rm->GetSounds("Resource/Sounds/EnemySE/Damage1.mp3");
	SE[1] = rm->GetSounds("Resource/Sounds/EnemySE/Damage2.mp3");
	SE[2] = rm->GetSounds("Resource/Sounds/EnemySE/Death.mp3");
	SE[3] = rm->GetSounds("Resource/Sounds/EnemySE/Attack02.mp3");
	SE[4] = rm->GetSounds("Resource/Sounds/UnitSE/Ranged/Ranged_Attack.mp3");

	// 音量設定
	for (int i = 0; i < 5; i++)
	{
		ChangeVolumeSoundMem(200, SE[i]);
	}

	alpha = 200;
	add = -ALPHA_ADD;

	// フラグ設定
	is_mobility = true;
	is_aggressive = true;
	is_AoE = false;

	// コリジョン設定
	collision.is_blocking = true;
	collision.object_type = eObjectType::Enemy;
	collision.hit_object_type.push_back(eObjectType::Player);
}

// 更新処理
void EnemyBase::Update(float delta_second)
{
	// HPが０になると死亡状態にする
	if (HP <= 0)
	{
		now_state = State::Death;
		collision.object_type = eObjectType::None;
	}

	// 持続ダメージを与える
	if (in_light == true && damage_rate >= 1.0f)
	{
		HP -= 1.0f;
		damage_rate = 0.0f;
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
		recovery_flame += delta_second;

		// 待機時間が終わったら攻撃状態にする
		if (recovery_flame >= recovery_time)
		{
			now_state = State::Move;
		}
	}

	// アニメーション管理処理
	AnimationControl(delta_second);

	// エフェクト管理処理
	EffectControl(delta_second);
}

// 描画処理
void EnemyBase::Draw(const Vector2D camera_pos) const
{
	// カメラ座標をもとに描画位置を計算
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

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
		DrawFormatString((int)position.x, (int)(position.y - 40.0f), color, "%.1f", HP);
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
void EnemyBase::Finalize()
{
	GameObjectManager* object = GameObjectManager::GetInstance();
	object->DestroyObject(this);
}

// 当たり判定通知処理
void EnemyBase::OnHitCollision(GameObject* hit_object)
{

}

// 攻撃範囲通知処理
void EnemyBase::OnAreaDetection(GameObject* hit_object)
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
			if (Anim_count == on_hit)
			{
				if (is_attack == false)
				{
					// 攻撃処理
					Attack(hit_object);

					is_attack = true;
				}
			}
		}
	}
}

// 攻撃範囲通知処理
void EnemyBase::NoHit()
{
	
}

// ライト範囲通知処理
void EnemyBase::InLightRange()
{
	
}

// ライト範囲通知処理
void EnemyBase::OutLightRange()
{
	
}

// HP管理処理
void EnemyBase::HPControl(float Damage)
{
	// エフェクトが終わっていたら開始する
	if (effect_count[1] == 0)
	{
		effect_count[1] = 1;
	}

	// ダメージ軽減
	if (!in_light)
	{
		Damage = 1;
		PlaySoundMem(SE[0], DX_PLAYTYPE_BACK);
	}

	// ダメージ反映
	this->HP -= Damage;
	PlaySoundMem(SE[1], DX_PLAYTYPE_BACK);
	if (this->HP < 0)
	{
		this->HP = 0;
		PlaySoundMem(SE[2], DX_PLAYTYPE_BACK);
	}
}

// 攻撃処理
void EnemyBase::Attack(GameObject* hit_object)
{
	// 攻撃対象にダメージを与える
	hit_object->HPControl(Damage);
}

// 移動処理
void EnemyBase::Movement(float delta_second)
{
	// 右向きに移動させる
	velocity.x = speed;

	// 移動の実行
	location += velocity * delta_second;
}

// アニメーション制御処理
void EnemyBase::AnimationControl(float delta_second)
{
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
			is_attack = false;
			recovery_flame = 0;
		}
		else if (Anim_count == 0)
		{
			// 攻撃SE再生
			PlaySoundMem(SE[3], DX_PLAYTYPE_BACK);
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
		Anim_flame = 0.0f;
	}
}

// エフェクト制御処理
void EnemyBase::EffectControl(float delta_second)
{
	for (int i = 0; i < 2; i++)
	{
		switch (i)
		{
		case 0:
			effect_flame[0] += delta_second;

			if (effect_flame[0] >= 0.2f)
			{
				// エフェクトを更新
				if (effect_count[0] < effect_max_count[0])
				{
					effect_count[0]++;
				}
				else
				{
					effect_count[0] = 0;
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

				effect_flame[0] = 0.0f;
			}
			break;
		case 1:
			effect_flame[1] += delta_second;

			if (effect_flame[1] >= 0.02f)
			{
				// エフェクトが０枚目ではないなら更新する
				if (effect_count[1] != 0)
				{
					if (effect_count[1] < effect_max_count[1])
					{
						effect_count[1]++;
					}
					else
					{
						effect_count[1] = 0;
					}
				}
				effect_flame[1] = 0.0f;
			}
		}
	}

	// ライトフラグの更新
	old_light = in_light;

	// エフェクトのアニメーション
	for (int i = 0; i < 2; i++)
	{
		effect[i] = effect_image[i][effect_count[i]];
	}
}
