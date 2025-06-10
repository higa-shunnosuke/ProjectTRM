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

	DecisionSE = rm->GetSounds("Resource/Sounds/StageSelect/Decision.mp3");
	CursorMoveSE= rm->GetSounds("Resource/Sounds/StageSelect/CursorMove.mp3");
	Stage_Image[0] = rm->GetImages("Resource/Images/BackGround/BlueMoonUI.png")[0];
	Stage_Image[1] = rm->GetImages("Resource/Images/BackGround/YelloMoonUI.png")[0];
	Stage_Image[2] = rm->GetImages("Resource/Images/BackGround/RedMoonUI.png")[0];
	BackGroued_Image = rm->GetImages("Resource/Images/BackGround/StageSelect.png")[0];

		ChangeFontType(DX_FONTTYPE_ANTIALIASING_EDGE);

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

	switch (State)
	{
	case Stage::DEFAULT:
	// �X�e�[�W�I���V�[���ɑJ�ڂ���
	if (input->GetKeyState(KEY_INPUT_RIGHT)		== eInputState::Pressed ||
		input->GetButtonState(XINPUT_BUTTON_DPAD_RIGHT) == eInputState::Pressed)
	{
		SerectStage += 1;
		if (SerectStage > ThardStage)
		{
			SerectStage = ThardStage;
		}
		else
		{
			x = 200;
			ChangeX = Set_StageX;
			State = Stage::LMOVE;
		}
	}
	// �X�e�[�W�I���V�[���ɑJ�ڂ���
	else if (input->GetKeyState(KEY_INPUT_LEFT) == eInputState::Pressed ||
		input->GetButtonState(XINPUT_BUTTON_DPAD_LEFT) == eInputState::Pressed)
	{
		SerectStage -= 1;
		if (SerectStage < FirstStage)
		{
			SerectStage = FirstStage;
		}
		else
		{
			ChangeX = x;
			x = -Set_StageX;
			State = Stage::RMOVE;
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
	break;
	case Stage::LMOVE:

		x -= 1;
		ChangeX -= 1;

		if (ChangeX == 200)
		{
		x = 200;
		ChangeX = Set_StageX;
		State = Stage::DEFAULT;
		}

		break;
	case Stage::RMOVE:

		x += 1;
		ChangeX += 1;

		if (x == 200)
		{
			x = 200;
			ChangeX = Set_StageX;
			State = Stage::DEFAULT;
		}
		break;
	case Stage::END:
		break;
	default:
		break;
	}

	// �e�N���X�̍X�V�������Ăяo��
	return __super::Update(delta_second);
}

// �`�揈��
void StageSelect::Draw() const
{

	int Cw = ChangeX + 100;

	switch (State)
	{
	case DEFAULT:
	{
		DrawExtendGraph((int)(0), (int)(0), (int)(1280), (int)(y + 620), BackGroued_Image, TRUE);

		DrawFormatString(x - 50, 50, 0xffffff, StageText[SerectStage - 1]);

		// �摜�𒆐S�ɕ`��
		DrawExtendGraph((int)(x), (int)(y), (int)(x + w), (int)(y + h), Stage_Image[SerectStage - 1], TRUE);

		
		DrawString(x, 350, StageFlabarText[SerectStage - 1], 0xffffff);
		if (SerectStage != FirstStage)
		{
			DrawString(Centher - 80, 650, StageSelectText[SerectStage - 1], 0xffffff);
		}
		else
		{
			DrawString(Centher, 650, StageSelectText[SerectStage - 1], 0xffffff);
		}
		break;

	}
	//���ɐi�݂܂��B
	case LMOVE:
	{

		DrawExtendGraph((int)(x-200), (int)(0), (int)(x + 1080), (int)(y + 620), BackGroued_Image, TRUE);
		if (SerectStage != FinalStage)
		{
			DrawExtendGraph((int)(ChangeX - 200), (int)(0), (int)(ChangeX + 1080), (int)(y + 620), BackGroued_Image, TRUE);
		}
		else
		{
			DrawExtendGraph((int)(ChangeX - 200), (int)(0), (int)(ChangeX + 1080), (int)(y + 620), BackGroued_Image, TRUE);
		}
		// �摜�𒆐S�ɕ`��
		DrawExtendGraph((int)(x), (int)(y), (int)(x + w), (int)(y + h), Stage_Image[SerectStage - 2], TRUE);
		DrawExtendGraph((int)(ChangeX), (int)(y), (int)(ChangeX + w), (int)(y + h), Stage_Image[SerectStage-1], TRUE);

		DrawFormatString(x - 50, 50, 0xffffff, StageText[SerectStage - 2]);
		DrawFormatString(ChangeX - 50, 50, 0xffffff, StageText[SerectStage - 1]);

		DrawString(x, 350, StageFlabarText[SerectStage - 2], 0xffffff);
		DrawString(ChangeX, 350, StageFlabarText[SerectStage - 1], 0xffffff);
		if (SerectStage != FirstStage)
		{

			DrawString(Centher + (x - 200), 650, StageSelectText[SerectStage - 2], 0xffffff);
			DrawString(Centher + (ChangeX - 280), 650, StageSelectText[SerectStage - 1], 0xffffff);
		}
		else
		{
			//1�X�e�[�W�ڂœ��͂��Ă��ʂ�Ȃ��̂ŁA���L�̃G���[�͖���
			DrawString(Centher + (x - 200), 650, StageSelectText[SerectStage - 2], 0xffffff);
			DrawString(Centher + (ChangeX - 200), 650, StageSelectText[SerectStage - 1], 0xffffff);
		}

	}
	break;

	case Stage::RMOVE:
	{
		DrawExtendGraph((int)(x-200), (int)(0), (int)(x + 1080), (int)(y + 620), BackGroued_Image, TRUE);
		DrawExtendGraph((int)(ChangeX-200), (int)(0), (int)(ChangeX + 1080), (int)(y + 620), BackGroued_Image, TRUE);

		// �摜�𒆐S�ɕ`��
		DrawExtendGraph((int)(ChangeX), (int)(y), (int)(ChangeX + w), (int)(y + h), Stage_Image[SerectStage], TRUE);
		DrawExtendGraph((int)(x), (int)(y), (int)(x + w), (int)(y + h), Stage_Image[SerectStage - 1], TRUE);

		DrawFormatString(ChangeX - 50, 50, 0xffffff, StageText[SerectStage]);
		DrawFormatString(x - 50, 50, 0xffffff, StageText[SerectStage - 1]);

		DrawString(ChangeX, 350, StageFlabarText[SerectStage], 0xffffff);
		DrawString(x, 350, StageFlabarText[SerectStage - 1], 0xffffff);

		if (SerectStage != FirstStage)
		{

			DrawString(Centher + (x - 200), 650, StageSelectText[SerectStage-1], 0xffffff);
			DrawString(Centher + (ChangeX - 200), 650, StageSelectText[SerectStage], 0xffffff);
		}
		else
		{
			DrawString(Centher + (x - 200), 650, StageSelectText[SerectStage-1], 0xffffff);
			DrawString(Centher + (ChangeX - 200), 650, StageSelectText[SerectStage], 0xffffff);
		}


		break;
	}
	case END:
		break;
	default:
		break;
	}
}
// �I������
void StageSelect::Finalize()
{
	ChangeFontType(DX_FONTTYPE_NORMAL);

	StopSoundMem(BGM);

	// �e�N���X�̏I�����������Ăяo��
	__super::Finalize();
}

// ���݂̃V�[���^�C�v�擾����
const eSceneType StageSelect::GetNowSceneType() const
{
	return eSceneType::stage_select;
}
