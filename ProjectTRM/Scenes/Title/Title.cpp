#include "Title.h"
#include "DxLib.h"

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
	
}

// �X�V����
eSceneType Title::Update(const float& delta_second)
{
	// ���͏����擾
	InputManager* input = InputManager::GetInstance();

	// �C���Q�[���V�[���ɑJ�ڂ���
	if (input->GetKeyState(KEY_INPUT_RETURN)==eInputState::Pressed)
	{
		return eSceneType::in_game;
	}
	if (input->GetButtonState(XINPUT_BUTTON_START)== eInputState::Pressed)
	{
		return eSceneType::in_game;
	}

	// �e�N���X�̍X�V�������Ăяo��
	return __super::Update(delta_second);
}

// �`�揈��
void Title::Draw() const
{
	SetFontSize(60);
	DrawFormatString(120, 140, 0xffffff, "Title");
	SetFontSize(32);
	DrawFormatString(100, 300, 0xffffff, "Enter�������Ă�������");

	// �e�N���X�̕`�揈�����Ăяo��
	__super::Draw();

}

// �I������
void Title::Finalize()
{
	// �e�N���X�̏I�����������Ăяo��
	__super::Finalize();
}

// ���݂̃V�[���^�C�v�擾����
const eSceneType Title::GetNowSceneType() const
{
	return eSceneType::title;
}
