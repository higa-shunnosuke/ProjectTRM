#include "P_Melee.h"

P_Melee::P_Melee() :
	Damage()
{

}

P_Melee::~P_Melee()
{

}

/// <summary>
/// 初期化処理
/// </summary>
void P_Melee::Initialize()
{

}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="delta_second">1フレームあたりの時間</param>
void P_Melee::Update(float delta_second)
{

}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="screen_offset">オフセット値</param>
void P_Melee::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// オフセット値を基に画像の描画を行う
	DrawRotaGraphF(camera_pos.x, camera_pos.y, 1.0, 0.0, image, TRUE);
}

/// <summary>
/// 終了時処理
/// </summary>
void P_Melee::Finalize()
{

}

/// <summary>
/// 当たり判定通知処理
/// </summary>
/// <param name="hit_object">当たったゲームオブジェクトのポインタ</param>
void P_Melee::OnHitCollision(GameObject* hit_object)
{

}

/// <summary>
/// HP管理処理
/// </summary>
/// <param name="hit_object">ダメージ</param>
void P_Melee::HPControl(int Damage)
{

}

/// <summary>
/// 攻撃処理
/// </summary>
void P_Melee::Attack()
{

}

/// <summary>
/// 移動処理
/// </summary>
/// <param name="hit_object">1フレームあたりの時間</param>
void P_Melee::Movement(float delta_second)
{

}

/// <summary>
/// アニメーション制御処理
/// </summary>
/// <param name="hit_object">1フレームあたりの時間</param>
void P_Melee::AnimationControl(float delta_second)
{

}
/// <summary>
/// エフェクト制御処理
/// </summary>
/// <param name="hit_object">1フレームあたりの時間</param>
void P_Melee::EffectControl(float delta_second)
{

}
