#include "Camera.h"
#include "../ProjectConfig.h"
#include "../Input/InputManager.h"
#include "DxLib.h"

// �R���X�g���N�^
Camera::Camera():
	location()
{

}

// �f�X�g���N�^
Camera::~Camera()
{

}

// ����������
void Camera::Initialize()
{
	// �J�����̏������W��ݒ�
	this->location.x = D_WIN_MAX_X / 2;
	this->location.y = D_WIN_MAX_Y / 2;
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
	if (input->GetKeyState(KEY_INPUT_RIGHT) == eInputState::Hold)
	{
		location.x += 1.0f;
	}
	if (input->GetKeyState(KEY_INPUT_LEFT) == eInputState::Hold)
	{
		location.x -= 1.0f;
	}
#endif

	/// �X�e�[�W�O�ɂ����Ȃ��悤�ɂ��鏈��
	float right,left;
	right = D_WIN_MAX_X / 2 + 20;
	left = right - D_WIN_MAX_X - 20;

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