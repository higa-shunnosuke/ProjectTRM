#include "InGame.h"
#include "DxLib.h"
#include <fstream>

#include "../../Utility/Camera/Camera.h"

#include "../../Objects/Block/Ground.h"
#include "../../Objects/Character/Player/Melee/P_Melee.h"
#include "../../Objects/Character/Player/Ranged/P_Ranged.h"
#include "../../Objects/Character/Player/Tank/P_Tank.h"
#include "../../Objects/Character/Player/Bonfire/Bonfire.h"
#include "../../Objects/Character/Player/Guardian/P_Guardian.h"

#include "../../Objects/Character/Enemy/Melee/E_Melee.h"
#include "../../Objects/Character/Enemy/Tank/E_Tank.h"
#include "../../Objects/Character/Enemy/Ranged/E_Ranged.h"
#include "../../Objects/Character/Enemy/Boss/Boss.h"

// �R���X�g���N�^
InGame::InGame():
	cost(),
	cooldown(),
	summon_flag(),
	player(nullptr),
	enemy(nullptr),
	unit_ui(),
	Sun_Level(1),
	BackGroundImage(),
	Click(),
	ClickUp(),
	Cost_Max(),
	SunImages(),
	sound(),
	bgmHandle(),
	SummonSE(),
	Text_Images(),
	Sun_Images(),
	Text_BackGround(),
	alpha(0)
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
	// �R�X�gUI
	unit_ui[0] = rm->GetImages("Resource/Images/Unit/Tank/Tank_Cost_New.png")[0];
	unit_ui[1] = rm->GetImages("Resource/Images/Unit/Melee/Melee_Cost_New.png")[0];
	unit_ui[2] = rm->GetImages("Resource/Images/Unit/Ranged/Ranged_Cost.png")[0];
	unit_ui[3] = rm->GetImages("Resource/Images/Unit/Guardian/Guardian_Cost.png")[0];
	unit_ui[4] = rm->GetImages("Resource/Images/BackGround/Sun.png")[0];
	// �w�i
	BackGroundImage[0] = rm->GetImages("Resource/Images/BackGround/BlueMoon.png")[0];
	BackGroundImage[1] = rm->GetImages("Resource/Images/BackGround/YelloMoon.png")[0];
	BackGroundImage[2] = rm->GetImages("Resource/Images/BackGround/RedMoon.png")[0];
	//����
	Text_Images[0] = rm->GetImages("Resource/Images/BackGround/text_fail.png")[0];
	Text_Images[1] = rm->GetImages("Resource/Images/BackGround/text_clear.png")[0];
	//���z
	std::string Imagestring, Imagepng, Number;;
	Imagestring = "Resource/Images/BackGround/Sun";
	Imagepng = ".png";

	//for (int i = 0; i < 10; i++)
	//{
	//	Number = std::to_string(i);
	//	SunImages[i] = rm->GetImages(Imagestring + Number + Imagepng)[0];
	//}

	Sun_Images = rm->GetImages(Imagestring + Imagepng)[0];

	Text_BackGround = rm->GetImages("Resource/Images/BackGround/Text.png")[0];


	// �����̓ǂݍ���
	// ����
	Click = rm->GetSounds("Resource/Sounds/InGame/Click.mp3");
	ClickUp = rm->GetSounds("Resource/Sounds/InGame/ClickUp.mp3");
	// BGM
	bgmHandle[0] = rm->GetSounds("Resource/Sounds/Ingame/BGM/Stage1.mp3");
	bgmHandle[1] = rm->GetSounds("Resource/Sounds/Ingame/BGM/Stage2.mp3");
	bgmHandle[2] = rm->GetSounds("Resource/Sounds/Ingame/BGM/Stage3.mp3");
	// ����
	bgmHandle[3] = rm->GetSounds("Resource/Sounds/Result/Win_BGM.mp3");
	//�s�k
	bgmHandle[4] = rm->GetSounds("Resource/Sounds/Result/Loose_BGM.mp3");
	// ����
	SummonSE[0] = rm->GetSounds("Resource/Sounds/Ingame/SummonAllies.mp3");
	SummonSE[1] = rm->GetSounds("Resource/Sounds/Ingame/SummonEnemy.mp3");
	for (int i = 0; i < 2; i++)
	{
		ChangeVolumeSoundMem(100, SummonSE[i]);
	}

	// ���C�g�}�b�v�̏�����
	LightMapManager* light_map = LightMapManager::GetInstance();
	light_map->Initialize();

	// �摜�̓ǂݍ���
	LoadImages();

	// BGM�Đ�
	switch (StageNumber)
	{
	case 1:
		// ���ʐݒ�
		ChangeVolumeSoundMem(100, bgmHandle[0]);
		// BGM�Đ�
		if (PlaySoundMem(bgmHandle[0], DX_PLAYTYPE_LOOP) == -1)
		{
			MessageBoxA(NULL, "BGM1�̍Đ��Ɏ��s���܂���", "�G���[", MB_OK);
		}
		// �X�e�[�W�T�C�Y�ݒ�
		ProjectConfig::STAGE_WIDTH = 1280;
		ProjectConfig::STAGE_HEIGHT = 720;
		break;
	case 2:
		// ���ʐݒ�
		ChangeVolumeSoundMem(100, bgmHandle[1]);
		// BGM�Đ�
		if (PlaySoundMem(bgmHandle[1], DX_PLAYTYPE_LOOP) == -1)
		{
			MessageBoxA(NULL, "BGM2�̍Đ��Ɏ��s���܂���", "�G���[", MB_OK);
		}
		// �X�e�[�W�T�C�Y�ݒ�
		ProjectConfig::STAGE_WIDTH = 1500;
		ProjectConfig::STAGE_HEIGHT = 720;
		break;
	default:
		// ���ʐݒ�
		ChangeVolumeSoundMem(100, bgmHandle[2]);
		// BGM�Đ�
		if (PlaySoundMem(bgmHandle[2], DX_PLAYTYPE_LOOP) == -1)
		{
			MessageBoxA(NULL, "BGM3�̍Đ��Ɏ��s���܂���", "�G���[", MB_OK);
		}
		// �X�e�[�W�T�C�Y�ݒ�
		ProjectConfig::STAGE_WIDTH = 1280;
		ProjectConfig::STAGE_HEIGHT = 720;
		break;
	}

	// �I�u�W�F�N�g�}�l�[�W���[�̃|�C���^
	GameObjectManager* object = GameObjectManager::GetInstance();
	// �J�����̃|�C���^
	Camera* camera = Camera::GetInstance();

	// �ޏ�����
	player = object->CreateObject<Oracle>(Vector2D(ProjectConfig::STAGE_WIDTH - 200, 630));
	player->SetInGamePoint(this);
	// �ْ[�Ґ���
	enemy = object->CreateObject<Heretic>(Vector2D(200, 630));
	enemy->SetInGamePoint(this);
	// ⾉ΐ���
	for (int i = 1; i < 3; i++)
	{
		object->CreateObject<Bonfire>(Vector2D(player->GetLocation().x - (250 * i), 630));
	}
	// �J��������
	camera->Initialize();
	camera->SetPlayer(player);

	// �J�[�\���̏�����
	cursor = 0;

	// �R�X�g�̏����� 
	cost = 0;
	prev_time = std::chrono::steady_clock::now();

}

// �X�V����
eSceneType InGame::Update(const float& delta_second)
{
	// ���j�b�g�ƃG�l�~�[�̃J�E���g����
	int Pcount_sum = 0;
	int Ecount_sum = 0;

	Pcount_sum = (int)P_Tank::GetCount() + (int)P_Melee::GetCount() + (int)P_Ranged::GetCount() + (int)P_Guardian::GetCount() ;
	Ecount_sum = (int)E_Tank::GetCount() + (int)E_Melee::GetCount() + (int)E_Ranged::GetCount()	;

	if (max_unit < Pcount_sum)
	{
		make_unit +=  Pcount_sum - max_unit ;
	}

	if(Pcount_sum < max_unit)
	{
		dead_unit -= max_unit - Pcount_sum;
	}
	max_unit = Pcount_sum;

	if (Ecount_sum < max_enemy)
	{
		dead_enemy = max_enemy - Ecount_sum;
	}
	max_enemy = Ecount_sum;

	// Z�L�[�Ńv���C���[�̏����i�f�o�b�N�p�j
	if (CheckHitKey(KEY_INPUT_Z))
	{
		IsPlayerWin(false);
		return eSceneType::result;
	}

	// ���s���菈��
	if (enemy->GetHP() <= 0 || player->GetHP() <= 0)
	{
		if (player->GetHP() <= 0)
		{
			if (!CheckSoundMem(bgmHandle[4]))
			{
				ChangeVolumeSoundMem(150, bgmHandle[4]);
				PlaySoundMem(bgmHandle[4], DX_PLAYTYPE_BACK);
			}
			IsPlayerWin(false);
			state = GameState::PLAYER_DEAD;
		}
		else
		{
			if (!CheckSoundMem(bgmHandle[3]))
			{
				ChangeVolumeSoundMem(100, bgmHandle[3]);
				PlaySoundMem(bgmHandle[3], DX_PLAYTYPE_BACK);
			}
			IsPlayerWin(true);
			state = GameState::BOSS_DEAD;
		}
		if (alpha < 510)
		{
			alpha++;
		}
	}

	// ���͏����擾
	InputManager* input = InputManager::GetInstance();

	//�J�����̏����擾
	Camera* camera = Camera::GetInstance();

	//�J�����̍X�V
	camera->Update();

	switch (state)
	{
	case GameState::PLAYING:
		// ���U���g�V�[���ɑJ�ڂ���
		if (input->GetKeyState(KEY_INPUT_RETURN) == eInputState::Pressed)
		{
			return eSceneType::result;
		}

		// ���j�b�g�I������
		UnitSelection();

		// �R�X�g�Ǘ�����
		RegenerateCost();

		break;
	case GameState::PLAYER_DEAD:
		if (player->GetDead())
		{
			return eSceneType::result;
		}
		break;

	case GameState::BOSS_DEAD:
		// �I�u�W�F�N�g�}�l�[�W���[�̃|�C���^
		GameObjectManager* object = GameObjectManager::GetInstance();
		// �I�u�W�F�N�g���X�g���擾
		std::vector<GameObject*> objects_list = object->GetObjectsList();

		// �G�폜
		if (!objects_list.empty())
		{
			for (GameObject* obj : objects_list)
			{
				// �G�l�~�[�ȊO�͖�������
				if (obj->GetCollision().object_type == eObjectType::Enemy &&
				// �ْ[�҂���������
					obj != enemy)
				{
					obj->Finalize();
				}
			}
		}

		camera->SetCameraPos(enemy->GetLocation());
		if (enemy->GetDead())
		{
			return eSceneType::result;
		}
		break;
	}

	// �e�N���X�̍X�V�������Ăяo��
	__super::Update(delta_second);
}

// �`�揈��
void InGame::Draw() const
{
	// �J�����̏��擾
	Camera* camera = Camera::GetInstance();

	// �w�i�摜�̂���
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
	int offset = ProjectConfig::STAGE_WIDTH - camera->GetScreeenSize().x / 2 - camera->GetCameraPos().x;
	switch (state)
	{
	case GameState::PLAYING:
	{
		DrawGraph(D_WIN_MAX_X / 2 - 700 + offset, ShowBackGround_Y, BackGroundImage[StageNumber - 1], 0);

		LightMapManager* light_map = LightMapManager::GetInstance();

		// �������Z����
		light_map->DrawLights(camera->GetCameraPos());

		// ���C�g�}�b�v��`��
		light_map->DrawLightMap();

		// �e�N���X�̕`�揈�����Ăяo��
		__super::Draw();

		// �{�^���T�C�Y
		const int button_width = 200;
		const int button_height = 200;

		// �{�^���̐�
		const int button_count = 5;

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


				switch (i)
				{
				case 0:
				{
					if (cost < 20)
					{
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
					}

				}
				break;
				case 1:
				{
					if (cost < 50)
					{
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
					}
					break;
				}
				case 2:
				{
					if (cost < 100)
					{
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
					}
					break;
				}
				case 3:
				{
					if (cost < 500)
					{
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
					}
					break;
				}
				case 4:
					if (cost < Sun_Level * 100)
					{
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
					}

				default:
					break;
				}

				// �g�i�w�i�j��`��
				DrawBox(x - (w - button_width) / 2, y - (h - button_height) / 2, x + w, y + h, GetColor(255, 255, 255), TRUE);
				//�L�����̕`��͈͂𐧌�
				SetDrawArea(x - (w - button_width) / 2, y - (h - button_height) / 2, x + w, y + h);
				
				
				if (i == 4)
				{
					// �L�����摜�𒆐S�ɕ`��
					DrawExtendGraph(
						(int)(x + (button_width - w * 1.5) / 2), (int)(y + (button_height - h * 1.5) / 2),
						(int)(x + (button_width + w * 1.7) / 2), (int)(y + (button_height + h * 1.7) / 2),
						Sun_Images, TRUE);

					SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

					// �`��͈͂����ɖ߂�
					SetDrawArea(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y);
					if (StageNumber == 1)
					{
						DrawExtendGraph(
							(int)x-330, (int)y + h,
							(int)x + 300, (int)y + h + 120,
							Text_BackGround, TRUE);

						DrawFormatString(x - 300, y + h + 40, 0x000000, "�M�x cost:���݂̍ő�l���ׂ�\n�R�X�g�̏���Ɖ񕜑��x���オ��");
					}

				}
				else
				{
					// �L�����摜�𒆐S�ɕ`��
					DrawExtendGraph(
						(int)(x + (button_width - w * 1.5) / 2), (int)(y + (button_height - h * 1.5) / 2),
						(int)(x + (button_width + w * 1.7) / 2), (int)(y + (button_height + h * 1.7) / 2),
						unit_ui[i], TRUE);

					SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

					// �`��͈͂����ɖ߂�
					SetDrawArea(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y);


					if (StageNumber == 1)
					{

						DrawExtendGraph(
							(int)x, (int)y + h,
							(int)x + 380, (int)y + h + 120,
							Text_BackGround, TRUE);

						switch (i)
						{
							case 0:
							{
								DrawFormatString(x + 30, y + h + 40, 0x000000, "���� cost:20\n�B�ꓔ�������");
							}
							break;
							case 1:
							{
								DrawFormatString(x + 30, y + h + 40, 0x000000, "���m cost:50\n�O�������");
							}
							break;
							case 2:
							{
								DrawFormatString(x + 30, y + h + 40, 0x000000, "�|�� cost:100\n����������U������");
							}
							break;
							case 3:
							{
								DrawFormatString(x + 30, y + h + 40, 0x000000, "�R�m cost:500\n���m�̏�ʌ݊�");
							}
							break;

						}
					}

					//DrawGraph(x, y, Text_BackGround, 1);
				}
			}
			else
			{

				switch (i)
				{
				case 0:
				{
					if (cost < 20)
					{
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
					}

				}
				break;
				case 1:
				{
					if (cost < 50)
					{
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
					}
					break;
				}
				case 2:
				{
					if (cost < 100)
					{
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
					}
					break;
				}
				case 3:
				{
					if (cost < 500)
					{
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
					}
					break;
				}
				case 4:
					if (cost < Sun_Level * 100)
					{
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
					}

				default:
					break;
				}

				if (i == 4)
				{
					if (cost < Sun_Level * 100 && Sun_Level < 9)
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);

					// �g�i�w�i�j��`��
					DrawBox(x, y, x + button_width, y + button_height, GetColor(100, 100, 100), TRUE);
					//�L�����̕`��͈͂𐧌�
					SetDrawArea(x, y, x + button_width, y + button_height);


					// �L�����摜�𒆐S�ɕ`��
					DrawExtendGraph(
						(int)(x + (button_width - w * 1.5) / 2), (int)(y + (button_height - h * 1.5) / 2),
						(int)(x + (button_width + w * 1.7) / 2), (int)(y + (button_height + h * 1.7) / 2),
						Sun_Images, TRUE);

					SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

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
						(int)(x + (button_width + w * 1.7) / 2), (int)(y + (button_height + h * 1.7) / 2),
						unit_ui[i], TRUE);

					SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);



				}
			}

			// �`��͈͂����ɖ߂�
			SetDrawArea(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y);
		}

		DrawFormatString(1000, 30, 0x00ffff, "Level:%d/10", Sun_Level);
		// �R�X�g�\��
		if (Sun_Level == 10)
		{
				DrawFormatString(1000, 680, 0xffffff, "Cost:%d/��", cost);
		}
		else
		{

			if (cost < Sun_Level * 100)
			{
				DrawFormatString(1000, 680, 0xffffff, "Cost:%d/%d", cost, Sun_Level * 100);
			}
			else
			{
				DrawFormatString(1000, 680, 0xffff00, "Cost:%d/%d", cost, Sun_Level * 100);
			}
		}


		if (ProjectConfig::DEBUG)
		{
			// �J�������W�`��
			DrawFormatString(500, 300, 0xffffff, "%f", camera->GetCameraPos().x);

			// �V�[�����̕`��
			SetFontSize(60);
			DrawFormatString(0, 0, 0xffffff, "InGame");
			SetFontSize(32);
			DrawFormatString(100, 300, 0xffffff, "Space�������Ă�������");
			DrawFormatString(1000, 0, 0xffffff, "Stage:%d", StageNumber);
		}

	}
	break;
	case GameState::PLAYER_DEAD:
		DrawGraph(D_WIN_MAX_X / 2 - 700 + offset, ShowBackGround_Y, BackGroundImage[StageNumber - 1], 0);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha / 2);
		DrawRotaGraph(D_WIN_MAX_X / 2 , D_WIN_MAX_Y / 2, 1.0f, 0.0f, Text_Images[0], true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		__super::Draw();
		break;
	case GameState::BOSS_DEAD:
	{

		DrawGraph(enemy->GetLocation().x - 100.0f, ShowBackGround_Y, BackGroundImage[StageNumber - 1], 0);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha / 2);
		DrawRotaGraph(D_WIN_MAX_X / 2, D_WIN_MAX_Y / 2, 1.0f, 0.0f, Text_Images[1], true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		LightMapManager* light_map = LightMapManager::GetInstance();

		// �������Z����
		light_map->DrawLights(camera->GetCameraPos());

		// �e�N���X�̕`�揈�����Ăяo��
		__super::Draw();

		// ���C�g�}�b�v��`��
		light_map->DrawLightMap();

		break;
	}
	}
	Vector2D P_position = player->GetLocation();
	P_position.x -= camera->GetCameraPos().x - D_WIN_MAX_X / 2;
	Vector2D E_position = enemy->GetLocation();
	E_position.x -= camera->GetCameraPos().x - D_WIN_MAX_X / 2;
	// �ޏ�HP�\��
	DrawBoxAA(P_position.x - 50.0f, P_position.y - 150.0f, P_position.x + (50.0f - (100 - player->GetHP())), P_position.y - 135.0f, 0xFFFFFF, true);
	// �ْ[��HP�\��
	DrawBoxAA(E_position.x - 50.0f, E_position.y - 150.0f, E_position.x + (50.0f - (100 - ((double)enemy->GetHP() / 500) * 100)), E_position.y - 135.0f, 0xFFFFFF, true);

}

// �I������
void InGame::Finalize()
{
	if (CheckSoundMem(bgmHandle[StageNumber - 1]) == 1)
	{
		StopSoundMem(bgmHandle[StageNumber - 1]);
	}
	
	// �e�N���X�̏I�����������Ăяo��
	__super::Finalize();


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

	// ����SE�Đ�
	PlaySoundMem(SummonSE[1], DX_PLAYTYPE_BACK);

	switch (e_enem)
	{
	case Tank:
		object->CreateObject<E_Tank>(Vector2D(enemy->GetLocation().x + 50.0f, enemy->GetLocation().y + 30.0f));
		break;
	case Melee:
		object->CreateObject<E_Melee>(Vector2D(enemy->GetLocation().x + 50.0f, enemy->GetLocation().y + 30.0f));
		break;
	case Range:
		object->CreateObject<E_Ranged>(Vector2D(enemy->GetLocation().x + 50.0f, enemy->GetLocation().y + 30.0f));
		break;
	case Boss:
		object->CreateObject<class Boss>(Vector2D(enemy->GetLocation().x + 50.0f, enemy->GetLocation().y + 0.0f));
		break;
	default:
		break;
	}
}

const int InGame::GetSunLevel() const
{
	return Sun_Level;
}

/// <summary>
/// ���݂̃X�e�[�g�擾����
/// </summary>
/// <returns></returns>
const GameState InGame::GetNowState() const
{
	return state;
}

//void InGame::FirstStage()
//{
//
//	if (cursor == 0)
//	{
//
//	}
//
//}

//	���j�b�g����
void InGame::UnitSelection()
{
	// ���͏����擾
	InputManager* input = InputManager::GetInstance();

	// �J�[�\������
	if (input->GetKeyState(KEY_INPUT_RIGHT) == eInputState::Pressed ||
		input->GetButtonState(XINPUT_BUTTON_DPAD_RIGHT) == eInputState::Pressed)
	{
		if (cursor < 4)
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
					// ����SE�Đ�
					PlaySoundMem(SummonSE[0], DX_PLAYTYPE_BACK);
					// �^���N�𐶐�
					GameObject* obj = object->CreateObject<P_Tank>(Vector2D(player->GetLocation().x, player->GetLocation().y + 30.0f));
					obj->SetInGamePoint(this);
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
					// ����SE�Đ�
					PlaySoundMem(SummonSE[0], DX_PLAYTYPE_BACK);
					// �ߐڂ𐶐�
					GameObject* obj = object->CreateObject<P_Melee>(Vector2D(player->GetLocation().x, player->GetLocation().y + 30.0f));
					obj->SetInGamePoint(this);
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
					// ����SE�Đ�
					PlaySoundMem(SummonSE[0], DX_PLAYTYPE_BACK);
					object->CreateObject<P_Ranged>(Vector2D(player->GetLocation().x, player->GetLocation().y + 30.0f))->SetInGamePoint(this);
					cost -= Range_Cost;
					//summon_flag[cursor] = true;
					summon_time[cursor] = std::chrono::steady_clock::now();
				}
			}
			break;
		case 3:
			if (summon_flag[cursor] == false)
			{
				if (cost - Guardian_Cost >= 0)
				{
					// ����SE�Đ�
					PlaySoundMem(SummonSE[0], DX_PLAYTYPE_BACK);
					// �ߐڂ𐶐�
					GameObject* obj = object->CreateObject<P_Guardian>(Vector2D(player->GetLocation().x, player->GetLocation().y + 30.0f));
					obj->SetInGamePoint(this);
					cost -= Guardian_Cost;
					//summon_flag[cursor] = true;
					summon_time[cursor] = std::chrono::steady_clock::now();
				}
			}
			break;
		case 4:
			if (Sun_Level != 10)
			{
				if (cost >= Sun_Level * 100)
				{
					cost = 0;
					Sun_Level++;
					PlaySoundMem(ClickUp, DX_PLAYTYPE_BACK);
				}
			}
			break;
		}
	}
}

//	�R�X�g�Ǘ�����
void InGame::RegenerateCost()
{
	auto now_time = std::chrono::steady_clock::now();

	if (cost < Sun_Level * 100 || Sun_Level == 10)
	{
		if (now_time - prev_time > std::chrono::milliseconds(500))
		{
			cost += Cost_UpNum + ((Sun_Level) * 5);
			if (Sun_Level == 10)
			{
				cost += 200;
			}
			else
			{
				if (cost >= Sun_Level * 100)
				{
					PlaySoundMem(Click, DX_PLAYTYPE_BACK);
					cost = Sun_Level * 100;
				}
			}
			prev_time = std::chrono::steady_clock::now();
		}
	}
	else
	{

		prev_time = std::chrono::steady_clock::now();
	}
}

const void InGame::LoadImages() const
{
	//���\�[�X�}�l�[�W���[�̃C���X�^���X�擾
	ResourceManager* rm = ResourceManager::GetInstance();

	//�摜�Ǎ���
	// �P�i
	rm->GetImages("Resource/Images/Unit/Ranged/Arrow.png", 1, 1, 1, 32, 32);


	//���s�A�j���[�V����
	rm->GetImages("Resource/Images/Unit/Melee/Melee_Walk.png", 4, 4, 1, 32, 32);
	rm->GetImages("Resource/Images/Unit/Tank/Tank_Walk.png", 4, 4, 1, 32, 32);
	rm->GetImages("Resource/Images/Unit/Ranged/Ranged_Walk.png", 4, 4, 1, 32, 32);
	rm->GetImages("Resource/Images/Unit/Guardian/Guardian_Walk.png", 3, 3, 1, 1024, 1024);
	rm->GetImages("Resource/Images/Enemy/Melee/Melee_Walk.png", 4, 4, 1, 32, 32);
	rm->GetImages("Resource/Images/Enemy/Tank/Tank_Walk.png", 4, 4, 1, 32, 32);
	rm->GetImages("Resource/Images/Enemy/Ranged/Ranged_Walk.png", 4, 4, 1, 32, 32);

	//�U���A�j���[�V����
	rm->GetImages("Resource/Images/Unit/Melee/Melee_Attack.png", 4, 4, 1, 32, 32);
	rm->GetImages("Resource/Images/Unit/Tank/Tank_Attack.png", 4, 4, 1, 32, 32);
	rm->GetImages("Resource/Images/Unit/Ranged/Ranged_Attack.png", 4, 4, 1, 32, 32);
	rm->GetImages("Resource/Images/Unit/Guardian/Guardian_Attack.png", 3, 3, 1, 1024, 1024);
	rm->GetImages("Resource/Images/Enemy/Melee/Melee_Attack.png", 4, 4, 1, 32, 32);
	rm->GetImages("Resource/Images/Enemy/Tank/Tank_Attack.png", 4, 4, 1, 32, 32);
	rm->GetImages("Resource/Images/Enemy/Ranged/Ranged_Attack.png", 4, 4, 1, 32, 32);

	//���S�A�j���[�V����
	rm->GetImages("Resource/Images/Unit/Melee/Melee_Down.png", 3, 3, 1, 32, 32);
	rm->GetImages("Resource/Images/Unit/Tank/Tank_Down.png", 3, 3, 1, 32, 32);
	rm->GetImages("Resource/Images/Unit/Ranged/Ranged_Down.png", 3, 3, 1, 32, 32);
	rm->GetImages("Resource/Images/Unit/Guardian/Guardian_Down.png", 3, 3, 1, 1024, 1024);



}