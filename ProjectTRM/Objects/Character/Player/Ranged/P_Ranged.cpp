#include "P_Ranged.h"

// コンストラクタ
P_Ranged::P_Ranged()
{

}

// デストラクタ
P_Ranged::~P_Ranged()
{

}

// 初期化処理
void P_Ranged::Initialize()
{
	// 画像の読み込み
	ResourceManager* rm = ResourceManager::GetInstance();

	is_mobility = true;

	collision.is_blocking = true;
	collision.object_type = eObjectType::Player;
	collision.hit_object_type.push_back(eObjectType::Enemy);
	collision.box_size = Vector2D(60.0f, 60.0f);
	collision.attack_size = Vector2D(100.0f, 100.0f);
	z_layer = 2;

	// HP初期化
	HP = 10;
}

// 更新処理
void P_Ranged::Update(float delta_second)
{
	// 移動処理
	Movement(delta_second);
}

// 描画処理
void P_Ranged::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// 近接ユニットの描画
	DrawBox((int)(position.x - collision.box_size.x / 2), (int)(position.y - collision.box_size.y / 2),
		(int)(position.x + collision.box_size.x / 2), (int)(position.y + collision.box_size.y / 2), 0xffff00, TRUE);

#ifdef DEBUG
	// 中心を表示
	DrawCircle((int)position.x, (int)position.y, 2, 0x0000ff, TRUE);
	// 当たり判定表示
	DrawBox((int)(position.x - collision.box_size.x / 2), (int)(position.y - collision.box_size.y / 2),
		(int)(position.x + collision.box_size.x / 2), (int)(position.y + collision.box_size.y / 2), 0x0000ff, FALSE);
	// 攻撃範囲を表示
	DrawBox((int)(position.x - collision.attack_size.x / 2), (int)(position.y - collision.attack_size.y / 2),
		(int)(position.x + collision.attack_size.x / 2), (int)(position.y + collision.attack_size.y / 2), 0x0000ff, FALSE);
#endif
}

// 終了時処理
void P_Ranged::Finalize()
{

}

// 当たり判定通知処理
void P_Ranged::OnHitCollision(GameObject* hit_object)
{

}

// HP管理処理
void P_Ranged::HPControl(int Damage)
{

}

// 移動処理
void P_Ranged::Movement(float delta_second)
{
	// 右へ移動
	velocity.x = -1.0f;

	// 移動の実行
	location.x += velocity.x * 10 * delta_second;
}

// アニメーション制御処理
void P_Ranged::AnimationControl(float delta_second)
{

}
// エフェクト制御処理
void P_Ranged::EffectControl(float delta_second)
{

}

// 攻撃処理
void P_Ranged::Attack()
{

}