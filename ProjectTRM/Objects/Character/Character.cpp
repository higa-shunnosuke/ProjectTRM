#include "Character.h"

// �R���X�g���N�^
Character::Character():
	now_state(),
	old_state()
{

}

// �f�X�g���N�^
Character::~Character()
{

}

// ����������
void Character::Initialize()
{

}

// �X�V����
void Character::Update(float delta_second)
{

}

// �`�揈��
void Character::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// �I�t�Z�b�g�l����ɉ摜�̕`����s��
	DrawRotaGraphF(camera_pos.x, camera_pos.y, 1.0, 0.0, image, TRUE);
}

// �I��������
void Character::Finalize()
{

}

// �ړ�����
void Character::Movement(float delta_second)
{

}

// �A�j���[�V�������䏈��
void Character::AnimationControl(float delta_second)
{

}
// �G�t�F�N�g���䏈��
void Character::EffectControl(float delta_second)
{

}

// �U������
void Character::Attack(GameObject* hit_object)
{

}