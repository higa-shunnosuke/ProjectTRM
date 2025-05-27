#include "Collision.h"
#include <math.h>
#include <cmath>

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
bool Collision::CheckCircleRectCollision(Vector2D circlePos, float radius, Vector2D BoxPos)
{
	// 円の中心と対象の最近接点の距離を計算
	float distance;
	float dx = circlePos.x - BoxPos.x;
	float dy = circlePos.y - BoxPos.y;
	distance = std::sqrt(dx * dx + dy * dy);

	return radius >= distance;
}