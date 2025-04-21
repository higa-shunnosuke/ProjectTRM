#include "Oracle.h"

// �R���X�g���N�^
Oracle::Oracle() :
	summon_flag()
{

}

// �f�X�g���N�^
Oracle::~Oracle()
{

}

// ����������
void Oracle::Initialize()
{
	// �摜�̓ǂݍ���
	ResourceManager* rm = ResourceManager::GetInstance();

	is_mobility = false;

	collision.is_blocking = true;
	collision.object_type = eObjectType::Player;
	collision.hit_object_type.push_back(eObjectType::Enemy);
	collision.box_size = Vector2D(60.0f, 120.0f);
}

// �X�V����
void Oracle::Update(float delta_second)
{

}

// �`�揈��
void Oracle::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// �I�t�Z�b�g�l����ɉ摜�̕`����s��
	// �ޏ��̕\��
	DrawBox((int)(position.x - collision.box_size.x / 2), (int)(position.y - collision.box_size.y / 2),
		(int)(position.x + collision.box_size.x / 2), (int)(position.y + collision.box_size.y / 2), 0xffff00, TRUE);

#ifdef DEBUG
	// �����蔻��\��
	DrawBox((int)(position.x - collision.box_size.x / 2), (int)(position.y - collision.box_size.y / 2),
		(int)(position.x + collision.box_size.x / 2), (int)(position.y + collision.box_size.y / 2), 0xff0000, FALSE);
	// ���S��`��
	DrawCircle(position.x, position.y, 2, 0xffffff, TRUE);
#endif
}

// �I��������
void Oracle::Finalize()
{

}

// �����蔻��ʒm����
void Oracle::OnHitCollision(GameObject* hit_object)
{

}

// HP�Ǘ�����
void Oracle::HPControl(int Damage)
{

}

// �ړ�����
void Oracle::Movement(float delta_second)
{

}

// �A�j���[�V�������䏈��
void Oracle::AnimationControl(float delta_second)
{

}
// �G�t�F�N�g���䏈��
void Oracle::EffectControl(float delta_second)
{

}

// �U������
void Oracle::Attack()
{

}