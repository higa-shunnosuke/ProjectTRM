#include "Title.h"

// �R���X�g���N�^
Title::Title()
{

}

// �f�X�g���N�^
Title::~Title()
{

}

// ����������
void Title::Initialize()
{
	// �e�N���X�̏������������Ăяo��
	__super::Initialize();

	// �摜�̓ǂݍ���
	ResourceManager* rm = ResourceManager::GetInstance();


	BackGroundImage = rm->GetImages("Resource/Images/BackGround/Title.png")[0];

	BGM = rm->GetSounds("Resource/Sounds/Title/BGM/OP.mp3");
	DecisionSE = rm->GetSounds("Resource/Sounds/Decision.mp3");

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

	// �C���Q�[���V�[���ɑJ�ڂ���
	if (input->GetKeyState(KEY_INPUT_RETURN)==eInputState::Pressed)
	{
		PlaySoundMem(DecisionSE, DX_PLAYTYPE_BACK);
		return eSceneType::stage_select;
	}
	if (input->GetButtonState(XINPUT_BUTTON_A) == eInputState::Pressed)
	{
		PlaySoundMem(DecisionSE, DX_PLAYTYPE_BACK);
		return eSceneType::stage_select;
	}
	if (input->GetKeyState(KEY_INPUT_SPACE) == eInputState::Pressed)
	{
		PlaySoundMem(DecisionSE, DX_PLAYTYPE_BACK);
		return eSceneType::end;
	}
	if (input->GetButtonState(XINPUT_BUTTON_B) == eInputState::Pressed)
	{
		PlaySoundMem(DecisionSE, DX_PLAYTYPE_BACK);
		return eSceneType::end;
	}

	Anim_flame += delta_second;
	if (Anim_flame >= 1.0f)
	{
		Anim_count ++;
		Anim_flame = 0.0f;
	}
	if (Anim_count > 1)
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

	DrawGraph(0, 0, BackGroundImage, 0);

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

	// �e�N���X�̕`�揈�����Ăяo��
	__super::Draw();

}

// �I������
void Title::Finalize()
{

	StopSoundMem(BGM);

	// �e�N���X�̏I�����������Ăяo��
	__super::Finalize();
}

// ���݂̃V�[���^�C�v�擾����
const eSceneType Title::GetNowSceneType() const
{
	return eSceneType::title;
}