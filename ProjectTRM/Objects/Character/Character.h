#pragma once

#include "../GameObject.h"
#include "string"
#include "vector"



enum State {
	Idle,
	Move,
	Attack,
	Damage,
	Death
};

#define MAX_ALPHA (255);
#define ALPHA_ADD (15);

// �Q�[���I�u�W�F�N�g���N���X
class Character:public GameObject
{
protected:
	Vector2D velocity;
	bool flip_flag;
	State now_state;
	State old_state;
	std::vector<int> effect;
	std::vector<int> animation;

	int		Anim_count = 0;
	float	Anim_flame = 0.0f;
	int     count;
	float   attack_flame = 0.0f;
	bool	attack_flag;
	float   dmage_flame = 0.0f;

	int alpha;
	int add;

public:
	//�R���X�g���N�^
	Character();
	//�f�X�g���N�^
	virtual ~Character();

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

protected:
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
	/// <summary>
	/// �U������
	/// </summary>
	virtual void Attack(GameObject* hit_object);
};