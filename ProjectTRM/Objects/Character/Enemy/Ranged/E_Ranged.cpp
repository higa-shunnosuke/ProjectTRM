#include "E_Ranged.h"

// �R���X�g���N�^
E_Ranged::E_Ranged()
{

}

// �f�X�g���N�^
E_Ranged::~E_Ranged()
{

}

// ����������
void E_Ranged::Initialize()
{

}

// �X�V����
void E_Ranged::Update(float delta_second)
{

}

// �`�揈��
void E_Ranged::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// �I�t�Z�b�g�l����ɉ摜�̕`����s��
	DrawRotaGraphF(camera_pos.x, camera_pos.y, 1.0, 0.0, image, TRUE);
}

// �I��������
void E_Ranged::Finalize()
{

}

// �����蔻��ʒm����
void E_Ranged::OnHitCollision(GameObject* hit_object)
{

}

// HP�Ǘ�����
void E_Ranged::HPControl(int Damage)
{

}

// �ړ�����
void E_Ranged::Movement(float delta_second)
{

}

// �A�j���[�V�������䏈��
void E_Ranged::AnimationControl(float delta_second)
{

}
// �G�t�F�N�g���䏈��
void E_Ranged::EffectControl(float delta_second)
{

}

// �U������
void E_Ranged::Attack()
{

}