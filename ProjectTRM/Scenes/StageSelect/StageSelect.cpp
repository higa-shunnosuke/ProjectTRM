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

}

// �X�V����
eSceneType StageSelect::Update(const float& delta_second)
{
	// ���͏����擾
	InputManager* input = InputManager::GetInstance();

	// �X�e�[�W�I���V�[���ɑJ�ڂ���
	if (input->GetKeyState(KEY_INPUT_RIGHT) == eInputState::Pressed)
	{
		SerectStage += 1;
		if (SerectStage > 3)
		{
			SerectStage = 3;
		}
	}
	// �X�e�[�W�I���V�[���ɑJ�ڂ���
	else if (input->GetKeyState(KEY_INPUT_LEFT) == eInputState::Pressed)
	{
		SerectStage -= 1;
		if (SerectStage < 1)
		{
			SerectStage = 1;
		}
	}

	// �C���Q�[���V�[���ɑJ�ڂ���
	if (input->GetKeyState(KEY_INPUT_RETURN) == eInputState::Pressed)
	{
		SetStageNumber(SerectStage);
		return eSceneType::in_game;
	}
	if (input->GetButtonState(XINPUT_BUTTON_START) == eInputState::Pressed)
	{
		SetStageNumber(SerectStage);
		return eSceneType::in_game;
	}

	// �e�N���X�̍X�V�������Ăяo��
	return __super::Update(delta_second);
}

// �`�揈��
void StageSelect::Draw() const
{

	int upnum1 = 0 , upnum2 = 0 , upnum3 = 0;


	SetFontSize(60);
	DrawFormatString(20, 40, 0xffffff, "StageSelect");
	SetFontSize(32);
	DrawFormatString(100, 300, 0xffffff, "Enter�������Ă�������");
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


	// �e�N���X�̕`�揈�����Ăяo��
	__super::Draw();
}

// �I������
void StageSelect::Finalize()
{
	// �e�N���X�̏I�����������Ăяo��
	__super::Finalize();
}

// ���݂̃V�[���^�C�v�擾����
const eSceneType StageSelect::GetNowSceneType() const
{
	return eSceneType::stage_select;
}
