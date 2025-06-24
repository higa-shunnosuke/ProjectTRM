#include "P_Guardian.h"
#include "../../../../Utility/LightMapManager.h"
#include "../../../../Scenes/InGame/InGame.h"

size_t P_Guardian::count = 0;
size_t P_Guardian::GetCount()
{
	return count;
}

// �R���X�g���N�^
P_Guardian::P_Guardian()
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

	collision.collision_size = Vector2D(60.0f, 60.0f);
	collision.hitbox_size = Vector2D(50.0f, 100.0f);
	z_layer = 1;

	//�U����
	basic_power = BASIC_Guardian_POWER;

	basic_speed = BASIC_Guardian_SPEED;

	// HP������
	HP = 300;

	__super::Initialize();
}

// �X�V����
void P_Guardian::Update(float delta_second)
{

	__super::Update(delta_second);
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
		DrawRotaGraphF(position.x, position.y + collision.collision_size.y / 2, 0.5, 0.0, effect_image, TRUE, flip_flag);
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
			DrawRotaGraphF(position.x - 20.0f, (position.y - 100.0f) + damage_time[i] * 100, 0.7, 0.0, minus, TRUE);
			DrawRotaGraphF(position.x, (position.y - 100.0f) + damage_time[i] * 100, 1.0, 0.0, text[reduction_amount[i]], TRUE);
		}
	}

	switch (now_state)
	{
	case State::Death:
		position.y -= Effect_count * 10 + Effect_flame * 100;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, effect_alpha);
		DrawRotaGraphF(position.x, position.y, 2.0, 0.0, effect_image, TRUE, flip_flag);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		break;
	default:
		break;
	}

	__super::Draw(camera_pos);
}

// �I��������
void P_Guardian::Finalize()
{
	__super::Finalize();
}

// �����蔻��ʒm����
void P_Guardian::OnHitCollision(GameObject* hit_object)
{

}

// �U���͈͒ʒm����
void P_Guardian::OnAreaDetection(GameObject* hit_object)
{
	__super::OnAreaDetection(hit_object);
}


// �ړ�����
void P_Guardian::Movement(float delta_second)
{
	__super::Movement(delta_second);
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
		if (Ingame->GetNowState() == GameState::BOSS_DEAD)
		{
			velocity.x = -BASIC_Guardian_SPEED - ((BASIC_Guardian_SPEED / 100) * (Ingame->GetSunLevel()));
		}
		else
		{
			velocity.x = BASIC_Guardian_SPEED + ((BASIC_Guardian_SPEED / 100) * (Ingame->GetSunLevel()));
		}
		image = animation[Anim_count + 10];
		break;
	case State::Attack:
		image = animation[Anim_count + 78];
		if (Anim_count == anim_max_count / 2 && Ingame->GetNowState() == GameState::PLAYING)
		{
			attack_flag = true;
			PlaySoundMem(sounds, DX_PLAYTYPE_BACK);
		}
		if (Anim_count == anim_max_count - 1)
		{
			now_state = State::Idle;
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
	__super::EffectControl(delta_second);
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
		__super::SoundControl();
	}
}