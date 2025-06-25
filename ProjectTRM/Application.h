#pragma once

#include "DxLib.h"

#include "Utility/Singleton.h"
#include "Utility/ProjectConfig.h"
#include "Utility/Input/InputManager.h"
#include "Scenes/SceneManager.h"

class Application : public Singleton<Application>
{
private:
	// �O���[�o���ϐ���`
	LONGLONG old_time = 0;			// �O��v���l
	LONGLONG now_time = 0;			// ���݌v���l
	float delta_second = 0.0f;		// �P�t���[��������̎���
	bool end_flg = false;			// �Q�[���I���t���O

public:

	// �N��
	bool WakeUp()
	{
		// �E�B���h�E���[�h�ŋN������
		ChangeWindowMode(TRUE);

		// �E�B���h�E�T�C�Y�̐ݒ�
		SetGraphMode(D_WIN_MAX_X, D_WIN_MAX_Y, D_COLOR_BIT);

		// �E�B���h�E�^�C�g���̐ݒ�
		SetWindowText("Torchguard �`Shadows Fall�`");

		// �����������s��Ȃ�
		SetWaitVSyncFlag(FALSE);

		// Log.txt�t�@�C���̐�������iDebug���[�h�̂ݐ�������j
#if _DEBUG
		SetOutApplicationLogValidFlag(TRUE);
#else
		SetOutApplicationLogValidFlag(FALSE);
#endif // _DEBUG

		// Dx���C�u�����̏�����
		if (DxLib_Init() == D_FAILURE)
		{
			throw std::string("Dx���C�u�����̏������Ɏ��s���܂����I\n");
		}

		// �`����\��ʂɔ��f����
		SetDrawScreen(DX_SCREEN_BACK);

		// �V�[���}�l�[�W���[�����擾
		SceneManager* manager = SceneManager::GetInstance();

		// �V�[���}�l�[�W���[��������
		manager->Initialize();

		// ��A�N�e�B�u��Ԃł����삳����
		SetAlwaysRunFlag(TRUE);

		return true;
	}

	// ���s
	void Run()
	{
		// �V�[���}�l�[�W���[�����擾
		SceneManager* manager = SceneManager::GetInstance();

		// ���͏����擾
		InputManager* input = InputManager::GetInstance();

		// ���C�����[�v
		while (ProcessMessage() == D_SUCCESS)
		{			
			// ���͏��̍X�V
			input->Update();

			// �t���[�����[�g�̐���
			UpdateDeltaTime();

			// �V�[���}�l�[�W���[���X�V
			manager->Update(delta_second);
		
			// �����I�����邩�m�F
			if ((input->GetButtonState(XINPUT_BUTTON_BACK) == eInputState::Released) ||
				(input->GetKeyState(KEY_INPUT_ESCAPE) == eInputState::Released))
			{
				// �Q�[�����I��
				QuitGame();
			}

			// �Q�[�����I�����邩�m�F
			if (end_flg == true)
			{
				// ���C�����[�v���I��
				break;
			}
		}
	}

	//�I��
	void Shutdown()
	{
		// �V�[���}�l�[�W���[�����擾
		SceneManager* manager = SceneManager::GetInstance();

		// �I��������
		manager->Finalize();

		// Dx���C�u�����̎g�p���I������
		DxLib_End();
	}

	//�Q�[���I���ʒm����
	void QuitGame()
	{
		// �Q�[���I���t���O�𗧂Ă�
		this->end_flg = true;
	}

private:
	void UpdateDeltaTime()
	{
		// ���ݎ��Ԃ̎擾�i�ʕb�j
		now_time = GetNowHiPerformanceCount();

		// �J�n���Ԃ��猻�ݎ��Ԃ܂łɌo�߂������Ԃ��v�Z����i�ʕb�j
		// ����\���ʕb���b�ɕϊ�����
		delta_second = (float)(now_time - old_time) * 1.0e-6f;

		// �v���J�n���Ԃ��X�V����
		old_time = now_time;

		// �f�B�X�v���C�̃��t���b�V�����[�g���擾����
		float refresh_rate = (float)GetRefreshRate();

		// �P�t���[��������̎��Ԃ�1/���t���b�V�����[�g�b�𒴂�����A������
		if (delta_second > (1.0f / refresh_rate))
		{
			delta_second = (1.0f / refresh_rate);
		}
	}
};