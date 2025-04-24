#include "P_Melee.h"

// �R���X�g���N�^
P_Melee::P_Melee() :
	Damage()
{

}

// �f�X�g���N�^
P_Melee::~P_Melee()
{

}

// ����������
void P_Melee::Initialize()
{
	// �摜�̓ǂݍ���
	ResourceManager* rm = ResourceManager::GetInstance();
	animation = rm->GetImages("Resource/Images/Unit/Melee/Melee_Walk.png", 4, 4, 1, 32, 32);

	is_mobility = true;
	is_aggressive = true;

	collision.is_blocking = true;
	collision.object_type = eObjectType::Player;
	collision.hit_object_type.push_back(eObjectType::Enemy);
	collision.box_size = Vector2D(60.0f, 60.0f);
	collision.attack_size = Vector2D(100.0f, 100.0f);
	z_layer = 2;

	attack_flag = false;

	now_state = State::Move;

	// �E�ֈړ�
	velocity.x = -5.0f;

	//�U����
	Damage = 4;

	// HP������
	HP = 20;
}

// �X�V����
void P_Melee::Update(float delta_second)
{

	// �ړ�����
	Movement(delta_second);


	if (attack_flag == true)
	{
		if (velocity.x < 0.0f)
		{
			now_state = State::Move;
		}
		else
		{
			now_state = State::Idle;
		}
		attack_flame -= delta_second;
		if (attack_flame <= 0.0f)
		{
			attack_flag = false;
		}
	}
	
	AnimationControl(delta_second);

	old_state = now_state;

}

// �`�揈��
void P_Melee::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// �ߐڃ��j�b�g�̕`��
	// �I�t�Z�b�g�l����ɉ摜�̕`����s��
	DrawRotaGraphF(position.x, position.y, 2.0, 0.0, image, TRUE, TRUE);
	/*DrawBox((int)(position.x - collision.box_size.x / 2), (int)(position.y - collision.box_size.y / 2),
		(int)(position.x + collision.box_size.x / 2), (int)(position.y + collision.box_size.y / 2), 0xffa000, TRUE);*/

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
void P_Melee::Finalize()
{

}

// �����蔻��ʒm����
void P_Melee::OnHitCollision(GameObject* hit_object)
{

}

// �U���͈͒ʒm����
void P_Melee::OnAreaDetection(GameObject* hit_object)
{
	Collision hit_col = hit_object->GetCollision();
	velocity.x = -5.0f;
	if (hit_col.object_type == eObjectType::Enemy)
	{
		velocity.x = 0.0f;
		if (attack_flag == false)
		{
			now_state = State::Attack;
		}
		else 
		{
			if (attack_flame <= 0.0f)
			{
				Attack(hit_object);
			}
		}
	}
}

// HP�Ǘ�����
void P_Melee::HPControl(int Damage)
{
	HP -= Damage;
	DrawFormatString(location.x, location.y - 40, 0xffffff, "%d", HP);
}

// �U������
void P_Melee::Attack(GameObject* hit_object)
{
	hit_object->HPControl(Damage);
	attack_flame = 2.0f;
}

// �ړ�����
void P_Melee::Movement(float delta_second)
{
		// �ړ��̎��s
		location.x += velocity.x * 10 * delta_second;
}

// �A�j���[�V�������䏈��
void P_Melee::AnimationControl(float delta_second)
{
	// �摜�̓ǂݍ���
	ResourceManager* rm = ResourceManager::GetInstance();

	if (old_state != now_state)
	{
		Anim_flame = 0;
		Anim_count = 0;
		count_flag = false;
	}

	Anim_flame += delta_second;

	if (Anim_flame >= 0.5f)
	{
		if (count_flag == false)
		{
			Anim_count++;
		}
		else
		{
			Anim_count--;
		}
		if (Anim_count >= 2)
		{
			count_flag = true;
		}
		else if (Anim_count <= 0)
		{
			count_flag = false;
		}
		Anim_flame = 0.0f;
	}
	switch (now_state)
	{
	case State::Idle:
		if (Anim_count > 0)
		{
			image = animation[0];
		}
		break;
	case State::Move:
		animation = rm->GetImages("Resource/Images/Unit/Melee/Melee_Walk.png", 4, 4, 1, 32, 32);
		image = animation[1 + Anim_count];
		break;
	case State::Attack:
		animation = rm->GetImages("Resource/Images/Unit/Melee/Melee_Attack.png", 4, 4, 1, 32, 32);
		image = animation[1 + Anim_count];
		if (count_flag == true)
		{
			attack_flag = true;
		}
		break;
	case State::Damage:
		break;
	case State::Death:
		break;
	default:
		break;
	}

}
// �G�t�F�N�g���䏈��
void P_Melee::EffectControl(float delta_second)
{

}
