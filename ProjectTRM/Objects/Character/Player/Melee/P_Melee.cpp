#include "P_Melee.h"
#include "Torch.h"
#include "../../../../Utility/LightMapManager.h"
#include "../../../../Scenes/InGame/InGame.h"

size_t P_Melee:: count = 0;
size_t P_Melee::GetCount()
{
	return count;
}

// �R���X�g���N�^
P_Melee::P_Melee() :
	Damage(),
	effect_image()
{
	count++;
}

// �f�X�g���N�^
P_Melee::~P_Melee()
{
	count--;
}

// ����������
void P_Melee::Initialize()
{
	// �摜�̓ǂݍ���
	ResourceManager* rm = ResourceManager::GetInstance();
	animation = rm->GetImages("Resource/Images/Unit/Melee/Melee_Walk.png", 4, 4, 1, 32, 32);
	Effect = rm->GetImages("Resource/Images/Effect/Melee_Attack_Effect.png", 3, 3, 1, 32, 32);
	sounds = rm->GetSounds("Resource/Sounds/UnitSE/damage02.wav");

	LightMapManager* light = LightMapManager::GetInstance();
	light->AddLight(this);
	collision.light_size = 1.0;

	is_mobility = true;
	is_aggressive = true;

	collision.is_blocking = true;
	collision.object_type = eObjectType::Player;
	collision.hit_object_type.push_back(eObjectType::Enemy);
	collision.collision_size = Vector2D(60.0f, 60.0f);
	collision.hitbox_size = Vector2D(70.0f, 100.0f);
	z_layer = 3;

	attack_flag = false;
	flip_flag = true;

	now_state = State::Move;

	// �E�ֈړ�
	velocity.x = BASIC_SPEED;

	//�U����
	Damage = BASIC_POWER;

	// HP������
	HP = 10;

	object = GameObjectManager::GetInstance();

	alpha = MAX_ALPHA;
	effect_alpha = MAX_ALPHA;
	add = -ALPHA_ADD;
}

// �X�V����
void P_Melee::Update(float delta_second)
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
			if (attack_flame <= 0.0f)
			{
				attack_flag = false;
				now_state = State::Move;
			}
		}

		dmage_flame -= delta_second;

		if (dmage_flame <= 0.0f)
		{
			dmage_flame = 0.0f;
			alpha = MAX_ALPHA;
			add = -ALPHA_ADD;
		}

		if (HP <= 0)
		{
			now_state = State::Death;
			add = -1;
		}
	}

	EffectControl(delta_second);

	SoundControl();

	if (Anim_count <= 2)
	{
		AnimationControl(delta_second);
	}
}

// �`�揈��
void P_Melee::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;
	position.y += z_layer * 5;

	// �ߐڃ��j�b�g�̕`��
	// �I�t�Z�b�g�l����ɉ摜�̕`����s��
	if (Anim_count <= 2)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawRotaGraphF(position.x, position.y, 2.0, 0.0, image, TRUE, flip_flag);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	
	switch (now_state)
	{
	case State::Attack:
		DrawRotaGraphF(position.x - (collision.collision_size.x / 2), position.y, 2.0, 0.0, effect_image, TRUE, flip_flag);
		break;
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
		// �U���͂�\��
		DrawFormatString(position.x, position.y - 20.0f, 0x00ffff, "%f", velocity.x);
	}
}

// �I��������
void P_Melee::Finalize()
{
	LightMapManager* light = LightMapManager::GetInstance();
	light->DeleteLight(this);
	//object->CreateObject<Torch>(this->location);
	object->DestroyObject(this);
}

// �����蔻��ʒm����
void P_Melee::OnHitCollision(GameObject* hit_object)
{

}

// �U���͈͒ʒm����
void P_Melee::OnAreaDetection(GameObject* hit_object)
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
void P_Melee::NoHit()
{
	// �ړ���Ԃɂ���
	if (now_state != State::Death)
	{
		now_state = State::Move;
	}
}

// HP�Ǘ�����
void P_Melee::HPControl(int Damage)
{
	// �U����ԂłȂ���΃_���[�W��Ԃɂ���
	if (now_state != State::Attack && now_state != State::Death)
	{
		//PlaySoundMem(sounds, DX_PLAYTYPE_BACK, true);
		now_state = State::Damage;
		__super::HPControl(Damage);
		dmage_flame = 1.0f;
	}
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

	//�X�e�[�g���ς�������̏�����
	if (old_state != now_state)
	{
		Anim_flame = 0;
		Anim_count = 0;
		con = 1;
		switch (now_state)
		{
		case State::Idle:
			animation = rm->GetImages("Resource/Images/Unit/Melee/Melee_Walk.png", 3, 3, 1, 32, 32);
			break;
		case State::Move:
			animation = rm->GetImages("Resource/Images/Unit/Melee/Melee_Walk.png", 3, 3, 1, 32, 32);
			break;
		case State::Attack:
			animation = rm->GetImages("Resource/Images/Unit/Melee/Melee_Attack.png", 4, 4, 1, 32, 32);
			if (Anim_count >= 2)
			{
				attack_flag = true;
				now_state = State::Idle;
			}
			break;
		case State::Damage:
			break;
		case State::Death:
			animation = rm->GetImages("Resource/Images/Unit/Melee/Melee_Down.png", 3, 3, 1, 32, 32);
			break;
		default:
			break;
		}
		old_state = now_state;
	}

	Anim_flame += delta_second;

	if (Anim_flame >= 0.1f)
	{
		Anim_count += con;

		if (Anim_count == 0 || Anim_count == 2)
		{
			con *= -1;
		}

		Anim_flame = 0.0f;
	}

	switch (now_state)
	{
	case State::Idle:	
		image = animation[0];
		break;
	case State::Move:
		velocity.x = BASIC_SPEED + ((BASIC_SPEED / 100) * (Ingame->GetSunLevel()));
		image = animation[1 + Anim_count];
		break;
	case State::Attack:
		image = animation[1 + Anim_count];
		if (Anim_count >= 2)
		{
			attack_flag = true;
			now_state = State::Move;
			velocity.x = BASIC_SPEED + ((BASIC_SPEED / 100) * (Ingame->GetSunLevel()));
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
			image = animation[1 + Anim_count];
		}
		else
		{
			image = animation[0];
		}
		break;
	case State::Death:
		image = animation[Anim_count];
		if (Anim_count >= 2)
		{
			object->CreateObject<Torch>(this->location);
			location.y -= Anim_count * 10 + Anim_flame * 10;
			Anim_count = 3;
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
void P_Melee::EffectControl(float delta_second)
{
	ResourceManager* rm = ResourceManager::GetInstance();

	if (now_state != old_state)
	{
		Effect_count = 0;
		Effect_flame = 0.0f;
		switch (now_state)
		{
		case State::Attack:
			Effect = rm->GetImages("Resource/Images/Effect/Melee_Attack_Effect.png", 3, 3, 1, 32, 32);
			break;
		case State::Damage:
			Effect = rm->GetImages("Resource/Images/Effect/Unit/Unit_Damage.png", 36, 6, 5, 100, 100);
			break;
		case State::Death:
			Effect = rm->GetImages("Resource/Images/Effect/Unit/Melee_Ghost.png", 1, 1, 1, 32, 32);
			break;
		default:
			break;
		}
	}

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

	switch (now_state)
	{
	case State::Idle:
		break;
	case State::Move:
		break;
	case State::Attack:	
		effect_image = Effect[Effect_count];
		break;
	case State::Damage:
		effect_image = Effect[Effect_count];
		if (dmage_flame <= 0.0f)
		{
			now_state = State::Move;
		}
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
	default:
		break;
	}
}

//SE�̐��䏈��
void P_Melee::SoundControl()
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
		ChangeVolumeSoundMem(150, sounds);
	}
}