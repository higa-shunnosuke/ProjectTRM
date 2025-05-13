#pragma once

#include "../Utility/Singleton.h"
#include "../Utility/ProjectConfig.h"
#include "../Objects/GameObject.h"
#include "../Utility/ResourceManager.h"

struct LightDetail {
	GameObject* object;
	float size;

	bool operator==(const LightDetail detail) const
	{
		return object == detail.object;
	}
};

/// <summary>
/// ���C�g�}�b�v�Ǘ��N���X
/// </summary>
class LightMapManager :public Singleton<LightMapManager>
{
private:
	std::vector<LightDetail> light_list;	// �Ǐ]���X�g
	int light_graph;						// ���̉摜
	int light_screen;						// ���C�g�}�b�v
	int screen_brightness;					// ��ʂ̖��邳�i0�`255�j

public:

	/// <summary>
	/// ����������
	/// </summary>
	void Initialize()
	{
		// �摜�̓ǂݍ���
		ResourceManager* rm = ResourceManager::GetInstance();
		light_graph = rm->GetImages("Resource/Images/Light/light.png", 1, 1, 1, 1024, 1024)[0];
		
		// ���C�g�}�b�v�𐶐�
		light_screen = MakeScreen(D_WIN_MAX_X, D_WIN_MAX_Y, TRUE);

		screen_brightness = 100;
	}

	/// <summary>
	/// ���C�g�ǉ�����
	/// </summary>
	/// <param name="obj">�Ǐ]����I�u�W�F�N�g</param>
	void AddLight(LightDetail light)
	{
		// �Ǐ]���X�g�ɒǉ�
		light_list.push_back(light);
	}

	/// <summary>
	/// ���C�g�폜����
	/// </summary>
	/// <param name="obj">�Ǐ]���Ă���I�u�W�F�N�g</param>
	void DeleteLight(GameObject* obj)
	{
		// �Ǐ]���X�g�̂���폜
		light_list.erase(
			std::remove_if(light_list.begin(), light_list.end(),
				[obj](const LightDetail& light)
				{
					return light.object == obj;
				}),
			light_list.end());
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
		
		// �ǐՃ��X�g���̍��W�Ɍ��̉摜�����Z����
		SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
		for (LightDetail light :light_list)
		{
			if (light.object != nullptr)
			{
				// ���W���擾
				Vector2D light_pos = light.object->GetLocation();

				// �J�������W�����Ƃɕ`��ʒu���v�Z
				light_pos.x -= camera_pos.x - D_WIN_MAX_X / 2;

				// ���C�g�}�b�v��Ɍ���`��
				DrawRotaGraphF(light_pos.x, light_pos.y,
					light.size, 0.0,light_graph, TRUE,0);
			}
		}
		// �u�����h���[�h��������
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		// �`����\��ʂɖ߂�
		SetDrawScreen(DX_SCREEN_BACK);
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
};