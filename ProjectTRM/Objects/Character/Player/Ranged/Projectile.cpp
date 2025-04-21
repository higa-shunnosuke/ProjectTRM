#include "Projectile.h"

// �R���X�g���N�^
Projectile::Projectile() :
	Damage()
{

}

// �f�X�g���N�^
Projectile::~Projectile()
{

}

// ����������
void Projectile::Initialize()
{

}

// �X�V����
void Projectile::Update(float delta_second)
{

}

// �`�揈��
void Projectile::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// �I�t�Z�b�g�l����ɉ摜�̕`����s��
	DrawRotaGraphF(camera_pos.x, camera_pos.y, 1.0, 0.0, image, TRUE);
}

// �I��������
void Projectile::Finalize()
{

}

// �����蔻��ʒm����
void Projectile::OnHitCollision(GameObject* hit_object)
{

}

// �ړ�����
void Projectile::Movement(float delta_second)
{

}

// �A�j���[�V�������䏈��
void Projectile::AnimationControl(float delta_second)
{

}
// �G�t�F�N�g���䏈��
void Projectile::EffectControl(float delta_second)
{

}
