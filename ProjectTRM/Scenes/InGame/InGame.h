#pragma once

#include "../SceneBase.h"

#include "../../Objects/Character/Player/Oracle/Oracle.h"
#include "../../Objects/Character/Enemy/Hertic/Heretic.h"

#include <chrono>

#define Cost_UpNum 5
#define Tank_Cost 20
#define Melee_Cost 50
#define Range_Cost 100
#define Guardian_Cost 500


enum class GameState
{
	PLAYING,
	BOSS_DEAD,
	PLAYER_DEAD,
	GAMESTART
};

// �C���Q�[���V�[��
class InGame : public SceneBase
{
private:
	int Text_BackGround;
	std::string Tutorial_Text[4] = { "�U���~�̗́~\n�B�ꓔ��������Ă���L����","�U���Z�̗́Z\n�M�̃��x���ɉ����ċ����Ȃ�I","�U���Z�̗́Z\n����������U������I�G�������Ȃ��ƍU�����ł��Ȃ�!!","�U�����̗́�\n�����\�ȃL�����N�^�[!!"};

	GameState state = GameState::PLAYING;
	std::chrono::steady_clock::time_point	summon_time[4];		// �����J�n����
	std::chrono::seconds					cooldown[3];		// �����N�[���_�E��
	std::chrono::steady_clock::time_point	prev_time;			// �R�X�g���Z�p�ϐ�
	int SunImages[10];
	int Sun_Images;
	int BackGroundImage[3];
	int Text_Images[2];				//���s���̕����摜
	int sound;
	int Click;
	int ClickUp;
	int Cost_Max;
	int Sun_Level;
	int alpha;
	Vector2D old_camerapos;
	int			cost;				// �R�X�g
	bool		summon_flag[4];		// �����t���O
	Oracle*		player;				// �ޏ��̃|�C���^
	Heretic*	enemy;				// �ْ[�҂̃|�C���^
	int			unit_ui[5];			// ���j�b�g�I��UI�̉摜
	int bgmHandle[5];				// BGM
	int SummonSE[2];				// ����SE

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

	/// <summary>
	/// �G��������
	/// </summary>
	/// <param name="e_enem">�G�̎��</param>
	void CreateEnemy(E_enemy e_enem);

	/// <summary>
	/// ���z�̃��x���擾����
	/// </summary>
	/// <returns></returns>
	const int GetSunLevel() const;

	/// <summary>
	/// ���݂̃X�e�[�g�擾����
	/// </summary>
	/// <returns></returns>
	const GameState GetNowState() const;

private:

	//void FirstStage();

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
	void RegenerateCost();

	/// <summary>
	/// �摜�Ǎ��ݏ���
	/// </summary>
	/// <returns></returns>
	const void LoadImages() const;
};
