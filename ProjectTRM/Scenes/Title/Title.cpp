#include "Title.h"

// �R���X�g���N�^
Title::Title()
{

}

// �f�X�g���N�^
Title::~Title()
{
	ChangeFontType(DX_FONTTYPE_NORMAL);

}

// ����������
void Title::Initialize()
{
	// �e�N���X�̏������������Ăяo��
	__super::Initialize();

	ChangeFontType(DX_FONTTYPE_ANTIALIASING_EDGE_8X8);

	MovieHandle = LoadGraph("Resource/Movie/TitleBack.mp4");

	// ���[�r�[�̍Đ��ʒu��0�b�ڂɕύX���܂�
	SeekMovieToGraph(MovieHandle, 0000);
	PlayMovieToGraph(MovieHandle);           // �čĐ�


	// �摜�̓ǂݍ���
	ResourceManager* rm = ResourceManager::GetInstance();


	BackGroundImage = rm->GetImages("Resource/Images/BackGround/Title.png")[0];
	BackBackGroundImage = rm->GetImages("Resource/Images/BackGround/TitleBack.png")[0];
	SellectImage = rm->GetImages("Resource/Images/Effect/Flamethrower.png",10,3,4,96,48);

	BGM = rm->GetSounds("Resource/Sounds/Title/BGM/OP.mp3");
	DecisionSE = rm->GetSounds("Resource/Sounds/Decision.mp3");
	Y = D_WIN_MAX_Y;
	Anim_flame = 0;
	Anim_count = 0;

	ChangeVolumeSoundMem(100, BGM);
	ChangeVolumeSoundMem(190, DecisionSE);
	if (PlaySoundMem(BGM, DX_PLAYTYPE_LOOP) == -1)
	{
		MessageBoxA(NULL, "BGM1�̍Đ��Ɏ��s���܂���", "�G���[", MB_OK);
	}
}

// �X�V����
eSceneType Title::Update(const float& delta_second)
{

	
	// ���͏����擾
	InputManager* input = InputManager::GetInstance();
	int state = GetMovieStateToGraph(MovieHandle);
	if (state == 0) { // ��~���
		SeekMovieToGraph(MovieHandle, 6500);        // �擪�ɖ߂�
		PlayMovieToGraph(MovieHandle);           // �Đ�
	}


	switch (State)
	{
	case SELECT_WAIT:
	{

		// �C���Q�[���V�[���ɑJ�ڂ���
		if (input->GetKeyState(KEY_INPUT_RETURN) == eInputState::Pressed)
		{
			PlaySoundMem(DecisionSE, DX_PLAYTYPE_BACK);
			State = TitleState::USEING;
		}
		if (input->GetButtonState(XINPUT_BUTTON_A) == eInputState::Pressed)
		{
			PlaySoundMem(DecisionSE, DX_PLAYTYPE_BACK);
			State = TitleState::USEING;
		}
		if (input->GetKeyState(KEY_INPUT_SPACE) == eInputState::Pressed)
		{
			PlaySoundMem(DecisionSE, DX_PLAYTYPE_BACK);
			return eSceneType::end;
		}

	}
	break;
	case USEING:
		Y--;
		if (Y == 450)
		{
			State = TitleState::SELECTED;
		}
		break;
	case SELECTED:
		if (input->GetButtonState(XINPUT_BUTTON_DPAD_UP) == eInputState::Pressed)
		{
			if (Selected != 0)
			{
				Selected--;
			}
		}
		else if (input->GetButtonState(XINPUT_BUTTON_DPAD_DOWN) == eInputState::Pressed)
		{
			if (Selected != MAX_SELECT)
			{
				Selected++;
			}
		}

		if (input->GetKeyState(KEY_INPUT_UP) == eInputState::Pressed)
		{
			if (Selected != 0)
			{
				Selected--;
			}
		}
		else if (input->GetKeyState(KEY_INPUT_DOWN) == eInputState::Pressed)
		{
			if (Selected != MAX_SELECT)
			{
				Selected++;
			}
		}
		else if ((input->GetButtonState(XINPUT_BUTTON_A) == eInputState::Pressed) || (input->GetKeyState(KEY_INPUT_RETURN) == eInputState::Pressed))
		{
			PlaySoundMem(DecisionSE, DX_PLAYTYPE_BACK);
			switch (Selected)
			{
			case 0:
				return eSceneType::stage_select;
			case MAX_SELECT:
				return eSceneType::end;
			default:
				break;
			}
		}
		break;
	case FINAL:
		break;
	default:
		break;
	}

	Anim_flame += delta_second*10.0f;
	if (Anim_flame >= 1.0f)
	{
		Anim_count++;
		Anim_flame = 0.0f;
	}
	if (Anim_count > 9)
	{
		Anim_count = 0;
		Anim_flame = 0.0f;
	}

	// ���݂̃V�[������ԋp����
	return GetNowSceneType();
}

// �`�揈��
void Title::Draw() const
{
	// ���[�r�[�f������ʂ����ς��ɕ`�悵�܂�
	DrawExtendGraph(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, MovieHandle, FALSE);
	DrawGraph(0, 0, BackGroundImage, 1);

	switch (State)
	{

	/*
	SetFontSize(120);
	DrawFormatString(500, 150, 0xff5500, "Project\n -TRM-");
	SetFontSize(60);

	if (Anim_count == 1)
	{
	DrawFormatString(500, 450, 0xff5500, "Press A Button");
	}
	DrawFormatString(1080, 620, 0xff5500, "B:End");

	if (ProjectConfig::DEBUG)
	{
		SetFontSize(60);
		DrawFormatString(100, 140, 0xffffff, "Title");
		SetFontSize(32);
		DrawFormatString(100, 300, 0xffffff, "Enter�������Ă�!");
	}
	*/

	case SELECT_WAIT:
		break;
	case USEING:
		DrawBox(0, Y, D_WIN_MAX_X, D_WIN_MAX_Y, 0x000000, 1);
		break;
	case SELECTED:
	{
		DrawBox(0, 450, D_WIN_MAX_X, D_WIN_MAX_Y, 0x000000, 1);
		DrawBox(0, 450, D_WIN_MAX_X, D_WIN_MAX_Y, 0xffffff, 0);
		DrawExtendGraphF(300, 450 + Selected * 84, 450, 582 + Selected * 84, SellectImage[Anim_count], true);

		SetFontSize(64);
		float SelectColor = 0xffff00, Color = 0xff0000;

		if (Selected == 0)
		{
			DrawString(415, 481, "Game Start", SelectColor);
			DrawString(415, 481 + 84, " Game End", Color);
		}
		else
		{
			DrawString(415, 481, " Game Start", Color);
			DrawString(415, 481 + 84, "Game End", SelectColor);
		}

		SetFontSize(32);
	}
		break;
	case FINAL:

		break;
	default:
		break;
	}

	// �e�N���X�̕`�揈�����Ăяo��
	__super::Draw();

}

// �I������
void Title::Finalize()
{
	ChangeFontType(DX_FONTTYPE_NORMAL);
	// �ǂݍ��񂾃��[�r�[�t�@�C���̃O���t�B�b�N�n���h���̍폜
	DeleteGraph(MovieHandle);
	StopSoundMem(BGM);

	// �e�N���X�̏I�����������Ăяo��
	__super::Finalize();
}

// ���݂̃V�[���^�C�v�擾����
const eSceneType Title::GetNowSceneType() const
{
	return eSceneType::title;
}