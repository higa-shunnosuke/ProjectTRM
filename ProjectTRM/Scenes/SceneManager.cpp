#include "SceneManager.h"
#include "SceneFactory.h"
#include "../Utility/LightMapManager.h"
#include <cmath>

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
	ChangeScene(eSceneType::title);

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
	if (input->GetButtonState(XINPUT_BUTTON_START) == eInputState::Pressed ||
		input->GetKeyState(KEY_INPUT_TAB) == eInputState::Pressed)
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

	bool is_hit;		// �U���t���O

	// �U������m�F����
	for (int i = 0; i < objects_list.size(); i++)
	{
		// �U�������Ȃ��I�u�W�F�N�g�̏ꍇ�U������m�F�����͂��Ȃ�
		if (objects_list[i]->GetAggressive() == false)
		{
			continue;
		}
		else
		{
			for (int j = 0; j < objects_list.size(); j++)
			{
				// �U������m�F����
				if (CheckHitBox(objects_list[i], objects_list[j]) == true)
				{
					is_hit = true;
				}
			}

			// �U���Ώۂ����Ȃ����Ƃ�ʒm����
			if (is_hit != true)
			{
				objects_list[i]->NoHit();
			}
		}
	}

	// ���Ô���m�F����
	LightMapManager* light = LightMapManager::GetInstance();
	std::vector<GameObject*> light_list = light->GetLightsList();

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
			// �ł��߂����C�g�Ƃ̔�����m�F
			else
			{
				// �ŒZ����
				float shortest_distance;
				int count = 0;

				for (int j = 0; j < light_list.size(); j++)
				{
					// �Q�_�Ԃ̋���
					float distance;

					float dx = objects_list[i]->GetLocation().x - light_list[j]->GetLocation().x;
					float dy = objects_list[i]->GetLocation().y - light_list[j]->GetLocation().y;
					distance = std::sqrt(dx * dx + dy * dy);

					// �ŒZ�����̏�����
					if (j == 0)
					{
						shortest_distance = distance;
					}

					// �ŒZ�������X�V
					if (shortest_distance > distance)
					{
						shortest_distance = distance;
						count = j;
					}
				}
				// ���Ô���m�F����
				CheckLightRange(objects_list[i], light_list[count]);
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

	if (next_scene != nullptr && current_scene != nullptr)
	{
	next_scene->win_flg = current_scene->win_flg;
	next_scene->StageNumber = current_scene->StageNumber;
	if(current_scene->max_unit != -1)
	{
		next_scene->make_unit = current_scene->make_unit;
		next_scene->dead_unit = current_scene->dead_unit;
		next_scene->max_unit = current_scene->max_unit;
		next_scene->make_enemy = current_scene->make_enemy;
		next_scene->dead_enemy = current_scene->dead_enemy;
		next_scene->max_enemy = current_scene->max_enemy;
	}
	
	}

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

	// �U�����肪�L�����m�F����
	if (tc.IsCheckHitTarget(pc.object_type) || pc.IsCheckHitTarget(tc.object_type))
	{
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
	else
	{
		return false;
	}
}

// ���Ì��m����
void SceneManager::CheckLightRange(GameObject* target, GameObject* partner)
{
	// �k���|�`�F�b�N
	if (partner == nullptr || target == nullptr)
	{
		return;
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
	}
	else
	{
		// �������Ă��Ȃ����Ƃ�ʒm����
		target->OutLightRange();
	}
}
