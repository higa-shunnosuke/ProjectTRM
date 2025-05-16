#include "P_Projectile.h"
#include "../../../GameObjectManager.h"
#include <cmath>

// コンストラクタ
P_Projectile::P_Projectile() :
	Damage(0),
    angle(0.0f),
    target_location(),
    old_location()
{

}

// デストラクタ
P_Projectile::~P_Projectile()
{

}

// 初期化処理
void P_Projectile::Initialize()
{
    ResourceManager* rm = ResourceManager::GetInstance();
    image = rm->GetImages("Resource/Images/Unit/Ranged/Arrow.png", 1, 1, 1, 32, 32)[0];

    is_mobility = true;
    is_aggressive = true;

    collision.is_blocking = true;
    collision.object_type = eObjectType::None;
    collision.hit_object_type.push_back(eObjectType::Enemy);
    collision.attack_size = Vector2D(60.0f, 60.0f);

    z_layer = 3;

    now_state = State::Idle;

    Damage = 6;

}

// 更新処理
void P_Projectile::Update(float delta_second)
{
    if (now_state == State::Idle)
    {
        // 初速度
        float speed = INITIAL_SPEED;

        // ターゲットまでの距離と角度を計算
        float dx = target_location.x - location.x;
        float dy = target_location.y - location.y;

        float distance = std::sqrt(dx * dx + dy * dy);

        // 簡易的に放物線になるような角度を計算（高さに余裕がある場合）
       float angle_init = std::atan2(dy + 0.5 * GRAVITY * (distance / speed) * (distance / speed), dx);

        // 初速度ベクトルに分解
        velocity.x = speed * std::cos(angle_init);
        velocity.y = speed * std::sin(angle_init);

        end_loc = location.y;

        now_state = State::Move;
    }

    if (now_state == State::Move)
    {
        Movement(delta_second);
    }

    angle = std::atan2((location.y - old_location.y), (location.x - old_location.x));

    old_location = location;
    if (location.y >= end_loc  + (collision.attack_size.y / 2))
    {
        Finalize();
    }
}

// 描画処理
void P_Projectile::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;


	// オフセット値を基に画像の描画を行う
	DrawRotaGraphF(position.x, position.y, 2.0, angle, image, TRUE);

#ifdef DEBUG

    // 中心を表示
    DrawCircle((int)position.x, (int)position.y, 2, 0x0000ff, TRUE);
    // 当たり判定表示
    DrawBox((int)(position.x - collision.box_size.x / 2), (int)(position.y - collision.box_size.y / 2),
        (int)(position.x + collision.box_size.x / 2), (int)(position.y + collision.box_size.y / 2), 0x0000ff, FALSE);
    // 攻撃範囲を表示
    DrawBox((int)(position.x - collision.attack_size.x / 2), (int)(position.y - collision.attack_size.y / 2),
        (int)(position.x + collision.attack_size.x / 2), (int)(position.y + collision.attack_size.y / 2), 0x0000ff, FALSE);
#endif
}

// 終了時処理
void P_Projectile::Finalize()
{
    GameObjectManager* object = GameObjectManager::GetInstance();
    object->DestroyObject(this);
}

// 当たり判定通知処理
void P_Projectile::OnHitCollision(GameObject* hit_object)
{

}

/// 攻撃範囲通知処理
void P_Projectile::OnAreaDetection(GameObject* hit_object)
{
    Collision hit_collision = hit_object->GetCollision();

    if (hit_collision.object_type == eObjectType::Enemy)
    {
        hit_object->HPControl(Damage);
        Finalize();
    }
    else if (hit_collision.object_type == eObjectType::Ground)
    {
        Finalize();
    }
}

// 移動処理
void P_Projectile::Movement(float delta_second)
{
    location.x += velocity.x * delta_second;
    location.y -= velocity.y * delta_second;

    // 重力の影響を加える
    velocity.y -= GRAVITY * delta_second;
}

// アニメーション制御処理
void P_Projectile::AnimationControl(float delta_second)
{

}
// エフェクト制御処理
void P_Projectile::EffectControl(float delta_second)
{

}

void P_Projectile::SetTargetLocation(Vector2D location) 
{
	target_location = location;
}