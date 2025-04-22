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

#define PLAYER_INITIAL_LOCATION 3 // �v���C���[�����ʒu(���u���b�N�ځj

// �R���X�g���N�^
InGame::InGame():
	cooldown()
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

	// �J�����̏����擾
	Camera* camera = Camera::GetInstance();
	// �J�������W�̏�����
	camera->SetCameraPos(Vector2D(D_WIN_MAX_X / 2, D_WIN_MAX_Y / 2));

	// �X�e�[�W�ǂݍ���
	LoadStage();

	// �J�[�\���̏�����
	cursor = 1;
}

// �X�V����
eSceneType InGame::Update(const float& delta_second)
{
	// ���͏����擾
	InputManager* input = InputManager::GetInstance();

	//�J�����̏����擾
	Camera* camera = Camera::GetInstance();

	//�J�������̍X�V
	camera->Update();

	// ���U���g�V�[���ɑJ�ڂ���
	if (input->GetKeyState(KEY_INPUT_RETURN) == eInputState::Pressed)
	{
		return eSceneType::result;
	}
	if (input->GetButtonState(XINPUT_BUTTON_START) == eInputState::Pressed)
	{
		return eSceneType::result;
	}
	
	// ���j�b�g�I������
	UnitSelection();

	// �e�N���X�̍X�V�������Ăяo��
	return __super::Update(delta_second);
}

// �`�揈��
void InGame::Draw() const
{
	// �e�N���X�̕`�揈�����Ăяo��
	__super::Draw();

	//���j�b�g�I���̕`��
	int options_size;	// �I�����̊g�嗦
	int space_size;		// ���Ԃ̑傫��

	// �p�l���̕`��
	DrawBox(100, 50, 1180, 290, 0xffffff, TRUE);
	
	// �I�����̕`��
	for (int i = 0; i < 3; i++)
	{
		if (i+1 == cursor)
		{
			options_size = 200;
			space_size = 140;

			DrawBox(60 + space_size + i * (options_size + space_size), 50 + 20,
					60 + space_size + i * (options_size + space_size) + options_size, 50 + 20 + options_size, 0x00ff00, TRUE);
		}
		else
		{
			options_size = 160;
			space_size = 170;

			DrawBox(60 + space_size + i * (options_size + space_size), 50 + 40,
					60 + space_size + i * (options_size + space_size) + options_size, 50 + 40 + options_size, 0xff0000, TRUE);
		}
	}

#if _DEBUG	
	// �p�l���̃}�X�ڕ`��
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			DrawBox(j * 40, 50 + i * 40,
				(j + 1) * 40, 50 + (i + 1) * 40, 0x0000ff, FALSE);
		}
	}

	DrawFormatString(290, 155, 0x000000, "A");
	DrawFormatString(635, 155, 0x000000, "B");
	DrawFormatString(970, 155, 0x000000, "C");

	// �V�[�����̕`��
	SetFontSize(60);
	DrawFormatString(0, 0, 0xffffff, "InGame");
	SetFontSize(32);
	DrawFormatString(100, 300, 0xffffff, "Space�������Ă�������");
#endif

}

// �I������
void InGame::Finalize()
{
	// �e�N���X�̏I�����������Ăяo��
	__super::Finalize();
}

// ���݂̃V�[���^�C�v�����擾����
const eSceneType InGame::GetNowSceneType() const
{
	return eSceneType::in_game;
}

// �X�e�[�W��������
void InGame::LoadStage()
{
	// �I�u�W�F�N�g�}�l�[�W���[�̃|�C���^
	GameObjectManager* object = GameObjectManager::GetInstance();
	
	FILE* fp = NULL;
	std::string file_name = "Resource/Map/Stage1.csv";

	// �w�肳�ꂽ�t�@�C�����J��
	errno_t result = fopen_s(&fp, file_name.c_str(), "r");

	// �G���[�`�F�b�N
	if (result != 0)
	{
		throw (file_name + "���J���܂���");
	}

	// �J�E���g�p�ϐ�
	int x = 0;			// ��
	int y = 0;			// �s

	// �t�@�C�����̕������m�F���Ă���
	while (true)
	{
		//���W�v�Z
		Vector2D location;
		location.x = (float)(x * BOX_SIZE + BOX_SIZE / 2 - D_WIN_MAX_X);
		location.y = (float)(y * BOX_SIZE + BOX_SIZE / 2);

		// �t�@�C������1�������o
		int c = fgetc(fp);

		// EOF�Ȃ烋�[�v�I��
		if (c == EOF)
		{
			break;
		}
		// ���s�����Ȃ�A���̍s��
		else if (c == '\n')
		{
			x = 0;
			y++;
		}
		// 0�Ȃ�A���̕�����
		else if (c == '0')
		{
			x++;
		}
		// G�Ȃ�A�n�ʂ𐶐�
		else if (c == 'G')
		{
			object->CreateObject<Ground>(Vector2D(location.x, location.y));
			x++;
		}
		// O�Ȃ�A�ޏ��𐶐�
		else if (c == 'O')
		{
			player = object->CreateObject<Oracle>(Vector2D(location.x, location.y - 30));
			x++;
		}
		// H�Ȃ�A�ْ[��
		else if (c == 'H')
		{
			enemy = object->CreateObject<Heretic>(Vector2D(location.x, location.y - 30));
			x++;
		}
	}
	// �J�����t�@�C�������
	fclose(fp);
}

//	���j�b�g�Ǐ���
void InGame::UnitSelection()
{
	// ���͏����擾
	InputManager* input = InputManager::GetInstance();

	// �J�[�\������
	if (input->GetKeyState(KEY_INPUT_RIGHT) == eInputState::Pressed)
	{
		if (cursor < 3)
		{
			cursor++;
		}
	}
	if (input->GetKeyState(KEY_INPUT_LEFT) == eInputState::Pressed)
	{
		if (cursor > 1)
		{
			cursor--;
		}
	}

	// ����
	if (input->GetKeyState(KEY_INPUT_SPACE) == eInputState::Pressed)
	{
		// �I�u�W�F�N�g�}�l�[�W���[�̃|�C���^
		GameObjectManager* object = GameObjectManager::GetInstance();

		switch (cursor)
		{
		case 1:
			object->CreateObject<P_Melee>(Vector2D(player->GetLocation().x, player->GetLocation().y + 30.0f));
			break;
		case 2:
			object->CreateObject<P_Ranged>(Vector2D(player->GetLocation().x, player->GetLocation().y + 30.0f));
			break;
		case 3:
			object->CreateObject<P_Tank>(Vector2D(player->GetLocation().x, player->GetLocation().y + 30.0f));
			break;
		}

		object->CreateObject<E_Melee>(Vector2D(enemy->GetLocation().x, enemy->GetLocation().y + 30.0f));

	}
}