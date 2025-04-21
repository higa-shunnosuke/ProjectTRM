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

	// オフセット値を基に画像の描画を行う
	DrawRotaGraphF(camera_pos.x, camera_pos.y, 1.0, 0.0, image, TRUE);
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