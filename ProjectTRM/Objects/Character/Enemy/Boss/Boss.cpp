#include "Boss.h"

// �R���X�g���N�^
Boss::Boss() :
	Damage()
{

}

// �f�X�g���N�^
Boss::~Boss()
{

}

// ����������
void Boss::Initialize()
{

}

// �X�V����
void Boss::Update(float delta_second)
{

}

// �`�揈��
void Boss::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// �I�t�Z�b�g�l����ɉ摜�̕`����s��
	DrawRotaGraphF(camera_pos.x, camera_pos.y, 1.0, 0.0, image, TRUE);
}

// �I��������
void Boss::Finalize()
{

}

// �����蔻��ʒm����
void Boss::OnHitCollision(GameObject* hit_object)
{

}

// HP�Ǘ�����
void Boss::HPControl(int Damage)
{

}

// �U������
void Boss::Attack(GameObject* hit_object)
{

}

// �ړ�����
void Boss::Movement(float delta_second)
{

}

// �A�j���[�V�������䏈��
void Boss::AnimationControl(float delta_second)
{

}
// �G�t�F�N�g���䏈��
void Boss::EffectControl(float delta_second)
{

}