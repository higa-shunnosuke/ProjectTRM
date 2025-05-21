#include "Boss.h"
#include "../../../GameObjectManager.h"

// �R���X�g���N�^
Boss::Boss() :
	Damage()
{

}

// �f�X�g���N�^
Boss::~Boss()
{

}

// ����������
void Boss::Initialize()
{
	is_mobility = true;
	is_aggressive = true;

	collision.is_blocking = true;
	collision.object_type = eObjectType::Enemy;
	collision.hit_object_type.push_back(eObjectType::Player);
	collision.collision_size = Vector2D(200.0f, 200.0f);
	collision.hitbox_size = Vector2D(300.0f, 300.0f);
	z_layer = 2;

	flip_flag = false;

	// �ŏ��̏�Ԃ��ړ��ɂ���
	now_state = State::Move;

	//�U����
	Damage = 5;

	// HP������
	HP = 800;

	alpha = MAX_ALPHA;
	add = -ALPHA_ADD;
}

// �X�V����
void Boss::Update(float delta_second)
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
	else if (now_state == State::Idle)
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
void Boss::Draw(const Vector2D camera_pos) const
{
	// �J�������W�����Ƃɕ`��ʒu���v�Z
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// �G�ߐڂ̕`��
	DrawRotaGraphF(position.x, position.y, 0.2, 0.0, image, TRUE, flip_flag);

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
		DrawBox((int)(position.x - collision.collision_size.x / 2), (int)(position.y - collision.collision_size.y / 2),
			(int)(position.x + collision.collision_size.x / 2), (int)(position.y + collision.collision_size.y / 2), 0x0000ff, FALSE);
		// �U���͈͂�\��
		DrawBox((int)position.x, (int)(position.y - collision.hitbox_size.y / 2),
			(int)(position.x + collision.hitbox_size.x), (int)(position.y + collision.hitbox_size.y / 2), 0x0000ff, FALSE);
	}
}

// �I��������
void Boss::Finalize()
{
	GameObjectManager* object = GameObjectManager::GetInstance();
	object->DestroyObject(this);
}

// �����蔻��ʒm����
void Boss::OnHitCollision(GameObject* hit_object)
{

}

// �U���͈͒ʒm����
void Boss::OnAreaDetection(GameObject* hit_object)
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
				// �U������
				Attack(hit_object);
			}
		}
	}
}

// �U���͈͒ʒm����
void Boss::NoHit()
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
void Boss::InLightRange()
{
	in_light = true;
}

// ���C�g�͈͒ʒm����
void Boss::OutLightRange()
{
	in_light = false;
}

// HP�Ǘ�����
void Boss::HPControl(int Damage)
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
void Boss::Attack(GameObject* hit_object)
{
	// �U���ΏۂɃ_���[�W��^����
	hit_object->HPControl(Damage);
}

// �ړ�����
void Boss::Movement(float delta_second)
{
	// �E�����Ɉړ�������
	velocity.x = 1.0f;

	// �ړ��̎��s
	location += velocity * 10 * delta_second;
}

// �A�j���[�V�������䏈��
void Boss::AnimationControl(float delta_second)
{
	// ��Ԃ��؂�ւ������J�E���g��������
	if (old_state != now_state)
	{
		Anim_count = 0;

		// �摜�̓ǂݍ���
		ResourceManager* rm = ResourceManager::GetInstance();
		int anime = NULL;

		// �e��Ԃ̃A�j���[�V�����摜�ɍ����ւ�
		switch (now_state)
		{
		case State::Idle:
			animation = rm->GetImages("Resource/Images/Enemy/Boss/Boss_Idol.png", 1, 1, 1, 1024, 1536);
			break;
		case State::Move:
			animation = rm->GetImages("Resource/Images/Enemy/Boss/Boss_Back.png", 1, 1, 1, 1024,1024);
			break;
		case State::Attack:
			animation = rm->GetImages("Resource/Images/Enemy/Boss/Boss_Attack_1.png", 1, 1, 1, 1024, 1024);
			anime = rm->GetImages("Resource/Images/Enemy/Boss/Boss_Attack_2.png", 1, 1, 1, 1024, 1024)[0];
			animation.push_back(anime);
			anime = rm->GetImages("Resource/Images/Enemy/Boss/Boss_Attack_3.png", 1, 1, 1, 1024, 1024)[0];
			animation.push_back(anime);
			anime = rm->GetImages("Resource/Images/Enemy/Boss/Boss_Idol.png", 1, 1, 1, 1024, 1536)[0];
			animation.push_back(anime);
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
		image = animation[0];
		break;
	case State::Attack:
		image = animation[Anim_count];
		// �d���J�n
		if (Anim_count == 3)
		{
			now_state = State::Idle;
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
void Boss::EffectControl(float delta_second)
{

}