#include "SceneManager.h"
#include "SceneFactory.h"
#include "../Utility/LightMapManager.h"

// �R���X�g���N�^
SceneManager::SceneManager() :
	current_scene(nullptr)
{

}

// �f�X�g���N
SceneManager::~SceneManager()
{
	// ����Y��h�~
	Finalize();
}

// ����������
void SceneManager::Initialize()
{
	// �ŏ��̃V�[�����^�C�g����ʂɂ���
	ChangeScene(eSceneType::loading);

#ifdef _DEBUG
	ProjectConfig::DEBUG = true;
#endif // _DEBUG
}

//  �X�V����
void SceneManager::Update(float delta_second)
{
	// ���͏����擾
	InputManager* input = InputManager::GetInstance();
	// �f�o�b�N���[�h�̐؂�ւ�
	if (input->GetKeyState(KEY_INPUT_TAB) == eInputState::Pressed)
	{
		ProjectConfig::DEBUG = !ProjectConfig::DEBUG;
	}

	// �V�[���̍X�V
	eSceneType next_scene_type = current_scene->Update(delta_second);

	// �I�u�W�F�N�g�}�l�[�W���[�̏��擾
	GameObjectManager* object = GameObjectManager::GetInstance();
	// �I�u�W�F�N�g���X�g���擾
	std::vector<GameObject*> objects_list = object->GetObjectsList();

	// �����蔻��m�F����
	for (int i = 0; i < objects_list.size(); i++)
	{
		// �����Ȃ��I�u�W�F�N�g�̏ꍇ�����蔻��m�F���������Ȃ�
		if (objects_list[i]->GetMobility() == false)
		{
			continue;
		}
		else
		{
			for (int j = 0; j < objects_list.size(); j++)
			{
				// �������m�̓����蔻��m�F�����͂��Ȃ�
				if (i == j)
				{
					continue;
				}

				// �����蔻��m�F����
				CheckCollision(objects_list[i], objects_list[j]);
			}
		}
	}

	// �U������m�F����
	for (GameObject* target : objects_list)
	{
		// �U�������Ȃ��I�u�W�F�N�g�̏ꍇ�U������m�F�����͂��Ȃ�
		if (target->GetAggressive() == false)
		{
			continue;
		}
		else
		{
			// �͈͍U��
			if (target->GetAoE() == true)
			{
				// �U���Ώۂ����邩
				bool is_hit = false;

				for (GameObject* partner : objects_list)
				{
					if (target == partner)
					{
						continue;
					}

					// �����蔻������擾
					Collision tc = target->GetCollision();
					Collision pc = partner->GetCollision();

					// �U�����肪�L����
					if (tc.IsCheckHitTarget(pc.object_type))
					{
						// �U������m�F����
						if (CheckHitBox(target, partner) == true)
						{
							is_hit = true;
						}
					}
				}

				if (is_hit == false)
				{
					// �U���Ώۂ����Ȃ����Ƃ�ʒm����
					target->NoHit();
				}
			}
			// �P�̍U��
			else
			{
				// �ŒZ�������X�e�[�W�T�C�Y�ŏ�����
				float shortest_distance = (float)ProjectConfig::STAGE_WIDTH;
				// �U���Ώ�
				GameObject* partner = nullptr;

				// ��₩��U���Ώۂ��m�肷��
				for (GameObject* potential_partner : objects_list)
				{
					// �������m�̓����蔻��m�F�����͂��Ȃ�
					if (target == potential_partner)
					{
						continue;
					}

					// �����蔻������擾
					Collision tc = target->GetCollision();
					Collision pc = potential_partner->GetCollision();

					// �U�����肪�L����
					if (tc.IsCheckHitTarget(pc.object_type))
					{
						// ���j�b�g���m�̋������v�Z
						float distance = fabsf(target->GetLocation().x - potential_partner->GetLocation().x);

						// �Œ��������X�V
						if (distance < shortest_distance)
						{
							shortest_distance = distance;
							partner = potential_partner;
						}
					}
					else
					{
						// ���肪�L���łȂ���Ζ�������
						continue;
					}
				}

				// �U������m�F����
				if (CheckHitBox(target, partner) == false)
				{
					// �U���Ώۂ����Ȃ����Ƃ�ʒm����
					target->NoHit();
				}
			}
		}
	}

	// ���Ô���m�F����
	LightMapManager* light = LightMapManager::GetInstance();
	std::vector<GameObject*> light_list = light->GetLightsList();
	bool in_light = false;	// ���C�g�t���O

	for (int i = 0; i < objects_list.size(); i++)
	{
		// �����Ȃ��I�u�W�F�N�g�̏ꍇ�����蔻��m�F���������Ȃ�
		if (objects_list[i]->GetMobility() == false)
		{
			continue;
		}
		else
		{
			// ���C�g���X�g�̒��g���Ȃ�������
			if (light_list.empty())
			{
				// �������Ă��Ȃ����Ƃ�ʒm����
				objects_list[i]->OutLightRange();
			}
			else
			{
				for (int j = 0; j < light_list.size(); j++)
				{
					// ���Ô���m�F����
					if (CheckLightRange(objects_list[i], light_list[j]) == true)
					{
						in_light = true;
						break;
					}
					else
					{
						in_light = false;
					}
				}

				// ���C�g�ɓ������Ă��Ȃ����Ƃ�ʒm����
				if (in_light != true)
				{
					objects_list[i]->OutLightRange();
				}
			}
		}
	}
	
	//�폜�\��
	////�I�u�W�F�N�g�폜���菈��
	//for (int i = 0; i < objects_list.size(); i++)
	//{
	//	if (objects_list[i]->GetHP() <= 0)
	//	{
	//		object->DestroyObject(objects_list[i]);
	//	}
	//}

	// �t�H���g�T�C�Y�ύX
	SetFontSize(32);

	// �`�揈��
	Draw();

	// ���݂̃V�[���^�C�v�����̃V�[���^�C�v�ƈ���Ă��邩�H
	if (current_scene->GetNowSceneType() != next_scene_type)
	{
		// �V�[���؂�ւ�����
		ChangeScene(next_scene_type);
	}
}

// �I��������
void SceneManager::Finalize()
{
	// �V�[����񂪐�������Ă���΁A�폜����
	if (current_scene != nullptr)
	{
		current_scene->Finalize();
		delete current_scene;
		current_scene = nullptr;
	}
}

// �`�揈��
void SceneManager::Draw() const
{
	// ��ʂ̏�����
	ClearDrawScreen();

	// �`��͈͂̏�����
	SetDrawArea(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y);

	// �V�[���̕`�揈��
	current_scene->Draw();

	// ����ʂ̓��e��\��ʂɔ��f����
	ScreenFlip();
}

// �V�[���؂�ւ�����
void SceneManager::ChangeScene(eSceneType next_type)
{
	// ���̃V�[���𐶐�����
	SceneBase* next_scene = SceneFactory::CreateScene(next_type);

	// �G���[�`�F�b�N
	if (next_scene == nullptr)
	{
		throw ("�V�[���������ł��܂���ł���\n");
	}

	/*--------------------�ʊ֐�������Ăق���--------------------*/
	// ���U���g�ւ̒l�n��
	if (next_scene != nullptr && current_scene != nullptr)
	{
		next_scene->win_flg = current_scene->win_flg;
		next_scene->StageNumber = current_scene->StageNumber;
		if (Result* res = dynamic_cast<Result*>(next_scene))
		{
			if (current_scene->max_unit != -1)
			{
				next_scene->make_unit = current_scene->make_unit;
				next_scene->dead_unit = current_scene->dead_unit;
				next_scene->max_unit = current_scene->max_unit;
				next_scene->make_enemy = current_scene->make_enemy;
				next_scene->dead_enemy = current_scene->dead_enemy;
				next_scene->max_enemy = current_scene->max_enemy;
			}
		}
	}
	/*---------------------------�����܂�---------------------------*/

	// �V�[����񂪊i�[����Ă�����A�폜����
	if (current_scene != nullptr)
	{
		current_scene->Finalize();
		delete current_scene;
	}

	// ���̃V�[���̏�����
	next_scene->Initialize();

	// ���݃V�[���̏㏑��
	current_scene = next_scene;
}

// �����蔻��m�F����
void SceneManager::CheckCollision(GameObject* target, GameObject* partner)
{
	// �k���|�`�F�b�N
	if (target == nullptr || partner == nullptr)
	{
		return;
	}

	// �����蔻������擾
	Collision tc = target->GetCollision();
	Collision pc = partner->GetCollision();

	// �ʂ蔲�����Ȃ��I�u�W�F�N�g���m�͓����蔻��m�F�����͂��Ȃ�
	if (tc.is_blocking == false || pc.is_blocking == false)
	{
		return;
	}

	// �����蔻�肪�L�����m�F����
	if (tc.IsCheckHitTarget(pc.object_type) || pc.IsCheckHitTarget(tc.object_type))
	{
		//�Q�̃I�u�W�F�N�g�̋������擾
		Vector2D diff = target->GetLocation() - partner->GetLocation();

		//�Q�̃I�u�W�F�N�g�̓����蔻��̑傫�����擾
		Vector2D box_size = (tc.collision_size + pc.collision_size) / 2.0f;
		
		// ��`���m�̓����蔻��
		if ((fabsf(diff.x) < box_size.x) && (fabsf(diff.y) < box_size.y))
		{
			// �������Ă��邱�Ƃ�ʒm����
			target->OnHitCollision(partner);
			partner->OnHitCollision(target);
		}
	}
}

// �U������m�F����
bool SceneManager::CheckHitBox(GameObject* target, GameObject* partner)
{
	// �k���|�`�F�b�N
	if (target == nullptr || partner == nullptr)
	{
		return false;
	}

	// �����蔻������擾
	Collision tc = target->GetCollision();
	Collision pc = partner->GetCollision();

	//�Q�̃I�u�W�F�N�g�̋������擾
	Vector2D diff = target->GetLocation() - partner->GetLocation();

	//�Q�̃I�u�W�F�N�g�̓����蔻��̑傫�����擾
	Vector2D box_size;
	box_size.x = tc.hitbox_size.x;
	box_size.y = (tc.hitbox_size.y + pc.collision_size.y) / 2.0f;

	// ��`���m�̓����蔻��
	if ((fabsf(diff.x) <= box_size.x) && (fabsf(diff.y) <= box_size.y))
	{
		// �������Ă��邱�Ƃ�ʒm����
		target->OnAreaDetection(partner);
		return true;
	}
	else
	{
		// �������Ă��Ȃ�
		return false;
	}
}

// ���Ì��m����
bool SceneManager::CheckLightRange(GameObject* target, GameObject* partner)
{
	// �k���|�`�F�b�N
	if (partner == nullptr || target == nullptr)
	{
		return false;
	}

	// �����蔻������擾
	Collision tc = target->GetCollision();
	Collision pc = partner->GetCollision();

	Vector2D circlePos = partner->GetLocation();
	float radius = pc.light_size * 100;
	Vector2D boxPos = target->GetLocation();
	Vector2D boxSize = pc.collision_size;

	if (tc.CheckCircleRectCollision(circlePos, radius, boxPos))
	{
		// �������Ă��邱�Ƃ�ʒm����
		target->InLightRange();
		return true;
	}
	else
	{
		return false;
	}
}
