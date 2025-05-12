#include "P_Projectile.h"

// コンストラクタ
P_Projectile::P_Projectile() :
	Damage()
{

}

// デストラクタ
P_Projectile::~P_Projectile()
{

}

// 初期化処理
void P_Projectile::Initialize()
{

}

// 更新処理
void P_Projectile::Update(float delta_second)
{

}

// 描画処理
void P_Projectile::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// オフセット値を基に画像の描画を行う
	DrawRotaGraphF(camera_pos.x, camera_pos.y, 1.0, 0.0, image, TRUE);
}

// 終了時処理
void P_Projectile::Finalize()
{

}

// 当たり判定通知処理
void P_Projectile::OnHitCollision(GameObject* hit_object)
{

}

// 移動処理
void P_Projectile::Movement(float delta_second)
{

}

// アニメーション制御処理
void P_Projectile::AnimationControl(float delta_second)
{

}
// エフェクト制御処理
void P_Projectile::EffectControl(float delta_second)
{

}
