#include "E_Melee.h"

// �R���X�g���N�^
E_Melee::E_Melee() :
	Damage()
{

}

// �f�X�g���N�^
E_Melee::~E_Melee()
{

}

// ����������
void E_Melee::Initialize()
{

}

// �X�V����
void E_Melee::Update(float delta_second)
{

}

// �`�揈��
void E_Melee::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// �I�t�Z�b�g�l����ɉ摜�̕`����s��
	DrawRotaGraphF(camera_pos.x, camera_pos.y, 1.0, 0.0, image, TRUE);
}

// �I��������
void E_Melee::Finalize()
{

}

// �����蔻��ʒm����
void E_Melee::OnHitCollision(GameObject* hit_object)
{

}

// HP�Ǘ�����
void E_Melee::HPControl(int Damage)
{

}

// �U������
void E_Melee::Attack()
{

}

// �ړ�����
void E_Melee::Movement(float delta_second)
{

}

// �A�j���[�V�������䏈��
void E_Melee::AnimationControl(float delta_second)
{

}
// �G�t�F�N�g���䏈��
void E_Melee::EffectControl(float delta_second)
{

}
