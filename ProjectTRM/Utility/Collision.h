#pragma once

#include "Vector2D.h"
#include <vector>

// �I�u�W�F�N�g�^�C�v
enum class eObjectType : unsigned char
{
	None,
	Player,	
	Enemy,
	Ground,
};

// �����蔻����N���X
class Collision
{
public:
	bool						is_blocking;		// ���蔲���t���O
	Vector2D					box_size;			// �I�u�W�F�N�g�̃T�C�Y
	Vector2D					attack_size;		// �U������̃T�C�Y
	Vector2D					pivot;				// �n�_�ƏI�_�i���΍��W�j
	eObjectType					object_type;		// ���g�̃I�u�W�F�N�g�^�C�v
	std::vector<eObjectType>	hit_object_type;	// �K�p����I�u�W�F�N�g�^�C�v
	float						radius;				// �~�̔��a
public:
	// �R���X�g���N�^
	Collision() :
		is_blocking(false),
		box_size(0.0f),
		attack_size(0.0f),
		pivot(0.0f),
		object_type(eObjectType::None),
		hit_object_type(),
		radius()
	{
		
	}

	// �f�X�g���N�^
	~Collision()
	{
		// �R���e�i�̒��g�����
		hit_object_type.clear();
	}

	/// <summary>
	/// �K�p�I�u�W�F�N�g���m�F���鏈��
	/// </summary>
	/// <param name="hit_object">����̃I�u�W�F�N�g�^�C�v</param>
	/// <returns>�K�p����I�u�W�F�N�g�Ȃ�Atrue</returns>
	bool IsCheckHitTarget(eObjectType hit_object) const;

	/// <summary>
	/// �~�Ƌ�`�̓����蔻��
	/// </summary>
	/// <param name="circlePos">�~�̒��S</param>
	/// <param name="radius">�~�̔��a</param>
	/// <param name="boxPos">��`�̒��S</param>
	/// <param name="box_size">��`�̑傫��</param>
	/// <returns></returns>
	bool CheckCircleRectCollision(Vector2D circlePos, float radius, Vector2D boxPos, Vector2D box_size);
};