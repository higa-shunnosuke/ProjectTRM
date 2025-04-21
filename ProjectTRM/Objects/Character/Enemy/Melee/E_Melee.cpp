#include "E_Melee.h"

// コンストラクタ
E_Melee::E_Melee() :
	Damage()
{

}

// デストラクタ
E_Melee::~E_Melee()
{

}

// 初期化処理
void E_Melee::Initialize()
{

}

// 更新処理
void E_Melee::Update(float delta_second)
{

}

// 描画処理
void E_Melee::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// オフセット値を基に画像の描画を行う
	DrawRotaGraphF(camera_pos.x, camera_pos.y, 1.0, 0.0, image, TRUE);
}

// 終了時処理
void E_Melee::Finalize()
{

}

// 当たり判定通知処理
void E_Melee::OnHitCollision(GameObject* hit_object)
{

}

// HP管理処理
void E_Melee::HPControl(int Damage)
{

}

// 攻撃処理
void E_Melee::Attack()
{

}

// 移動処理
void E_Melee::Movement(float delta_second)
{

}

// アニメーション制御処理
void E_Melee::AnimationControl(float delta_second)
{

}
// エフェクト制御処理
void E_Melee::EffectControl(float delta_second)
{

}
