#pragma once

#include "../SceneBase.h"

#include "../../Objects/Character/Player/Oracle/Oracle.h"
#include "../../Objects/Character/Enemy/Hertic/Heretic.h"

#include <chrono>

#define SIZE_X 211
#define SIZE_Y 16

class InGame : public SceneBase
{
private:
	std::chrono::steady_clock::time_point summon_time[3];	// �����J�n����
	std::chrono::seconds cooldown[3];						// �����N�[���_�E��
	std::chrono::steady_clock::time_point cost_time;		// �R�X�g�ҋ@����
	int cost;				// �R�X�g
	bool summon_flag[3];	// �����t���O
	Oracle* player;			// �ޏ��̃|�C���^
	Heretic* enemy;			// �ْ[�҂̃|�C���^
	int unit_ui[3];			// ���j�b�g�I��UI�̉摜
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

	void CreateEnemy(E_enemy e_enem);

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

	/// <summary>
	/// �摜�Ǎ��ݏ���
	/// </summary>
	/// <returns></returns>
	const void LoadImages() const;
};
