#include "Oracle.h"

// コンストラクタ
Oracle::Oracle() :
	summon_flag()
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

	is_mobility = false;

	collision.is_blocking = true;
	collision.object_type = eObjectType::Player;
	collision.hit_object_type.push_back(eObjectType::Enemy);
	collision.box_size = Vector2D(60.0f, 120.0f);
	collision.attack_size = Vector2D(400.0f, 200.0f);
	z_layer = 1;

	// HP初期化
	HP = 100;
}

// 更新処理
void Oracle::Update(float delta_second)
{

}

// 描画処理
void Oracle::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// 巫女の描画
	DrawBox((int)(position.x - collision.box_size.x / 2), (int)(position.y - collision.box_size.y / 2),
		(int)(position.x + collision.box_size.x / 2), (int)(position.y + collision.box_size.y / 2), 0xff0000, TRUE);

#ifdef DEBUG
	// 中心を表示
	DrawCircle(position.x, position.y, 2, 0x0000ff, TRUE);
	// 当たり判定表示
	DrawBox((int)(position.x - collision.box_size.x / 2), (int)(position.y - collision.box_size.y / 2),
		(int)(position.x + collision.box_size.x / 2), (int)(position.y + collision.box_size.y / 2), 0x0000ff, FALSE);
	// 攻撃範囲を表示
	DrawBox((int)(position.x - collision.attack_size.x / 2), (int)(position.y - collision.attack_size.y / 2),
		(int)(position.x + collision.attack_size.x / 2), (int)(position.y + collision.attack_size.y / 2), 0x0000ff, FALSE);
#endif
}

// 終了時処理
void Oracle::Finalize()
{

}

// 当たり判定通知処理
void Oracle::OnHitCollision(GameObject* hit_object)
{

}

// HP管理処理
void Oracle::HPControl(int Damage)
{

}

// 移動処理
void Oracle::Movement(float delta_second)
{

}

// アニメーション制御処理
void Oracle::AnimationControl(float delta_second)
{

}
// エフェクト制御処理
void Oracle::EffectControl(float delta_second)
{

}

// 攻撃処理
void Oracle::Attack()
{

}