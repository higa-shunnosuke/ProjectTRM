#include "E_Melee.h"

// �R���X�g���N�^
E_Melee::E_Melee() :
	Damage()
{

}

// �f�X�g���N�^
E_Melee::~E_Melee()
{

}

// ����������
void E_Melee::Initialize()
{
	// �摜�̓ǂݍ���
	ResourceManager* rm = ResourceManager::GetInstance();

	is_mobility = true;
	is_aggressive = true;

	collision.is_blocking = true;
	collision.object_type = eObjectType::Enemy;
	collision.hit_object_type.push_back(eObjectType::Player);
	collision.box_size = Vector2D(60.0f, 60.0f);
	collision.attack_size = Vector2D(100.0f, 100.0f);
	z_layer = 2;

	attack_flag = false;

	// �E�ֈړ�
	velocity.x = 1.0f;

	//�U����
	Damage = 2;

	// HP������
	HP = 20;
}

// �X�V����
void E_Melee::Update(float delta_second)
{
	// �ړ�����
	Movement(delta_second);

	if (attack_flag == true)
	{
		attack_flame += delta_second;
	}
}

// �`�揈��
void E_Melee::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// �ߐڃ��j�b�g�̕`��
	DrawBox((int)(position.x - collision.box_size.x / 2), (int)(position.y - collision.box_size.y / 2),
		(int)(position.x + collision.box_size.x / 2), (int)(position.y + collision.box_size.y / 2), 0xff00a0, TRUE);

#ifdef DEBUG
	//�c��HP�̕\��
	DrawFormatString(position.x, position.y - 40.0f, 0xffffff, "%d", HP);

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
void E_Melee::Finalize()
{

}

// �����蔻��ʒm����
void E_Melee::OnHitCollision(GameObject* hit_object)
{

}

// �U���͈͒ʒm����
void E_Melee::OnAreaDetection(GameObject* hit_object)
{
	Collision hit_col = hit_object->GetCollision();
	velocity.x = 1.0f;
	if (hit_col.object_type == eObjectType::Player)
	{
		attack_flag = true;
		velocity.x = 0.0f;
		if (attack_flame >= 2.0f)
		{
			Attack(hit_object);
		}
	}
}

// HP�Ǘ�����
void E_Melee::HPControl(int Damage)
{
	HP -= Damage;
}

// �U������
void E_Melee::Attack(GameObject* hit_object)
{
	hit_object->HPControl(Damage);
	attack_flag = false;
	attack_flame = 0.0f;
}

// �ړ�����
void E_Melee::Movement(float delta_second)
{

	// �ړ��̎��s
	location.x += velocity.x * 10 * delta_second;
}

// �A�j���[�V�������䏈��
void E_Melee::AnimationControl(float delta_second)
{

}
// �G�t�F�N�g���䏈��
void E_Melee::EffectControl(float delta_second)
{

}
