#include "Camera.h"
#include "../ProjectConfig.h"
#include "../Input/InputManager.h"

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
		location.x -= 1.0f;
	}
	if (input->GetLeftStick().x > 0.3f)
	{
		location.x += 1.0f;
	}

	//�X�e�[�W�O�ɂ����Ȃ��悤�ɂ��鏈��
	float right;
	right = STAGE_SIZE_X * BOX_SIZE - D_WIN_MAX_X / 2;
	if (location.x > right)
	{
		location.x = right;
	}
	if (location.x < D_WIN_MAX_X / 2)
	{
		location.x = D_WIN_MAX_X / 2;
	}
}

// �J�������擾����
Vector2D Camera::GetCameraPos()
{
	return this->location;
}

// �ǐՑΏۏ��ݒ菈��
void Camera::SetCameraPos(Vector2D location)
{
	this->location = location;
}