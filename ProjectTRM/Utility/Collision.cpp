#include "Collision.h"
#include <math.h>
#include <cmath>

// �K�p�I�u�W�F�N�g���m�F���鏈��
bool Collision::IsCheckHitTarget(eObjectType hit_object) const
{
	// �K�p����I�u�W�F�N�g�^�C�v�Ȃ�Atrue
	for (eObjectType type : hit_object_type)
	{
		if (type == hit_object)
		{
			return true;
		}
	}

	return false;
}

// �~�Ƌ�`�̓����蔻��
bool Collision::CheckCircleRectCollision(Vector2D circlePos, float radius, Vector2D BoxPos)
{
	// �~�̒��S�ƑΏۂ̍ŋߐړ_�̋������v�Z
	float distance;
	float dx = circlePos.x - BoxPos.x;
	float dy = circlePos.y - BoxPos.y;
	distance = std::sqrt(dx * dx + dy * dy);

	return radius >= distance;
}