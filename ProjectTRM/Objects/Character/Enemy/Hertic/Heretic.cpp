#include "Heretic.h"

// �R���X�g���N�^
Heretic::Heretic() :
	summon_flag()
{

}

// �f�X�g���N�^
Heretic::~Heretic()
{

}

// ����������
void Heretic::Initialize()
{

}

// �X�V����
void Heretic::Update(float delta_second)
{

}

// �`�揈��
void Heretic::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// �I�t�Z�b�g�l����ɉ摜�̕`����s��
	DrawRotaGraphF(camera_pos.x, camera_pos.y, 1.0, 0.0, image, TRUE);
}

// �I��������
void Heretic::Finalize()
{

}

// �����蔻��ʒm����
void Heretic::OnHitCollision(GameObject* hit_object)
{

}

// HP�Ǘ�����
void Heretic::HPControl(int Damage)
{

}

// �ړ�����
void Heretic::Movement(float delta_second)
{

}

// �A�j���[�V�������䏈��
void Heretic::AnimationControl(float delta_second)
{

}
// �G�t�F�N�g���䏈��
void Heretic::EffectControl(float delta_second)
{

}

// �U������
void Heretic::Attack()
{

}