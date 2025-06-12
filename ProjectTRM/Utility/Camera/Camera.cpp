#include "Camera.h"
#include "../ProjectConfig.h"
#include "../Input/InputManager.h"
#include "../../Objects/GameObjectManager.h"
#include "../Collision.h"
#include <cmath>

// �R���X�g���N�^
Camera::Camera():
	location(),
	size(),
	oracle(nullptr),
	tracking_flag(false)
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
	this->location.x = ProjectConfig::STAGE_WIDTH - (size.x / 2);
	this->location.y = size.y / 2;
}

// �X�V����
void Camera::Update()
{
	//�ǐՏ���
	InputManager* input = InputManager::GetInstance();

	// �蓮�X�N���[��
	if (input->GetLeftStick().x < 0.0f)
	{
		tracking_flag = false;
		location.x += (float)input->GetLeftStick().x * 1.5;
	}
	else if (input->GetLeftStick().x > 0.0f)
	{
		tracking_flag = false;
		location.x += (float)input->GetLeftStick().x * 1.5;
	}

	// �����X�N���[��
	if (tracking_flag == true)
	{
		// �I�u�W�F�N�g�}�l�[�W���[�̏��擾
		GameObjectManager* object = GameObjectManager::GetInstance();
		// �I�u�W�F�N�g���X�g���擾
		std::vector<GameObject*> objects_list = object->GetObjectsList();

		// �Œ�������������
		float longest_distance = 0.0f;
		GameObject* front_line = nullptr;

		if (!objects_list.empty() && oracle != nullptr)
		{
			for (GameObject* obj : objects_list)
			{
				// ���j�b�g�ȊO�͖�������
				if (obj->GetCollision().object_type != eObjectType::Player ||
					obj->GetMobility() == false)
				{
					continue;
				}

				// �ޏ��ƃ��j�b�g�̋������v�Z
				Vector2D diff = obj->GetLocation() - oracle->GetLocation();
				float distance = sqrtf(diff.x * diff.x + diff.y * diff.y);

				// �Œ��������X�V
				if (distance > longest_distance)
				{
					longest_distance = distance;
					front_line = obj;
				}
			}
			// �O����ǐ�
			if (front_line != nullptr)
			{
				location.x = front_line->GetLocation().x;
			}
		}
	}
	
	// �őO���ֈړ�
	if (input->GetButtonState(XINPUT_BUTTON_LEFT_SHOULDER) == eInputState::Pressed)
	{
		tracking_flag = true;
	}

	// �������W�ֈړ�
	if (input->GetButtonState(XINPUT_BUTTON_RIGHT_SHOULDER) == eInputState::Pressed)
	{
		tracking_flag = false;
		// �J�����̍��W��������
		this->location.x = ProjectConfig::STAGE_WIDTH - (size.x / 2);
		this->location.y = size.y / 2;
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
	right = ProjectConfig::STAGE_WIDTH - (size.x / 2);
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

// �ޏ����ݒ菈��
void Camera::SetPlayer(Oracle* oracle)
{
	this->oracle = oracle;
}