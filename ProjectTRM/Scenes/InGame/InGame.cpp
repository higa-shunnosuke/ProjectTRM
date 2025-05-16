#include "InGame.h"
#include "DxLib.h"
#include <fstream>

#include "../../Utility/StageData.h"
#include "../../Utility/Camera/Camera.h"

#include "../../Objects/Block/Ground.h"
#include "../../Objects/Character/Player/Melee/P_Melee.h"
#include "../../Objects/Character/Player/Ranged/P_Ranged.h"
#include "../../Objects/Character/Player/Tank/P_Tank.h"

#include "../../Objects/Character/Enemy/Melee/E_Melee.h"
#include "../../Objects/Character/Enemy/Tank/E_Tank.h"
#include "../../Objects/Character/Enemy/Ranged/E_Ranged.h"

// �R���X�g���N�^
InGame::InGame():
	cost(),
	cooldown(),
	summon_flag(),
	player(nullptr),
	enemy(nullptr),
	unit_ui(),
	Cost_Click_Count(),
	Cost_value(1)
{
	
}

// �f�X�g���N�^
InGame::~InGame()
{
	// ����Y��h�~
	Finalize();
}

// ����������
void InGame::Initialize()
{
	// �e�N���X�̏������������Ăяo��
	__super::Initialize();




	// �摜�̓ǂݍ���
	ResourceManager* rm = ResourceManager::GetInstance();
	unit_ui[0]= rm->GetImages("Resource/Images/Unit/Tank/Tank_Walk.png", 4, 4, 1, 32, 32)[0]	;
	unit_ui[1]= rm->GetImages("Resource/Images/Unit/Melee/Melee_Walk.png", 4, 4, 1, 32, 32)[0]	;
	unit_ui[2] = rm->GetImages("Resource/Images/Unit/Ranged/Ranged_Walk.png", 4, 4, 1, 32, 32)[0];
	unit_ui[3] = rm->GetImages("Resource/Images/BackGround/Sun.png")[0];
	BackGroundImage[0] = rm->GetImages("Resource/Images/BackGround/BlueMoon.png")[0];
	BackGroundImage[1] = rm->GetImages("Resource/Images/BackGround/YelloMoon.png")[0];
	BackGroundImage[2]= rm->GetImages("Resource/Images/BackGround/RedMoon.png")[0];

	Click = rm->GetSounds("Resource/Sounds/InGame/Click.mp3");
	ClickUp = rm->GetSounds("Resource/Sounds/InGame/ClickUp.mp3");

	// ���C�g�}�b�v�̏�����
	LightMapManager* light_map = LightMapManager::GetInstance();
	light_map->Initialize();

	// �J�������W�̏�����
	Camera* camera = Camera::GetInstance();
	camera->Initialize();

	// �X�e�[�W�ǂݍ���
	//LoadStage();

	// �摜�̓ǂݍ���
	LoadImages();

	// �I�u�W�F�N�g�}�l�[�W���[�̃|�C���^
	GameObjectManager* object = GameObjectManager::GetInstance();


	int locationy = 750;
	int locationx = 1350;
	for (int count = 0; count < 3; count++)
	{
		for (int locationx = 1350; locationx > 0; locationx -= 60)
		{
			object->CreateObject<Ground>(Vector2D(locationx, locationy));
		}
		locationy -= 60;
	}

	switch (StageNumber)
	{
	case 1:
		player = object->CreateObject<Oracle>(Vector2D(1000, 630));

		enemy = object->CreateObject<Heretic>(Vector2D(100, 630));
		enemy->SetInGamePoint(this);
		break;

	case 2:
		player = object->CreateObject<Oracle>(Vector2D(1170, 630));

		enemy = object->CreateObject<Heretic>(Vector2D(30, 630));
		enemy->SetInGamePoint(this);
		break;
	default:
		player = object->CreateObject<Oracle>(Vector2D(1170, 630));

		enemy = object->CreateObject<Heretic>(Vector2D(30, 630));
		enemy->SetInGamePoint(this);
		break;
	}

	// �J�[�\���̏�����
	cursor = 0;

	// �R�X�g�̏����� 
	cost = 0;	
	prev_time = std::chrono::steady_clock::now();

	//// �N�[���_�E�� / �����t���O�̏�����
	//for (int i = 0; i < 3; i++)
	//{
	//	cooldown[i] = std::chrono::seconds((i+1) * 2);
	//	summon_flag[i] = false;
	//}
}

// �X�V����
eSceneType InGame::Update(const float& delta_second)
{
	move_camera = 0.0f;

	if (enemy->GetHP() <= 0 || player->GetHP() <= 0)
	{
		if(player->GetHP() <= 0)
		{
			IsPlayerWin(false);
		return eSceneType::result;
		}
		else	
		{
			IsPlayerWin(true);
			return eSceneType::result;
		}
	}

	// ���͏����擾
	InputManager* input = InputManager::GetInstance();

	//�J�����̏����擾
	Camera* camera = Camera::GetInstance();

	//�J�����̍X�V
	camera->Update();

	//�@�|�C���g���Z�₟������
	if ((input->GetKeyState(KEY_INPUT_Y) == eInputState::Pressed) || (input->GetButtonState(XINPUT_BUTTON_Y) == eInputState::Pressed))
	{
		cost += 1;
	}

	// ���U���g�V�[���ɑJ�ڂ���
	if (input->GetKeyState(KEY_INPUT_RETURN) == eInputState::Pressed)
	{
		return eSceneType::result;
	}
	else if (input->GetButtonState(XINPUT_BUTTON_START) == eInputState::Pressed)
	{
		return eSceneType::result;
	}
	
	// ���j�b�g�I������
	UnitSelection();

	// �R�X�g�Ǘ�����
	RegenerateCost();

	//// �N�[���_�E���Ǘ�����
	//CooldownManagement(delta_second);

#ifdef DEBUG
	
#endif // DEBUG

	if (old_camerapos.x != camera->GetCameraPos().x)
	{
		move_camera = old_camerapos.x - camera->GetCameraPos().x;
	old_camerapos = camera->GetCameraPos();

	}


	// �e�N���X�̍X�V�������Ăяo��
	return __super::Update(delta_second);
}

// �`�揈��
void InGame::Draw() const
{

	// �J�����̏��擾
	Camera* camera = Camera::GetInstance();
	float ShowBackGround_Y = 0;
	switch (StageNumber)
	{
	case 1:
		ShowBackGround_Y = -120.0f;
		break;
	case 2:
		ShowBackGround_Y = -100.0f;
		break;
	case 3:
		 ShowBackGround_Y = -125.0f;
		break;
	default:
		break;
	}
		DrawGraph(camera->GetCameraPos().x - 700.0f, ShowBackGround_Y, BackGroundImage[StageNumber-1], 0);

	

	// �������Z����
	LightMapManager* light_map = LightMapManager::GetInstance();
	light_map->DrawLights(camera->GetCameraPos());
	
	// �e�N���X�̕`�揈�����Ăяo��
	__super::Draw();

	// ���C�g�}�b�v��`��
	light_map->DrawLightMap();

	// �{�^���T�C�Y
	const int button_width = 200;
	const int button_height = 200;

	// �{�^���̐�
	const int button_count = 4;

	// �{�^���̑��������v�Z
	int total_buttons_width = button_count * button_width;
	// �Ԋu�̑��������v�Z
	int total_spacing = D_WIN_MAX_X - total_buttons_width;
	// �{�^���̊Ԋu�̌v�Z
	int spacing = total_spacing / (button_count + 1);

	for (int i = 0; i < button_count; ++i) 
	{
		//	�I�����̈ʒu
		int x = spacing + i * (button_width + spacing);
		int y = 60;

		// �g��p�ϐ�
		int w = button_width;
		int h = button_height;

		if (i == cursor) 
		{
			// �I�𒆂̂��̂����g��i1.2�{�j
			int w = (int)(button_width * 1.2);
			int h = (int)(button_height * 1.2);

			// �g�i�w�i�j��`��
			DrawBox(x - (w - button_width) / 2, y - (h - button_height) / 2, x + w, y + h, GetColor(255, 255, 255), TRUE);
			//�L�����̕`��͈͂𐧌�
			SetDrawArea(x - (w - button_width) / 2, y - (h - button_height) / 2, x + w, y + h);

			// �L�����摜�𒆐S�ɕ`��
			DrawExtendGraph(
				(int)(x + (button_width - w * 1.5) / 2), (int)(y + (button_height - h * 1.5) / 2),
				(int)(x + (button_width + w * 1.7) / 2), (int)(y + (button_height + h * 1.7) / 2),
				unit_ui[i], TRUE);
		}
		else
		{
			// �g�i�w�i�j��`��
			DrawBox(x, y, x + button_width, y + button_height, GetColor(100, 100, 100), TRUE);
			//�L�����̕`��͈͂𐧌�
			SetDrawArea(x, y, x + button_width, y + button_height);

			// �L�����摜�𒆐S�ɕ`��
			DrawExtendGraph(
				(int)(x + (button_width - w * 1.5) / 2), (int)(y + (button_height - h * 1.5) / 2),
				(int)(x + (button_width + w * 1.5) / 2), (int)(y + (button_height + h * 1.5) / 2),
				unit_ui[i], TRUE);
		}

		// �`��͈͂����ɖ߂�
		SetDrawArea(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y);
	}

	DrawFormatString(1000, 40, 0x00ffff, "Level:%d", Cost_value);

	// �R�X�g�\��
	DrawFormatString(1200, 10, 0xffffff, "%d", cost);


#if _DEBUG
	// �J�������W�`��
	DrawFormatString(500, 300, 0xffffff, "%f", camera->GetCameraPos().x);

	// �V�[�����̕`��
	SetFontSize(60);
	DrawFormatString(0, 0, 0xffffff, "InGame");
	SetFontSize(32);
	DrawFormatString(100, 300, 0xffffff, "Space�������Ă�������");
	DrawFormatString(1000, 0, 0xffffff, "Stage:%d", StageNumber);

#endif
}

// �I������
void InGame::Finalize()
{
	// �e�N���X�̏I�����������Ăяo��
	__super::Finalize();

	//SceneBase::win_flg = this->win_flg;

}

// ���݂̃V�[���^�C�v�����擾����
const eSceneType InGame::GetNowSceneType() const
{
	return eSceneType::in_game;
}


// �G��������
void InGame::CreateEnemy(E_enemy e_enem)
{
	// �I�u�W�F�N�g�}�l�[�W���[�̏����擾
	GameObjectManager* object = GameObjectManager::GetInstance();

	switch (e_enem)
	{
	case Tank:
		object->CreateObject<E_Tank>(Vector2D(enemy->GetLocation().x, enemy->GetLocation().y + 30.0f));
		break;
	case Melee:
		object->CreateObject<E_Melee>(Vector2D(enemy->GetLocation().x, enemy->GetLocation().y + 30.0f));
		break;
	case Range:
		object->CreateObject<E_Ranged>(Vector2D(enemy->GetLocation().x, enemy->GetLocation().y + 30.0f));
		break;
	case Boss:
		break;
	default:
		break;
	}
}

//	���j�b�g����
void InGame::UnitSelection()
{
	// ���͏����擾
	InputManager* input = InputManager::GetInstance();

	// �J�[�\������
	if (input->GetKeyState(KEY_INPUT_RIGHT) == eInputState::Pressed ||
		input->GetButtonState(XINPUT_BUTTON_DPAD_RIGHT) == eInputState::Pressed)
	{
		if (cursor < 3)
		{
			cursor++;
		}
	}
	if (input->GetKeyState(KEY_INPUT_LEFT) == eInputState::Pressed ||
		input->GetButtonState(XINPUT_BUTTON_DPAD_LEFT) == eInputState::Pressed)
	{
		if (cursor > 0)
		{
			cursor--;
		}
	}

	// ����
	if (input->GetKeyState(KEY_INPUT_SPACE) == eInputState::Pressed ||
		input->GetButtonState(XINPUT_BUTTON_A) == eInputState::Pressed)
	{
		// �I�u�W�F�N�g�}�l�[�W���[�̃|�C���^
		GameObjectManager* object = GameObjectManager::GetInstance();

		switch (cursor)
		{
			// �^���N����
		case 0:
			if (summon_flag[cursor] == false)
			{
				if (cost - Tank_Cost >= 0)
				{
					// �^���N�𐶐�
					GameObject* obj = object->CreateObject<P_Tank>(Vector2D(player->GetLocation().x, player->GetLocation().y + 30.0f));
					cost -= Tank_Cost;
					
					//summon_flag[cursor] = true;
					summon_time[cursor] = std::chrono::steady_clock::now();
				}
			}
			break;
			// �ߐڏ���
		case 1:
			if (summon_flag[cursor] == false)
			{
				if (cost - Melee_Cost >= 0)
				{
					// �ߐڂ𐶐�
					GameObject* obj = object->CreateObject<P_Melee>(Vector2D(player->GetLocation().x, player->GetLocation().y + 30.0f));
					cost -= Melee_Cost;
					//summon_flag[cursor] = true;
					summon_time[cursor] = std::chrono::steady_clock::now();
				}
			}
			break;
			// ����������
		case 2:
			if (summon_flag[cursor] == false)
			{
				if (cost - Range_Cost >= 0)
				{
					object->CreateObject<P_Ranged>(Vector2D(player->GetLocation().x, player->GetLocation().y + 30.0f));
					cost -= Range_Cost;
					//summon_flag[cursor] = true;
					summon_time[cursor] = std::chrono::steady_clock::now();
				}
			}
			break;
		case 3:
			cost += Cost_value;

			Cost_Click_Count++;
			if (Cost_Click_Count > 10 * Cost_value)
			{
				Cost_Click_Count = 0;
				Cost_value++;
				PlaySoundMem(ClickUp, DX_PLAYTYPE_BACK);
			}
			else
			{
			PlaySoundMem(Click, DX_PLAYTYPE_BACK);
			}

			break;
		}
	}
}

//	�R�X�g�Ǘ�����
void InGame::RegenerateCost()
{
	auto now_time = std::chrono::steady_clock::now();

	if (now_time - prev_time > std::chrono::milliseconds(500))
	{
		cost++;
		prev_time = std::chrono::steady_clock::now();
	}
}

// �N�[���_�E������
void InGame::CooldownManagement()
{
	auto now_time = std::chrono::steady_clock::now();

	for (int i = 0; i < 3; i++)
	{
		if (summon_flag[i] == false)
		{
			continue;
		}
		else
		{
			switch (i)
			{
			case 0:
				if (now_time - summon_time[0] >= cooldown[0])
				{
					summon_flag[0] = false;
				}
				break;
			case 1:
				if (now_time - summon_time[1] >= cooldown[1])
				{
					summon_flag[1] = false;
				}
				break;
			case 2:
				if (now_time - summon_time[2] >= cooldown[2])
				{
					summon_flag[2] = false;
				}
				break;
			}
		}
	}
}

const void InGame::LoadImages() const
{
	//���\�[�X�}�l�[�W���[�̃C���X�^���X�擾
	ResourceManager* rm = ResourceManager::GetInstance();

	//�摜�Ǎ���
	rm->GetImages("Resource/Images/Unit/Melee/Melee_Walk.png", 4, 4, 1, 32, 32);
	rm->GetImages("Resource/Images/Unit/Tank/Tank_Walk.png", 4, 4, 1, 32, 32);
	rm->GetImages("Resource/Images/Unit/Ranged/Ranged_Walk.png", 4, 4, 1, 32, 32);
	rm->GetImages("Resource/Images/Enemy/Melee/Melee_Walk.png", 4, 4, 1, 32, 32);
	rm->GetImages("Resource/Images/Enemy/Tank/Tank_Walk.png", 4, 4, 1, 32, 32);
	rm->GetImages("Resource/Images/Enemy/Ranged/Ranged_Walk.png", 4, 4, 1, 32, 32);
	rm->GetImages("Resource/Images/Unit/Melee/Melee_Attack.png", 4, 4, 1, 32, 32);
	rm->GetImages("Resource/Images/Unit/Tank/Tank_Attack.png", 4, 4, 1, 32, 32);
	rm->GetImages("Resource/Images/Unit/Ranged/Ranged_Attack.png", 4, 4, 1, 32, 32);
	rm->GetImages("Resource/Images/Enemy/Melee/Melee_Attack.png", 4, 4, 1, 32, 32);
	rm->GetImages("Resource/Images/Enemy/Tank/Tank_Attack.png", 4, 4, 1, 32, 32);
}