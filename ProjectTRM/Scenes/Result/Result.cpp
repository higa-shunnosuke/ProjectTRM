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

	// �摜�̓ǂݍ���
	ResourceManager* rm = ResourceManager::GetInstance();


	DeadImage[0] = rm->GetImages("Resource/Images/Enemy/Heretic/NotDead.png")[0];
	DeadImage[1] = rm->GetImages("Resource/Images/Enemy/Heretic/ImDead.png")[0];
}

// �X�V����
eSceneType Result::Update(const float& delta_second)
{
	// ���͏����擾
	InputManager* input = InputManager::GetInstance();

	if (win_flg)
	{
		// �X�e�[�W�I���V�[���ɑJ�ڂ���
		if (input->GetKeyState(KEY_INPUT_SPACE) == eInputState::Pressed)
		{
			return eSceneType::stage_select;
		}
		else if (input->GetButtonState(XINPUT_BUTTON_A) == eInputState::Pressed)
		{
			return eSceneType::stage_select;
		}

		// �^�C�g���V�[���ɑJ�ڂ���
		if (input->GetKeyState(KEY_INPUT_X) == eInputState::Pressed)
		{
			return eSceneType::title;
		}
		else if (input->GetButtonState(XINPUT_BUTTON_B) == eInputState::Pressed)
		{
			return eSceneType::title;
		}
	}
	else
	{
		// �C���Q�[���V�[���ɑJ�ڂ���
		if (input->GetKeyState(KEY_INPUT_SPACE) == eInputState::Pressed)
		{
			return eSceneType::in_game;
		}
		else if (input->GetButtonState(XINPUT_BUTTON_A) == eInputState::Pressed)
		{
			return eSceneType::in_game;
		}
		// �^�C�g���V�[���ɑJ�ڂ���
		else if (input->GetKeyState(KEY_INPUT_X) == eInputState::Pressed)
		{
			return eSceneType::title;
		}
		else if (input->GetButtonState(XINPUT_BUTTON_B) == eInputState::Pressed)
		{
			return eSceneType::title;
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
	
	// �e�N���X�̍X�V�������Ăяo��
	return __super::Update(delta_second);

}

// �`�揈��
void Result::Draw() const
{
	SetFontSize(60);
	DrawFormatString(120, 140, 0xffffff, "Result");
	SetFontSize(32);
	if (win_flg)
	{
		DrawString(500, 500, "A : Stage Select", 0xffffff);
		DrawString(500, 550, "B : Title", 0xffffff);

	}
	else
	{
		SetFontSize(80);
		DrawFormatString(500, 350, 0xffffff, "%d", count);
		SetFontSize(32);
		DrawString(500, 500, "A : Retry", 0xffffff);
		DrawString(500, 550, "B : Title", 0xffffff);

	}

	// �e�N���X�̕`�揈�����Ăяo��
 	__super::Draw();
}

// �I������
void Result::Finalize()
{
	// �e�N���X�̏I�����������Ăяo��
	__super::Finalize();
}

// ���݂̃V�[���^�C�v�����擾����
const eSceneType Result::GetNowSceneType() const
{
	return eSceneType::result;
}
