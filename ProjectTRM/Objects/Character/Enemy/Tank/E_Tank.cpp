#include "E_Tank.h"
size_t E_Tank::count = 0;
size_t E_Tank::GetCount()
{
	return count;
}


// �R���X�g���N�^
E_Tank::E_Tank() :
	Damage()
{

}

// �f�X�g���N�^
E_Tank::~E_Tank()
{

}

// ����������
void E_Tank::Initialize()
{
	// �摜�̓ǂݍ���
	ResourceManager* rm = ResourceManager::GetInstance();
	animation = rm->GetImages("Resource/Images/Enemy/Melee/Melee_Walk.png", 4, 4, 1, 32, 32);

	is_mobility = true;
	is_aggressive = true;

	collision.is_blocking = true;
	collision.object_type = eObjectType::Enemy;
	collision.hit_object_type.push_back(eObjectType::Player);
	collision.box_size = Vector2D(60.0f, 60.0f);
	collision.attack_size = Vector2D(100.0f, 100.0f);
	z_layer = 2;

	attack_flag = false;
	flip_flag = false;

	now_state = State::Move;

	// �E�ֈړ�
	velocity.x = 5.0f;

	//�U����
	Damage = 8;

	// HP������
	HP = 1;
}

// �X�V����
void E_Tank::Update(float delta_second)
{
	// �ړ�����
	Movement(delta_second);

	if (attack_flag == true)
	{
		if (velocity.x > 0.0f)
		{
			now_state = State::Move;
			attack_flame = 0.0f;
		}
		else
		{
			attack_flame -= delta_second;
			now_state = State::Idle;
		}
		if (attack_flame <= 0.0f)
		{
			attack_flag = false;
		}
	}

	AnimationControl(delta_second);

	old_state = now_state;
}

// �`�揈��
void E_Tank::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// �I�t�Z�b�g�l����ɉ摜�̕`����s��
	DrawRotaGraphF(camera_pos.x, camera_pos.y, 1.0, 0.0, image, TRUE);
}

// �I��������
void E_Tank::Finalize()
{

}

// �����蔻��ʒm����
void E_Tank::OnHitCollision(GameObject* hit_object)
{

}

// �U���͈͒ʒm����
void E_Tank::OnAreaDetection(GameObject* hit_object)
{
	Collision hit_col = hit_object->GetCollision();
	if (hit_col.object_type == eObjectType::Player)
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
	else
	{
		velocity.x = 5.0f;
	}
}

// �U������
void E_Tank::Attack(GameObject* hit_object)
{
	hit_object->HPControl(Damage);
	attack_flame = 2.0f;
}

// �ړ�����
void E_Tank::Movement(float delta_second)
{

	// �ړ��̎��s
	location.x += velocity.x * 10 * delta_second;
}

// �A�j���[�V�������䏈��
void E_Tank::AnimationControl(float delta_second)
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

	if (Anim_flame >= 0.1f)
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
		animation = rm->GetImages("Resource/Images/Enemy/Tank/Tank_Walk.png", 4, 4, 1, 32, 32);
		image = animation[1 + Anim_count];
		break;
	case State::Attack:
		animation = rm->GetImages("Resource/Images/Enemy/Tank/Tank_Attack.png", 4, 4, 1, 32, 32);
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
void E_Tank::EffectControl(float delta_second)
{

}