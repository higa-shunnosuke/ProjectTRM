#include "E_Ranged.h"

E_Ranged::E_Ranged()
{

}

E_Ranged::~E_Ranged()
{

}

/// <summary>
/// ����������
/// </summary>
void E_Ranged::Initialize()
{

}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="delta_second">1�t���[��������̎���</param>
void E_Ranged::Update(float delta_second)
{

}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="screen_offset">�I�t�Z�b�g�l</param>
void E_Ranged::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// �I�t�Z�b�g�l����ɉ摜�̕`����s��
	DrawRotaGraphF(camera_pos.x, camera_pos.y, 1.0, 0.0, image, TRUE);
}

/// <summary>
/// �I��������
/// </summary>
void E_Ranged::Finalize()
{

}

/// <summary>
/// �����蔻��ʒm����
/// </summary>
/// <param name="hit_object">���������Q�[���I�u�W�F�N�g�̃|�C���^</param>
void E_Ranged::OnHitCollision(GameObject* hit_object)
{

}

/// <summary>
/// HP�Ǘ�����
/// </summary>
/// <param name="hit_object">�_���[�W</param>
void E_Ranged::HPControl(int Damage)
{

}

/// <summary>
/// �ړ�����
/// </summary>
/// <param name="hit_object">1�t���[��������̎���</param>
void E_Ranged::Movement(float delta_second)
{

}

/// <summary>
/// �A�j���[�V�������䏈��
/// </summary>
/// <param name="hit_object">1�t���[��������̎���</param>
void E_Ranged::AnimationControl(float delta_second)
{

}
/// <summary>
/// �G�t�F�N�g���䏈��
/// </summary>
/// <param name="hit_object">1�t���[��������̎���</param>
void E_Ranged::EffectControl(float delta_second)
{

}

/// <summary>
/// �U������
/// </summary>
void E_Ranged::Attack()
{

}