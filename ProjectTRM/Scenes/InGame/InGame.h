#pragma once

#include "../SceneBase.h"

#include "../../Objects/Character/Player/Oracle/Oracle.h"
#include "../../Objects/Character/Enemy/Hertic/Heretic.h"

#define SIZE_X 211
#define SIZE_Y 16

class InGame : public SceneBase
{
private:
	int cost_count;				// �R�X�g
	float cost_time;			// �R�X�g
	int cooldown_count[3];	// �����N�[���_�E��
	float cooldown_time[3];		// �����N�[���_�E��
	Oracle* player;				// �ޏ��̃|�C���^
	Heretic* enemy;				// �ْ[�҂̃|�C���^

public:
	// �R���X�g���N�^
	InGame();
	// �f�X�g���N�^
	virtual ~InGame();

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

private:
	/// <summary>
	/// �X�e�[�W�̐�������
	/// </summary>
	void LoadStage();

	/// <summary>
	///	���j�b�g�Ǐ���
	/// </summary>
	void UnitSelection();

	/// <summary>
	///	�R�X�g�Ǘ�����
	/// </summary>
	void CostManagement(const float& delta_second);
	
	/// <summary>
	/// �N�[���_�E���Ǘ�����
	/// </summary>
	void CooldownManagement(const float& delta_second);
};
