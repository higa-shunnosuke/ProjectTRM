#include "GameObject.h"
#include "../Scenes/SceneBase.h"
#include "DxLib.h"

GameObject::GameObject() :
	location(0.0f),
	image(NULL),
	z_layer(0),
	HP(0),
	is_mobility(false),
	is_aggressive(false)
{

}

GameObject::~GameObject()
{

}

// 初期化処理
void GameObject::Initialize()
{

}

// 更新処理
void GameObject::Update(float delta_second)
{

}

// 描画処理
void GameObject::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// オフセット値を基に画像の描画を行う
	DrawRotaGraphF(camera_pos.x, camera_pos.y, 1.0, 0.0, image, TRUE);
}

// 終了時処理
void GameObject::Finalize()
{

}

// 当たり判定通知処理
void GameObject::OnHitCollision(GameObject* hit_object)
{

}

// 攻撃範囲通知処理
void GameObject::OnAreaDetection(GameObject* hit_object)
{

}

// 攻撃範囲通知処理
void GameObject::NoHit()
{

}

// ライト範囲通知処理
void GameObject::InLightRange()
{

}

// ライト範囲通知処理
void GameObject::OutLightRange()
{

}

// 位置座標取得処理
const Vector2D& GameObject::GetLocation() const
{
	return location;
}

// 位置情報変更処理
void GameObject::SetLocation(const Vector2D& location)
{
	this->location = location;
}

// 当たり判定取得処理
const Collision& GameObject::GetCollision() const
{
	return collision;
}

// Zレイヤー情報取得処理
const unsigned char GameObject::GetZLayer() const
{
	return z_layer;
}

// 可動性情報の取得処理
const bool GameObject::GetMobility() const
{
	return is_mobility;
}

// 攻撃性情報の取得処理
const bool GameObject::GetAggressive() const
{
	return is_aggressive;
}

// 範囲攻撃フラグ取得処理
const bool GameObject::GetAoE() const
{
	return is_AoE;
}

// HP管理処理
void GameObject::HPControl(float Damage)
{
	this->HP -= Damage;
	if (this->HP < 0)
	{
		this->HP = 0;
	}
}

// HP取得処理
const int GameObject::GetHP() const
{
	return this->HP;
}

// ライト内フラグ取得処理
const bool GameObject::GetInLight() const
{
	return this->in_light;
}

/// <summary>
/// InGameの情報取得
/// </summary>
/// <param name="point">InGameSceneのPointer</param>
void GameObject::SetInGamePoint(class InGame* point)
{
	Ingame = point;
}