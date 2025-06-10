#include "Camera.h"
#include "../ProjectConfig.h"
#include "../Input/InputManager.h"
#include "DxLib.h"

// �R���X�g���N�^
Camera::Camera():
	location(),
	size()
{

}

// �f�X�g���N�^
Camera::~Camera()
{

}

// ����������
void Camera::Initialize()
{
	// �X�N���[���̏����T�C�Y��ݒ�
	this->size.x = D_WIN_MAX_X;
	this->size.y = D_WIN_MAX_Y;

	// �J�����̏������W��ݒ�
	this->location.x = STAGE_WIDTH - (size.x / 2);
	this->location.y = size.y / 2;
}

// �X�V����
void Camera::Update()
{
	//�ǐՏ���
	InputManager* input = InputManager::GetInstance();

	if (input->GetLeftStick().x < 0.0f)
	{
		location.x += input->GetLeftStick().x;
	}
	if (input->GetLeftStick().x > 0.3f)
	{
		location.x += input->GetLeftStick().x;
	}

#ifdef DEBUG
	//�L�[�{�[�h����
	if (input->GetKeyState(KEY_INPUT_D) == eInputState::Hold)
	{
		location.x += 2.0f;
	}
	if (input->GetKeyState(KEY_INPUT_A) == eInputState::Hold)
	{
		location.x -= 2.0f;
	}
#endif

	/// �X�e�[�W�O�ɂ����Ȃ��悤�ɂ��鏈��
	float right,left;
	right = STAGE_WIDTH - (size.x / 2);
	left = size.x / 2;

	//�E�[�̐���
	if (location.x > right)
	{
		location.x = right;
	}
	//���[�̐���
	if (location.x < left)
	{
		location.x = left;
	}
}

// �J�������W�擾����
Vector2D Camera::GetCameraPos()
{
	return this->location;
}

// �J�������W�ݒ菈��
void Camera::SetCameraPos(Vector2D location)
{
	this->location = location;
}

// �X�N���[���T�C�Y�擾����
Vector2D Camera::GetScreeenSize()
{
	return this->size;
}
