#include "P_Guardian.h"
#include "../../../../Utility/LightMapManager.h"
#include "../../../../Scenes/InGame/InGame.h"

size_t P_Guardian::count = 0;
size_t P_Guardian::GetCount()
{
	return count;
}

// �R���X�g���N�^
P_Guardian::P_Guardian() :
	Damage(),
	effect_image(),
	object(nullptr),
	anim_max_count(),
	sounds(),
	effect_alpha(),
	effect_max_count()
{
	count++;
}

// �f�X�g���N�^
P_Guardian::~P_Guardian()
{
	count--;
}

// ����������
void P_Guardian::Initialize()
{
	// �摜�̓ǂݍ���
	ResourceManager* rm = ResourceManager::GetInstance();
	animation = rm->GetImages("Resource/Images/Unit/Guardian/Guardian_All.png", 121, 11, 11, 96, 96);
	text = rm->GetImages("Resource/Images/BackGround/numbers.png", 10, 5, 2, 32, 32);

	//LightMapManager* light = LightMapManager::GetInstance();
	//light->AddLight(this);
	//collision.light_size = 1.0;

	is_mobility = true;
	is_aggressive = true;

	collision.is_blocking = true;
	collision.object_type = eObjectType::Player;
	collision.hit_object_type.push_back(eObjectType::Enemy);
	collision.collision_size = Vector2D(60.0f, 60.0f);
	collision.hitbox_size = Vector2D(50.0f, 100.0f);
	z_layer = 1;

	attack_flag = false;
	flip_flag = true;

	now_state = State::Summon;

	//�U����
	Damage = BASIC_POWER;

	// HP������
	HP = 50;

	object = GameObjectManager::GetInstance();


	alpha = MAX_ALPHA;
	add = -ALPHA_ADD;
}

// �X�V����
void P_Guardian::Update(float delta_second)
{

	Damage = BASIC_POWER + (Ingame->GetSunLevel() / 5);

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
			if (attack_flame <= 0.0f && now_state != State::Summon)
			{
				attack_flag = false;
				now_state = State::Move;
			}
		}

		if (!reduction_amount.empty())
		{
			for (int i = reduction_amount.size() - 1; i >= 0; --i) {
				if (damage_time[i] <= 0.0f) {
					reduction_amount.erase(reduction_amount.begin() + i);
					damage_time.erase(damage_time.begin() + i); // damage_time �����킹�ď���
				}
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
void P_Guardian::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;
	position.y += z_layer * 8;

	//�����w�`��
	if (now_state == State::Summon)
	{
		DrawRotaGraphF(position.x, position.y + collision.collision_size.y / 3, 0.5, 0.0, effect_image, TRUE, flip_flag);
	}

	// �ߐڃ��j�b�g�̕`��
	// �I�t�Z�b�g�l����ɉ摜�̕`����s��
	if (Anim_count <= anim_max_count)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawRotaGraphF(position.x, position.y, 1.0, 0.0, image, TRUE, flip_flag);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	if (!reduction_amount.empty() && now_state != State::Death)
	{
		for (int i = reduction_amount.size() - 1; i >= 0; --i) {
			DrawRotaGraphF(position.x, (position.y - 100.0f) + damage_time[i] * 100, 1.0, 0.0, text[reduction_amount[i]], TRUE);
		}
	}

	/*DrawBox((int)(position.x - collision.box_size.x / 2), (int)(position.y - collision.box_size.y / 2),
		(int)(position.x + collision.box_size.x / 2), (int)(position.y + collision.box_size.y / 2), 0xffa000, TRUE);*/

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
	}
}

// �I��������
void P_Guardian::Finalize()
{
	//LightMapManager* light = LightMapManager::GetInstance();
	//light->DeleteLight(this);
	object->DestroyObject(this);
}

// �����蔻��ʒm����
void P_Guardian::OnHitCollision(GameObject* hit_object)
{

}

// �U���͈͒ʒm����
void P_Guardian::OnAreaDetection(GameObject* hit_object)
{
	//���݂̃X�e�[�^�X�����S��Ԃ��ǂ���
	if (now_state != State::Death)
	{
		Collision hit_col = hit_object->GetCollision();

		if (hit_col.object_type == eObjectType::Enemy)
		{
			velocity.x = 0.0f;
			if (attack_flag == false && now_state != State::Summon)
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
void P_Guardian::NoHit()
{
	// �ړ���Ԃɂ���
	if (now_state != State::Death && now_state != State::Summon)
	{
		now_state = State::Move;
	}
}

// HP�Ǘ�����
void P_Guardian::HPControl(int Damage)
{
	// �U����ԂłȂ���΃_���[�W��Ԃɂ���
	if (now_state != State::Attack && now_state != State::Death)
	{
		//PlaySoundMem(sounds, DX_PLAYTYPE_BACK, true);
		reduction_amount.push_back(Damage);
		__super::HPControl(Damage);
		damage_time.push_back(0.5f);
	}
}


// �U������
void P_Guardian::Attack(GameObject* hit_object)
{
	hit_object->HPControl(Damage);
	attack_flame = 2.0f;
}

// �ړ�����
void P_Guardian::Movement(float delta_second)
{
	// �ړ��̎��s
	location.x += velocity.x * 10 * delta_second;
}

// �A�j���[�V�������䏈��
void P_Guardian::AnimationControl(float delta_second)
{
	// �摜�̓ǂݍ���
	ResourceManager* rm = ResourceManager::GetInstance();

	//�X�e�[�g���ς�������̏�����
	if (old_state != now_state)
	{
		Anim_flame = 0;
		Anim_count = 0;
		con = 1;
		switch (now_state)
		{
		case State::Idle:
		case State::Summon:
			anim_max_count = 9;
			break;
		case State::Move:
			anim_max_count = 5;
			break;
		case State::Attack:
			anim_max_count = 5;
			if (Anim_count == anim_max_count)
			{
				attack_flag = true;
				now_state = State::Idle;
			}
			break;
		case State::Death:
			anim_max_count = 11;
			break;
		default:
			break;
		}
		old_state = now_state;
	}

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
	case State::Summon:
		image = animation[Anim_count];
		break;
	case State::Move:
		velocity.x = BASIC_SPEED + ((BASIC_SPEED / 100) * (Ingame->GetSunLevel() - 1));
		image = animation[Anim_count + 10];
		break;
	case State::Attack:
		image = animation[Anim_count + 78];
		if (Anim_count == anim_max_count / 2 && Ingame->GetNowState() == GameState::PLAYING)
		{
			PlaySoundMem(sounds, DX_PLAYTYPE_BACK);
		}
		if (Anim_count == anim_max_count - 1)
		{
			attack_flag = true;
			now_state = State::Move;
			velocity.x = BASIC_SPEED + ((BASIC_SPEED / 100) * (Ingame->GetSunLevel()));
		}
		break;
	case State::Death:
		image = animation[Anim_count + 104];
		if (Anim_count == anim_max_count - 1)
		{
			Anim_count += 2;
			collision.hit_object_type.clear();
			collision.object_type = eObjectType::None;
		}
		break;
	default:
		break;
	}

}
// �G�t�F�N�g���䏈��
void P_Guardian::EffectControl(float delta_second)
{
	if (now_state != old_state)
	{
		ResourceManager* rm = ResourceManager::GetInstance();
		switch (now_state)
		{
		case State::Summon:
			Effect = rm->GetImages("Resource/Images/Effect/Magic_Remove.png", 10, 5, 2, 192, 192);
			effect_max_count = 10;
			break;
		case State::Death:
			Effect = rm->GetImages("Resource/Images/Effect/Unit/Ghost.png", 1, 1, 1, 50, 50);
			effect_max_count = 29;
			break;
		default:
			break;
		}
	}

	Effect_flame += delta_second;
	if (Effect_flame >= 0.1f)
	{
		Effect_count++;
		if (Effect_count > effect_max_count)
		{
			Effect_count = 0;
		}
		Effect_flame = 0;
	}
	if (!damage_time.empty())
	{
		for (int i = 0; i < damage_time.size(); i++)
		{
			damage_time[i] -= delta_second;
		}
	}

	switch (now_state)
	{
	case State::Summon:
		if (Effect_count == effect_max_count)
		{
			now_state = State::Move;
			break;
		}
		effect_image = Effect[Effect_count];
		break;
	case State::Death:
		effect_image = Effect[0];
		effect_alpha += add;
		//���S�ɓ����ɂȂ�����I�u�W�F�N�g����
		if (effect_alpha <= 0)
		{
			effect_alpha = 0;
			Finalize();
		}
		break;
	}
}

//SE�̐��䏈��
void P_Guardian::SoundControl()
{
	if (now_state != old_state)
	{
		ResourceManager* rm = ResourceManager::GetInstance();
		switch (now_state)
		{

		case State::Attack:
			sounds = rm->GetSounds("Resource/Sounds/UnitSE/Tank/Tank_Hit.mp3");
			break;
		default:
			break;
		}
		ChangeVolumeSoundMem(90, sounds);
	}
}