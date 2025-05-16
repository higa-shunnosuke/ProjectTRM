#pragma once

#include "../Utility/Singleton.h"
#include "../Utility/ProjectConfig.h"
#include "../Objects/GameObject.h"
#include "../Utility/ResourceManager.h"
#include "../Utility/Collision.h"

/// <summary>
/// ���C�g�}�b�v�Ǘ��N���X
/// </summary>
class LightMapManager :public Singleton<LightMapManager>
{
private:
	std::vector<GameObject*> lights_list;	// ���C�g���X�g
	int light_graph = 0;					// ���̉摜
	int light_screen = 0;					// ���C�g�}�b�v
	int screen_brightness = 0;				// ��ʂ̖��邳�i0�`255�j

public:

	/// <summary>
	/// ����������
	/// </summary>
	void Initialize()
	{
		// �摜�̓ǂݍ���
		ResourceManager* rm = ResourceManager::GetInstance();
		light_graph = rm->GetImages("Resource/Images/Light/light01.png", 1, 1, 1, 1024, 1024)[0];
		
		// ���C�g�}�b�v�𐶐�
		light_screen = MakeScreen(D_WIN_MAX_X, D_WIN_MAX_Y, TRUE);

		screen_brightness = 100;
	}

	/// <summary>
	/// ���C�g�ǉ�����
	/// </summary>
	/// <param name="obj">�Ǐ]����I�u�W�F�N�g</param>
	void AddLight(GameObject* obj)
	{
		// ���C�g���X�g�ɒǉ�
		lights_list.push_back(obj);
	}

	/// <summary>
	/// ���C�g�폜����
	/// </summary>
	/// <param name="obj">�Ǐ]���Ă���I�u�W�F�N�g</param>
	void DeleteLight(GameObject* obj)
	{
		// ���C�g���X�g�̂���폜
		lights_list.erase(
			std::remove(lights_list.begin(), lights_list.end(), obj));
	}

	/// <summary>
	/// ���̉��Z��������
	/// </summary>
	void DrawLights(const Vector2D camera_pos) const
	{
		// �`�������C�g�}�b�v�ɔ��f����
		SetDrawScreen(light_screen);
		// ���C�g�}�b�v�̏������i�Â��O���[�œh��j
		DrawBox(0, 0, 1280, 720,
			GetColor(screen_brightness, screen_brightness, screen_brightness),
			TRUE);
		
		// ���C�g���X�g���̍��W�Ɍ��̉摜�� ALPHA ����
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
		for (GameObject* light :lights_list)
		{
			if (light != nullptr)
			{
				// ���W���擾
				Vector2D light_pos = light->GetLocation();

				// �J�������W�����Ƃɕ`��ʒu���v�Z
				light_pos.x -= camera_pos.x - D_WIN_MAX_X / 2;
				// ���C�g�̔��a���擾
				Collision lc = light->GetCollision();
				float radius = lc.light_size;
				// ���C�g�}�b�v��Ɍ���`��
				DrawRotaGraphF(light_pos.x, light_pos.y,
					radius, 0.0,light_graph, TRUE,0);
			}
		}
		// �u�����h���[�h��������
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		// �`����\��ʂɖ߂�
		SetDrawScreen(DX_SCREEN_BACK);

#ifdef DEBUG
		for (GameObject* light : lights_list)
		{
			if (light != nullptr)
			{
				// ���W���擾
				Vector2D light_pos = light->GetLocation();

				// �J�������W�����Ƃɕ`��ʒu���v�Z
				light_pos.x -= camera_pos.x - D_WIN_MAX_X / 2;
				// ���C�g�̔��a���擾
				Collision lc = light->GetCollision();
				float radius = lc.light_size;
				// ���C�g�͈͂�\��
				DrawCircle((int)light_pos.x, (int)light_pos.y, (int)(radius * 100),
					0x0000ff, 0, TRUE);
			}
		}
		
#endif
	}

	/// <summary>
	/// ���C�g�}�b�v�`�揈��
	/// </summary>
	void DrawLightMap() const
	{
		// ���C�g�}�b�v����Z�������ĕ`��
		SetDrawBlendMode(DX_BLENDMODE_MUL, 255);
		DrawGraph(0, 0, light_screen, TRUE);
		
		// �u�����h���[�h��������
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	/// <summary>
	/// ��ʂ̖��邳��������
	/// </summary>
	/// <param name="value">���邳�i0�`255�j</param>
	void SetBrightness(int value)
	{
		// ��ʂ̖��邳�𒲐�
		screen_brightness = value;
	}

	/// <summary>
	///  ���C�g���X�g�擾����
	/// </summary>
	/// <returns>���C�g���X�g</returns>
	const std::vector<GameObject*>& GetLightsList() const
	{
		return lights_list;
	}
};