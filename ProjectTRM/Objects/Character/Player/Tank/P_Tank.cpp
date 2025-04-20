#include "P_Tank.h"

P_Tank::P_Tank() :
	Damage()
{

}

P_Tank::~P_Tank()
{

}

/// <summary>
/// ����������
/// </summary>
void P_Tank::Initialize()
{

}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="delta_second">1�t���[��������̎���</param>
void P_Tank::Update(float delta_second)
{

}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="screen_offset">�I�t�Z�b�g�l</param>
void P_Tank::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// �I�t�Z�b�g�l����ɉ摜�̕`����s��
	DrawRotaGraphF(camera_pos.x, camera_pos.y, 1.0, 0.0, image, TRUE);
}

/// <summary>
/// �I��������
/// </summary>
void P_Tank::Finalize()
{

}

/// <summary>
/// �����蔻��ʒm����
/// </summary>
/// <param name="hit_object">���������Q�[���I�u�W�F�N�g�̃|�C���^</param>
void P_Tank::OnHitCollision(GameObject* hit_object)
{

}

/// <summary>
/// HP�Ǘ�����
/// </summary>
/// <param name="hit_object">�_���[�W</param>
void P_Tank::HPControl(int Damage)
{

}

/// <summary>
/// �U������
/// </summary>
void P_Tank::Attack()
{

}

/// <summary>
/// �ړ�����
/// </summary>
/// <param name="hit_object">1�t���[��������̎���</param>
void P_Tank::Movement(float delta_second)
{

}

/// <summary>
/// �A�j���[�V�������䏈��
/// </summary>
/// <param name="hit_object">1�t���[��������̎���</param>
void P_Tank::AnimationControl(float delta_second)
{

}
/// <summary>
/// �G�t�F�N�g���䏈��
/// </summary>
/// <param name="hit_object">1�t���[��������̎���</param>
void P_Tank::EffectControl(float delta_second)
{

}