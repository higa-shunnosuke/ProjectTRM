#pragma once

#include "../SceneBase.h"

struct LoadItem {
	std::string type;	// "image" or "sound"
	std::string path;	// �t�@�C���p�X
};

// ���[�f�B���O�V�[��
class Loading :public SceneBase
{
private:
	SceneBase* next_scene;	// ���̃V�[�����
	int count;				// �ǂݍ��񂾃A�C�e���̐�
	int size;				// �ǂݍ��ރA�C�e���̐�
	bool is_complete;		// �ǂݍ��݊����t���O

	// ���[�h���X�g
	std::vector<LoadItem> load_list = {
		// �^�C�g��
		{"image","BackGround/Title"},
		// �X�e�[�W�Z���N�g
		{"image","BackGround/BlueMoonUI"},
		{"image","BackGround/YelloMoonUI"},
		{"image","BackGround/RedMoonUI"},
		{"image","BackGround/StageSelect"},
		// �C���Q�[��
		{"image","Unit/Tank/Tank_Cost_New"},
		{"image","Unit/Melee/Melee_Cost_New"},
		{"image","Unit/Ranged/Ranged_Cost"},
		{"image","Unit/Guardian/Guardian_Cost"},
		{"image","BackGround/Sun"},
		{"image","BackGround/BlueMoon"},
		{"image","BackGround/YelloMoon"},
		{"image","BackGround/RedMoon"},
		{"image","BackGround/text_fail"},
		{"image","BackGround/text_clear"},
		{"image","BackGround/Text"},
		{"image","BackGround/TextLeft"},
		{"image","BackGround/cost_base"},
		// ���U���g
		{"image","BackGround/Result"},
		// �^�C�g��
		{"sound","Decision"},
		{"sound","Title/BGM/OP"},
		// �X�e�[�W�Z���N�g
		{"sound","StageSelect/CursorMove"},
		{"sound","StageSelect/BGM/StageSelect"},
		// �C���Q�[��
		{"sound","InGame/Click"},
		{"sound","InGame/ClickUp"},
		{"sound","Ingame/BGM/Stage1"},
		{"sound","Ingame/BGM/Stage2"},
		{"sound","Ingame/BGM/Stage3"},
		{"sound","Ingame/SummonAllies"},
		{"sound","Ingame/SummonEnemy"},
		// ���U���g
		{"sound","Result/Win_BGM"},
		{"sound","Result/Loose_BGM"},
		{"sound","Result/CursorMove"},
		{"sound","Result/Lost"},
		{"sound","Result/WINSE"},
		{"sound","Result/CountDown"},
		// �G���h
		{"sound","Ending/ED"},

	};

public:
	// �R���X�g���N�^
	Loading();
	// �f�X�g���N�^
	virtual ~Loading();

	/// <summary>
	/// ����������
	/// </summary>
	virtual void Initialize() override;

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="delta_second">1�t���[��������̎���</param>
	/// <returns>���̃V�[���^�C�v���</returns>
	virtual eSceneType Update(const float& delta_second) override;

	/// <summary>
	/// �`�揈��
	/// </summary>
	virtual void Draw() const override;

	/// <summary>
	/// �I��������
	/// </summary>
	virtual void Finalize() override;

	/// <summary>
	/// ���݂̃V�[���^�C�v�����擾����
	/// </summary>
	/// <returns>���݂̃V�[���^�C�v</returns>
	virtual const eSceneType GetNowSceneType() const override;

	/// <summary>
	/// ���̃V�[���^�C�v�ݒ菈��
	/// </summary>
	void SetNextSceneType(SceneBase* next_scene);
};

