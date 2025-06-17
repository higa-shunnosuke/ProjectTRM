#include "P_Ranged.h"
#include "P_Projectile.h"
#include "../../../GameObjectManager.h"
#include "../../../../Scenes/InGame/InGame.h"

size_t P_Ranged::count = 0;
size_t P_Ranged::GetCount()
{
	return count;
}

// �R���X�g���N�^
P_Ranged::P_Ranged() :
	target_loc(0.0f)
{
	count++;
}

// �f�X�g���N�^
P_Ranged::~P_Ranged()
{
	count--;
}

// ����������
void P_Ranged::Initialize()
{
	// �摜�̓ǂݍ���
	ResourceManager* rm = ResourceManager::GetInstance();
	animation = rm->GetImages("Resource/Images/Unit/Ranged/Archer_All.png", 55, 11, 5, 64, 64);
	effect_image = rm->GetImages("Resource/Images/Effect/Unit/Ghost.png", 1, 1, 1, 32, 32)[0];

	collision.collision_size = Vector2D(60.0f, 60.0f);
	collision.hitbox_size = Vector2D(500.0f, 100.0f);
	z_layer = 2;

	// HP������
	HP = 10;

	basic_power = 0;
	basic_speed = BASIC_Ranged_SPEED;

	__super::Initialize();
}

// �X�V����
void P_Ranged::Update(float delta_second)
{
	__super::Update(delta_second);
}

// �`�揈��
void P_Ranged::Draw(const Vector2D camera_pos) const
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
		DrawRotaGraphF(position.x, position.y, 1.4, 0.0, image, TRUE, flip_flag);
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
	__super::Draw(camera_pos);
}

// �I��������
void P_Ranged::Finalize()
{
	object->DestroyObject(this);
}

// �����蔻��ʒm����
void P_Ranged::OnHitCollision(GameObject* hit_object)
{
	
}

// �U���͈͒ʒm����
void P_Ranged::OnAreaDetection(GameObject* hit_object)
{

	target_loc.y = hit_object->GetLocation().y;
	//���݂̃X�e�[�^�X�����S��Ԃ��ǂ���
	if (now_state != State::Death)
	{
		Collision hit_col = hit_object->GetCollision();

		if (hit_col.object_type == eObjectType::Enemy)
		{
			if (hit_object->GetInLight() || target_loc.y == 630.0f)
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
			else
			{
				if (target_loc.x <= hit_object->GetLocation().x)
				{

					//�Q�̃I�u�W�F�N�g�̋������擾
					Vector2D diff = this->GetLocation() - hit_object->GetLocation();

					//�Q�̃I�u�W�F�N�g�̓����蔻��̑傫�����擾
					Vector2D box_size = (this->collision.collision_size + hit_col.collision_size) / 2.0f;

					// ��`���m�̓����蔻��
					if ((fabsf(diff.x) < box_size.x) && (fabsf(diff.y) < box_size.y))
					{
						velocity.x = 0;
						now_state = State::Idle;
						target_loc = 0.0f;
					}
					else
					{
						velocity.x = BASIC_Ranged_SPEED + ((BASIC_Ranged_SPEED / 100) * (Ingame->GetSunLevel()));
						now_state = State::Move;
						target_loc = 0.0f;
					}
				}
			}
		}
	}
}

// �ړ�����
void P_Ranged::Movement(float delta_second)
{
	//�e�N���X�̈ړ�����
	__super::Movement(delta_second);
}

// �A�j���[�V�������䏈��
void P_Ranged::AnimationControl(float delta_second)
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
			anim_max_count = 4;
			break;
		case State::Move:
			anim_max_count = 7;
			break;
		case State::Attack:
			anim_max_count = 10;
			if (Anim_count == anim_max_count)
			{
				attack_flag = true;
				now_state = State::Idle;
			}
			break;
		case State::Death:
			anim_max_count = 5;
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
		velocity.x = BASIC_Ranged_SPEED + ((BASIC_Ranged_SPEED / 100) * (Ingame->GetSunLevel()));
		image = animation[Anim_count + 23];
		break;
	case State::Attack:
		image = animation[Anim_count + 12];
		if (Anim_count == anim_max_count - 2)
		{
			attack_flag = true;
			now_state = State::Idle;
		}
		break;
	case State::Death:
		image = animation[Anim_count + 45];
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
void P_Ranged::EffectControl(float delta_second)
{
	__super::EffectControl(delta_second);
}

//SE�̐��䏈��
void P_Ranged::SoundControl()
{
	if (now_state != old_state)
	{
		ResourceManager* rm = ResourceManager::GetInstance();
		switch (now_state)
		{

		case State::Attack:
			sounds = rm->GetSounds("Resource/Sounds/UnitSE/Ranged/Ranged_Attack.mp3");
			break;
		default:
			break;
		}
		__super::SoundControl();
	}
}

// �U������
void P_Ranged::Attack(GameObject* hit_object)
{
	if (Ingame->GetNowState() == GameState::PLAYING)
	{
		PlaySoundMem(sounds, DX_PLAYTYPE_BACK);
	}
	P_Projectile* obj = object->CreateObject<P_Projectile>(this->location);
	obj->SetTargetLocation(hit_object->GetLocation());
	obj->SetInGamePoint(Ingame);
	target_loc = 0.0f;
	attack_flame = 2.0f;
}