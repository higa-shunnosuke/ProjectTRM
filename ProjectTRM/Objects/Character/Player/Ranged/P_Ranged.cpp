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

}

// 更新処理
void P_Ranged::Update(float delta_second)
{

}

// 描画処理
void P_Ranged::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// オフセット値を基に画像の描画を行う
	DrawRotaGraphF(camera_pos.x, camera_pos.y, 1.0, 0.0, image, TRUE);
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