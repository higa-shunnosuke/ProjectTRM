#include "Oracle.h"
#include "../Guardian/P_Guardian.h"
#include "../../../../Scenes/InGame/InGame.h"


// �R���X�g���N�^
Oracle::Oracle() :
	summon_flag(),
	player(),
	old_sun_level(),
	now_sun_level(),
	power_up(false),
	effect_image(),
	anime_max_count(),
	JustDead(false)
{

}

// �f�X�g���N�^
Oracle::~Oracle()
{

}

// ����������
void Oracle::Initialize()
{
	// �摜�̓ǂݍ���
	ResourceManager* rm = ResourceManager::GetInstance();
	EffectImage = rm->GetImages("Resource/Images/Effect/Unit/sprite-sheet.png", 15, 5, 3, 128, 128);
	animation = rm->GetImages("Resource/Images/Unit/Oracle/Oracle_Idle.png",7,7,1,128,128);
	anime_max_count = 7;

	is_mobility = false;
	is_aggressive = false;

	collision.is_blocking = true;
	collision.object_type = eObjectType::Player;
	collision.hit_object_type.push_back(eObjectType::Enemy);
	collision.collision_size = Vector2D(60.0f, 120.0f);
	collision.hitbox_size = Vector2D(200.0f, 200.0f);
	z_layer = 1;

	now_state = State::Summon;

	flip_flag = true;

	summon_flag = false;

	old_sun_level = 1;

	velocity.x = -5.0f;

	// HP������
	HP = 100;
}

// �X�V����
void Oracle::Update(float delta_second)
{
	if (ProjectConfig::DEBUG)
	{
		if (CheckHitKey(KEY_INPUT_6))
		{
			HPControl(1);
		}
	}

	if (Ingame->GetSunLevel() != old_sun_level)
	{
		power_up = !power_up;
		old_sun_level = Ingame->GetSunLevel();
	}
	if (now_state == State::Summon)
	{
		Movement(delta_second);
	}

	AnimationControl(delta_second);

	EffectControl(delta_second);
	/*if (summon_flag == true)
	{
		attack_flame += delta_second;
	}*/
}

// �`�揈��
void Oracle::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	if (ProjectConfig::DEBUG)
	{
		// �ޏ��̕`��
		DrawBox((int)(position.x - collision.collision_size.x / 2), (int)(position.y - collision.collision_size.y / 2),//������ς���
			(int)(position.x + collision.collision_size.x / 2), (int)(position.y + collision.collision_size.y / 2), 0xff0000, TRUE);
		// ���S��\��
		DrawCircle((int)position.x, (int)position.y, 2, 0x0000ff, TRUE);
		// �����蔻��\��
		DrawBox((int)(position.x - collision.collision_size.x / 2), (int)(position.y - collision.collision_size.y / 2),
			(int)(position.x + collision.collision_size.x / 2), (int)(position.y + collision.collision_size.y / 2), 0x0000ff, FALSE);
		// �U���͈͂�\��
		DrawBox((int)position.x, (int)(position.y - collision.hitbox_size.y / 2),
			(int)(position.x - collision.hitbox_size.x), (int)(position.y + collision.hitbox_size.y / 2), 0x0000ff, FALSE);
		//�c��HP�̕\��
		DrawFormatString(position.x, position.y - 40.0f, 0xffffff, "%d", HP);
	}


	if (now_state != State::Death)
	{
		DrawBoxAA(position.x - 50.0f, position.y - 150.0f, position.x + (50.0f - (100 - HP)), position.y - 135.0f, 0xFFFFFF, true);
	}

	//�X�^�[�g���ƕ��펞�̕`����@�̂��݂킯
	if (now_state == State::Summon)
	{
		DrawRotaGraphF(move_location - 15.0f, position.y - 30.0,
			1.5, 0.0, image, TRUE, flip_flag);
	}
	else
	{
		DrawRotaGraphF(position.x - 15.0f, position.y - 30.0f,
			1.5, 0.0, image, TRUE, flip_flag);
	}

	//���z�̃��x���A�b�v���G�t�F�N�g�`��
	if (power_up)
	{
		DrawRotaGraph(position.x, position.y + 15.0f, 1.0, 0.0, effect_image, TRUE);
	}

}

// �I��������
void Oracle::Finalize()
{

}

// �����蔻��ʒm����
void Oracle::OnHitCollision(GameObject* hit_object)
{

}

// �U���͈͒ʒm����
void Oracle::OnAreaDetection(GameObject* hit_object)
{
	//Collision hit_col = hit_object->GetCollision();
	//velocity.x = -1.0f;
	//if (hit_col.object_type == eObjectType::Enemy)
	//{
	//	summon_flag = true;
	//	velocity.x = 0.0f;
	//	if (attack_flame >= 2.0f)
	//	{
	//		//�����Ń��j�b�g�̏���������H

	//	}
	//}
}

// �U������
void Oracle::Attack()
{
	
}


// HP�Ǘ�����
void Oracle::HPControl(float Damage)
{
	HP -= Damage;
	
	GameObjectManager* object = GameObjectManager::GetInstance();

	if (!summon_flag)
	{

			summon_flag = true;
	
			object->CreateObject<P_Guardian>(Vector2D(location.x, location.y + 15.0f))->SetInGamePoint(Ingame);
	}

	if (this->HP <= 0)
	{
		now_state = State::Death;
		this->HP = 0.0f;
		z_layer = 10;
	}

}

// �ړ�����
void Oracle::Movement(float delta_second)
{
	// �ړ��̎��s
	move_location += velocity.x * 10 * delta_second;
	if (move_location <= location.x + 15.0f)
	{
		now_state = State::Idle;
	}
}

bool Oracle::GetDead()
{
	return JustDead;
}

void Oracle::SetLocation(const Vector2D& location)
{
	__super::SetLocation(location);
	move_location = location.x + 100.0f;
}

State Oracle::GetNowState() const
{
	return now_state;
}

// �A�j���[�V�������䏈��
void Oracle::AnimationControl(float delta_second)
{
	ResourceManager* rm = ResourceManager::GetInstance();
	if (now_state != old_state)
	{
		Anim_count = 0;		
		switch (now_state)
		{
		case State::Summon:
			animation = rm->GetImages("Resource/Images/Unit/Oracle/Oracle_Walk.png", 7, 7, 1, 128, 128);
			anime_max_count = 7;
			break;
		case State::Idle:
			animation = rm->GetImages("Resource/Images/Unit/Oracle/Oracle_Idle.png", 7, 7, 1, 128, 128);
			anime_max_count = 7;
			break;
		case State::Death:
			animation = rm->GetImages("Resource/Images/Unit/Oracle/Oracle_Dead.png", 5, 5, 1, 128, 128);
			anime_max_count = 7;
		}
		old_state = now_state;
	}

	auto now_time = std::chrono::steady_clock::now();

	switch (now_state)
	{
	case Idle:
	case Summon:
		if (now_time - anime_time > std::chrono::milliseconds(100))
		{
			Anim_count++;
			anime_time = std::chrono::steady_clock::now();

			if (Anim_count >= anime_max_count)
			{
				Anim_count = 0;
			}
		}
		break;
	case Death:
		if (now_time - anime_time > std::chrono::milliseconds(250))
		{
			Anim_count++;
			anime_time = std::chrono::steady_clock::now();

			if (Anim_count >= anime_max_count)
			{
				Anim_count = 0;
			}
		}
		break;
	}


	switch (now_state)
	{
	case Idle:
	case Summon:
		image = animation[Anim_count];
		break;
	case Death:
		if (Anim_count < anime_max_count - 2)
		{
			image = animation[Anim_count];
		}
		else
		{
			image = animation[4];
		}
		if (Anim_count == anime_max_count - 1)
		{
			JustDead = true;
		}
		break;
	default:
		break;
	}
}

// �G�t�F�N�g���䏈��
void Oracle::EffectControl(float delta_second)
{
	if (power_up)
	{
		Effect_flame += delta_second;
		if (Effect_flame >= 0.1f)
		{
			Effect_count++;
			if (Effect_count > 11)
			{
				Effect_count = 0;
				power_up = !power_up;
			}
			Effect_flame = 0;
		}
		effect_image = EffectImage[Effect_count];
	}
}