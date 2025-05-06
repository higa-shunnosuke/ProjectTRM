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
	this->location.x = BOX_SIZE * STAGE_SIZE_X - (D_WIN_MAX_X / 2);
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
	right = D_WIN_MAX_X / 2 + 40;
	left = D_WIN_MAX_X / 2;

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