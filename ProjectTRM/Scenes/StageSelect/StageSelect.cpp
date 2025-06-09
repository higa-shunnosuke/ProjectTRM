#include "StageSelect.h"

// �R���X�g���N�^
StageSelect::StageSelect()
{

}

// �f�X�g���N�^
StageSelect::~StageSelect()
{

}

// ����������
void StageSelect::Initialize()
{
	// �e�N���X�̏������������Ăяo��
	__super::Initialize();

	// �摜�̓ǂݍ���
	ResourceManager* rm = ResourceManager::GetInstance();

	DecisionSE = rm->GetSounds("Resource/Sounds/StageSelect/����.mp3");
	CursorMoveSE= rm->GetSounds("Resource/Sounds/StageSelect/�J�[�\���ړ�.mp3");

	Stage_Image[0] = rm->GetImages("Resource/Images/BackGround/BlueMoon.png")[0];
	Stage_Image[1] = rm->GetImages("Resource/Images/BackGround/YelloMoon.png")[0];
	Stage_Image[2] = rm->GetImages("Resource/Images/BackGround/RedMoon.png")[0];

	BGM = rm->GetSounds("Resource/Sounds/StageSelect/StageSelect.mp3");

	ChangeVolumeSoundMem(100, BGM);
	if (PlaySoundMem(BGM, DX_PLAYTYPE_LOOP) == -1)
	{
		MessageBoxA(NULL, "BGM1�̍Đ��Ɏ��s���܂���", "�G���[", MB_OK);
	}

}

// �X�V����
eSceneType StageSelect::Update(const float& delta_second)
{
	// ���͏����擾
	InputManager* input = InputManager::GetInstance();

	// �X�e�[�W�I���V�[���ɑJ�ڂ���
	if (input->GetKeyState(KEY_INPUT_RIGHT)		== eInputState::Pressed ||
		input->GetButtonState(XINPUT_BUTTON_DPAD_RIGHT) == eInputState::Pressed)
	{
		SerectStage += 1;
		if (SerectStage > 4)
		{
			SerectStage = 4;
		}
		else
		{
			PlaySoundMem(CursorMoveSE, DX_PLAYTYPE_BACK);
		}
	}
	// �X�e�[�W�I���V�[���ɑJ�ڂ���
	else if (input->GetKeyState(KEY_INPUT_LEFT) == eInputState::Pressed ||
		input->GetButtonState(XINPUT_BUTTON_DPAD_LEFT) == eInputState::Pressed)
	{
		SerectStage -= 1;
		if (SerectStage < 1)
		{
			SerectStage = 1;
		}
		else
		{
			PlaySoundMem(CursorMoveSE, DX_PLAYTYPE_BACK);
		}
	}

	// �C���Q�[���V�[���ɑJ�ڂ���
	if (input->GetKeyState(KEY_INPUT_RETURN) == eInputState::Pressed)
	{
		SetStageNumber(SerectStage);
		PlaySoundMem(DecisionSE, DX_PLAYTYPE_BACK);
		return eSceneType::in_game;
	}
	if (input->GetButtonState(XINPUT_BUTTON_A) == eInputState::Pressed)
	{
		SetStageNumber(SerectStage);
		PlaySoundMem(DecisionSE, DX_PLAYTYPE_BACK);
		return eSceneType::in_game;
	}

	// �e�N���X�̍X�V�������Ăяo��
	return __super::Update(delta_second);
}

// �`�揈��
void StageSelect::Draw() const
{
	/*
	{
		int upnum1 = 0, upnum2 = 0, upnum3 = 0;


		SetFontSize(60);
		DrawFormatString(20, 40, 0xffffff, "Stage Select");
		SetFontSize(32);
		DrawFormatString(100, 400, 0xffffff, "A Button");
		DrawFormatString(100, 450, 0x000000, "Stage:%d", SerectStage);


		switch (SerectStage)
		{
		case 1:
			upnum1 += 50;
			DrawBox(149, 149, 401, 351, 0xffffff, 0);
			break;
		case 2:
			upnum2 += 50;
			DrawBox(399, 149, 651, 351, 0xffffff, 0);
			break;
		case 3:
			upnum3 += 50;
			DrawBox(649, 149, 901, 351, 0xffffff, 0);
			break;
		default:
			break;
		}

		DrawBox(200 - upnum1, 200 - upnum1, 350 + upnum1, 300 + upnum1, 0x00ffff, 1);
		DrawBox(450 - upnum2, 200 - upnum2, 600 + upnum2, 300 + upnum2, 0x00ffff, 1);
		DrawBox(700 - upnum3, 200 - upnum3, 850 + upnum3, 300 + upnum3, 0x00ffff, 1);

	}*/

	// �{�^���T�C�Y
	const int button_width = 200;
	const int button_height = 200;

	// �{�^���̐�
	const int button_count = 3;

	// �{�^���̑��������v�Z
	int total_buttons_width = button_count * button_width;
	// �Ԋu�̑��������v�Z
	int total_spacing = D_WIN_MAX_X - total_buttons_width;
	// �{�^���̊Ԋu�̌v�Z
	int spacing = total_spacing / (button_count + 1);

	for (int i = 0; i < button_count; ++i)
	{
		//	�I�����̈ʒu
		int x = spacing + i * (button_width + spacing);
		int y = 120;

		// �g��p�ϐ�
		int w = button_width;
		int h = button_height;

		if (i == SerectStage - 1)
		{
			// �I�𒆂̂��̂����g��i1.2�{�j
			int w = (int)(button_width * 1.2);
			int h = (int)(button_height * 1.2);

			// �g�i�w�i�j��`��
			DrawBox(x - (w - button_width) / 2, y - (h - button_height) / 2, x + w, y + h, GetColor(255, 255, 255), TRUE);
			//�L�����̕`��͈͂𐧌�
			SetDrawArea(x - (w - button_width) / 2, y - (h - button_height) / 2, x + w, y + h);
			{
				// �L�����摜�𒆐S�ɕ`��
				DrawExtendGraph(
					(int)(x + (button_width - w * 1.5) / 2), (int)(y + (button_height - h * 1.5) / 2),
					(int)(x + (button_width + w * 1.7) / 2), (int)(y + (button_height + h * 1.7) / 2),
					Stage_Image[i], TRUE);
			}
		}
		else
		{

			// �g�i�w�i�j��`��
			DrawBox(x, y, x + button_width, y + button_height, GetColor(100, 100, 100), TRUE);
			//�L�����̕`��͈͂𐧌�
			SetDrawArea(x, y, x + button_width, y + button_height);

			// �L�����摜�𒆐S�ɕ`��
			DrawExtendGraph(
				(int)(x + (button_width - w * 1.5) / 2), (int)(y + (button_height - h * 1.5) / 2),
				(int)(x + (button_width + w * 1.7) / 2), (int)(y + (button_height + h * 1.7) / 2),
				Stage_Image[i], TRUE);
		}
		// �`��͈͂����ɖ߂�
		SetDrawArea(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y);

	}
		// �e�N���X�̕`�揈�����Ăяo��
		__super::Draw();
	
}

// �I������
void StageSelect::Finalize()
{

	StopSoundMem(BGM);

	// �e�N���X�̏I�����������Ăяo��
	__super::Finalize();
}

// ���݂̃V�[���^�C�v�擾����
const eSceneType StageSelect::GetNowSceneType() const
{
	return eSceneType::stage_select;
}
