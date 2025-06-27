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
	// �Y�[���{����ݒ�
	zoom = 2.0f;

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
	InputManager* input = InputManager::GetInstance();

	// �Y�[��
	Zoom();

	// �X�N���[��
	Scroll();

	// �őO���ֈړ�
	if (input->GetButtonState(XINPUT_BUTTON_LEFT_SHOULDER) == eInputState::Pressed)
	{
		tracking_flag = true;
	}

	// �������W�ֈړ�
	if (input->GetButtonState(XINPUT_BUTTON_RIGHT_SHOULDER) == eInputState::Pressed)
	{
		tracking_flag = false;
		// �J�����̏�����
		zoom = 2.0f;
		this->location.x = ProjectConfig::STAGE_WIDTH - (size.x / 2);
		this->location.y = size.y / 2;
	}

	/// �X�e�[�W�O�ɂ����Ȃ��悤�ɂ��鏈��
	float maxCameraX = 1280 - (1280 / zoom);
	if (location.x < 0) location.x = 0.0f;
	if (location.x > maxCameraX) location.x = maxCameraX;
}

// �`�揈��
void Camera::Draw(int back_buffer)
{
	// �X�P�[�����O��̃T�C�Y
	int drawW = (int)(D_WIN_MAX_X * zoom);
	int drawH = (int)(D_WIN_MAX_Y * zoom);

	// �`��ʒu�iX �̓X�N���[���ɉ����āAY �͉����킹�j
	int drawX = -(int)(location.x * zoom);
	int drawY = 720 - drawH;  // ��ʉ��[�ɍ��킹��

	// �w�i���g��E�k���{�X�N���[�����ĕ`��
	DrawExtendGraph(drawX, drawY, drawX + drawW, drawY + drawH, back_buffer, TRUE);
}

// �Y�[������
void Camera::Zoom()
{
	// �Y�[���ύX�O�̒l���L�^
	float prevZoom = zoom;

	// �}�E�X
	int wheel = GetMouseWheelRotVol();
	if (wheel != 0) 
	{
		// �Y�[���{�����v�Z
		zoom += wheel * 0.1f;
	}
	// �R���g���[���[
	InputManager* input = InputManager::GetInstance();
	if (input->GetLeftTrigger() > 0.0f)
	{
		// �{�����v�Z
		zoom -= input->GetLeftTrigger() * 0.01f;
	}
	if (input->GetRightTrigger() > 0.0f)
	{
		// �{�����v�Z
		zoom += input->GetRightTrigger() * 0.01f;
	}


	if (zoom < ZOOM_MIN) zoom = ZOOM_MIN;
	if (zoom > ZOOM_MAX) zoom = ZOOM_MAX;

	// ��ʒ����̃��[���h���W��ۂ��߂ɃJ�����ʒu��␳
	float centerX = location.x + (640.0f / prevZoom);
	float centerY = location.y + (360.0f / prevZoom);

	location.x = centerX - (640.0f / zoom);
	location.y = centerY - (360.0f / zoom);
}

// �X�N���[������
void Camera::Scroll()
{
	InputManager* input = InputManager::GetInstance();

	// �蓮�X�N���[���i�R���g���[���[�j
	if (input->GetLeftStick().x < 0.0f)
	{
		tracking_flag = false;
		location.x += (float)(input->GetLeftStick().x * 1.5);
	}
	else if (input->GetLeftStick().x > 0.0f)
	{
		tracking_flag = false;
		location.x += (float)(input->GetLeftStick().x * 1.5);
	}
	// �蓮�X�N���[���i�L�[�{�[�h�j
	if (input->GetKeyState(KEY_INPUT_D) == eInputState::Hold)
	{
		location.x += 2.0f;
	}
	if (input->GetKeyState(KEY_INPUT_A) == eInputState::Hold)
	{
		location.x -= 2.0f;
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
}

// �J�������W�ݒ菈��
void Camera::SetCameraPos(Vector2D location)
{
	this->location = location;
}

// �ޏ����ݒ菈��
void Camera::SetPlayer(Oracle* oracle)
{
	this->oracle = oracle;
}

void Camera::SetZoom(float zoom)
{
	this->zoom = zoom;
}

float Camera::GetZoom()
{
	return zoom;
}