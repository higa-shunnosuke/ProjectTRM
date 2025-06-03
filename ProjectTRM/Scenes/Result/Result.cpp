#include "Result.h"

// �R���X�g���N�^
Result::Result()
{
	
}

// �f�X�g���N�^
Result::~Result()
{

}

// ����������
void Result::Initialize()
{
	// �e�N���X�̏������������Ăяo��
	__super::Initialize();

	ResourceManager* rm = ResourceManager::GetInstance();

	bgmHandle[0] = rm->GetSounds("Resource/Sounds/Result/Lost.mp3");
	bgmHandle[1] = rm->GetSounds("Resource/Sounds/Result/Loose_BGM.mp3");
	bgmHandle[2] = rm->GetSounds("Resource/Sounds/Result/Win_BGM.mp3");
	bgmHandle[3] = rm->GetSounds("Resource/Sounds/Result/Wining.mp3");

	if (win_flg)
	{
		if (bgmHandle[3] == -1)
		{
			MessageBoxA(NULL, "BGM1�̓ǂݍ��݂Ɏ��s���܂���", "�G���[", MB_OK);
		}
		else
		{
			// ���ʐݒ�
			ChangeVolumeSoundMem(150, bgmHandle[3]);
			// BGM�Đ�
			if (PlaySoundMem(bgmHandle[3], DX_PLAYTYPE_BACK) == -1)
			{
				MessageBoxA(NULL, "BGM1�̍Đ��Ɏ��s���܂���", "�G���[", MB_OK);
			}
		}
	}
	else
	{
		if (bgmHandle[1] == -1)
		{
			MessageBoxA(NULL, "BGM2�̓ǂݍ��݂Ɏ��s���܂���", "�G���[", MB_OK);
		}
		else
		{
			// ���ʐݒ�
			ChangeVolumeSoundMem(150, bgmHandle[1]);
			// BGM�Đ�
			if (PlaySoundMem(bgmHandle[1], DX_PLAYTYPE_BACK) == -1)
			{
				MessageBoxA(NULL, "BGM2�̍Đ��Ɏ��s���܂���", "�G���[", MB_OK);
			}
		}
	}


}

// �X�V����
eSceneType Result::Update(const float& delta_second)
{
	// ���͏����擾
	InputManager* input = InputManager::GetInstance();

	if (win_flg)
	{
		switch (cursor)
		{
		case 1:
		{
			if (input->GetKeyState(KEY_INPUT_UP) == eInputState::Pressed)
			{
				cursor++;
			}
			if (input->GetButtonState(XINPUT_BUTTON_DPAD_UP) == eInputState::Pressed)
			{
				cursor++;
			}
			// �X�e�[�W�I���V�[���ɑJ�ڂ���
 			 if (input->GetKeyState(KEY_INPUT_SPACE) == eInputState::Pressed)
			{
				return eSceneType::title;
			}
			else if (input->GetButtonState(XINPUT_BUTTON_A) == eInputState::Pressed)
			{
				 return eSceneType::title;
			}
		}
		break;
		case 2:
		{

			if (input->GetKeyState(KEY_INPUT_DOWN) == eInputState::Pressed)
			{
				cursor--;
			}
			else if (input->GetButtonState(XINPUT_BUTTON_DPAD_DOWN) == eInputState::Pressed)
			{
				cursor--;
			}
			// �^�C�g���V�[���ɑJ�ڂ���
			if (input->GetKeyState(KEY_INPUT_SPACE) == eInputState::Pressed)
			{
				return eSceneType::stage_select;
			}
			else if (input->GetButtonState(XINPUT_BUTTON_A) == eInputState::Pressed)
			{
				return eSceneType::stage_select;
			}
		}
		break;
		default:
			if (input->GetKeyState(KEY_INPUT_UP) == eInputState::Pressed)
			{
				cursor = 2;
			}
			else if (input->GetButtonState(XINPUT_BUTTON_DPAD_UP) == eInputState::Released)
			{
				cursor = 2;
			}
			if (input->GetKeyState(KEY_INPUT_DOWN) == eInputState::Pressed)
			{
				cursor = 1;
			}
			else if (input->GetButtonState(XINPUT_BUTTON_DPAD_DOWN) == eInputState::Pressed)
			{
				cursor = 1;
			}
			break;
		}
	}
	else
	{
		switch (cursor)
		{
		case 1:
		{
			if (input->GetKeyState(KEY_INPUT_UP) == eInputState::Pressed)
			{
				cursor++;
			}
			else if (input->GetButtonState(XINPUT_BUTTON_DPAD_UP) == eInputState::Pressed)
			{
				cursor++;
			}
			// �X�e�[�W�I���V�[���ɑJ�ڂ���
			if (input->GetKeyState(KEY_INPUT_SPACE) == eInputState::Pressed)
			{
				return eSceneType::title;
			}
			else if (input->GetButtonState(XINPUT_BUTTON_A) == eInputState::Pressed)
			{
				return eSceneType::title;
			}
		}
		break;
		case 2:
		{

			if (input->GetKeyState(KEY_INPUT_DOWN) == eInputState::Pressed)
			{
				cursor--;
			}
			else if (input->GetButtonState(XINPUT_BUTTON_DPAD_DOWN) == eInputState::Pressed)
			{
				cursor--;
			}
			// �^�C�g���V�[���ɑJ�ڂ���
			if (input->GetKeyState(KEY_INPUT_SPACE) == eInputState::Pressed)
			{
				return eSceneType::in_game;
			}
			else if (input->GetButtonState(XINPUT_BUTTON_A) == eInputState::Pressed)
			{
				return eSceneType::in_game;
			}
		}
		break;
		default:
			if (input->GetKeyState(KEY_INPUT_UP) == eInputState::Pressed)
			{
				cursor = 2;
			}
			else if (input->GetButtonState(XINPUT_BUTTON_DPAD_UP) == eInputState::Released)
			{
				cursor = 2;
			}
			else if (input->GetKeyState(KEY_INPUT_DOWN) == eInputState::Pressed)
			{
				cursor = 1;
			}
			else if (input->GetButtonState(XINPUT_BUTTON_DPAD_DOWN) == eInputState::Released)
			{
				cursor = 1;
			}
			break;
		}
		auto now_time = std::chrono::steady_clock::now();

		if (now_time - prev_time > std::chrono::milliseconds(1000))
		{
			count--;
			prev_time = std::chrono::steady_clock::now();
		}
		else if (count == 0)
		{
			return eSceneType::title;
		}
	}
	auto  now_time = std::chrono::steady_clock::now();

	if (now_time - select_time > std::chrono::milliseconds(500))
	{
		if (show)
		{
			show = false;
		}
		else
		{
			show = true;
		}
		select_time = std::chrono::steady_clock::now();
	}

	if (cursor == -1)
	{
		if (now_time - choice_time > std::chrono::milliseconds(1000))
		{
			cursor = 2;
		}
	}
	

	// �e�N���X�̍X�V�������Ăяo��
	return __super::Update(delta_second);
}

// �`�揈��
void Result::Draw() const
{

	DrawFormatString(500, 150, 0xffffff, "���S�������j�b�g:%d\n���Y�������j�b�g:%d", -dead_unit, make_unit - 1);

	SetFontSize(60);
	DrawFormatString(120, 140, 0xffffff, "Result");
	SetFontSize(32);
	if (win_flg)
	{
		switch (cursor)
		{
			case 1:
			{
				DrawCircle(480, 566, 10, 0xffffff);
				if (show != true)
				{
					DrawString(500, 550, ": Title", 0xffffff);
				}
				DrawString(500, 500, ": Stage Select", 0xffffff);
			}
			break;
			case 2:
			{
				DrawCircle(480, 516, 10, 0xffffff);
				if (show != true)
				{
					DrawString(500, 500, ": Stage Select", 0xffffff);
				}
				DrawString(500, 550, ": Title", 0xffffff);
			}
			break;
		default:
		{
			DrawString(500, 500, ": Stage Select", 0xffffff);
			DrawString(500, 550, ": Title", 0xffffff);
		}
			break;
		}

	}
	else
	{
		SetFontSize(80);
		DrawFormatString(500, 350, 0xffffff, "%d", count);
		SetFontSize(32);		
		switch (cursor)
		{
		case 1:
		{
			DrawCircle(480, 566, 10, 0xffffff);
			if (show != true)
			{
				DrawString(500, 550, ": Title", 0xffffff);
			}
			DrawString(500, 500, ": ReStart", 0xffffff);
		}
		break;
		case 2:
		{
			DrawCircle(480, 516, 10, 0xffffff);
			if (show != true)
			{
				DrawString(500, 500, ": ReStart", 0xffffff);
			}
			DrawString(500, 550, ": Title", 0xffffff);
		}
		break;
		default:
		{
			DrawString(500, 500, ": ReStart", 0xffffff);
			DrawString(500, 550, ": Title", 0xffffff);
		}
		break;
		}

	}

	// �e�N���X�̕`�揈�����Ăяo��
 	__super::Draw();
}

// �I������
void Result::Finalize()
{
	
	if (win_flg)
	{
		StopSoundMem(bgmHandle[3]);
	}
	else
	{
		StopSoundMem(bgmHandle[1]);
	}
	
	// �e�N���X�̏I�����������Ăяo��
	__super::Finalize();
}

// ���݂̃V�[���^�C�v�����擾����
const eSceneType Result::GetNowSceneType() const
{
	return eSceneType::result;
}
