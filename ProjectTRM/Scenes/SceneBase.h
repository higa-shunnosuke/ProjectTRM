#pragma once

#include <vector>
#include <string>
#include "DxLib.h"

#include "../Utility/ProjectConfig.h"
#include "../Utility/ResourceManager.h"
#include "../Utility/Input/InputManager.h"
#include "../Utility/Camera/Camera.h"
#include "../Objects/GameObjectManager.h"


/// <summary>
/// �V�[���^�C�v
/// </summary>
enum class eSceneType
{
	title,			// �^�C�g��
	stage_select,	// �X�e�[�W�Z���N�g
	in_game,		// �C���Q�[��
	re_start,		// ���X�^�[�g
	result,			// ���U���g
	end,			// �I��
};

/// <summary>
/// �V�[�����N���X
/// </summary>
class SceneBase
{
protected:
	// �e�V�[�������L������
	int cursor;					// �J�[�\��
public:

	bool win_flg;				//�v���C���[�������������ǂ���
	int StageNumber;


	// �R���X�g���N�^
	SceneBase() :
		cursor()
	{

	}
	// �f�X�g���N�^
	virtual ~SceneBase()
	{
		// ����Y��h�~
		Finalize();
	}

public:
	/// <summary>
	/// ����������
	/// </summary>
	virtual void Initialize()
	{

	}

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="delta_second">1�t���[��������̎���</param>
	/// <returns>���̃V�[���^�C�v���</returns>
	virtual eSceneType Update(const float& delta_second)
	{
		// �I�u�W�F�N�g�}�l�[�W���[�̏����擾
		GameObjectManager* object = GameObjectManager::GetInstance();
		// �I�u�W�F�N�g���X�g�̍X�V
		object->CheckCreateObject();

		// ���X�g���̃I�u�W�F�N�g���X�V����
		for (GameObject* obj : object->GetObjectsList())
		{
			obj->Update(delta_second);
		}

		// �f�X�g���C���X�g�̍X�V
		object->CheckDestroyObject();

		// ���݂̃V�[������ԋp����
		return GetNowSceneType();
	}

	/// <summary>
	/// �`�揈��
	/// </summary>
	virtual void Draw() const
	{
		// �J�����̏��擾
		Camera* camera = Camera::GetInstance();

		// �I�u�W�F�N�g�}�l�[�W���[�̏����擾
		GameObjectManager* object = GameObjectManager::GetInstance();
		// �I�u�W�F�N�g���X�g���̃I�u�W�F�N�g��`�悷��
		for (GameObject* obj : object->GetObjectsList())
		{
			// �J�����̍��W�������œn��
			obj->Draw(camera->GetCameraPos());
		}
	}

	/// <summary>
	/// �I��������
	/// </summary>
	virtual void Finalize()
	{
		// �I�u�W�F�N�g�}�l�[�W���[�̏����擾
		GameObjectManager* object = GameObjectManager::GetInstance();
		// �I�u�W�F�N�g���X�g���̃I�u�W�F�N�g��j��
		object->DestroyAllObject();
	}

	/// <summary>
	/// ���݂̃V�[���^�C�v�擾����
	/// </summary>
	/// <returns>���݂̃V�[���^�C�v���</returns>
	virtual const eSceneType GetNowSceneType() const = 0;

	void IsPlayerWin(bool setflg)
	{
		win_flg = setflg;
	}
	void SetStageNumber(int Num)
	{
		StageNumber = Num;
	}

};

