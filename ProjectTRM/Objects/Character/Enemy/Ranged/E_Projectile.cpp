#include "E_Projectile.h"
#include "../../../GameObjectManager.h"
#include <cmath>

#define GRAVITY (9.8f)
#define INITIAL_SPEED (100.0f)

// コンストラクタ
E_Projectile::E_Projectile() :
    Damage(),
    angle(),
    target_location(),
    old_location(),
    end_loc()
{

}

// デストラクタ
E_Projectile::~E_Projectile()
{

}

// 初期化処理
void E_Projectile::Initialize()
{
    // 画像の読み込み
    ResourceManager* rm = ResourceManager::GetInstance();
    image = rm->GetImages("Resource/Images/Unit/Ranged/Arrow.png", 1, 1, 1, 32, 32)[0];
    // 音源の読み込み
    SE = rm->GetSounds("Resource/Sounds/UnitSE/Ranged/Arrow_Hit.mp3");

    is_mobility = true;
    is_aggressive = true;

    collision.is_blocking = true;
    collision.object_type = eObjectType::Projectile;
    collision.hit_object_type.push_back(eObjectType::Player);
    collision.hitbox_size = Vector2D(30.0f, 20.0f);

    z_layer = 3;

    Damage = 5.0f;

}

// 更新処理
void E_Projectile::Update(float delta_second)
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
        float angle_init = std::atan2(dy + 0.7 * GRAVITY * (distance / speed) * (distance / speed), dx);

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
    if (location.y >= end_loc + (collision.hitbox_size.y / 2))
    {
        Finalize();
    }
}

// 描画処理
void E_Projectile::Draw() const
{
    // オフセット値を基に画像の描画を行う
    DrawRotaGraphF(location.x, location.y, 2.0, angle, image, TRUE);

    if (ProjectConfig::DEBUG)
    {
        // 中心を表示
        DrawCircle((int)location.x, (int)location.y, 2, 0x0000ff, TRUE);
        // 当たり判定表示
        DrawBox((int)(location.x - collision.collision_size.x / 2), (int)(location.y - collision.collision_size.y / 2),
            (int)(location.x + collision.collision_size.x / 2), (int)(location.y + collision.collision_size.y / 2), 0x0000ff, FALSE);
        // 攻撃範囲を表示
        DrawBox((int)location.x, (int)(location.y - collision.hitbox_size.y / 2),
            (int)(location.x + collision.hitbox_size.x), (int)(location.y + collision.hitbox_size.y / 2), 0x0000ff, FALSE);
    }
}

// 終了時処理
void E_Projectile::Finalize()
{
    GameObjectManager* object = GameObjectManager::GetInstance();
    object->DestroyObject(this);
}

// 当たり判定通知処理
void E_Projectile::OnHitCollision(GameObject* hit_object)
{

}

/// 攻撃範囲通知処理
void E_Projectile::OnAreaDetection(GameObject* hit_object)
{
    Collision hit_collision = hit_object->GetCollision();

    if (hit_collision.object_type == eObjectType::Player)
    {
        PlaySoundMem(SE, DX_PLAYTYPE_BACK);
        hit_object->HPControl(Damage);
        Finalize();
    }
    else
    {
        Finalize();
    }
}

// 移動処理
void E_Projectile::Movement(float delta_second)
{
    location.x += velocity.x * delta_second;
    location.y -= velocity.y * delta_second;

    // 重力の影響を加える
    velocity.y -= GRAVITY * delta_second;
}

// アニメーション制御処理
void E_Projectile::AnimationControl(float delta_second)
{

}
// エフェクト制御処理
void E_Projectile::EffectControl(float delta_second)
{

}

// ターゲット設定処理
void E_Projectile::SetTargetLocation(Vector2D location)
{
    target_location = location;
}

// ダメージ設定処理
void E_Projectile::SetDamage(float Damage)
{
    this->Damage = Damage;
}