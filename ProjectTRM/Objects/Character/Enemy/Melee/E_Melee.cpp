#include "E_Melee.h"
#include "../../../GameObjectManager.h"

// �G�ߐڂ̃J�E���^��������
size_t E_Melee::count = 0;

// �G�ߐڂ̐��擾����
size_t E_Melee::GetCount()
{
	return count;
}

// �R���X�g���N�^
E_Melee::E_Melee() :
	Damage(),
	anim_max_count(),
	recovery_time(),
	damage_rate(),
	anim_rate()
{
	count++;
}

// �f�X�g���N�^
E_Melee::~E_Melee()
{
	count--;
}

// ����������
void E_Melee::Initialize()
{
	is_mobility = true;
	is_aggressive = true;

	collision.is_blocking = true;
	collision.object_type = eObjectType::Enemy;
	collision.hit_object_type.push_back(eObjectType::Player);
	collision.box_size = Vector2D(60.0f, 100.0f);
	collision.attack_size = Vector2D(100.0f, 90.0f);
	z_layer = 2;

	flip_flag = false;

	// �ŏ��̏�Ԃ��ړ��ɂ���
	now_state = State::Move;

	//�U����
	Damage = 5;

	// HP������
	HP = 200;

	alpha = MAX_ALPHA;
	add = -ALPHA_ADD;
}

// �X�V����
void E_Melee::Update(float delta_second)
{
	// �����_���[�W��^����
	if (in_light == true && damage_rate >= 0.1f)
	{
		HP -= 1;
		damage_rate = 0;
	}
	else
	{
		damage_rate += delta_second;
	}

	// �ړ�����
	if (now_state == State::Move)
	{
		Movement(delta_second);
	}
	// �ҋ@����
	else if (now_state == State::Idle)
	{
		recovery_time += delta_second;
	}

	// �A�j���[�V�����Ǘ�����
	AnimationControl(delta_second);

	// HP���O�ɂȂ�ƏI������
	if (HP <= 0)
	{
		now_state = State::Death;
	}
}

// �`�揈��
void E_Melee::Draw(const Vector2D camera_pos) const
{
	// �摜�̂���
	Vector2D offset;
	offset.x = 30.0f;
	offset.y = -30.0f;

	// �J�������W�����Ƃɕ`��ʒu���v�Z
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// �G�ߐڂ̕`��
	DrawRotaGraphF(position.x + offset.x, position.y + offset.y,
		2.0, 0.0, image, TRUE, flip_flag);

	if (ProjectConfig::DEBUG)
	{	//�c��HP�̕\��
		if (in_light == true)
		{
			DrawFormatString((int)position.x, (int)(position.y - 40.0f), 0xffffff, "%d", HP);
		}
		else
		{
			DrawFormatString((int)position.x, (int)(position.y - 40.0f), 0xff0000, "%d", HP);
		}

		// ���S��\��
		DrawCircle((int)position.x, (int)position.y, 2, 0x0000ff, TRUE);
		// �����蔻��\��
		/*DrawBox((int)(position.x - collision.box_size.x / 2), (int)(position.y - collision.box_size.y / 2),
			(int)(position.x + collision.box_size.x / 2), (int)(position.y + collision.box_size.y / 2), 0xff00a0, TRUE);*/
		DrawBox((int)(position.x - collision.box_size.x / 2), (int)(position.y - collision.box_size.y / 2),
			(int)(position.x + collision.box_size.x / 2), (int)(position.y + collision.box_size.y / 2), 0x0000ff, FALSE);
		// �U���͈͂�\��
		DrawBox((int)(position.x - collision.attack_size.x / 2), (int)(position.y - collision.attack_size.y / 2),
			(int)(position.x + collision.attack_size.x / 2), (int)(position.y + collision.attack_size.y / 2), 0x0000ff, FALSE);
	}
}

// �I��������
void E_Melee::Finalize()
{
	GameObjectManager* object = GameObjectManager::GetInstance();
	object->DestroyObject(this);
}

// �����蔻��ʒm����
void E_Melee::OnHitCollision(GameObject* hit_object)
{

}

// �U���͈͒ʒm����
void E_Melee::OnAreaDetection(GameObject* hit_object)
{
	// ���m�����I�u�W�F�N�g�̃R���W�������
	Collision hit_col = hit_object->GetCollision();

	// ���m�����I�u�W�F�N�g���v���C���[��������
	if (hit_col.object_type == eObjectType::Player)
	{
		// �ړ���ԂȂ�U����Ԃɂ���
		if (now_state == State::Move)
		{
			now_state = State::Attack;
		}
		// �ҋ@��ԂȂ�ҋ@����
		else if (now_state == State::Idle)
		{
			// �ҋ@���Ԃ��I�������U����Ԃɂ���
			if (recovery_time >= 1.0f)
			{
				now_state = State::Attack;
			}
		}
		// �U����ԂȂ�U������
		else if (now_state == State::Attack)
		{
			if (Anim_count == anim_max_count)
			{
				// �U������
				Attack(hit_object);
			}
		}
	}
}

// �U���͈͒ʒm����
void E_Melee::NoHit()
{
	// �ҋ@��ԂȂ�ҋ@����
	if (now_state == State::Idle)
	{
		// �ҋ@���Ԃ��I�������ړ���Ԃɂ���
		if (recovery_time >= 1.0f)
		{
			now_state = State::Move;
		}
	}
}

// ���C�g�͈͒ʒm����
void E_Melee::InLightRange()
{
	in_light = true;
}

// ���C�g�͈͒ʒm����
void E_Melee::OutLightRange()
{
	in_light = false;
}

// HP�Ǘ�����
void E_Melee::HPControl(int Damage)
{
	// �_���[�W�y��
	if (!in_light)
	{
		Damage = (int)(Damage * 0.5);
	}

	this->HP -= Damage;
	if (this->HP < 0)
	{
		this->HP = 0;
	}
}

// �U������
void E_Melee::Attack(GameObject* hit_object)
{
	// �U���ΏۂɃ_���[�W��^����
	hit_object->HPControl(Damage);
}

// �ړ�����
void E_Melee::Movement(float delta_second)
{
	// �E�����Ɉړ�������
	velocity.x = 5.0f;

	// �ړ��̎��s
	location += velocity * 10 * delta_second;
}

// �A�j���[�V�������䏈��
void E_Melee::AnimationControl(float delta_second)
{
	// ��Ԃ��؂�ւ������J�E���g��������
	if (old_state != now_state)
	{
		Anim_count = 0;

		// �摜�̓ǂݍ���
		ResourceManager* rm = ResourceManager::GetInstance();

		// �e��Ԃ̃A�j���[�V�����摜�ɍ����ւ�
		switch (now_state)
		{
		case State::Idle:
			animation = rm->GetImages("Resource/Images/Enemy/Melee/E_Melee_Idle.png", 4, 4, 1, 100, 75);
			image = animation[Anim_count];
			anim_max_count = 3;
			anim_rate = 0.3f;
			break;
		case State::Move:
			animation = rm->GetImages("Resource/Images/Enemy/Melee/E_Melee_Walk.png", 6, 6, 1, 100, 75);
			image = animation[Anim_count];
			anim_max_count = 5;
			anim_rate = 0.1f;
			break;
		case State::Attack:
			animation = rm->GetImages("Resource/Images/Enemy/Melee/E_Melee_Attack.png", 8, 8, 1, 100, 75);
			image = animation[Anim_count];
			anim_max_count = 7;
			anim_rate = 0.1f;
			break;
		case State::Damage:
			break;
		case State::Death:
			animation = rm->GetImages("Resource/Images/Enemy/Melee/E_Melee_Dead.png", 4, 4, 1, 100, 75);
			image = animation[Anim_count];
			anim_max_count = 3;
			anim_rate = 0.2f;
			break;
		}
	}

	// ��ԍX�V����
	old_state = now_state;

	// �A�j���[�V�����̎��s
	switch (now_state)
	{
	case State::Idle:
		image = animation[Anim_count];
		break;
	case State::Move:
		image = animation[Anim_count];
		break;
	case State::Attack:
		image = animation[Anim_count];
		// �d���J�n
		if (Anim_count == anim_max_count)
		{
			now_state = State::Idle;
			recovery_time = 0;
		}
		break;
	case State::Death:
		image = animation[Anim_count];
		// �d���J�n
		if (Anim_count == anim_max_count)
		{
			Finalize();
		}
		break;
	}

	// �A�j���[�V�����̍X�V
	Anim_flame += delta_second;

	// �A�j���[�V�����Ԋu
	if (Anim_flame >= anim_rate)
	{
		// ���̃A�j���[�V�����ɐi�߂�
		if (Anim_count < anim_max_count)
		{
			Anim_count++;
		}
		else
		{
			Anim_count = 0;
		}

		// �A�j���[�V�����J�n���Ԃ̏�����
		Anim_flame = 0;
	}
}
// �G�t�F�N�g���䏈��
void E_Melee::EffectControl(float delta_second)
{

}
