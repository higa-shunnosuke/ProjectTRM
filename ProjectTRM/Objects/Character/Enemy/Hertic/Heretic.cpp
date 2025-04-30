#include "Heretic.h"

#include"../../../../Scenes/InGame/InGame.h"

#include "../../Player/Tank/P_Tank.h"
#include "../../Player/Melee/P_Melee.h"
#include "../../Player/Ranged/P_Ranged.h"

#include "../Tank/E_Tank.h"
#include "../Ranged/E_Ranged.h"
#include "../Melee/E_Melee.h"

#define Enemy_Plan_Evaluation // ���]���^
#ifdef Enemy_Plan_Evaluation
#else
#define Enemy_Plan_WAVE//�E�F�[�u�^
#endif // Evalution


// �R���X�g���N�^
Heretic::Heretic() :
	summon_flag(),
	CountFlame(0.0f),
	CountTime(0),
	Cost(0)
{

}

// �f�X�g���N�^
Heretic::~Heretic()
{

}

// ����������
void Heretic::Initialize()
{
	// �摜�̓ǂݍ���
	ResourceManager* rm = ResourceManager::GetInstance();
	GOM = GOM->GetInstance();

	is_mobility = false;

	collision.is_blocking = true;
	collision.object_type = eObjectType::Enemy;
	collision.hit_object_type.push_back(eObjectType::Player);
	collision.box_size = Vector2D(60.0f, 120.0f);
	collision.attack_size = Vector2D(400.0f, 200.0f);
	z_layer = 1;

	// HP������
	HP = 100;
}

// �X�V����
void Heretic::Update(float delta_second)
{
	if (Fstflag)
	{
		Ingame->CreateEnemy(E_enemy::Melee);
		Fstflag = false;
	}

	CountFlame += 1.0f;
	if (CountFlame < 60.0f)
	{
		CountTime++;
		Cost+= 10;
		CountFlame = 0.0f;
	}

	int Pcount_sum =0;
	int Ecount_sum =0;

	int Ptank_count	  = (int)P_Tank::  GetCount() * TANK_eva;
	int Pmelee_count  = (int)P_Melee:: GetCount() * MELEE_eva;
	int Prange_count  = (int)P_Ranged::GetCount() * RANGE_eva;
	Pcount_sum += (Ptank_count + Pmelee_count + Prange_count);

	int Etank_count  = (int)E_Tank::  GetCount() * TANK_eva;
	int Emelee_count = (int)E_Melee:: GetCount() * MELEE_eva;
	int Erange_count = (int)E_Ranged::GetCount() * RANGE_eva;
	Ecount_sum += (Etank_count + Emelee_count + Erange_count);

	//�E�R�X�g���O�ȉ��ɂȂ�Ȃ琶�����Ȃ��B
	if (Cost >= ENEMY_BOTTOM_COST)
	{
		//�E��������ł�
		//���̕ӂ̏����͊֐��ɔ�΂������ȁc
	}
	//�E�R�X�g���ő�ɂȂ�Ȃ�莝���ő�R�X�g�𐶐�����B
	if (Cost >= 500)
	{
		//�E��������ł�
		//���̕ӂ̏����͊֐��ɔ�΂������ȁc
	}
	//�E����̕]���������Ȃ����ۂɎ莝���ő�R�X�g�𐶐�����B
	if (Pcount_sum > Ecount_sum)
	{

	}
	//�E����̕]�����Ⴍ�Ȃ����ۂɎ莝���ŏ��R�X�g�𐶐�����B
	else
	{

	}

}

// �`�揈��
void Heretic::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// �ْ[�҂̕`��
	DrawBox((int)(position.x - collision.box_size.x / 2), (int)(position.y - collision.box_size.y / 2),
		(int)(position.x + collision.box_size.x / 2), (int)(position.y + collision.box_size.y / 2), 0x0000ff, TRUE);

#ifdef DEBUG
	// ���S��\��
	DrawCircle((int)position.x, (int)position.y, 2, 0xff0000, TRUE);
	// �����蔻��\��
	DrawBox((int)(position.x - collision.box_size.x / 2), (int)(position.y - collision.box_size.y / 2),
		(int)(position.x + collision.box_size.x / 2), (int)(position.y + collision.box_size.y / 2), 0xff0000, FALSE);
	// �U���͈͂�\��
	DrawBox((int)(position.x - collision.attack_size.x / 2), (int)(position.y - collision.attack_size.y / 2),
		(int)(position.x + collision.attack_size.x / 2), (int)(position.y + collision.attack_size.y / 2), 0xff0000, FALSE);
#endif
}

// �I��������
void Heretic::Finalize()
{

}

void Heretic::SetInGamePoint(InGame* point)
{
	Ingame = point;
}

// �����蔻��ʒm����
void Heretic::OnHitCollision(GameObject* hit_object)
{

}

// HP�Ǘ�����
void Heretic::HPControl(int Damage)
{

}

// �ړ�����
void Heretic::Movement(float delta_second)
{

}

// �A�j���[�V�������䏈��
void Heretic::AnimationControl(float delta_second)
{

}
// �G�t�F�N�g���䏈��
void Heretic::EffectControl(float delta_second)
{

}

// �U������
void Heretic::Attack()
{

}