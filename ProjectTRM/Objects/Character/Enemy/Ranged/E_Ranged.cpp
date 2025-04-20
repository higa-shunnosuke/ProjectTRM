#include "E_Ranged.h"

E_Ranged::E_Ranged()
{

}

E_Ranged::~E_Ranged()
{

}

/// <summary>
/// 初期化処理
/// </summary>
void E_Ranged::Initialize()
{

}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="delta_second">1フレームあたりの時間</param>
void E_Ranged::Update(float delta_second)
{

}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="screen_offset">オフセット値</param>
void E_Ranged::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// オフセット値を基に画像の描画を行う
	DrawRotaGraphF(camera_pos.x, camera_pos.y, 1.0, 0.0, image, TRUE);
}

/// <summary>
/// 終了時処理
/// </summary>
void E_Ranged::Finalize()
{

}

/// <summary>
/// 当たり判定通知処理
/// </summary>
/// <param name="hit_object">当たったゲームオブジェクトのポインタ</param>
void E_Ranged::OnHitCollision(GameObject* hit_object)
{

}

/// <summary>
/// HP管理処理
/// </summary>
/// <param name="hit_object">ダメージ</param>
void E_Ranged::HPControl(int Damage)
{

}

/// <summary>
/// 移動処理
/// </summary>
/// <param name="hit_object">1フレームあたりの時間</param>
void E_Ranged::Movement(float delta_second)
{

}

/// <summary>
/// アニメーション制御処理
/// </summary>
/// <param name="hit_object">1フレームあたりの時間</param>
void E_Ranged::AnimationControl(float delta_second)
{

}
/// <summary>
/// エフェクト制御処理
/// </summary>
/// <param name="hit_object">1フレームあたりの時間</param>
void E_Ranged::EffectControl(float delta_second)
{

}

/// <summary>
/// 攻撃処理
/// </summary>
void E_Ranged::Attack()
{

}