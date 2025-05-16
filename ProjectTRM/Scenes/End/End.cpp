#include "End.h"
#include "../../Application.h"

// �R���X�g���N�^
End::End()
{

}

// �f�X�g���N�^
End::~End()
{

}

// ����������
void End::Initialize()
{
	// �e�N���X�̏������������Ăяo��
	__super::Initialize();

	// �摜�̓ǂݍ���
	ResourceManager* rm = ResourceManager::GetInstance();

}

// �X�V����
eSceneType End::Update(const float& delta_second)
{
	// ���͏����擾
	InputManager* input = InputManager::GetInstance();

	// ENTER�L�[�ASTART�{�^���������ꂽ��
	if (input->GetKeyState(KEY_INPUT_RETURN) == eInputState::Pressed ||
		input->GetButtonState(XINPUT_BUTTON_A) == eInputState::Pressed)
	{
		//�Q�[�����I��������
		Application* app = Application::GetInstance();
		app->QuitGame();
	}

	// �e�N���X�̍X�V�������Ăяo��
	return __super::Update(delta_second);
}

// �`�揈��
void End::Draw() const
{
	SetFontSize(60);
	DrawFormatString(120, 140, 0xffffff, "End");
	SetFontSize(32);
	DrawFormatString(100, 300, 0xffffff, "A�{�^���������Ă�������");

	// �e�N���X�̕`�揈�����Ăяo��
	__super::Draw();

}

// �I������
void End::Finalize()
{
	// �e�N���X�̏I�����������Ăяo��
	__super::Finalize();
}

// ���݂̃V�[���^�C�v�擾����
const eSceneType End::GetNowSceneType() const
{
	return eSceneType::end;
}
