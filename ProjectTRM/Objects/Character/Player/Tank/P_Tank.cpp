#include "P_Tank.h"

// �R���X�g���N�^
P_Tank::P_Tank() :
	Damage()
{

}

// �f�X�g���N�^
P_Tank::~P_Tank()
{

}

// ����������
void P_Tank::Initialize()
{

}

// �X�V����
void P_Tank::Update(float delta_second)
{

}

// �`�揈��
void P_Tank::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// �I�t�Z�b�g�l����ɉ摜�̕`����s��
	DrawRotaGraphF(camera_pos.x, camera_pos.y, 1.0, 0.0, image, TRUE);
}

// �I��������
void P_Tank::Finalize()
{

}

// �����蔻��ʒm����
void P_Tank::OnHitCollision(GameObject* hit_object)
{

}

// HP�Ǘ�����
void P_Tank::HPControl(int Damage)
{

}

// �U������
void P_Tank::Attack()
{

}

// �ړ�����
void P_Tank::Movement(float delta_second)
{

}

// �A�j���[�V�������䏈��
void P_Tank::AnimationControl(float delta_second)
{

}
// �G�t�F�N�g���䏈��
void P_Tank::EffectControl(float delta_second)
{

}