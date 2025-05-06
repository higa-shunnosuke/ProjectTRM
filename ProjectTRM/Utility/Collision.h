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
	Vector2D					attack_size;			// �U������̃T�C�Y
	Vector2D					pivot;				// �n�_�ƏI�_�i���΍��W�j
	eObjectType					object_type;		// ���g�̃I�u�W�F�N�g�^�C�v
	std::vector<eObjectType>	hit_object_type;	// �K�p����I�u�W�F�N�g�^�C�v

public:
	// �R���X�g���N�^
	Collision() :
		is_blocking(false),
		box_size(0.0f),
		attack_size(0.0f),
		pivot(0.0f),
		object_type(eObjectType::None),
		hit_object_type()
	{
		
	}

	// �f�X�g���N�^
	~Collision()
	{
		// �R���e�i�̒��g�����
		hit_object_type.clear();
	}

	// �����蔻��L���m�F����
	bool IsCheckHitTarget(eObjectType hit_object) const;
};