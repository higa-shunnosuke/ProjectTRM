#include "E_Tank.h"

// �R���X�g���N�^
E_Tank::E_Tank() :
	Damage()
{

}

// �f�X�g���N�^
E_Tank::~E_Tank()
{

}

// ����������
void E_Tank::Initialize()
{

}

// �X�V����
void E_Tank::Update(float delta_second)
{

}

// �`�揈��
void E_Tank::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// �I�t�Z�b�g�l����ɉ摜�̕`����s��
	DrawRotaGraphF(camera_pos.x, camera_pos.y, 1.0, 0.0, image, TRUE);
}

// �I��������
void E_Tank::Finalize()
{

}

// �����蔻��ʒm����
void E_Tank::OnHitCollision(GameObject* hit_object)
{

}

// HP�Ǘ�����
void E_Tank::HPControl(int Damage)
{

}

// �U������
void E_Tank::Attack(GameObject* hit_object)
{

}

// �ړ�����
void E_Tank::Movement(float delta_second)
{

}

// �A�j���[�V�������䏈��
void E_Tank::AnimationControl(float delta_second)
{

}
// �G�t�F�N�g���䏈��
void E_Tank::EffectControl(float delta_second)
{

}