#include "E_Melee.h"
#include "../../../GameObjectManager.h"

// �G�ߐڂ̃J�E���^��������
size_t E_Melee::count = 0;

// �G�ߐڂ̐��擾����
size_t E_Melee::GetCount()
{
	return count;
}

// �R���X�g���N�^
E_Melee::E_Melee()
{
	count++;
}

// �f�X�g���N�^
E_Melee::~E_Melee()
{
	count--;
}

// ����������
void E_Melee::Initialize()
{
	// �e�N���X�̏�����
	__super::Initialize();
	
	// �t���O�ݒ�
	is_mobility = true;
	is_aggressive = true;

	// �R���W�����ݒ�
	collision.is_blocking = true;
	collision.object_type = eObjectType::Enemy;
	collision.hit_object_type.push_back(eObjectType::Player);
	collision.collision_size = Vector2D(50.0f, 100.0f);
	collision.hitbox_size = Vector2D(90.0f, 120.0f);
	z_layer = 2;

	// �摜���]�Ȃ�
	flip_flag = false;

	// �ŏ��̏�Ԃ��ړ��ɂ���
	now_state = State::Move;

	//�U����
	Damage = 4;

	// HP������
	HP = 20;

	// �X�s�[�h�̏�����
	speed = 80.0f;
}

// �X�V����
void E_Melee::Update(float delta_second)
{
	// �e�N���X�̍X�V
	__super::Update(delta_second);
}

// �`�揈��
void E_Melee::Draw(const Vector2D camera_pos) const
{
	// �摜�̂���
	Vector2D offset;
	offset.x = 30.0f;
	offset.y = -25.0f;

	// �J�������W�����Ƃɕ`��ʒu���v�Z
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// �G�ߐڂ̕`��
	DrawRotaGraphF(position.x + offset.x, position.y + offset.y,
		2.0, 0.0, image, TRUE, flip_flag);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawRotaGraphF(position.x, position.y - 70.0f,
		3.0, 0.0, effect, TRUE, flip_flag);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// �e�N���X�̕`��
	__super::Draw(camera_pos);
}

// �I��������
void E_Melee::Finalize()
{
	// �e�N���X�̏I��
	__super::Finalize();
}

// �����蔻��ʒm����
void E_Melee::OnHitCollision(GameObject* hit_object)
{

}

// �U���͈͒ʒm����
void E_Melee::OnAreaDetection(GameObject* hit_object)
{
	// �e�N���X�̍U���͈͒ʒm����
	__super::OnAreaDetection(hit_object);
}

// �U���͈͒ʒm����
void E_Melee::NoHit()
{
	
}

// ���C�g�͈͒ʒm����
void E_Melee::InLightRange()
{
	in_light = true;
	Damage = 2;
	speed = 40.0f;
}

// ���C�g�͈͒ʒm����
void E_Melee::OutLightRange()
{
	in_light = false;
	Damage = 4;
	speed = 80.0f;
}

// �ړ�����
void E_Melee::Movement(float delta_second)
{
	// �e�N���X�̈ړ�����
	__super::Movement(delta_second);
}

// �A�j���[�V�������䏈��
void E_Melee::AnimationControl(float delta_second)
{
	// ��Ԃ��؂�ւ������J�E���g��������
	if (old_state != now_state)
	{
		Anim_count = 0;

		// �摜�̓ǂݍ���
		ResourceManager* rm = ResourceManager::GetInstance();

		// �e��Ԃ̃A�j���[�V�����摜�ɍ����ւ�
		switch (now_state)
		{
		case State::Idle:
			animation = rm->GetImages("Resource/Images/Enemy/Melee/E_Melee_Idle.png", 4, 4, 1, 100, 75);
			image = animation[Anim_count];
			anim_max_count = 3;
			anim_rate = 0.3f;
			break;
		case State::Move:
			animation = rm->GetImages("Resource/Images/Enemy/Melee/E_Melee_Walk.png", 6, 6, 1, 100, 75);
			image = animation[Anim_count];
			anim_max_count = 5;
			anim_rate = 0.1f;
			break;
		case State::Attack:
			animation = rm->GetImages("Resource/Images/Enemy/Melee/E_Melee_Attack.png", 8, 8, 1, 100, 75);
			image = animation[Anim_count];
			anim_max_count = 7;
			anim_rate = 0.1f;
			break;
		case State::Damage:
			break;
		case State::Death:
			animation = rm->GetImages("Resource/Images/Enemy/Melee/E_Melee_Dead.png", 4, 4, 1, 100, 75);
			image = animation[Anim_count];
			anim_max_count = 3;
			anim_rate = 0.2f;
			break;
		}
	}

	// �e�N���X�̃A�j���[�V����]
	__super::AnimationControl(delta_second);
}
// �G�t�F�N�g���䏈��
void E_Melee::EffectControl(float delta_second)
{
	// �e�N���X�̃G�t�F�N�g
	__super::EffectControl(delta_second);
}
