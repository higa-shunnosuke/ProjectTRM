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
	ChangeScene(eSceneType::title);
}

//  �X�V����
void SceneManager::Update(float delta_second)
{
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
				// �������m�̓�����͈͌��m�����͂��Ȃ�
				if (i == j)
				{
					continue;
				}

				//�U���O�̑Ώۂ�HP
				int old_HP = objects_list[j]->GetHP();

				// �U������m�F����
				CheckHitBox(objects_list[i], objects_list[j]);

				// �U�����Ă����烋�[�v�𔲂���
				if (old_HP > objects_list[j]->GetHP())
				{
					break;
				}
			}
		}
	}

	// ���Ô���m�F����
	LightMapManager* light = LightMapManager::GetInstance();
	std::vector<LightDetail> light_list = light->GetLightsList();

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
			if (light_list.size() == 0)
			{
				// �������Ă��Ȃ����Ƃ�ʒm����
				objects_list[i]->OutLightRange();
			}
			else
			{
				for (int j = 0; j < light_list.size(); j++)
				{
					// ���Ô���m�F����
					if (CheckLightRange(objects_list[i], light_list[j]) != false)
					{
						break;
					}
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

	if (next_scene != nullptr && current_scene != nullptr)
	{
	next_scene->win_flg = current_scene->win_flg;
	next_scene->StageNumber = current_scene->StageNumber;
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
		Vector2D box_size = (tc.box_size + pc.box_size) / 2.0f;
		
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
void SceneManager::CheckHitBox(GameObject* target, GameObject* partner)
{
	// �k���|�`�F�b�N
	if (target == nullptr || partner == nullptr)
	{
		return;
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
		Vector2D box_size = (tc.attack_size + pc.box_size) / 2.0f;
		
		// ��`���m�̓����蔻��
		if ((fabsf(diff.x) <= box_size.x) && (fabsf(diff.y) <= box_size.y))
		{
			// �������Ă��邱�Ƃ�ʒm����
			target->OnAreaDetection(partner);
		}
		else
		{
			// �������Ă��Ȃ����Ƃ�ʒm����
			target->NoHit();
		}
	}
}

// ���Ì��m����
bool SceneManager::CheckLightRange(GameObject* target, LightDetail partner)
{
	// �k���|�`�F�b�N
	if (partner.object == nullptr || target == nullptr)
	{
		return 1;
	}

	// �����蔻������擾
	Collision tc = target->GetCollision();
	Collision pc = partner.object->GetCollision();

	// ���肪�L�����m�F����
	if (tc.IsCheckHitTarget(pc.object_type) || pc.IsCheckHitTarget(tc.object_type))
	{
		Vector2D circlePos = partner.object->GetLocation();
		float radius = partner.size;
		Vector2D boxPos = target->GetLocation();
		Vector2D boxSize = pc.box_size;
		
		if (tc.CheckCircleRectCollision(circlePos,radius,boxPos,boxSize))
		{
			// �������Ă��邱�Ƃ�ʒm����
			target->InLightRange();
			return true;
		}
		else
		{
			// �������Ă��Ȃ����Ƃ�ʒm����
			target->OutLightRange();
			return false;
		}
	}
}
