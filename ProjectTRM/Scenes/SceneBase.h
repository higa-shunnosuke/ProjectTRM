#pragma once

#include <vector>
#include <string>
#include "DxLib.h"

#include "../Utility/ProjectConfig.h"
#include "../Utility/ResourceManager.h"
#include "../Utility/Input/InputManager.h"
#include "../Utility/Input/InputEventManager.h"
#include "../Objects/GameObjectManager.h"


/// <summary>
/// �V�[���^�C�v
/// </summary>
enum class eSceneType
{
	title,		// �^�C�g��
	in_game,	// �C���Q�[��
	re_start,	// ���X�^�[�g
	result,		// ���U���g
	exit,		// �I��
};

/// <summary>
/// �V�[�����N���X
/// </summary>
class SceneBase
{
protected:
	// �e�V�[�������L������
	


public:
	// �R���X�g���N�^
	SceneBase()
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

		// ���݂̃V�[������ԋp����
		return GetNowSceneType();
	}

	/// <summary>
	/// �`�揈��
	/// </summary>
	virtual void Draw() const
	{
		
		
	}

	/// <summary>
	/// �I��������
	/// </summary>
	virtual void Finalize()
	{
		
	}

	/// <summary>
	/// ���݂̃V�[���^�C�v�擾����
	/// </summary>
	/// <returns>���݂̃V�[���^�C�v���</returns>
	virtual const eSceneType GetNowSceneType() const = 0;

};

