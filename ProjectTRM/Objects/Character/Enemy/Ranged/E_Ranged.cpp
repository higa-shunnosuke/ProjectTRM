#include "E_Ranged.h"
#include "../../../GameObjectManager.h"
#include "E_Projectile.h"

// �G�������̃J�E���^��������
size_t E_Ranged::count = 0;

// �G�������̐��擾����
size_t E_Ranged::GetCount()
{
	return count;
}

// �R���X�g���N�^
E_Ranged::E_Ranged():
	anime_time(),
	recovery_time()
{
	count++;
}

// �f�X�g���N�^
E_Ranged::~E_Ranged()
{
	count--;

}

// ����������
void E_Ranged::Initialize()
{
	is_mobility = true;
	is_aggressive = true;

	collision.is_blocking = true;
	collision.object_type = eObjectType::Enemy;
	collision.hit_object_type.push_back(eObjectType::Player);
	collision.box_size = Vector2D(60.0f, 60.0f);
	collision.attack_size = Vector2D(500.0f, 100.0f);
	z_layer = 2;

	flip_flag = false;

	// �ŏ��̏�Ԃ��ړ��ɂ���
	now_state = State::Move;

	// HP������
	HP = 20;

	//�G��̂ɂ̂ݍU�����邽�߂̃t���O
	attack_flag = false;

	alpha = MAX_ALPHA;
	add = -ALPHA_ADD;
}

// �X�V����
void E_Ranged::Update(float delta_second)
{
	// �����_���[�W��^����
	if (in_light == true && anime_time >= 0.1f)
	{
		HP -= 1;
		anime_time = 0;
	}

	// �ړ�����
	if (now_state == State::Move)
	{
		Movement(delta_second);
	}
	// �ҋ@����
	else if (now_state==State::Idle)
	{
		recovery_time += delta_second;
	}

	// �A�j���[�V�����Ǘ�����
	AnimationControl(delta_second);

	// ��ԍX�V����
	old_state = now_state;

	// HP���O�ɂȂ�ƏI������
	if (HP <= 0)
	{
		Finalize();
	}

}

// �`�揈��
void E_Ranged::Draw(const Vector2D camera_pos) const
{
	// �J�������W�����Ƃɕ`��ʒu���v�Z
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// �������̕`��
	DrawRotaGraphF(position.x, position.y, 2.0, 0.0, image, TRUE, flip_flag);

#ifdef DEBUG
	//�c��HP�̕\��
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
#endif
}

// �I��������
void E_Ranged::Finalize()
{
	GameObjectManager* object = GameObjectManager::GetInstance();
	object->DestroyObject(this);
}

// �����蔻��ʒm����
void E_Ranged::OnHitCollision(GameObject* hit_object)
{

}

// �U���͈͒ʒm����
void E_Ranged::OnAreaDetection(GameObject* hit_object)
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
			if (Anim_count == 3)
			{
				if (!attack_flag)
				{
					// �U������
					Attack(hit_object);
				}
			}
		}
	}
}

// �U���͈͒ʒm����
void E_Ranged::NoHit()
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
	// �U����ԂłȂ���Έړ���Ԃɂ���
	else if (now_state != State::Attack)
	{
		now_state = State::Move;
	}
}

// ���C�g�͈͒ʒm����
void E_Ranged::InLightRange()
{
	in_light = true;
}

// ���C�g�͈͒ʒm����
void E_Ranged::OutLightRange()
{
	in_light = false;
}

// HP�Ǘ�����
void E_Ranged::HPControl(int Damage)
{
	// �_���[�W�y��
	if (!in_light)
	{
		Damage *= 0.5;
	}

	this->HP -= (int)Damage;
	if (this->HP < 0)
	{
		this->HP = 0;
	}
}

// �U������
void E_Ranged::Attack(GameObject* hit_object)
{
	GameObjectManager* object = GameObjectManager::GetInstance();
	object->CreateObject<E_Projectile>(this->location)->SetTargetLocation(hit_object->GetLocation());
	attack_flag = true;
}

// �ړ�����
void E_Ranged::Movement(float delta_second)
{
	// �E�����Ɉړ�������
	velocity.x = 5.0f;

	// �ړ��̎��s
	location += velocity * 10 * delta_second;
}

// �A�j���[�V�������䏈��
void E_Ranged::AnimationControl(float delta_second)
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
			animation = rm->GetImages("Resource/Images/Enemy/Ranged/Ranged_Walk.png", 4, 4, 1, 32, 32);
			break;
		case State::Move:
			animation = rm->GetImages("Resource/Images/Enemy/Ranged/Ranged_Walk.png", 4, 4, 1, 32, 32);
			break;
		case State::Attack:
			animation = rm->GetImages("Resource/Images/Enemy/Ranged/Ranged_Attack.png", 4, 4, 1, 32, 32);
			image = animation[Anim_count];
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
			attack_flag = false;
			recovery_time = 0;
		}
		break;
	case State::Damage:
		break;
	case State::Death:
		break;
	}

	// �A�j���[�V�����̍X�V
	anime_time += delta_second;

	// �A�j���[�V�����Ԋu
	if (anime_time >= 0.1f)
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

		if (in_light == false)
		{
			// �A�j���[�V�����J�n���Ԃ̏�����
			anime_time = 0;
		}
	}
}
// �G�t�F�N�g���䏈��
void E_Ranged::EffectControl(float delta_second)
{

}