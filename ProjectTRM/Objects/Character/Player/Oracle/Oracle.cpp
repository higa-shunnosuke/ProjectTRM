#include "Oracle.h"

// �R���X�g���N�^
Oracle::Oracle() :
	summon_flag()
{

}

// �f�X�g���N�^
Oracle::~Oracle()
{

}

// ����������
void Oracle::Initialize()
{

}

// �X�V����
void Oracle::Update(float delta_second)
{

}

// �`�揈��
void Oracle::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// �I�t�Z�b�g�l����ɉ摜�̕`����s��
	DrawRotaGraphF(camera_pos.x, camera_pos.y, 1.0, 0.0, image, TRUE);
}

// �I��������
void Oracle::Finalize()
{

}

// �����蔻��ʒm����
void Oracle::OnHitCollision(GameObject* hit_object)
{

}

// HP�Ǘ�����
void Oracle::HPControl(int Damage)
{

}

// �ړ�����
void Oracle::Movement(float delta_second)
{

}

// �A�j���[�V�������䏈��
void Oracle::AnimationControl(float delta_second)
{

}
// �G�t�F�N�g���䏈��
void Oracle::EffectControl(float delta_second)
{

}

// �U������
void Oracle::Attack()
{

}