#include "E_Melee.h"

E_Melee::E_Melee() :
	Damage()
{

}

E_Melee::~E_Melee()
{

}

/// <summary>
/// ����������
/// </summary>
void E_Melee::Initialize()
{

}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="delta_second">1�t���[��������̎���</param>
void E_Melee::Update(float delta_second)
{

}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="screen_offset">�I�t�Z�b�g�l</param>
void E_Melee::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// �I�t�Z�b�g�l����ɉ摜�̕`����s��
	DrawRotaGraphF(camera_pos.x, camera_pos.y, 1.0, 0.0, image, TRUE);
}

/// <summary>
/// �I��������
/// </summary>
void E_Melee::Finalize()
{

}

/// <summary>
/// �����蔻��ʒm����
/// </summary>
/// <param name="hit_object">���������Q�[���I�u�W�F�N�g�̃|�C���^</param>
void E_Melee::OnHitCollision(GameObject* hit_object)
{

}

/// <summary>
/// HP�Ǘ�����
/// </summary>
/// <param name="hit_object">�_���[�W</param>
void E_Melee::HPControl(int Damage)
{

}

/// <summary>
/// �U������
/// </summary>
void E_Melee::Attack()
{

}

/// <summary>
/// �ړ�����
/// </summary>
/// <param name="hit_object">1�t���[��������̎���</param>
void E_Melee::Movement(float delta_second)
{

}

/// <summary>
/// �A�j���[�V�������䏈��
/// </summary>
/// <param name="hit_object">1�t���[��������̎���</param>
void E_Melee::AnimationControl(float delta_second)
{

}
/// <summary>
/// �G�t�F�N�g���䏈��
/// </summary>
/// <param name="hit_object">1�t���[��������̎���</param>
void E_Melee::EffectControl(float delta_second)
{

}
