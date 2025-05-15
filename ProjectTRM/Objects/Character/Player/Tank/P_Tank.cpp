#include "P_Tank.h"
#include "../../../GameObjectManager.h"
#if _DEBUG
#include "../../../../Utility/Input/InputManager.h"
#endif

size_t P_Tank::count =0;
size_t P_Tank::GetCount()
{
	return count;
}

// �R���X�g���N�^
P_Tank::P_Tank() :
	Damage()
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
	// �摜�̓ǂݍ���
	ResourceManager* rm = ResourceManager::GetInstance();
	animation = rm->GetImages("Resource/Images/Unit/Tank/Tank_Walk.png", 4, 4, 1, 32, 32);
	effect_image = rm->GetImages("Resource/Images/Effect/Unit/Tank_Ghost.png", 1, 1, 1, 32, 32)[0];

	light = LightMapManager::GetInstance();
	LightDetail detail;
	detail.object = this;
	detail.size = 1.0f;
	light->AddLight(detail);

	is_mobility = true;
	is_aggressive = true;

	collision.is_blocking = true;
	collision.object_type = eObjectType::Player;
	collision.hit_object_type.push_back(eObjectType::Enemy);
	collision.box_size = Vector2D(60.0f, 60.0f);
	collision.attack_size = Vector2D(100.0f, 100.0f);
	z_layer = 1;

	//���]�t���O
	flip_flag = true;

	//�U���t���O
	attack_flag = false;

	//������
	now_state = State::Move;

	// �E�ֈړ�
	velocity.x = -3.0f;

	//�U����
	Damage = 1;

	// HP������
	HP = 45;

	lane = rand() % 3 + 1;

	alpha = MAX_ALPHA;
	effect_alpha = MAX_ALPHA;
	add = -ALPHA_ADD;
}

// �X�V����
void P_Tank::Update(float delta_second)
{
#if _DEBUG
	InputManager* input = InputManager::GetInstance();
	if (input->GetKeyState(KEY_INPUT_K) == eInputState::Pressed)
	{
		HP = 0;
	}
#endif

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
				attack_flame -= delta_second;
			}
			if (attack_flame <= 0.0f)
			{
				attack_flag = false;
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

	if (Anim_count <= 2)
	{
		AnimationControl(delta_second);
	}
	EffectControl(delta_second);



	old_state = now_state;
}

// �`�揈��
void P_Tank::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	position.y += z_layer * 8;

	position.y -= lane * 3;

	// ����̕`��
	// �I�t�Z�b�g�l����ɉ摜�̕`����s��
	if (Anim_count <= 2)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawRotaGraphF(position.x, position.y, 2.0, 0.0, image, TRUE, flip_flag);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	if (now_state == State::Attack)
	{
		DrawRotaGraphF(position.x - (collision.box_size.x / 2), position.y, 2.0, 0.0, effect_image, TRUE, flip_flag);
	}
	else if (now_state == State::Death)
	{
		position.y -= Anim_count * 10 + Anim_flame * 10;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, effect_alpha);
		DrawRotaGraphF(position.x, position.y, 2.0, 0.0, effect_image, TRUE, flip_flag);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	/*DrawBox((int)(position.x - collision.box_size.x / 2), (int)(position.y - collision.box_size.y / 2),
		(int)(position.x + collision.box_size.x / 2), (int)(position.y + collision.box_size.y / 2), 0xff00ff, TRUE);*/

#ifdef DEBUG
	// ���S��\��
	DrawCircle((int)position.x, (int)position.y, 2, 0x0000ff, TRUE);
	// �����蔻��\��
	DrawBox((int)(position.x - collision.box_size.x / 2), (int)(position.y - collision.box_size.y / 2),
		(int)(position.x + collision.box_size.x / 2), (int)(position.y + collision.box_size.y / 2), 0x0000ff, FALSE);
	// �U���͈͂�\��
	DrawBox((int)(position.x - collision.attack_size.x / 2), (int)(position.y - collision.attack_size.y / 2),
		(int)(position.x + collision.attack_size.x / 2), (int)(position.y + collision.attack_size.y / 2), 0x0000ff, FALSE);
#endif
}

// �I��������
void P_Tank::Finalize()
{
	GameObjectManager* object = GameObjectManager::GetInstance();
	object->DestroyObject(this);
}

// �����蔻��ʒm����
void P_Tank::OnHitCollision(GameObject* hit_object)
{

}

// �U���͈͒ʒm����
void P_Tank::OnAreaDetection(GameObject* hit_object)
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
		else
		{
			velocity.x = -3.0f;
		}
	}
}

// HP�Ǘ�����
void P_Tank::HPControl(int Damage)
{
	// �U����ԂłȂ���΃_���[�W��Ԃɂ���
	if (now_state != State::Attack)
	{
		now_state = State::Damage;
		dmage_flame = 1.0f;
	}

	__super::HPControl(Damage);
}

// �U������
void P_Tank::Attack(GameObject* hit_object)
{
	hit_object->HPControl(Damage);
	attack_flame = 2.0f;
}

// �ړ�����
void P_Tank::Movement(float delta_second)
{
	// �ړ��̎��s
	location.x += velocity.x * 10 * delta_second;
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
		con = 1;
		switch (now_state)
		{
		case State::Idle:
			animation = rm->GetImages("Resource/Images/Unit/Tank/Tank_Walk.png", 3, 3, 1, 32, 32);
			break;
		case State::Move:
			animation = rm->GetImages("Resource/Images/Unit/Tank/Tank_Walk.png", 3, 3, 1, 32, 32);
			break;
		case State::Attack:
			animation = rm->GetImages("Resource/Images/Unit/Tank/Tank_Attack.png", 4, 4, 1, 32, 32);
			if (Anim_count >= 2)
			{
				attack_flag = true;
				now_state = State::Idle;
			}
			break;
		case State::Damage:
			break;
		case State::Death:
			animation = rm->GetImages("Resource/Images/Unit/Tank/Tank_Down.png", 3, 3, 1, 32, 32);
			break;
		default:
			break;
		}
	}

	Anim_flame += delta_second;

	if (Anim_flame >= 0.1f)
	{
		Anim_count += con;

		if (Anim_count <= 0 || Anim_count >= 2)
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

		image = animation[1 + Anim_count];
		break;
	case State::Attack:
		image = animation[1 + Anim_count];
		if (Anim_count >= 2)
		{
			attack_flag = true;
			now_state = State::Idle;

		}
		break;
	case State::Damage:
		alpha += add;
		if (alpha <= 0 || alpha >= 255)
		{
			add = -add;
		}
		image = animation[0];
		break;
	case State::Death:
		image = animation[Anim_count];
		if (Anim_count >= 2)
		{
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
void P_Tank::EffectControl(float delta_second)
{
	ResourceManager* rm = ResourceManager::GetInstance();

	if (now_state != old_state)
	{
		switch (now_state)
		{
		case State::Death:
			effect_image = rm->GetImages("Resource/Images/Effect/Unit/Tank_Ghost.png", 1, 1, 1, 32, 32)[0];
			break;
		default:
			break;
		}
	}

	if (Anim_count > 2)
	{
		location.y -= 50.0f * delta_second;
	}

	switch (now_state)
	{
	case State::Death:
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