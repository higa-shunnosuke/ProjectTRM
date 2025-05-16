#include "Collision.h"
#include <math.h>

// 適用オブジェクトか確認する処理
bool Collision::IsCheckHitTarget(eObjectType hit_object) const
{
	// 適用するオブジェクトタイプなら、true
	for (eObjectType type : hit_object_type)
	{
		if (type == hit_object)
		{
			return true;
		}
	}

	return false;
}

// 円と矩形の当たり判定
bool Collision::CheckCircleRectCollision(Vector2D circlePos, float radius, Vector2D boxPos, Vector2D box_size)
{
	// 円の中心と矩形の最近接点の距離を計算
	float nearestX = (std::max)(boxPos.x, (std::min)(circlePos.x, boxPos.x + box_size.x));
	float nearestY = (std::max)(boxPos.y, (std::min)(circlePos.y, boxPos.y + box_size.y));

	float dx = circlePos.x - nearestX;
	float dy = circlePos.y - nearestY;

	return (dx * dx + dy * dy) < (radius * radius);
}