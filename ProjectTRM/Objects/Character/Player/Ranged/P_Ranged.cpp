#include "P_Ranged.h"

// �R���X�g���N�^
P_Ranged::P_Ranged()
{

}

// �f�X�g���N�^
P_Ranged::~P_Ranged()
{

}

// ����������
void P_Ranged::Initialize()
{

}

// �X�V����
void P_Ranged::Update(float delta_second)
{

}

// �`�揈��
void P_Ranged::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// �I�t�Z�b�g�l����ɉ摜�̕`����s��
	DrawRotaGraphF(camera_pos.x, camera_pos.y, 1.0, 0.0, image, TRUE);
}

// �I��������
void P_Ranged::Finalize()
{

}

// �����蔻��ʒm����
void P_Ranged::OnHitCollision(GameObject* hit_object)
{

}

// HP�Ǘ�����
void P_Ranged::HPControl(int Damage)
{

}

// �ړ�����
void P_Ranged::Movement(float delta_second)
{

}

// �A�j���[�V�������䏈��
void P_Ranged::AnimationControl(float delta_second)
{

}
// �G�t�F�N�g���䏈��
void P_Ranged::EffectControl(float delta_second)
{

}

// �U������
void P_Ranged::Attack()
{

}