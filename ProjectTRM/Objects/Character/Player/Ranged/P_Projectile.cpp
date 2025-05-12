#include "P_Projectile.h"

// �R���X�g���N�^
P_Projectile::P_Projectile() :
	Damage()
{

}

// �f�X�g���N�^
P_Projectile::~P_Projectile()
{

}

// ����������
void P_Projectile::Initialize()
{

}

// �X�V����
void P_Projectile::Update(float delta_second)
{

}

// �`�揈��
void P_Projectile::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// �I�t�Z�b�g�l����ɉ摜�̕`����s��
	DrawRotaGraphF(camera_pos.x, camera_pos.y, 1.0, 0.0, image, TRUE);
}

// �I��������
void P_Projectile::Finalize()
{

}

// �����蔻��ʒm����
void P_Projectile::OnHitCollision(GameObject* hit_object)
{

}

// �ړ�����
void P_Projectile::Movement(float delta_second)
{

}

// �A�j���[�V�������䏈��
void P_Projectile::AnimationControl(float delta_second)
{

}
// �G�t�F�N�g���䏈��
void P_Projectile::EffectControl(float delta_second)
{

}
