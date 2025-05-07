#include "E_Melee.h"

// �G�ߐڂ̃J�E���^��������
size_t E_Melee::count = 0;

// �G�ߐڂ̐��擾����
size_t E_Melee::GetCount()
{
	return count;
}

// �R���X�g���N�^
E_Melee::E_Melee() :
	Damage()
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
	// ���ݎ������擾
	anime_time = std::chrono::steady_clock::now();

	is_mobility = true;
	is_aggressive = true;

	collision.is_blocking = true;
	collision.object_type = eObjectType::Enemy;
	collision.hit_object_type.push_back(eObjectType::Player);
	collision.box_size = Vector2D(60.0f, 60.0f);
	collision.attack_size = Vector2D(100.0f, 100.0f);
	z_layer = 2;

	flip_flag = false;

	// �ŏ��̏�Ԃ��ړ��ɂ���
	now_state = State::Move;

	//�U����
	Damage = 2;

	// HP������
	HP = 20;

	alpha = MAX_ALPHA;
	add = -ALPHA_ADD;
}

// �X�V����
void E_Melee::Update(float delta_second)
{
	// �ړ�����
	if (now_state == State::Move)
	{
		Movement(delta_second);
	}

	// �d������
	if (now_state == State::Damage)
	{
		// ���ݎ������擾
		auto now_time = std::chrono::steady_clock::now();

		// �d������
		if (now_time - recovery_time > std::chrono::milliseconds(1000))
		{
			now_state = State::Move;
		}
	}

	// �A�j���[�V�����Ǘ�����
	AnimationControl(delta_second);

	// ��ԍX�V����
	old_state = now_state;
}

// �`�揈��
void E_Melee::Draw(const Vector2D camera_pos) const
{
	// �J�������W�����Ƃɕ`��ʒu���v�Z
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// �G�ߐڂ̕`��
	DrawRotaGraphF(position.x, position.y, 2.0, 0.0, image, TRUE, flip_flag);

#ifdef DEBUG
	//�c��HP�̕\��
	DrawFormatString((int)position.x, (int)(position.y - 40.0f), 0xffffff, "%d", HP);

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
	// ���m�����I�u�W�F�N�g�̃R���W�������
	Collision hit_col = hit_object->GetCollision();

	// ���m�����I�u�W�F�N�g���v���C���[��������
	if (hit_col.object_type == eObjectType::Player)
	{
		// �ړ���ԂȂ�U����Ԃɂ���
		if (now_state == State::Move)
		{
			now_state = State::Attack;
			// �U������
			Attack(hit_object);
		}
		// �ҋ@��ԂȂ�U����Ԃɂ���
		else if (now_state == State::Idle)
		{
			// ���ݎ������擾
			auto now_time = std::chrono::steady_clock::now();

			// �ҋ@����
			if (now_time - recovery_time > std::chrono::milliseconds(2000))
			{
				now_state = State::Attack;
				// �U������
				Attack(hit_object);
			}
		}
	}
}

// HP�Ǘ�����
void E_Melee::HPControl(int Damage)
{
	// �U����ԂłȂ���΃_���[�W��Ԃɂ���
	if (now_state != State::Attack)
	{
		now_state = State::Damage;
	}

	__super::HPControl(Damage);
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
			animation = rm->GetImages("Resource/Images/Enemy/Melee/Melee_Walk.png", 4, 4, 1, 32, 32);
			break;
		case State::Move:
			animation = rm->GetImages("Resource/Images/Enemy/Melee/Melee_Walk.png", 4, 4, 1, 32, 32);
			break;
		case State::Attack:
			animation = rm->GetImages("Resource/Images/Enemy/Melee/Melee_Attack.png", 4, 4, 1, 32, 32);
			image = animation[Anim_count];
			// �d���J�n
			if (Anim_count == 3)
			{
				now_state = State::Idle;
				recovery_time = std::chrono::steady_clock::now();
			}
			break;
		case State::Damage:
			break;
		case State::Death:
			break;
		}
	}

	// �A�j���[�V�����̎��s
	switch (now_state)
	{
	case State::Idle:
		image = animation[0];
		break;
	case State::Move:
		image = animation[Anim_count];
		break;
	case State::Attack:
		image = animation[Anim_count];
		// �d���J�n
		if (Anim_count == 3)
		{
			now_state = State::Idle;
			recovery_time = std::chrono::steady_clock::now();
		}
		break;
	case State::Damage:
		// �_���[�W��ԊJ�n
		recovery_time = std::chrono::steady_clock::now();
		break;
	case State::Death:
		break;
	}

	// ���ݎ������擾
	auto now_time = std::chrono::steady_clock::now();

	// �A�j���[�V�����Ԋu
	if (now_time - anime_time > std::chrono::milliseconds(500))
	{
		// ���̃A�j���[�V�����ɐi�߂�
		if (Anim_count < 3)
		{
			Anim_count++;
		}
		else
		{
			Anim_count = 0;
		}

		// �A�j���[�V�����J�n���Ԃ̍X�V
		anime_time = std::chrono::steady_clock::now();
	}
}
// �G�t�F�N�g���䏈��
void E_Melee::EffectControl(float delta_second)
{

}
