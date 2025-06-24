#include "P_Tank.h"
#include "Torch.h"
#include "../../../GameObjectManager.h"
#include "../../../../Scenes/InGame/InGame.h"

size_t P_Tank::count =0;
size_t P_Tank::GetCount()
{
	return count;
}

// �R���X�g���N�^
P_Tank::P_Tank() :
	light(nullptr)
{
	count++;
}

// �f�X�g���N�^
P_Tank::~P_Tank()
{
	count--;
}

// ����������
void P_Tank::Initialize()
{
	// �摜���̓ǂݍ���
	ResourceManager* rm = ResourceManager::GetInstance();
	animation = rm->GetImages("Resource/Images/Unit/Tank/Tank_Unit_Walk.png", 9, 9, 1, 48, 32);
	effect_image = rm->GetImages("Resource/Images/Effect/Unit/Ghost.png", 1, 1, 1, 50, 50)[0];
	sounds = rm->GetSounds("Resource/Sounds/UnitSE/Tank/Tank_Attack.mp3");


	light = LightMapManager::GetInstance();
	light->AddLight(this);
	collision.light_size = 2.0;

	collision.collision_size = Vector2D(60.0f, 60.0f);
	collision.hitbox_size = Vector2D(100.0f, 100.0f);
	z_layer = 1;


	// HP������
	HP = 40;

	__super::Initialize();
}

// �X�V����
void P_Tank::Update(float delta_second)
{

	__super::Update(delta_second);
}

// �`�揈��
void P_Tank::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;
	position.y += z_layer * 2;

	//�����w�`��
	if (now_state == State::Summon)
	{
		DrawRotaGraphF(position.x + collision.collision_size.y / 3, position.y + collision.collision_size.y / 2, 0.5, 0.0, effect_image, TRUE, flip_flag);
	}

	// ����̕`��
	// �I�t�Z�b�g�l����ɉ摜�̕`����s��
	if (Anim_count <= anim_max_count - 1)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawRotaGraphF(position.x, position.y, 2.5, 0.0, image, TRUE, flip_flag);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	if (!reduction_amount.empty() && now_state != State::Death)
	{
		for (int i = reduction_amount.size() - 1; i >= 0; --i) {
			DrawRotaGraphF(position.x - 20.0f, (position.y - 100.0f) + damage_time[i] * 100, 0.7, 0.0, minus, TRUE);
			DrawRotaGraphF(position.x, (position.y - 100.0f) + damage_time[i] * 100, 1.0, 0.0, text[reduction_amount[i]], TRUE);
		}
	}

	//�X�e�[�g�ɂ���ăG�t�F�N�g�̕`����s��
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
	/*DrawBox((int)(position.x - collision.box_size.x / 2), (int)(position.y - collision.box_size.y / 2),
		(int)(position.x + collision.box_size.x / 2), (int)(position.y + collision.box_size.y / 2), 0xff00ff, TRUE);*/

	__super::Draw(camera_pos);
}

// �I��������
void P_Tank::Finalize()
{
	light->DeleteLight(this);
	__super::Finalize();
}

// �����蔻��ʒm����
void P_Tank::OnHitCollision(GameObject* hit_object)
{

}

// �U���͈͒ʒm����
void P_Tank::OnAreaDetection(GameObject* hit_object)
{
	//���݂̃X�e�[�^�X�����S��Ԃ��ǂ���
	if (now_state != State::Death && Ingame->GetNowState() == GameState::PLAYING)
	{
		Collision hit_col = hit_object->GetCollision();

		if (hit_col.object_type == eObjectType::Enemy)
		{
			velocity.x = 0.0f;
			if (now_state != State::Summon)
			{
				now_state = State::Idle;
			}
		}
	}
}

// �ړ�����
void P_Tank::Movement(float delta_second)
{
	__super::Movement(delta_second);
}

// �A�j���[�V�������䏈��
void P_Tank::AnimationControl(float delta_second)
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
			animation = rm->GetImages("Resource/Images/Unit/Tank/Tank_Unit_Idle.png", 10, 10, 1, 48, 32);
			anim_max_count = 10;
			break;
		case State::Move:
			animation = rm->GetImages("Resource/Images/Unit/Tank/Tank_Unit_Walk.png", 9, 9, 1, 48, 32);
			anim_max_count = 9;
			break;
		case State::Damage:
			PlaySoundMem(sounds, DX_PLAYTYPE_BACK);
			break;
		case State::Death:
			animation = rm->GetImages("Resource/Images/Unit/Tank/Tank_Unit_Down.png", 8, 8, 1, 48, 32);
			anim_max_count = 8;
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
			velocity.x = -BASIC_Tank_SPEED - ((BASIC_Tank_SPEED / 100) * (Ingame->GetSunLevel()));
		}
		else
		{
			velocity.x = BASIC_Tank_SPEED + ((BASIC_Tank_SPEED / 100) * (Ingame->GetSunLevel()));
		}
		image = animation[Anim_count];
		break;
	case State::Death:
		image = animation[Anim_count];
		if (Anim_count == anim_max_count - 1)
		{
			object->CreateObject<Torch>(this->location);
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
void P_Tank::EffectControl(float delta_second)
{
	__super::EffectControl(delta_second);
}

//SE�̐��䏈��
void P_Tank::SoundControl()
{
	if (now_state != old_state)
	{
		ResourceManager* rm = ResourceManager::GetInstance();
		switch (now_state)
		{

		case State::Attack:
			sounds = rm->GetSounds("Resource/Sounds/UnitSE/Tank/Tank_Attack.mp3");
			break;
		default:
			break;
		}
		__super::SoundControl();
	}
}