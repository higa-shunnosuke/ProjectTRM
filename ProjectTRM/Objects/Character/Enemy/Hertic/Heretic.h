#pragma once

#include "../../Character.h"

#define Enemy_Think_Standard
#define ENEMY_BOTTOM_COST 50

#ifdef Enemy_Think_Standard//��{�I�ȋ��{

#define TANK_eva	 10
#define MELEE_eva	 30
#define RANGE_eva	 20
#define BOSS_eva	 50

#define TANK_cost	 50
#define MELEE_cost	 100
#define RANGE_cost	 300
#define BOSS_cost	 500


#endif // Enemy_Think_Standard



enum E_enemy :int
{
	Tank,
	Melee,
	Range,
	Boss
};

// �Q�[���I�u�W�F�N�g���N���X
class Heretic :public Character
{
private:
	State nowsta = State::Idle;
	std::vector<int> EffectImage;
	int Player_evaluation[4];
	int Enemy_evaluation[3];
	int Cost;
	bool	JustDead = false;	//���S���A�j���[�V�������I����Ē��x����
	bool	Fstflag = false;	//���񐶐��p:Debug�œG�̓������m�F�������ł��傤�Hs
	bool	summon_flag;	//�����t���O
	bool	summon_effect;	//�����t���O
	std::vector<GameObject*> ObjectList;
	int DeadImage[2];

	class	InGame* Ingame;	//InGame�̃|�C���^�[
	float	CountFlame;		//flame�̊Ǘ�
	int		CountTime;		//�����^�ł̊Ǘ�

public:



	//�R���X�g���N�^
	Heretic();
	//�f�X�g���N�^
	virtual ~Heretic();

	/// <summary>
	/// ����������
	/// </summary>
	virtual void Initialize();
	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="delta_second">1�t���[��������̎���</param>
	virtual void Update(float delta_second);
	/// <summary>
	/// �`�揈��
	/// </summary>
	virtual void Draw(const Vector2D camera_pos) const;
	/// <summary>
	/// �I��������
	/// </summary>
	virtual void Finalize();
	/// <summary>
	/// �G�l�~�[��������
	/// </summary>
	/// <param name = "point">: InGameScene��Pointer</param>
	void SetInGamePoint(class InGame* point);

	void ThinkingEnemy();


public:
	/// <summary>
	/// �����蔻��ʒm����
	/// </summary>
	/// <param name="hit_object">���������Q�[���I�u�W�F�N�g�̃|�C���^</param>
	virtual void OnHitCollision(GameObject* hit_object);

public:
	/// <summary>
	/// HP�Ǘ�����
	/// </summary>
	/// <param name="hit_object">�_���[�W</param>
	virtual void HPControl(int Damage);

	bool GetDead();

private:
	/// <summary>
	/// �U������
	/// </summary>
	virtual void Attack();
	/// <summary>
	/// �ړ�����
	/// </summary>
	/// <param name="hit_object">1�t���[��������̎���</param>
	virtual void Movement(float delta_second);
	/// <summary>
	/// �A�j���[�V�������䏈��
	/// </summary>
	/// <param name="hit_object">1�t���[��������̎���</param>
	virtual void AnimationControl(float delta_second);
	/// <summary>
	/// �G�t�F�N�g���䏈��
	/// </summary>
	/// <param name="hit_object">1�t���[��������̎���</param>
	virtual void EffectControl(float delta_second);
};