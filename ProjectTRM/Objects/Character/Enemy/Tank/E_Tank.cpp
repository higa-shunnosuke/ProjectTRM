#include "E_Tank.h"

// コンストラクタ
E_Tank::E_Tank() :
	Damage()
{

}

// デストラクタ
E_Tank::~E_Tank()
{

}

// 初期化処理
void E_Tank::Initialize()
{

}

// 更新処理
void E_Tank::Update(float delta_second)
{

}

// 描画処理
void E_Tank::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// オフセット値を基に画像の描画を行う
	DrawRotaGraphF(camera_pos.x, camera_pos.y, 1.0, 0.0, image, TRUE);
}

// 終了時処理
void E_Tank::Finalize()
{

}

// 当たり判定通知処理
void E_Tank::OnHitCollision(GameObject* hit_object)
{

}

// HP管理処理
void E_Tank::HPControl(int Damage)
{

}

// 攻撃処理
void E_Tank::Attack(GameObject* hit_object)
{

}

// 移動処理
void E_Tank::Movement(float delta_second)
{

}

// アニメーション制御処理
void E_Tank::AnimationControl(float delta_second)
{

}
// エフェクト制御処理
void E_Tank::EffectControl(float delta_second)
{

}