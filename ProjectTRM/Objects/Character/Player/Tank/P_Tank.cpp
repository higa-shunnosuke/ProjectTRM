#include "P_Tank.h"

// �R���X�g���N�^
P_Tank::P_Tank() :
	Damage()
{

}

// �f�X�g���N�^
P_Tank::~P_Tank()
{

}

// ����������
void P_Tank::Initialize()
{
	// �摜�̓ǂݍ���
	ResourceManager* rm = ResourceManager::GetInstance();

	is_mobility = true;

	collision.is_blocking = true;
	collision.object_type = eObjectType::Player;
	collision.hit_object_type.push_back(eObjectType::Enemy);
	collision.box_size = Vector2D(60.0f, 60.0f);
	collision.attack_size = Vector2D(100.0f, 100.0f);
	z_layer = 2;

	// HP������
	HP = 50;
}

// �X�V����
void P_Tank::Update(float delta_second)
{
	// �ړ�����
	Movement(delta_second);
}

// �`�揈��
void P_Tank::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// �ߐڃ��j�b�g�̕`��
	DrawBox((int)(position.x - collision.box_size.x / 2), (int)(position.y - collision.box_size.y / 2),
		(int)(position.x + collision.box_size.x / 2), (int)(position.y + collision.box_size.y / 2), 0xff00ff, TRUE);

#ifdef DEBUG
	// ���S��\��
	DrawCircle((int)position.x, (int)position.y, 2, 0x0000ff, TRUE);
	// �����蔻��\��
	DrawBox((int)(position.x - collision.box_size.x / 2), (int)(position.y - collision.box_size.y / 2),
		(int)(position.x + collision.box_size.x / 2), (int)(position.y + collision.box_size.y / 2), 0x0000ff, FALSE);
	// �U���͈͂�\��
	DrawBox((int)(position.x - collision.attack_size.x / 2), (int)(position.y - collision.attack_size.y / 2),
		(int)(position.x + collision.attack_size.x / 2), (int)(position.y + collision.attack_size.y / 2), 0x0000ff, FALSE);
#endif
}

// �I��������
void P_Tank::Finalize()
{

}

// �����蔻��ʒm����
void P_Tank::OnHitCollision(GameObject* hit_object)
{

}

// HP�Ǘ�����
void P_Tank::HPControl(int Damage)
{

}

// �U������
void P_Tank::Attack(GameObject* hit_object)
{

}

// �ړ�����
void P_Tank::Movement(float delta_second)
{
	// �E�ֈړ�
	velocity.x = -1.0f;

	// �ړ��̎��s
	location.x += velocity.x * 10 * delta_second;
}

// �A�j���[�V�������䏈��
void P_Tank::AnimationControl(float delta_second)
{

}
// �G�t�F�N�g���䏈��
void P_Tank::EffectControl(float delta_second)
{

}