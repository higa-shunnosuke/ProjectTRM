#include "P_Melee.h"

// �R���X�g���N�^
P_Melee::P_Melee() :
	Damage()
{

}

// �f�X�g���N�^
P_Melee::~P_Melee()
{

}

// ����������
void P_Melee::Initialize()
{

}

// �X�V����
void P_Melee::Update(float delta_second)
{

}

// �`�揈��
void P_Melee::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// �I�t�Z�b�g�l����ɉ摜�̕`����s��
	DrawRotaGraphF(camera_pos.x, camera_pos.y, 1.0, 0.0, image, TRUE);
}

// �I��������
void P_Melee::Finalize()
{

}

// �����蔻��ʒm����
void P_Melee::OnHitCollision(GameObject* hit_object)
{

}

// HP�Ǘ�����
void P_Melee::HPControl(int Damage)
{

}

// �U������
void P_Melee::Attack()
{

}

// �ړ�����
void P_Melee::Movement(float delta_second)
{

}

// �A�j���[�V�������䏈��
void P_Melee::AnimationControl(float delta_second)
{

}
// �G�t�F�N�g���䏈��
void P_Melee::EffectControl(float delta_second)
{

}
