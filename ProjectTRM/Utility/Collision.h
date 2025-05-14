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
	Vector2D					attack_size;		// 攻撃判定のサイズ
	Vector2D					pivot;				// 始点と終点（相対座標）
	eObjectType					object_type;		// 自身のオブジェクトタイプ
	std::vector<eObjectType>	hit_object_type;	// 適用するオブジェクトタイプ
	float						radius;				// 円の半径
public:
	// コンストラクタ
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

	// デストラクタ
	~Collision()
	{
		// コンテナの中身を解放
		hit_object_type.clear();
	}

	/// <summary>
	/// 適用オブジェクトか確認する処理
	/// </summary>
	/// <param name="hit_object">相手のオブジェクトタイプ</param>
	/// <returns>適用するオブジェクトなら、true</returns>
	bool IsCheckHitTarget(eObjectType hit_object) const;

	/// <summary>
	/// 円と矩形の当たり判定
	/// </summary>
	/// <param name="circlePos">円の中心</param>
	/// <param name="radius">円の半径</param>
	/// <param name="boxPos">矩形の中心</param>
	/// <param name="box_size">矩形の大きさ</param>
	/// <returns></returns>
	bool CheckCircleRectCollision(Vector2D circlePos, float radius, Vector2D boxPos, Vector2D box_size);
};