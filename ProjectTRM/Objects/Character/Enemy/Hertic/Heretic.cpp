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


//#define ENEMY_TEST

#include"../../../../Utility/Input/InputManager.h"
#include "../../Player/Guardian/P_Guardian.h"


//#define Enemy_Plan_Evaluation // ���]���^
#ifdef Enemy_Plan_Evaluation
#else
#define Enemy_Plan_WAVE//�E�F�[�u�^
#endif // Evalution


// �R���X�g���N�^
Heretic::Heretic() :
	EffectCount(), 
	anime_time(),
	summon_flag(),
	CountFlame(0.0f),
	CountTime(0),
	Cost(0),
	summon_effect(),
	Player_evaluation(),
	Ingame(),
	Enemy_evaluation()
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

	animation = rm->GetImages("Resource/Images/Enemy/Heretic/Idle.png", 8, 8, 1, 250, 250);
	DeadImage = rm->GetImages("Resource/Images/Enemy/Heretic/Death.png", 7, 7, 1, 250, 250);
	AppearImage = rm->GetImages("Resource/Images/Enemy/Heretic/Attack2.png", 8, 8, 1, 250, 250);
	EffectImage = rm->GetImages("Resource/Images/Effect/Gravity-Sheet.png", 20, 4, 5, 96, 80);
	image = animation[0];
	SoptLight = rm->GetImages("Resource/Images/Enemy/Heretic/SpotLight.png")[0];

	is_mobility = false;
	
	collision.is_blocking = true;
	collision.object_type = eObjectType::Enemy;
	collision.hit_object_type.push_back(eObjectType::Player);
	collision.collision_size = Vector2D(60.0f, 120.0f);
	collision.hitbox_size = Vector2D(400.0f, 200.0f);
	z_layer = 1;

	nowsta = State::Summon;

	// HP������
	HP = 500.0f;
}

// �X�V����
void Heretic::Update(float delta_second)
{
	auto now_time = std::chrono::steady_clock::now();

	switch (nowsta)
	{
	case Death:
		if (now_time - prev_time > std::chrono::milliseconds(2000))
		{
			if (Iam_Dead == false)
			{
				Iam_Dead = true;
			}
			else
			{
				JustDead = true;
			}
			prev_time = std::chrono::steady_clock::now();
		}

		if (now_time - anime_time > std::chrono::milliseconds(250))
		{
			Anim_count++;
			anime_time = std::chrono::steady_clock::now();

			if (Anim_count >= 6)
			{
				Anim_count = 6;
			}
		}
		// ���S�A�j���[�V����
		image = DeadImage[Anim_count];
		break;

	case Idle:

		if (ProjectConfig::DEBUG)
		{
			if (CheckHitKey(KEY_INPUT_5))
			{
				HPControl(1);
			}
		}

		summon_flag = false;

#ifdef ENEMY_TEST

		InputManager* input = InputManager::GetInstance();

		if (input->GetKeyState(KEY_INPUT_1) == eInputState::Pressed)
		{
			Ingame->CreateEnemy(E_enemy::Tank);
			summon_flag = true;
		}
		else if (input->GetKeyState(KEY_INPUT_2) == eInputState::Pressed)
		{
			Ingame->CreateEnemy(E_enemy::Melee);
			summon_flag = true;
		}
		else if (input->GetKeyState(KEY_INPUT_3) == eInputState::Pressed)
		{
			Ingame->CreateEnemy(E_enemy::Range);
			summon_flag = true;
		}
		else if (input->GetKeyState(KEY_INPUT_4) == eInputState::Pressed)
		{
			Ingame->CreateEnemy(E_enemy::Boss);
			summon_flag = true;
		}
#else

		if (Cost >= 0)
		{
			switch (Ingame->StageNumber)
			{
			case 1:
				FirstStageEnemy();
				break;
			default:
				ThinkingEnemy();
				break;
			}
		}

#endif
		if (summon_flag)
		{
			summon_effect = true;
		}

		now_time = std::chrono::steady_clock::now();

		if (now_time - prev_time > std::chrono::milliseconds(500))
		{
			Cost += 1;
			prev_time = std::chrono::steady_clock::now();
		}

		if (summon_effect)
		{
			if (now_time - efect_time > std::chrono::milliseconds(50))
			{
				EffectCount++;
				efect_time = std::chrono::steady_clock::now();

				if (EffectCount >= 20)
				{
					EffectCount = 0;
					summon_effect = false;
				}
			}

		}

		if (now_time - anime_time > std::chrono::milliseconds(100))
		{
			Anim_count++;
			anime_time = std::chrono::steady_clock::now();

			if (Anim_count >= 8)
			{
				Anim_count = 0;
			}
		}

		// �ҋ@�A�j���[�V����
		image = animation[Anim_count];
		break;

	default:
		 now_time = std::chrono::steady_clock::now();

		 if (now_time - anime_time > std::chrono::milliseconds(200))
		 {
			 Anim_count++;
			 anime_time = std::chrono::steady_clock::now();

			 if (Anim_count >= 8)
			 {
				 Anim_count = 0;
			 }
		 }

		 // �ҋ@�A�j���[�V����
		 image = AppearImage[Anim_count];

		 if (Ingame->GetNowState() != GameState::GAMESTART)
		 {
			 nowsta = State::Idle;
		 }

		break;
	}
}

// �`�揈��
void Heretic::Draw() const
{
	switch (nowsta)
	{
	case Death:
		if (Iam_Dead)
		{
			int w = 1;
			int h = 1;

			DrawGraphF(location.x - collision.collision_size.x / 2-50.0f, location.y - collision.collision_size.y / 2 - 50.0f, SoptLight, true);

			//// �L�����摜�𒆐S�ɕ`��		
			DrawRotaGraphF(location.x - 0.0f, location.y - 20.0f, 2.0, 0.0, image, TRUE, flip_flag);

		}
		else
		{

			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
			DrawGraphF(location.x - collision.collision_size.x / 2 - 50.0f, location.y - collision.collision_size.y / 2 - 50.0f, SoptLight, true);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
			// �L�����摜�𒆐S�ɕ`��
			DrawRotaGraphF(location.x - 0.0f, location.y - 20.0f, 2.0, 0.0, image, TRUE, flip_flag);

		}
		break;

	default:

		if (ProjectConfig::DEBUG)
		{	//�w�i�Ō����Ȃ��c�������摜�������邩������
			DrawBox((int)(location.x - collision.collision_size.x / 2), (int)(location.y - collision.collision_size.y / 2),
				(int)(location.x + collision.collision_size.x / 2), (int)(location.y + collision.collision_size.y / 2), 0xffffff, TRUE);
			DrawFormatString(0, 70, 0xFFFFFF, "5:Enemy Damage");
		}

#ifdef ENEMY_TEST	
		DrawFormatString(10, 600, 0xFFFFFF, "1:Tank");
		DrawFormatString(10, 630, 0xFFFFFF, "2:Melee");
		DrawFormatString(10, 660, 0xFFFFFF, "3:Range");
		DrawFormatString(10, 690, 0xFFFFFF, "4:Boss");
#endif // ENEMY_TEST

		DrawRotaGraphF(location.x - 0.0f, location.y - 20.0f,
			2.0, 0.0, image, TRUE, flip_flag);

		// �ْ[�҂̕`��
		//DrawBox((int)(position.x - collision.box_size.x / 2), (int)(position.y - collision.box_size.y / 2),
		//	(int)(position.x + collision.box_size.x / 2), (int)(position.y + collision.box_size.y / 2), 0x0000ff, TRUE);

		DrawBoxAA(location.x - 50.0f, location.y - 150.0f, location.x + (50.0f - (100 - ((double)HP / 500) * 100)), location.y - 135.0f, 0xFFFFFF, true);

		if (summon_effect)
		{
			DrawGraphF(location.x + 10.0f, location.y - 30.0f, EffectImage[EffectCount], true);
		}

		if (ProjectConfig::DEBUG)
		{
			DrawFormatString(500, 20, 0xFFFFFF, "EC:%d", Cost);
			DrawFormatString(600, 20, 0xFFFFFF, "time:%d", RushTimer);
			// ���S��\��
			DrawCircle((int)location.x, (int)location.y, 2, 0xff0000, TRUE);
			// �����蔻��\��
			DrawBox((int)(location.x - collision.collision_size.x / 2), (int)(location.y - collision.collision_size.y / 2),
				(int)(location.x + collision.collision_size.x / 2), (int)(location.y + collision.collision_size.y / 2), 0xff0000, FALSE);
			// �U���͈͂�\��
			DrawBox((int)location.x, (int)(location.y - collision.hitbox_size.y / 2),
				(int)(location.x + collision.hitbox_size.x), (int)(location.y + collision.hitbox_size.y / 2), 0xff0000, FALSE);
		}
		break;
	}
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
	int Pcount_num = 0;
	int Ecount_sum = 0;
	int Ecount_num = 0;

	int Pguard_count = (int)P_Guardian::GetCount() * Guardian_eva;
	int Ptank_count = (int)P_Tank::GetCount() * TANK_eva;
	int Pmelee_count = (int)P_Melee::GetCount() * MELEE_eva;
	int Prange_count = (int)P_Ranged::GetCount() * RANGE_eva;
	Pcount_sum += (Ptank_count + Pmelee_count + Prange_count + Pguard_count);
	Pcount_num += (Ptank_count / TANK_eva + Pmelee_count / MELEE_eva + Prange_count / RANGE_eva + Pguard_count / Guardian_eva);

	int Etank_count = (int)E_Tank::GetCount() * TANK_eva;
	int Emelee_count = (int)E_Melee::GetCount() * MELEE_eva;
	int Erange_count = (int)E_Ranged::GetCount() * RANGE_eva;
	int Eboss_count = (int)Boss::GetCount() * BOSS_eva;
	Ecount_sum += (Etank_count + Emelee_count + Erange_count + Eboss_count);
	Ecount_num += (Etank_count / TANK_eva + Emelee_count / MELEE_eva + Erange_count / RANGE_eva + Eboss_count / BOSS_eva);

#ifdef Enemy_Plan_Evaluation

	if (old_EnemySum > Ecount_sum)
	{
		Cost += 100;
	}
	old_EnemySum = Ecount_sum;


	switch (Ingame->StageNumber)
	{
	case 2:
		//�X�^���_�[�h�^
	{
		//�E��������ł�
		//�^���N��5�̂��L�[�v
		//�O�q��3��
		//��L�𖞂����Č�q

		//����I�����������I�I�����Ȃ�����c�I�I
		if (Pcount_sum > Ecount_sum && HP != 500)
		{
			//�y���z
			//���ꂪ���C�́c�؂�D��I�I�I�I
			Cost -= BOSS_cost;
			SamonEnemy(E_enemy::Boss);
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
				SamonEnemy(E_enemy::Tank);
				summon_flag = true;
			}
			//�^���N���肵����낵�A�ߐڐ��Y����
			else if (((Emelee_count / MELEE_eva) < (Erange_count / RANGE_eva) + 3))
			{
				Cost -= MELEE_cost;
				SamonEnemy(E_enemy::Melee);
				summon_flag = true;
			}
			//�������������قǍK����˂�c�ł��A�^���N�Ƌߐڂ̕����D���Ȃ��c�Y�܂���
			//���C�܂���ŋ}�ɐ��Y�ł���悤�ɂ��Ă�����
			else
			{
				Cost -= RANGE_cost;
				SamonEnemy(E_enemy::Range);
				summon_flag = true;
			}
		}
		//���A���Ă������(�@�z�W�z�W)
		else if ((Pcount_sum < Ecount_sum))
		{
			//�ł����C�͖��f���Ȃ��D�G�Ȋ��Ȃ��I�I
			Cost -= (MELEE_cost);
			SamonEnemy(E_enemy::Melee);
			summon_flag = true;
		}
		//���̃��C�Ɓc�h�R���!!��肨��c�c(�J�n���_)
		else
		{
			Cost -= (TANK_cost - 5);//�����������y�����Đ��Y���āA���ɂȂ���
			SamonEnemy(E_enemy::Tank);
			summon_flag = true;
		}
	}
	break;
	case 3:
		//���b�V��(�g�̂悤�ɏP������)
	{
		//�E����̕]���������Ȃ����ۂɎ莝���ő�R�X�g�𐶐�����B
		if (Pcount_sum > Ecount_sum + 1000)
		{
			//�y�v���g�ł̂݁z
			Cost -= BOSS_cost;
			SamonEnemy(E_enemy::Boss);
			summon_flag = true;
		}
		//�E����̕]�����Ⴍ�Ȃ����ۂɎ莝���ŏ��R�X�g�𐶐�����B
		else
		{
			//�E��������ł�
			//�^���N�͏��Ȃ�
			//�O�q�𑽂�
			//�O�q5�̂Ō�q���o��
			if ((Etank_count / TANK_eva) <= 1)
			{
				Cost -= (TANK_cost - 5);
				SamonEnemy(E_enemy::Tank);
				summon_flag = true;
			}
			else if ((Emelee_count / MELEE_eva) <= 1)
			{
				Cost -= (MELEE_cost - 10);
				SamonEnemy(E_enemy::Melee);
				summon_flag = true;
			}
			else
			{
				Cost -= (RANGE_cost - 30);
				SamonEnemy(E_enemy::Range);
				summon_flag = true;
			}
		}
	}
	break;
	default:
		break;
	}
#else

	auto now_time = std::chrono::steady_clock::now();


	if (Time_rush)
	{
		RushTimer = 5000 - Ecount_num;
	}
	else
	{
		RushTimer = 3000 + Ecount_num;
	}

	if (now_time - rush_time > std::chrono::milliseconds(RushTimer))
	{
		if (Time_rush)
		{
			Time_rush = false;
		}
		else
		{
			Time_rush = true;
		}
		rush_time = std::chrono::steady_clock::now();
	}

	switch (Ingame->StageNumber)
	{
	case 2:
	{

		if (Time_rush != true)
		{
			if (Pcount_num > 20 && Eboss_count < 2)
			{
				//�y���z
				//���ꂪ���C�́c�؂�D��I�I�I�I
				Cost -= BOSS_cost;
				SamonEnemy(E_enemy::Boss);
				summon_flag = true;
			}
			else
			{
				//�O�q5�̂Ō�q���o��
				if ((Etank_count / TANK_eva) <= 1)
				{
					Cost -= (TANK_cost - 5);
					SamonEnemy(E_enemy::Tank);
					summon_flag = true;
				}
				else if ((Emelee_count / MELEE_eva) <= 1)
				{
					Cost -= (MELEE_cost - 10);
					SamonEnemy(E_enemy::Melee);
					summon_flag = true;
				}
				else
				{
					Cost -= (RANGE_cost - 30);
					SamonEnemy(E_enemy::Range);
					summon_flag = true;
				}
			}
		}
		else
		{
			if (Pcount_num > 20 && Eboss_count < 2)
			{
				//�y���z
				//���ꂪ���C�́c�؂�D��I�I�I�I
				Cost -= Ecount_num;
				SamonEnemy(E_enemy::Boss);
				summon_flag = true;
			}

			//�O�q5�̂Ō�q���o��
			if ((Etank_count / TANK_eva) <= 5)
			{
				Cost -= Ecount_num;
				SamonEnemy(E_enemy::Tank);
				summon_flag = true;
			}
			else if ((Emelee_count / MELEE_eva) <= 3)
			{
				Cost -= Ecount_num;
				SamonEnemy(E_enemy::Melee);
				summon_flag = true;
			}
			else
			{
				Cost -= Ecount_num;
				SamonEnemy(E_enemy::Range);
				summon_flag = true;
			}
		}

	}
	break;
	case 3:
	{
		if (Time_rush != true)
		{
			if (Pcount_num > 20 && Eboss_count/ BOSS_eva < 3)
			{
				//�y���z
				//���ꂪ���C�́c�؂�D��I�I�I�I
				Cost -= BOSS_cost - 100;
				SamonEnemy(E_enemy::Boss);
				summon_flag = true;
			}
			else
			{
				//�O�q5�̂Ō�q���o��
				if ((Etank_count / TANK_eva) <= 1)
				{
					Cost -= (TANK_cost - 5);
					SamonEnemy(E_enemy::Tank);
					summon_flag = true;
				}
				else if ((Emelee_count / MELEE_eva) <= 1)
				{
					Cost -= (MELEE_cost - 10);
					SamonEnemy(E_enemy::Melee);
					summon_flag = true;
				}
				else
				{
					Cost -= (RANGE_cost - 25);
					SamonEnemy(E_enemy::Range);
					summon_flag = true;
				}
			}
		}
		else
		{
			if (Pcount_num > 20 && Eboss_count / BOSS_eva < 2)
			{
				//�y���z
				//���ꂪ���C�́c�؂�D��I�I�I�I
				Cost -= Ecount_num/10;
				SamonEnemy(E_enemy::Boss);

				summon_flag = true;
			}
			//�O�q5�̂Ō�q���o��
			else if ((Etank_count / TANK_eva) <= 5)
			{
				Cost -= Ecount_num/10;
				SamonEnemy(E_enemy::Tank);
				summon_flag = true;
			}
			else if ((Emelee_count / MELEE_eva) <= 1)
			{
				Cost -= Ecount_num/10;
				SamonEnemy(E_enemy::Melee);
				summon_flag = true;
			}
			else
			{
				Cost -= Ecount_num /10;
				SamonEnemy(E_enemy::Range);
				summon_flag = true;
			}
		}
	}
	break;

#endif // Enemy_Plan_Evaluation
	}
}

void Heretic::FirstStageEnemy()
{
	//��R�̋ߐڂ��o���˂�
	Cost -= MELEE_cost;
	SamonEnemy(E_enemy::Melee);
	summon_flag = true;
}

// �����蔻��ʒm����
void Heretic::OnHitCollision(GameObject* hit_object)
{

}

// HP�Ǘ�����
void Heretic::HPControl(float Damage)
{
	__super::HPControl(Damage);

	if (this->HP <= 0)
	{
		if (nowsta != Death)
		{
			Anim_count = 0;
		}

		nowsta = State::Death;
		image = DeadImage[0];
	}
	//else
	//{
	//nowsta = State::Damage;
	//}

}

bool Heretic::GetDead()
{
	return JustDead;
}

void Heretic::SamonEnemy(int e_enem)
{
	// �I�u�W�F�N�g�}�l�[�W���[�̏����擾
	GameObjectManager* object = GameObjectManager::GetInstance();

	switch (e_enem)
	{
	case Tank:
		object->CreateObject<E_Tank>(Vector2D(GetLocation().x + 50.0f, GetLocation().y + 30.0f));
		break;
	case Melee:
		object->CreateObject<E_Melee>(Vector2D(GetLocation().x + 50.0f, GetLocation().y + 30.0f));
		break;
	case Range:
		object->CreateObject<E_Ranged>(Vector2D(GetLocation().x + 50.0f, GetLocation().y + 30.0f));
		break;
	case Boss:
		object->CreateObject<class Boss>(Vector2D(GetLocation().x + 50.0f, GetLocation().y + 0.0f));
		break;
	default:
		break;
	}
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