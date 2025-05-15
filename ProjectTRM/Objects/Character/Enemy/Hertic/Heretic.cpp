#include "Heretic.h"

#include"../../../../Scenes/InGame/InGame.h"

#include "../../Player/Tank/P_Tank.h"
#include "../../Player/Melee/P_Melee.h"
#include "../../Player/Ranged/P_Ranged.h"

#include "../Tank/E_Tank.h"
#include "../Ranged/E_Ranged.h"
#include "../Melee/E_Melee.h"
#include "../Boss/Boss.h"

#define COST_CHARGE 60.0f
#define COST_UPNUM 10


#define TEST

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
	Cost(30),
	summon_effect()
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

	EffectImage = rm->GetImages("Resource/Images/Effect/EnemyPawn.png", 13, 13, 1, 64, 64);
	image = rm->GetImages("Resource/Images/Enemy/Heretic/Heretic_Stand.png")[0];

	if (image == NULL)
	{
		printf("WTF");
	}
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
#ifdef DEBUG
	if (CheckHitKey(KEY_INPUT_5))
	{
		HP--;
	}
#endif // DEBUG

	summon_flag = false;

#if 1

#else
	CountFlame += 1.0f;
	if (CountFlame > COST_CHARGE)
	{
		CountTime++;
		if (CountTime == 0)
		{
			nowsta = State::Idle;
		}
		Cost+= COST_UPNUM;
		CountFlame = 0.0f;
	}
#endif // 1


#ifdef TEST

	if (CheckHitKey(KEY_INPUT_1))
	{
		Ingame->CreateEnemy(E_enemy::Tank);
		summon_flag = true;
	}
	else if (CheckHitKey(KEY_INPUT_2))
	{
		Ingame->CreateEnemy(E_enemy::Melee);
		summon_flag = true;
	}	
	else if (CheckHitKey(KEY_INPUT_3))
	{
		Ingame->CreateEnemy(E_enemy::Range);
		summon_flag = true;
	}
	else if (CheckHitKey(KEY_INPUT_4))
	{
		Ingame->CreateEnemy(E_enemy::Boss);
		summon_flag = true;
	}
#else
	if (Cost >= 0)
	{
	ThinkingEnemy();
	}
#endif

	if (summon_flag)
	{
		summon_effect = true;
	}

	auto now_time = std::chrono::steady_clock::now();

	if (now_time - prev_time > std::chrono::milliseconds(500))
	{
		Cost += 1;
		prev_time = std::chrono::steady_clock::now();

	}

	if (summon_effect)
	{
		if (now_time - efect_time > std::chrono::milliseconds(100))
		{
			Anim_count++;
			efect_time = std::chrono::steady_clock::now();

			if (Anim_count >= EffectImage.size())
			{
				Anim_count = 0;
				summon_effect = false;
			}
		}

	}
}

// �`�揈��
void Heretic::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

#ifdef DEBUG
	//�w�i�Ō����Ȃ��c�������摜�������邩������
	DrawBox((int)(position.x - collision.box_size.x / 2), (int)(position.y - collision.box_size.y / 2),
		(int)(position.x + collision.box_size.x / 2), (int)(position.y + collision.box_size.y / 2), 0xffffff, TRUE);
	DrawFormatString(0, 70, 0xFFFFFF, "5:Enemy Damage");
#endif

#ifdef TEST	
	DrawFormatString(0, 100, 0xFFFFFF,  "1:Tank");
	DrawFormatString(0, 130, 0xFFFFFF,  "2:Melee");
	DrawFormatString(0, 160, 0xFFFFFF, "3:Range");
	DrawFormatString(0, 10, 0xFFFFFF, "4:Boss");

#endif // TEST


	if (nowsta == State::Damage)
	{
		if (CountTime == -2)
		{
			position.x -= 5.0f;
		}
		else if (CountTime == -1)
		{
			position.x += 5.0f;
		}
	}

	DrawGraphF(position.x - collision.box_size.x / 2 - 10.0f, position.y - collision.box_size.y / 2, image, true);

	// �ْ[�҂̕`��
	//DrawBox((int)(position.x - collision.box_size.x / 2), (int)(position.y - collision.box_size.y / 2),
	//	(int)(position.x + collision.box_size.x / 2), (int)(position.y + collision.box_size.y / 2), 0x0000ff, TRUE);


	DrawBoxAA(position.x - 50.0f, position.y - 90.0f, position.x + (50.0f - (100 - HP)), position.y - 75.0f, 0xFFFFFF, true);

	if (summon_effect)
	{
		DrawGraphF(position.x, position.y, EffectImage[Anim_count], true);
	}




#ifdef DEBUG
	DrawFormatString(0, 50, 0xFFFFFF, "EC:%d", Cost);
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
	EffectImage.clear();
}

void Heretic::SetInGamePoint(InGame* point)
{
	Ingame = point;
}

void Heretic::ThinkingEnemy()
{
	int Pcount_sum = 0;
	int Ecount_sum = 0;

	int Ptank_count = (int)P_Tank::GetCount() * TANK_eva;
	int Pmelee_count = (int)P_Melee::GetCount() * MELEE_eva;
	int Prange_count = (int)P_Ranged::GetCount() * RANGE_eva;
	Pcount_sum += (Ptank_count + Pmelee_count + Prange_count);

	int Etank_count = (int)E_Tank::GetCount() * TANK_eva;
	int Emelee_count = (int)E_Melee::GetCount() * MELEE_eva;
	int Erange_count = (int)E_Ranged::GetCount() * RANGE_eva;
	Ecount_sum += (Etank_count + Emelee_count + Erange_count);


	//�X�^���_�[�h�^
	{
		//�E��������ł�
		//�^���N��5�̂��L�[�v
		//�O�q��3��
		//��L�𖞂����Č�q

		//�E�R�X�g���ő�ɂȂ�Ȃ�莝���ő�R�X�g�𐶐�����ŁI
		if (Cost >= 500)
		{
			//�E��������ł�
			Cost -= RANGE_cost;
			Ingame->CreateEnemy(E_enemy::Range);
			summon_flag = true;
		}

		//����I�����������I�I�����Ȃ�����c�I�I
		if (Pcount_sum > Ecount_sum + 80)
		{
			//�y���z
			//���ꂪ���C�́c�؂�D��I�I�I�I
			Cost -= BOSS_cost;
			Ingame->CreateEnemy(E_enemy::Boss);
			summon_flag = true;
		}
		//����̕�����͕]�������Ȃ��c����I
		else if (Pcount_sum > Ecount_sum)
		{
			//��������ŁI�I
			////�^���N�͈�萔���L�[�v
			////�O�q�����Ȃ��Əo��
			////�O�q�������ƌ�q���o��
			//��������I
			if ((Etank_count / TANK_eva) < 4)
			{
				Cost -= TANK_cost;
				Ingame->CreateEnemy(E_enemy::Tank);
				summon_flag = true;
			}
			//�^���N���肵����낵�A�ߐڐ��Y����
			else if (((Emelee_count / MELEE_eva) < (Erange_count / RANGE_eva) + 3))
			{
				Cost -= MELEE_cost;
				Ingame->CreateEnemy(E_enemy::Melee);
				summon_flag = true;
			}
			//�������������قǍK����˂�c�ł��A�^���N�Ƌߐڂ̕����D���Ȃ��c�Y�܂���
			//���C�܂���ŋ}�ɐ��Y�ł���悤�ɂ��Ă�����
			else
			{
				Cost -= RANGE_cost;
				Ingame->CreateEnemy(E_enemy::Range);
				summon_flag = true;
			}
		}
		//���A���Ă������(�@�z�W�z�W)
		else if ((Pcount_sum < Ecount_sum))
		{
			//�ł����C�͖��f���Ȃ��D�G�Ȋ��Ȃ��I�I
			Cost -= MELEE_cost + 50;//���r�߃v
			Ingame->CreateEnemy(E_enemy::Melee);
			summon_flag = true;
		}
		//���̃��C�Ɓc�h�R���!!��肨��c�c(�J�n���_)
		else
		{
			Cost -= TANK_cost - 10;//�����������y�����Đ��Y���āA���ɂȂ���
			Ingame->CreateEnemy(E_enemy::Tank);
			summon_flag = true;
		}
	}

	//�U���I
	//{
	//	//�E�R�X�g���ő�ɂȂ�Ȃ�莝���ő�R�X�g�𐶐�����B
	//	if (Cost >= 500)
	//	{
	//		//�E��������ł�
	//		Cost -= RANGE_cost;
	//		Ingame->CreateEnemy(E_enemy::Range);
	//		summon_flag = true;
	//		//���̕ӂ̏����͊֐��ɔ�΂������ȁc
	//	}
	//	//�E����̕]���������Ȃ����ۂɎ莝���ő�R�X�g�𐶐�����B
	//	if (Pcount_sum > Ecount_sum + 100 && Cost >= MELEE_cost)
	//	{
	//		//�y�v���g�ł̂݁z
	//		//�R�X�g���}�C�i�X�ɂȂ��Ă��؋����ďo���܂��傤
	//		Cost -= BOSS_cost;
	//		Ingame->CreateEnemy(E_enemy::Boss);
	//		summon_flag = true;
	//	}
	//	//�E����̕]�����Ⴍ�Ȃ����ۂɎ莝���ŏ��R�X�g�𐶐�����B
	//	else if (Cost >= ENEMY_BOTTOM_COST)
	//	{
	//		//�E��������ł�
	//		//�^���N�͏��Ȃ�
	//		//�O�q�𑽂�
	//		//�O�q5�̂Ō�q���o��
	//		if ((Etank_count / TANK_eva) < 2)
	//		{
	//			Cost -= TANK_cost;
	//			Ingame->CreateEnemy(E_enemy::Tank);
	//			summon_flag = true;
	//		}
	//		else if (((Emelee_count / MELEE_eva) <= (Erange_count / RANGE_eva) + 5) && Cost >= MELEE_cost)
	//		{
	//			Cost -= MELEE_cost;
	//			Ingame->CreateEnemy(E_enemy::Melee);
	//			summon_flag = true;
	//		}
	//		else if (Cost >= RANGE_cost)
	//		{
	//			Cost -= RANGE_cost;
	//			Ingame->CreateEnemy(E_enemy::Range);
	//			summon_flag = true;
	//		}
	//	}
	//}
}

// �����蔻��ʒm����
void Heretic::OnHitCollision(GameObject* hit_object)
{

}

// HP�Ǘ�����
void Heretic::HPControl(int Damage)
{
	__super::HPControl(Damage);
	

	nowsta = State::Damage;

	if (CountTime > 0)
	{
	CountTime = -2;
	}

}

bool Heretic::GetDead()
{
	return JustDead;
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