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
void Character::Draw() const
{
	// �I�t�Z�b�g�l����ɉ摜�̕`����s��
	DrawRotaGraphF(location.x, location.y, 1.0, 0.0, image, TRUE);
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

// SE���䏈��
void Character::SoundControl()
{

}

// �U������
void Character::Attack(GameObject* hit_object)
{

}