#include "Torch.h"

// コンストラクタ
Torch::Torch()
{

}

// デストラクタ
Torch::~Torch()
{

}

// 初期化処理
void Torch::Initialize()
{

}

// 更新処理
void Torch::Update(float delta_second)
{

}

// 描画処理
void Torch::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// オフセット値を基に画像の描画を行う
	DrawRotaGraphF(camera_pos.x, camera_pos.y, 1.0, 0.0, image, TRUE);
}

// 終了時処理
void Torch::Finalize()
{

}

// 当たり判定通知処理
void Torch::OnHitCollision(GameObject* hit_object)
{

}

// アニメーション制御処理
void Torch::AnimationControl(float delta_second)
{

}
// エフェクト制御処理
void Torch::EffectControl(float delta_second)
{

}