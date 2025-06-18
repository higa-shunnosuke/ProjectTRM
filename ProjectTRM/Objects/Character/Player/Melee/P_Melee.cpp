#include "P_Melee.h"
#include "../../../../Utility/LightMapManager.h"
#include "../../../../Scenes/InGame/InGame.h"
#include <iostream >

size_t P_Melee:: count = 0;
size_t P_Melee::GetCount()
{
	return count;
}

// �R���X�g���N�^
P_Melee::P_Melee()
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
	animation = rm->GetImages("Resource/Images/Unit/Melee/Unit_Melee_Walk.png", 10, 10, 1, 100, 55);
	Effect = rm->GetImages("Resource/Images/Effect/Magic_Remove.png", 10, 5, 2, 192, 192);
	sounds = rm->GetSounds("Resource/Sounds/UnitSE/damage02.wav");

	collision.collision_size = Vector2D(60.0f, 60.0f);
	collision.hitbox_size = Vector2D(70.0f, 100.0f);
	z_layer = 3;

	attack_flag = false;
	flip_flag = true;

	now_state = State::Summon;

	//�U����
	basic_power = BASIC_Melee_POWER;

	basic_speed = BASIC_Melee_SPEED;

	// HP������
	HP = 30;

	__super::Initialize();

}

// �X�V����
void P_Melee::Update(float delta_second)
{	
	// �e�N���X�̍X�V
	__super::Update(delta_second);
}

// �`�揈��
void P_Melee::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;
	position.y += z_layer * 2;

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
		DrawRotaGraphF(position.x, position.y, 1.5, 0.0, image, TRUE, flip_flag);
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
void P_Melee::Finalize()
{
	__super::Finalize();
}

// �����蔻��ʒm����
void P_Melee::OnHitCollision(GameObject* hit_object)
{

}

// �U���͈͒ʒm����
void P_Melee::OnAreaDetection(GameObject* hit_object)
{
	// �e�N���X�̍U���͈͒ʒm����
	__super::OnAreaDetection(hit_object);

}

// �ړ�����
void P_Melee::Movement(float delta_second)
{
	//�e�N���X�̈ړ�����
	__super::Movement(delta_second);
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
		switch (now_state)
		{
		case State::Idle:
		case State::Summon:
			animation = rm->GetImages("Resource/Images/Unit/Melee/Unit_Melee_Idle.png", 8, 8, 1, 100, 55);
			anim_max_count = 8;
			break;
		case State::Move:
			animation = rm->GetImages("Resource/Images/Unit/Melee/Unit_Melee_Walk.png", 10, 10, 1, 100, 55);
			anim_max_count = 10;
			break;
		case State::Attack:
			animation = rm->GetImages("Resource/Images/Unit/Melee/Unit_Melee_Attack.png", 9, 9, 1, 100, 55);
			anim_max_count = 9;
			if (Anim_count == anim_max_count)
			{				
				attack_flag = true;
				now_state = State::Idle;
			}
			break;
		case State::Death:
			animation = rm->GetImages("Resource/Images/Unit/Melee/Unit_Melee_Death.png", 9, 9, 1, 100, 55);
			anim_max_count = 9;
			break;
		default:
			break;
		}
		old_state = now_state;
	}

	// �e�N���X�̃A�j���[�V����]
	__super::AnimationControl(delta_second);

}
// �G�t�F�N�g���䏈��
void P_Melee::EffectControl(float delta_second)
{
	//�e�N���X�̃G�t�F�N�g���䏈��
	__super::EffectControl(delta_second);
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
		//�e�N���X��SE���䏈��
		__super::SoundControl();
	}
}