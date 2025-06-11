#include "Boss.h"
#include "../../../GameObjectManager.h"

// �R���X�g���N�^
Boss::Boss()
{

}

// �f�X�g���N�^
Boss::~Boss()
{

}

// ����������
void Boss::Initialize()
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
	collision.collision_size = Vector2D(180.0f, 180.0f);
	collision.hitbox_size = Vector2D(200.0f, 300.0f);
	z_layer = 2;

	// �摜���]����
	flip_flag = true;

	// �ŏ��̏�Ԃ��ړ��ɂ���
	now_state = State::Move;

	//�U����
	Damage = 6;

	// HP������
	HP = 800;

	// �X�s�[�h�̏�����
	speed = 20.0f;

	// ���J�o���[�̏�����
	recovery_time = 6.0f;
}

// �X�V����
void Boss::Update(float delta_second)
{
	// �e�N���X�̍X�V
	__super::Update(delta_second);
}

// �`�揈��
void Boss::Draw(const Vector2D camera_pos) const
{
	// �摜�̂���
	Vector2D offset;
	offset.x = 100.0f;
	offset.y = -50.0f;

	// �J�������W�����Ƃɕ`��ʒu���v�Z
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// �G�ߐڂ̕`��
	DrawRotaGraphF(position.x + offset.x, position.y + offset.y,
		3.0, 0.0, image, TRUE, flip_flag);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawRotaGraphF(position.x, position.y - 110.0f,
		5.0, 0.0, effect, TRUE, flip_flag);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// �e�N���X�̕`��
	__super::Draw(camera_pos);
}

// �I��������
void Boss::Finalize()
{
	// �e�N���X�̏I��
	__super::Finalize();
}

// �����蔻��ʒm����
void Boss::OnHitCollision(GameObject* hit_object)
{

}

// �U���͈͒ʒm����
void Boss::OnAreaDetection(GameObject* hit_object)
{
	// �e�N���X�̍U���͈͒ʒm����
	__super::OnAreaDetection(hit_object);
}

// �U���͈͒ʒm����
void Boss::NoHit()
{
	
}

// ���C�g�͈͒ʒm����
void Boss::InLightRange()
{
	in_light = true;
	Damage = 3;
	speed = 10.0f;
}

// ���C�g�͈͒ʒm����
void Boss::OutLightRange()
{
	in_light = false;
	Damage = 6;
	speed = 20.0f;
}

// �ړ�����
void Boss::Movement(float delta_second)
{
	// �e�N���X�̈ړ�����
	__super::Movement(delta_second);
}

// �A�j���[�V�������䏈��
void Boss::AnimationControl(float delta_second)
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
			animation = rm->GetImages("Resource/Images/Enemy/Boss/Boss_Idle.png", 8, 8, 1, 140, 93);
			image = animation[Anim_count];
			anim_max_count = 7;
			anim_rate = 0.2f;
			break;
		case State::Move:
			animation = rm->GetImages("Resource/Images/Enemy/Boss/Boss_Walk.png", 8, 8, 1, 140, 93);
			image = animation[Anim_count];
			anim_max_count = 7;
			anim_rate = 0.2f;
			break;
		case State::Attack:
			animation = rm->GetImages("Resource/Images/Enemy/Boss/Boss_Attack.png", 10, 10, 1, 140, 93);
			image = animation[Anim_count];
			anim_max_count = 9;
			anim_rate = 0.15f;
			break;
		case State::Damage:
			break;
		case State::Death:
			animation = rm->GetImages("Resource/Images/Enemy/Boss/Boss_Death.png", 10, 10, 1, 140, 93);
			image = animation[Anim_count];
			anim_max_count = 9;
			anim_rate = 0.3f;
			break;
		}
	}

	// �e�N���X�̃A�j���[�V����]
	__super::AnimationControl(delta_second);
}
// �G�t�F�N�g���䏈��
void Boss::EffectControl(float delta_second)
{
	// �e�N���X�̃G�t�F�N�g
	__super::EffectControl(delta_second);
}