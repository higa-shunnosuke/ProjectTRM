#include "EnemyBase.h"
#include "../../GameObjectManager.h"

// �R���X�g���N�^
EnemyBase::EnemyBase() :
	anim_max_count(),
	recovery_time(),
	recovery_flame(),
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
EnemyBase::~EnemyBase()
{
	
}

// ����������
void EnemyBase::Initialize()
{
	// �摜�̓ǂݍ���
	ResourceManager* rm = ResourceManager::GetInstance();
	effect_image = rm->GetImages("Resource/Images/Effect/Smoke-Sheet.png", 19, 4, 5, 80, 80);
	effect = effect_image[0];

	alpha = 200;
	add = -ALPHA_ADD;
}

// �X�V����
void EnemyBase::Update(float delta_second)
{
	// HP���O�ɂȂ�Ǝ��S��Ԃɂ���
	if (HP <= 0)
	{
		now_state = State::Death;
		collision.object_type = eObjectType::None;
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
		recovery_flame += delta_second;

		// �ҋ@���Ԃ��I�������U����Ԃɂ���
		if (recovery_flame >= recovery_time)
		{
			now_state = State::Move;
		}
	}

	// �A�j���[�V�����Ǘ�����
	AnimationControl(delta_second);

	// �G�t�F�N�g�Ǘ�����
	EffectControl(delta_second);
}

// �`�揈��
void EnemyBase::Draw(const Vector2D camera_pos) const
{
	// �J�������W�����Ƃɕ`��ʒu���v�Z
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

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
void EnemyBase::Finalize()
{
	GameObjectManager* object = GameObjectManager::GetInstance();
	object->DestroyObject(this);
}

// �����蔻��ʒm����
void EnemyBase::OnHitCollision(GameObject* hit_object)
{

}

// �U���͈͒ʒm����
void EnemyBase::OnAreaDetection(GameObject* hit_object)
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
			}
		}
	}
}

// �U���͈͒ʒm����
void EnemyBase::NoHit()
{
	
}

// ���C�g�͈͒ʒm����
void EnemyBase::InLightRange()
{
	
}

// ���C�g�͈͒ʒm����
void EnemyBase::OutLightRange()
{
	
}

// HP�Ǘ�����
void EnemyBase::HPControl(int Damage)
{
	// �_���[�W�y��
	if (!in_light)
	{
		Damage = 1;
	}

	// �_���[�W���f
	this->HP -= Damage;
	if (this->HP < 0)
	{
		this->HP = 0;
	}
}

// �U������
void EnemyBase::Attack(GameObject* hit_object)
{
	// �U���ΏۂɃ_���[�W��^����
	hit_object->HPControl(Damage);
}

// �ړ�����
void EnemyBase::Movement(float delta_second)
{
	// �E�����Ɉړ�������
	velocity.x = speed;

	// �ړ��̎��s
	location += velocity * delta_second;
}

// �A�j���[�V�������䏈��
void EnemyBase::AnimationControl(float delta_second)
{
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
			recovery_flame = 0;
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

	// �A�j���[�V�����̍X�V
	Anim_flame += delta_second;

	// ���ɓ����Ă�����A�j���[�V������x������
	float delay = 1.0f;
	if (in_light == true && (now_state != State::Death && now_state != State::Attack))
	{
		delay = 2.0f;
	}

	// �A�j���[�V�����Ԋu
	if (Anim_flame >= anim_rate * delay)
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
void EnemyBase::EffectControl(float delta_second)
{
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
}
