#include "Boss.h"

Boss::Boss() :
	Damage()
{

}

Boss::~Boss()
{

}

/// <summary>
/// ����������
/// </summary>
void Boss::Initialize()
{

}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="delta_second">1�t���[��������̎���</param>
void Boss::Update(float delta_second)
{

}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="screen_offset">�I�t�Z�b�g�l</param>
void Boss::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// �I�t�Z�b�g�l����ɉ摜�̕`����s��
	DrawRotaGraphF(camera_pos.x, camera_pos.y, 1.0, 0.0, image, TRUE);
}

/// <summary>
/// �I��������
/// </summary>
void Boss::Finalize()
{

}

/// <summary>
/// �����蔻��ʒm����
/// </summary>
/// <param name="hit_object">���������Q�[���I�u�W�F�N�g�̃|�C���^</param>
void Boss::OnHitCollision(GameObject* hit_object)
{

}

/// <summary>
/// HP�Ǘ�����
/// </summary>
/// <param name="hit_object">�_���[�W</param>
void Boss::HPControl(int Damage)
{

}

/// <summary>
/// �U������
/// </summary>
void Boss::Attack()
{

}

/// <summary>
/// �ړ�����
/// </summary>
/// <param name="hit_object">1�t���[��������̎���</param>
void Boss::Movement(float delta_second)
{

}

/// <summary>
/// �A�j���[�V�������䏈��
/// </summary>
/// <param name="hit_object">1�t���[��������̎���</param>
void Boss::AnimationControl(float delta_second)
{

}
/// <summary>
/// �G�t�F�N�g���䏈��
/// </summary>
/// <param name="hit_object">1�t���[��������̎���</param>
void Boss::EffectControl(float delta_second)
{

}