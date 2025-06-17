#pragma once

#include "../../Character.h"

#define GRAVITY (9.8f)
#define INITIAL_SPEED (100.0f)
#define BASIC_POWER (5)

// �Q�[���I�u�W�F�N�g���N���X
class P_Projectile :public Character
{
private:
	float Damage;	//�_���[�W
	Vector2D target_location;
	Vector2D old_location;
	float angle;
	float end_loc;
	int sounds;	//��

public:
	//�R���X�g���N�^
	P_Projectile();
	//�f�X�g���N�^
	virtual ~P_Projectile();

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

public:
	/// <summary>
	/// �����蔻��ʒm����
	/// </summary>
	/// <param name="hit_object">���������Q�[���I�u�W�F�N�g�̃|�C���^</param>
	virtual void OnHitCollision(GameObject* hit_object);

	/// <summary>
	/// �U���͈͒ʒm����
	/// </summary>
	/// <param name="hit_object">���������Q�[���I�u�W�F�N�g�̃|�C���^</param>
	virtual void OnAreaDetection(GameObject* hit_object);

private:
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
	/// SE���䏈��
	/// </summary>
	virtual void SoundControl();

public:
	void SetTargetLocation(Vector2D location);

};