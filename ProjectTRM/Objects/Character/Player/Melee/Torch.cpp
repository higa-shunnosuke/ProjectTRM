#include "Torch.h"

Torch::Torch()
{

}

Torch::~Torch()
{

}

/// <summary>
/// ����������
/// </summary>
void Torch::Initialize()
{

}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="delta_second">1�t���[��������̎���</param>
void Torch::Update(float delta_second)
{

}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="screen_offset">�I�t�Z�b�g�l</param>
void Torch::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// �I�t�Z�b�g�l����ɉ摜�̕`����s��
	DrawRotaGraphF(camera_pos.x, camera_pos.y, 1.0, 0.0, image, TRUE);
}

/// <summary>
/// �I��������
/// </summary>
void Torch::Finalize()
{

}

/// <summary>
/// �����蔻��ʒm����
/// </summary>
/// <param name="hit_object">���������Q�[���I�u�W�F�N�g�̃|�C���^</param>
void Torch::OnHitCollision(GameObject* hit_object)
{

}

/// <summary>
/// �A�j���[�V�������䏈��
/// </summary>
/// <param name="hit_object">1�t���[��������̎���</param>
void Torch::AnimationControl(float delta_second)
{

}
/// <summary>
/// �G�t�F�N�g���䏈��
/// </summary>
/// <param name="hit_object">1�t���[��������̎���</param>
void Torch::EffectControl(float delta_second)
{

}