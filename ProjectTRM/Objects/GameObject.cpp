#include "GameObject.h"
#include "../Scenes/SceneBase.h"
#include "DxLib.h"

GameObject::GameObject() :
	location(0.0f),
	image(NULL),
	z_layer(0),
	is_mobility(false)
{

}

GameObject::~GameObject()
{

}

/// <summary>
/// 初期化処理
/// </summary>
void GameObject::Initialize()
{

}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="delta_second">1フレームあたりの時間</param>
void GameObject::Update(float delta_second)
{

}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="screen_offset">オフセット値</param>
void GameObject::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// オフセット値を基に画像の描画を行う
	DrawRotaGraphF(camera_pos.x, camera_pos.y, 1.0, 0.0, image, TRUE);
}

/// <summary>
/// 終了時処理
/// </summary>
void GameObject::Finalize()
{

}

/// <summary>
/// 当たり判定通知処理
/// </summary>
/// <param name="hit_object">当たったゲームオブジェクトのポインタ</param>
void GameObject::OnHitCollision(GameObject* hit_object)
{

}

/// <summary>
/// 位置座標取得処理
/// </summary>
/// <returns>位置座標情報</returns>
const Vector2D& GameObject::GetLocation() const
{
	return location;
}

/// <summary>
/// 位置情報変更処理
/// </summary>
/// <param name="location">変更したい位置情報</param>
void GameObject::SetLocation(const Vector2D& location)
{
	this->location = location;
}

/// <summary>
/// 当たり判定取得処理
/// </summary>
/// <returns>当たり判定情報</returns>
const Collision& GameObject::GetCollision() const
{
	return collision;
}

/// <summary>
/// Zレイヤー情報取得処理
/// </summary>
/// <returns>Zレイヤー情報</returns>
const unsigned char GameObject::GetZLayer() const
{
	return z_layer;
}

/// <summary>
/// 可動性情報の取得処理
/// </summary>
/// <returns>可動性情報</returns>
const bool GameObject::GetMobility() const
{
	return is_mobility;
}
