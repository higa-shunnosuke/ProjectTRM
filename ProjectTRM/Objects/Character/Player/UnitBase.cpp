#include "UnitBase.h"

// �R���X�g���N�^
UnitBase::UnitBase() :
	Damage(),
	sounds(),
	basic_power(),
	basic_speed(),
	effect_image(),
	effect_alpha(),
	light(nullptr)
{

}

// �f�X�g���N�^
UnitBase::~UnitBase()
{

}

// ����������
void UnitBase::Initialize()
{
	light = LightMapManager::GetInstance();
	light->AddLight(this);

	is_mobility = true;
	is_aggressive = true;

	collision.is_blocking = true;
	collision.object_type = eObjectType::Player;
	collision.hit_object_type.push_back(eObjectType::Enemy);


	//�ړ����x
	velocity.x = basic_speed;

	//�U����
	Damage = basic_power;

	//���]�t���O
	flip_flag = true;

	//�U���t���O
	attack_flag = false;

	//������
	now_state = State::Move;

	alpha = MAX_ALPHA;
	effect_alpha = MAX_ALPHA;
	add = -ALPHA_ADD;
}

// �X�V����
void UnitBase::Update(float delta_second)
{
	Damage = basic_power + (Ingame->GetSunLevel() / 5);

	if (ProjectConfig::DEBUG)
	{
		InputManager* input = InputManager::GetInstance();
		if (input->GetKeyState(KEY_INPUT_K) == eInputState::Pressed)
		{
			HP = 0;
		}
	}

	if (now_state != State::Death)
	{
		// �ړ�����
		Movement(delta_second);

		if (attack_flag == true)
		{
			if (velocity.x < 0.0f)
			{
				now_state = State::Move;
				attack_flame = 0.0f;
			}
			else
			{
				now_state = State::Idle;
				attack_flame -= delta_second * (1 + (Ingame->GetSunLevel() / 10));
			}
			if (attack_flame <= 0.0f)
			{
				attack_flag = false;
				now_state = State::Move;
			}
		}


		if (HP <= 0)
		{
			now_state = State::Death;
			add = -1;
		}
	}

	EffectControl(delta_second);

	SoundControl();

	if (Anim_count <= anim_max_count)
	{
		AnimationControl(delta_second);
	}
}

// �`�揈��
void UnitBase::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	//���C���[�����Ƃɍ��W�����炷
	position.y += z_layer * 5;

	// ����̕`��
	// �I�t�Z�b�g�l����ɉ摜�̕`����s��
	if (Anim_count <= anim_max_count)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawRotaGraphF(position.x, position.y, 2.0, 0.0, image, TRUE, flip_flag);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	//�X�e�[�g�ɂ���ăG�t�F�N�g�̕`����s��
	switch (now_state)
	{
	case State::Damage:
		DrawRotaGraphF(position.x, position.y, 1.0, 0.0, effect_image, TRUE, flip_flag);
		break;
	case State::Death:
		position.y -= Effect_count * 10 + Effect_flame * 10;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, effect_alpha);
		DrawRotaGraphF(position.x, position.y, 2.0, 0.0, effect_image, TRUE, flip_flag);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		break;
	default:
		break;
	}
	/*DrawBox((int)(position.x - collision.box_size.x / 2), (int)(position.y - collision.box_size.y / 2),
		(int)(position.x + collision.box_size.x / 2), (int)(position.y + collision.box_size.y / 2), 0xff00ff, TRUE);*/

	if (ProjectConfig::DEBUG)
	{
		// ���S��\��
		DrawCircle((int)position.x, (int)position.y, 2, 0x0000ff, TRUE);
		// �����蔻��\��
		DrawBox((int)(position.x - collision.collision_size.x / 2), (int)(position.y - collision.collision_size.y / 2),
			(int)(position.x + collision.collision_size.x / 2), (int)(position.y + collision.collision_size.y / 2), 0x0000ff, FALSE);
		// �U���͈͂�\��
		DrawBox((int)position.x, (int)(position.y - collision.hitbox_size.y / 2),
			(int)(position.x - collision.hitbox_size.x), (int)(position.y + collision.hitbox_size.y / 2), 0x0000ff, FALSE);
		// HP��\��
		DrawFormatString(position.x, position.y - 20.0f, 0x00ffff, "%d", HP);
	}
}

// �I��������
void UnitBase::Finalize()
{
	GameObjectManager* object = GameObjectManager::GetInstance();
	LightMapManager* light = LightMapManager::GetInstance();
	light->DeleteLight(this);
	object->DestroyObject(this);
}

// �����蔻��ʒm����
void UnitBase::OnHitCollision(GameObject* hit_object)
{

}

// �U���͈͒ʒm����
void UnitBase::OnAreaDetection(GameObject* hit_object)
{
	//���݂̃X�e�[�^�X�����S��Ԃ��ǂ���
	if (now_state != State::Death)
	{
		Collision hit_col = hit_object->GetCollision();

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
}

// �U���͈͒ʒm����
void UnitBase::NoHit()
{
	// �ړ���Ԃɂ���
	if (now_state != State::Death)
	{
		velocity.x = basic_speed + ((basic_speed / 100) * (Ingame->GetSunLevel()));
	}
}

// HP�Ǘ�����
void UnitBase::HPControl(int Damage)
{
	// �U����ԂłȂ���΃_���[�W��Ԃɂ���
	if (now_state != State::Attack)
	{
		now_state = State::Damage;
	}

	__super::HPControl(Damage);
}

// �U������
void UnitBase::Attack(GameObject* hit_object)
{

	PlaySoundMem(sounds, DX_PLAYTYPE_BACK, TRUE);
	hit_object->HPControl(Damage);
	attack_flame = 2.0f;
}

// �ړ�����
void UnitBase::Movement(float delta_second)
{
	// �ړ��̎��s
	location.x += velocity.x * 10 * delta_second;
}

// �A�j���[�V�������䏈��
void UnitBase::AnimationControl(float delta_second)
{

	Anim_flame += delta_second;

	if (Anim_flame >= 0.1f)
	{
		Anim_count++;

		if (Anim_count == anim_max_count)
		{
			Anim_count = 0;
		}

		Anim_flame = 0.0f;
	}

	switch (now_state)
	{
	case State::Idle:
		image = animation[Anim_count];
		break;
	case State::Move:
		velocity.x = basic_speed + ((basic_speed / 100) * (Ingame->GetSunLevel()));
		image = animation[Anim_count];
		break;
	case State::Attack:
		image = animation[Anim_count];
		if (Anim_count == anim_max_count / 2)
		{
			PlaySoundMem(sounds, DX_PLAYTYPE_BACK);
		}
		if (Anim_count == anim_max_count - 1)
		{
			attack_flag = true;
			now_state = State::Move;
			velocity.x = basic_speed + ((basic_speed / 100) * (Ingame->GetSunLevel()));
		}
		break;
	case State::Damage:
		alpha += add;
		if (alpha <= 0 || alpha >= 255)
		{
			add = -add;
		}
		if (velocity.x < 0.0f)
		{
			image = animation[Anim_count];
		}
		else
		{
			image = animation[0];
		}
		break;
	case State::Death:
		image = animation[Anim_count];
		if (Anim_count == anim_max_count - 1)
		{

			location.y -= Anim_count * 10 + Anim_flame * 10;
			Anim_count += 2;
			//���̃I�u�W�F�N�g�̎ז������Ȃ��悤�ɃI�u�W�F�N�g�^�C�v�̏���
			collision.hit_object_type.clear();
			collision.object_type = eObjectType::None;
			LightMapManager* light = LightMapManager::GetInstance();
			light->DeleteLight(this);
		}
		break;
	default:
		break;
	}
}
// �G�t�F�N�g���䏈��
void UnitBase::EffectControl(float delta_second)
{

	Effect_flame += delta_second;
	if (Effect_flame >= 0.1f)
	{
		if (Effect_count < 29)
		{
			Effect_count++;
		}
		else
		{
			Effect_count = 0;
		}
		Effect_flame = 0;
	}
}

//SE�̐��䏈��
void UnitBase::SoundControl()
{

}