#include "Boss.h"
#include "../../../GameObjectManager.h"

// �R���X�g���N�^
Boss::Boss() :
	anim_max_count(),
	recovery_time(),
	damage_rate(),
	anim_rate(),
	effect_flame(),
	effect(),
	effect_count(),
	speed(),
	Damage(),
	old_light(false)
{

}

// �f�X�g���N�^
Boss::~Boss()
{

}

// ����������
void Boss::Initialize()
{
	// �摜�̓ǂݍ���
	ResourceManager* rm = ResourceManager::GetInstance();
	effect_image = rm->GetImages("Resource/Images/Effect/Smoke-Sheet.png", 19, 4, 5, 80, 80);
	effect = effect_image[0];

	is_mobility = true;
	is_aggressive = true;

	collision.is_blocking = true;
	collision.object_type = eObjectType::Enemy;
	collision.hit_object_type.push_back(eObjectType::Player);
	collision.collision_size = Vector2D(180.0f, 180.0f);
	collision.hitbox_size = Vector2D(250.0f, 300.0f);
	z_layer = 2;

	flip_flag = true;

	// �ŏ��̏�Ԃ��ړ��ɂ���
	now_state = State::Move;

	//�U����
	Damage = 5;

	// HP������
	HP = 800;

	// �X�s�[�h�̏�����
	speed = 20.0f;

	alpha = 240;
	add = -ALPHA_ADD;
}

// �X�V����
void Boss::Update(float delta_second)
{
	// HP���O�ɂȂ�ƏI������
	if (HP <= 0)
	{
		now_state = State::Death;
	}

	// �����_���[�W��^����
	if (in_light == true && damage_rate >= 1.0f)
	{
		HPControl(1);
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

		// �ҋ@���Ԃ��I�������U����Ԃɂ���
		if (recovery_time >= 4.0f)
		{
			now_state = State::Move;
		}
	}

	// �A�j���[�V�����Ǘ�����
	AnimationControl(delta_second);
}

// �`�揈��
void Boss::Draw(const Vector2D camera_pos) const
{
	// �摜�̂���
	Vector2D offset;
	offset.x = 100.0f;
	offset.y = -50.0f;

	// �J�������W�����Ƃɕ`��ʒu���v�Z
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// �G�ߐڂ̕`��
	DrawRotaGraphF(position.x + offset.x, position.y + offset.y,
		3.0, 0.0, image, TRUE, flip_flag);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawRotaGraphF(position.x, position.y - 110.0f,
		5.0, 0.0, effect, TRUE, flip_flag);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	if (ProjectConfig::DEBUG)
	{
		int color;
		if (in_light == true) {
			color = 0xffffff;
		}
		else {
			color = 0xff0000;
		}
		//�c��HP�̕\��
		DrawFormatString((int)position.x, (int)(position.y - 40.0f), color, "%.1f", HP);
		// ���S��\��
		DrawCircle((int)position.x, (int)position.y, 2, 0x0000ff, TRUE);
		// �����蔻��\��
		DrawBox((int)(position.x - collision.collision_size.x / 2), (int)(position.y - collision.collision_size.y / 2),
			(int)(position.x + collision.collision_size.x / 2), (int)(position.y + collision.collision_size.y / 2), 0xff0000, FALSE);
		// �U���͈͂�\��
		DrawBox((int)position.x, (int)(position.y - collision.hitbox_size.y / 2),
			(int)(position.x + collision.hitbox_size.x), (int)(position.y + collision.hitbox_size.y / 2), 0xff0000, FALSE);
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
		// �U����ԂȂ�U������
		else if (now_state == State::Attack)
		{
			if (Anim_count == anim_max_count)
			{
				// �U������
				Attack(hit_object);

				// �d���J�n
				now_state = State::Idle;
				recovery_time = 0;
			}
		}
	}
}

// �U���͈͒ʒm����
void Boss::NoHit()
{
	
}

// ���C�g�͈͒ʒm����
void Boss::InLightRange()
{
	in_light = true;
	Damage = 3;
	speed = 10.0f;
}

// ���C�g�͈͒ʒm����
void Boss::OutLightRange()
{
	in_light = false;
	Damage = 5;
	speed = 20.0f;
}

// HP�Ǘ�����
void Boss::HPControl(int Damage)
{
	// �_���[�W�y��
	if (!in_light)
	{
		Damage = 1;
	}

	// �_���[�W���f
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
	velocity.x = speed;

	// �ړ��̎��s
	location += velocity * delta_second;
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

		// �e��Ԃ̃A�j���[�V�����摜�ɍ����ւ�
		switch (now_state)
		{
		case State::Idle:
			animation = rm->GetImages("Resource/Images/Enemy/Boss/Boss_Idle.png", 8, 8, 1, 140, 93);
			image = animation[Anim_count];
			anim_max_count = 7;
			anim_rate = 0.3f;
			break;
		case State::Move:
			animation = rm->GetImages("Resource/Images/Enemy/Boss/Boss_Walk.png", 8, 8, 1, 140, 93);
			image = animation[Anim_count];
			anim_max_count = 7;
			anim_rate = 0.3f;
			break;
		case State::Attack:
			animation = rm->GetImages("Resource/Images/Enemy/Boss/Boss_Attack.png", 10, 10, 1, 140, 93);
			image = animation[Anim_count];
			anim_max_count = 9;
			anim_rate = 0.15f;
			break;
		case State::Damage:
			break;
		case State::Death:
			animation = rm->GetImages("Resource/Images/Enemy/Boss/Boss_Death.png", 10, 10, 1, 140, 93);
			image = animation[Anim_count];
			anim_max_count = 9;
			anim_rate = 0.3f;
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
		// ���S
		if (Anim_count == anim_max_count)
		{
			Finalize();
		}
		break;
	}

	// �G�t�F�N�g�̍X�V
	effect_flame += delta_second;

	if (effect_flame >= 0.2f)
	{
		if (effect_count < 18)
		{
			effect_count++;
		}
		else
		{
			effect_count = 0;
		}

		// �G�t�F�N�g�̓���������
		if (old_light == false)
		{
			if (alpha < 200)
			{
				alpha += 40;
			}
			else
			{
				alpha = 200;
			}
		}
		else if (old_light == true)
		{
			if (alpha > 0)
			{
				alpha -= 40;
			}
			else
			{
				alpha = 0;
			}
		}

		effect_flame = 0.0f;
	}

	// ���C�g�t���O�̍X�V
	old_light = in_light;

	// �G�t�F�N�g�̃A�j���[�V����
	effect = effect_image[Anim_count];

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
		Anim_flame = 0.0f;
	}
}
// �G�t�F�N�g���䏈��
void Boss::EffectControl(float delta_second)
{

}