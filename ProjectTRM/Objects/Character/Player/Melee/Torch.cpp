#include "Torch.h"

// �R���X�g���N�^
Torch::Torch()
{

}

// �f�X�g���N�^
Torch::~Torch()
{

}

// ����������
void Torch::Initialize()
{

}

// �X�V����
void Torch::Update(float delta_second)
{

}

// �`�揈��
void Torch::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// �I�t�Z�b�g�l����ɉ摜�̕`����s��
	DrawRotaGraphF(camera_pos.x, camera_pos.y, 1.0, 0.0, image, TRUE);
}

// �I��������
void Torch::Finalize()
{

}

// �����蔻��ʒm����
void Torch::OnHitCollision(GameObject* hit_object)
{

}

// �A�j���[�V�������䏈��
void Torch::AnimationControl(float delta_second)
{

}
// �G�t�F�N�g���䏈��
void Torch::EffectControl(float delta_second)
{

}