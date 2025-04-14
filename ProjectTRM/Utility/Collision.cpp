#include "Collision.h"
#include <math.h>

/// <summary>
/// 適用オブジェクトか確認する処理
/// </summary>
/// <param name="hit_object">相手のオブジェクトタイプ</param>
/// <returns>適用するオブジェクトなら、true</returns>
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
