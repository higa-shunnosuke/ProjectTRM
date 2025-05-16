#include "Collision.h"
#include <math.h>

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
bool Collision::CheckCircleRectCollision(Vector2D circlePos, float radius, Vector2D boxPos, Vector2D box_size)
{
	// �~�̒��S�Ƌ�`�̍ŋߐړ_�̋������v�Z
	float nearestX = (std::max)(boxPos.x, (std::min)(circlePos.x, boxPos.x + box_size.x));
	float nearestY = (std::max)(boxPos.y, (std::min)(circlePos.y, boxPos.y + box_size.y));

	float dx = circlePos.x - nearestX;
	float dy = circlePos.y - nearestY;

	return (dx * dx + dy * dy) < (radius * radius);
}