#pragma once

#include "../Character.h"

// �G�x�[�X�N���X
class EnemyBase :public Character
{
protected:
	int anim_max_count;		// �A�j���[�V�����摜�z��̗v�f��
	float anim_rate;		// �A�j���[�V�����̑���
	float damage_rate;		// �����_���[�W�̕p�x
	float recovery_time;	// �d������
	float recovery_flame;	// �d�����Ԃ̌v�Z
	float speed;			// ��������
	float Damage;				// �_���[�W
	bool old_light;			// �O��̃��C�g�t���O

	// �G�t�F�N�g
	int effect[2];								// �`��p�ϐ�
	std::vector<int> effect_image[2];			// �A�j���[�V�����摜
	float effect_flame[2];						// �A�j���[�V�����̑���
	int effect_count[2];						// �A�j���[�V�����̃J�E���g
	int effect_max_count[2];					// �A�j���[�V�����摜�z��̗v�f��

	// �T�E���h
	int SE[3];		// �Đ��p�ϐ�

public:
	//�R���X�g���N�^
	EnemyBase();
	//�f�X�g���N�^
	virtual ~EnemyBase();

	/// <summary>
	/// ����������
	/// </summary>
	virtual void Initialize() override;
	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="delta_second">1�t���[��������̎���</param>
	virtual void Update(float delta_second) override;
	/// <summary>
	/// �`�揈��
	/// </summary>
	virtual void Draw(const Vector2D camera_pos) const override;
	/// <summary>
	/// �I��������
	/// </summary>
	virtual void Finalize() override;

public:
	/// <summary>
	/// �����蔻��ʒm����
	/// </summary>
	/// <param name="hit_object">���������Q�[���I�u�W�F�N�g�̃|�C���^</param>
	virtual void OnHitCollision(GameObject* hit_object) override;
	/// <summary>
	/// �U���͈͒ʒm����
	/// </summary>
	/// <param name="hit_object">���������Q�[���I�u�W�F�N�g�̃|�C���^</param>
	virtual void OnAreaDetection(GameObject* hit_object) override;
	/// <summary>
	/// �U���͈͒ʒm����
	/// </summary>
	virtual void NoHit() override;
	/// <summary>
	/// ���C�g�͈͒ʒm����
	/// </summary>
	virtual void InLightRange() override;
	/// <summary>
	/// ���C�g�͈͒ʒm����
	/// </summary>
	virtual void OutLightRange() override;
	/// <summary>
	/// HP�Ǘ�����
	/// </summary>
	/// <param name="hit_object">�_���[�W</param>
	virtual void HPControl(int Damage) override;

protected:
	/// <summary>
	/// �U������
	/// </summary>
	virtual void Attack(GameObject* hit_object);
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