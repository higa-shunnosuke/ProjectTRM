#pragma once

#include "Vector2D.h"
#include <vector>

// オブジェクトタイプ
enum class eObjectType : unsigned char
{
	None,
	Player,
	Enemy,
	Ground,
	
};

// 当たり判定基底クラス
class Collision
{
public:
	bool						is_blocking;		// すり抜けフラグ
	Vector2D					box_size;			// オブジェクトのサイズ
	Vector2D					pivot;				// 始点と終点（相対座標）
	eObjectType					object_type;		// 自身のオブジェクトタイプ
	std::vector<eObjectType>	hit_object_type;	// 適用するオブジェクトタイプ

public:
	Collision() :
		is_blocking(false),
		box_size(0.0f),
		pivot(0.0f),
		object_type(eObjectType::None),
		hit_object_type()
	{
		
	}
	~Collision()
	{
		hit_object_type.clear();
	}

	// 当たり判定有効確認処理
	bool IsCheckHitTarget(eObjectType hit_object) const;
};