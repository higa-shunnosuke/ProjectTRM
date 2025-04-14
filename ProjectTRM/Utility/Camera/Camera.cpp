#include "Camera.h"
#include "../ProjectConfig.h"

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


	//�X�e�[�W�O�ɂ����Ȃ��悤�ɂ��鏈��
	float right;
	right = STAGE_SIZE_X * BOX_SIZE - D_WIN_MAX_X / 2;
	if (location.x > right)
	{
		location.x = right;
	}
}

// �J�������擾����
Vector2D Camera::GetCameraPos()
{
	return this->location;
}

// �ǐՑΏۏ��ݒ菈��
void Camera::SetPlayer()
{
	
}