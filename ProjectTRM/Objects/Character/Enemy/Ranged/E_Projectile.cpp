#include "E_Projectile.h"
#include "../../../GameObjectManager.h"
#include <cmath>

// �R���X�g���N�^
E_Projectile::E_Projectile() :
    Damage(0),
    angle(0.0f),
    target_location(),
    old_location()
{

}

// �f�X�g���N�^
E_Projectile::~E_Projectile()
{

}

// ����������
void E_Projectile::Initialize()
{
    ResourceManager* rm = ResourceManager::GetInstance();
    image = rm->GetImages("Resource/Images/Unit/Ranged/Arrow.png", 1, 1, 1, 32, 32)[0];

    is_mobility = true;
    is_aggressive = true;

    collision.is_blocking = true;
    collision.object_type = eObjectType::None;
    collision.hit_object_type.push_back(eObjectType::Player);
    collision.hitbox_size = Vector2D(30.0f, 60.0f);

    z_layer = 3;

    now_state = State::Idle;

    lane = rand() % 3 + 1;

    Damage = 5;

}

// �X�V����
void E_Projectile::Update(float delta_second)
{
    if (now_state == State::Idle)
    {
        // �����x
        float speed = INITIAL_SPEED;

        // �^�[�Q�b�g�܂ł̋����Ɗp�x���v�Z
        float dx = target_location.x - location.x;
        float dy = target_location.y - location.y;

        float distance = std::sqrt(dx * dx + dy * dy);

        // �ȈՓI�ɕ������ɂȂ�悤�Ȋp�x���v�Z�i�����ɗ]�T������ꍇ�j
        float angle_init = std::atan2(dy + 0.5 * GRAVITY * (distance / speed) * (distance / speed), dx);

        // �����x�x�N�g���ɕ���
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

// �`�揈��
void E_Projectile::Draw(const Vector2D camera_pos) const
{
    Vector2D position = this->GetLocation();
    position.x -= camera_pos.x - D_WIN_MAX_X / 2;
    position.y -= lane * 3;


    // �I�t�Z�b�g�l����ɉ摜�̕`����s��
    DrawRotaGraphF(position.x, position.y, 2.0, angle, image, TRUE);

    if (ProjectConfig::DEBUG)
    {
        // ���S��\��
        DrawCircle((int)position.x, (int)position.y, 2, 0x0000ff, TRUE);
        // �����蔻��\��
        DrawBox((int)(position.x - collision.collision_size.x / 2), (int)(position.y - collision.collision_size.y / 2),
            (int)(position.x + collision.collision_size.x / 2), (int)(position.y + collision.collision_size.y / 2), 0x0000ff, FALSE);
        // �U���͈͂�\��
        DrawBox((int)position.x, (int)(position.y - collision.hitbox_size.y / 2),
            (int)(position.x + collision.hitbox_size.x), (int)(position.y + collision.hitbox_size.y / 2), 0x0000ff, FALSE);
    }
}

// �I��������
void E_Projectile::Finalize()
{
    GameObjectManager* object = GameObjectManager::GetInstance();
    object->DestroyObject(this);
}

// �����蔻��ʒm����
void E_Projectile::OnHitCollision(GameObject* hit_object)
{

}

/// �U���͈͒ʒm����
void E_Projectile::OnAreaDetection(GameObject* hit_object)
{
    Collision hit_collision = hit_object->GetCollision();

    if (hit_collision.object_type == eObjectType::Player)
    {
        hit_object->HPControl(Damage);
        Finalize();
    }
    else if (hit_collision.object_type == eObjectType::Ground)
    {
        Finalize();
    }
}

// �ړ�����
void E_Projectile::Movement(float delta_second)
{
    location.x += velocity.x * delta_second;
    location.y -= velocity.y * delta_second;

    // �d�͂̉e����������
    velocity.y -= GRAVITY * delta_second;
}

// �A�j���[�V�������䏈��
void E_Projectile::AnimationControl(float delta_second)
{

}
// �G�t�F�N�g���䏈��
void E_Projectile::EffectControl(float delta_second)
{

}

void E_Projectile::SetTargetLocation(Vector2D location)
{
    target_location = location;
}