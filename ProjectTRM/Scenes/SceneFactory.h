#pragma once

#include "SceneBase.h"
#include "Title/Title.h"
#include "StageSelect/StageSelect.h"
#include "InGame/InGame.h"
#include "Result/Result.h"
#include "Loading/Loading.h"
#include "End/End.h"

class SceneFactory
{
public:

	/// <summary>
	/// �V�[����������
	/// </summary>
	/// <param name="next_type">���̃V�[���^�C�v</param>
	/// <returns>���������V�[�����̃|�C���^</returns>
	static SceneBase* CreateScene(eSceneType next_type)
	{
		// �V�[���^�C�v�ɂ���āA��������V�[����؂�ւ���
		switch (next_type)
		{
			// ���[�f�B���O
		case eSceneType::loading:
			return dynamic_cast<SceneBase*>(new Loading());
			// �^�C�g��
		case eSceneType::title:
			return dynamic_cast<SceneBase*>(new Title());
			// �X�e�[�W�Z���N�g
		case eSceneType::stage_select:
			return dynamic_cast<SceneBase*>(new StageSelect);
			// �C���Q�[���A���X�^�[�g
		case eSceneType::in_game:
		case eSceneType::re_start:
			return dynamic_cast<SceneBase*>(new InGame());
			// ���U���g
		case eSceneType::result:
			return dynamic_cast<SceneBase*>(new Result());
			// �G���h
		case eSceneType::end:
			return dynamic_cast<SceneBase*>(new End());
		default:
			return nullptr;
		}
	}
};