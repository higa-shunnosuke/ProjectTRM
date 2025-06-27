#include "Boss.h"
#include "../../../GameObjectManager.h"

// 敵近接のカウンタを初期化
size_t Boss::count = 0;

// 敵遠距離の数取得処理
size_t Boss::GetCount()
{
	return count;
}

// コンストラクタ
Boss::Boss()
{
	count++;
}

// デストラクタ
Boss::~Boss()
{
	count--;
}

// 初期化処理
void Boss::Initialize()
{
	// 親クラスの初期化
	__super::Initialize();

	// フラグ設定
	is_AoE = true;

	// コリジョン設定
	collision.collision_size = Vector2D(180.0f, 180.0f);
	collision.hitbox_size = Vector2D(200.0f, 300.0f);
	z_layer = 2;

	// 画像反転あり
	flip_flag = true;

	// 最初の状態を移動にする
	now_state = State::Move;

	//攻撃力
	Damage = 6.0f;

	// HP初期化
	HP = 800;

	// スピードの初期化
	speed = 20.0f;

	// リカバリーの初期化
	recovery_time = 6.0f;
}

// 更新処理
void Boss::Update(float delta_second)
{
	// 親クラスの更新
	__super::Update(delta_second);
}

// 描画処理
void Boss::Draw() const
{
	// 画像のずれ
	Vector2D offset;
	offset.x = 100.0f;
	offset.y = -50.0f;

	// 敵近接の描画
	DrawRotaGraphF(location.x + offset.x, location.y + offset.y,
		3.0, 0.0, image, TRUE, flip_flag);

	if (now_state != State::Death)
	{
		// エフェクト１の描画
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawRotaGraphF(location.x, location.y - 110.0f,
			5.0, 0.0, effect[0], TRUE, flip_flag);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// エフェクト２の描画
		DrawRotaGraphF(location.x - 5.0f, location.y + 5.0f,
			1.0, 0.0, effect[1], TRUE, flip_flag);
	}
	
	// 親クラスの描画
	__super::Draw();
}

// 終了時処理
void Boss::Finalize()
{
	// 親クラスの終了
	__super::Finalize();
}

// 当たり判定通知処理
void Boss::OnHitCollision(GameObject* hit_object)
{

}

// 攻撃範囲通知処理
void Boss::OnAreaDetection(GameObject* hit_object)
{
	// 親クラスの攻撃範囲通知処理
	__super::OnAreaDetection(hit_object);
}

// 攻撃範囲通知処理
void Boss::NoHit()
{
	
}

// ライト範囲通知処理
void Boss::InLightRange()
{
	in_light = true;
	Damage = 3.0f;
	speed = 10.0f;
}

// ライト範囲通知処理
void Boss::OutLightRange()
{
	in_light = false;
	Damage = 6.0f;
	speed = 20.0f;
}

// 移動処理
void Boss::Movement(float delta_second)
{
	// 親クラスの移動処理
	__super::Movement(delta_second);
}

// アニメーション制御処理
void Boss::AnimationControl(float delta_second)
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
			animation = rm->GetImages("Resource/Images/Enemy/Boss/Boss_Idle.png", 8, 8, 1, 140, 93);
			image = animation[Anim_count];
			anim_max_count = 7;
			anim_rate = 0.2f;
			break;
		case State::Move:
			animation = rm->GetImages("Resource/Images/Enemy/Boss/Boss_Walk.png", 8, 8, 1, 140, 93);
			image = animation[Anim_count];
			anim_max_count = 7;
			anim_rate = 0.2f;
			break;
		case State::Attack:
			animation = rm->GetImages("Resource/Images/Enemy/Boss/Boss_Attack.png", 10, 10, 1, 140, 93);
			image = animation[Anim_count];
			anim_max_count = 9;
			anim_rate = 0.15f;
			on_hit = 4;
			break;
		case State::Damage:
			break;
		case State::Death:
			animation = rm->GetImages("Resource/Images/Enemy/Boss/Boss_Death.png", 10, 10, 1, 140, 93);
			image = animation[Anim_count];
			anim_max_count = 9;
			anim_rate = 0.2f;
			break;
		}
	}

	// 親クラスのアニメーション]
	__super::AnimationControl(delta_second);
}
// エフェクト制御処理
void Boss::EffectControl(float delta_second)
{
	// 親クラスのエフェクト
	__super::EffectControl(delta_second);
}