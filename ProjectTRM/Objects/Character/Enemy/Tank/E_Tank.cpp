#include "E_Tank.h"

E_Tank::E_Tank() :
	Damage()
{

}

E_Tank::~E_Tank()
{

}

/// <summary>
/// 初期化処理
/// </summary>
void E_Tank::Initialize()
{

}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="delta_second">1フレームあたりの時間</param>
void E_Tank::Update(float delta_second)
{

}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="screen_offset">オフセット値</param>
void E_Tank::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// オフセット値を基に画像の描画を行う
	DrawRotaGraphF(camera_pos.x, camera_pos.y, 1.0, 0.0, image, TRUE);
}

/// <summary>
/// 終了時処理
/// </summary>
void E_Tank::Finalize()
{

}

/// <summary>
/// 当たり判定通知処理
/// </summary>
/// <param name="hit_object">当たったゲームオブジェクトのポインタ</param>
void E_Tank::OnHitCollision(GameObject* hit_object)
{

}

/// <summary>
/// HP管理処理
/// </summary>
/// <param name="hit_object">ダメージ</param>
void E_Tank::HPControl(int Damage)
{

}

/// <summary>
/// 攻撃処理
/// </summary>
void E_Tank::Attack()
{

}

/// <summary>
/// 移動処理
/// </summary>
/// <param name="hit_object">1フレームあたりの時間</param>
void E_Tank::Movement(float delta_second)
{

}

/// <summary>
/// アニメーション制御処理
/// </summary>
/// <param name="hit_object">1フレームあたりの時間</param>
void E_Tank::AnimationControl(float delta_second)
{

}
/// <summary>
/// エフェクト制御処理
/// </summary>
/// <param name="hit_object">1フレームあたりの時間</param>
void E_Tank::EffectControl(float delta_second)
{

}