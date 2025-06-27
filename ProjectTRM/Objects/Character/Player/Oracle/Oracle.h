#pragma once

#include "../../Character.h"
#include <chrono>

// �Q�[���I�u�W�F�N�g���N���X
class Oracle :public Character
{
private:
	bool summon_flag;	//�����t���O
	Oracle* player;			// �ޏ��̃|�C���^
	int max_Hp = 100;
	std::vector<int>EffectImage;
	int old_sun_level;
	int now_sun_level;
	bool power_up;
	int effect_image;
	bool	JustDead = false;	//���S���A�j���[�V�������I����Ē��x����
	int anime_max_count;
	float move_location;
	std::chrono::steady_clock::time_point	anime_time;			// �A�j���[�V�������Z�p�ϐ�
	int sounds;


public:
	//�R���X�g���N�^
	Oracle();
	//�f�X�g���N�^
	virtual ~Oracle();

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
	virtual void Draw() const;
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

public:
	/// <summary>
	/// HP�Ǘ�����
	/// </summary>
	/// <param name="hit_object">�_���[�W</param>
	virtual void HPControl(float Damage);

	bool GetDead();

	/// <summary>
	/// �ʒu���ύX����
	/// </summary>
	/// <param name="location">�ύX�������ʒu���</param>
	void SetLocation(const Vector2D& location) override;

	State GetNowState() const;

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