#include "Result.h"

// �R���X�g���N�^
Result::Result()
{
	// �e�N���X�̏������������Ăяo��
	__super::Initialize();
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

}

// �X�V����
eSceneType Result::Update(const float& delta_second)
{
	// ���͏����擾
	InputManager* input = InputManager::GetInstance();

	// �C���Q�[���V�[���ɑJ�ڂ���
	if (input->GetKeyState(KEY_INPUT_RETURN) == eInputState::Pressed)
	{
		return eSceneType::end;
	}
	if (input->GetButtonState(XINPUT_BUTTON_START) == eInputState::Pressed)
	{
		return eSceneType::title;
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
	DrawFormatString(100, 300, 0xffffff, "Enter�������Ă�������");

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
