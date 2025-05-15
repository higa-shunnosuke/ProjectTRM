#pragma once

#include "DxLib.h"
#include "../Utility/Collision.h"
#include "../Utility/ProjectConfig.h"
#include "../Utility/ResourceManager.h"

// ゲームオブジェクト基底クラス
class GameObject
{
protected:
	Vector2D		location;		// 位置座標
	Collision		collision;		// 当たり判定情報
	int				image;			// 画像情報
	int				z_layer;		// レイヤー情報
	int				HP;				// キャラクターのHP
	bool			is_mobility;	// 可動性
	bool			is_aggressive;	// 攻撃性
	bool			in_light;		// ライトの範囲内にいるか

public:
	GameObject();
	virtual ~GameObject();

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Initialize();
	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="delta_second">1フレームあたりの時間</param>
	virtual void Update(float delta_second);
	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw(const Vector2D camera_pos) const;
	/// <summary>
	/// 終了時処理
	/// </summary>
	virtual void Finalize();

public:
	/// <summary>
	/// 当たり判定通知処理
	/// </summary>
	/// <param name="hit_object">当たったゲームオブジェクトのポインタ</param>
	virtual void OnHitCollision(GameObject* hit_object);

	/// <summary>
	/// 攻撃範囲通知処理
	/// </summary>
	/// <param name="hit_object">当たったゲームオブジェクトのポインタ</param>
	virtual void OnAreaDetection(GameObject* hit_object);

	/// <summary>
	/// 攻撃範囲通知処理
	/// </summary>
	virtual void NoHit();

	/// <summary>
	/// ライト範囲通知処理
	/// </summary>
	virtual void InLightRange();

	/// <summary>
	/// ライト範囲通知処理
	/// </summary>
	virtual void OutLightRange();
public:
	/// <summary>
	/// 位置座標取得処理
	/// </summary>
	/// <returns>位置座標情報</returns>
	const Vector2D& GetLocation() const;

	/// <summary>
	/// 位置情報変更処理
	/// </summary>
	/// <param name="location">変更したい位置情報</param>
	void SetLocation(const Vector2D& location);

	/// <summary>
	/// 当たり判定取得処理
	/// </summary>
	/// <returns>当たり判定情報</returns>
	const Collision& GetCollision() const;

	/// <summary>
	/// Ｚレイヤー情報取得処理
	/// </summary>
	/// <returns>Ｚレイヤー情報</returns>
	const unsigned char GetZLayer() const;

	/// <summary>
	/// 可動性情報の取得処理
	/// </summary>
	/// <returns>可動性情報</returns>
	const bool GetMobility() const;
	
	/// <summary>
	/// 攻撃性情報の取得処理
	/// </summary>
	/// <returns>可動性情報</returns>
	const bool GetAggressive() const;

	/// <summary>
	/// HP管理処理
	/// </summary>
	/// <param name="hit_object">ダメージ</param>
	virtual void HPControl(int Damage);

	/// <summary>
	/// HP取得処理
	/// </summary>
	/// <returns>HPの情報</returns>
	const virtual int GetHP() const;

	/// <summary>
	/// ライト内フラグ取得処理
	/// </summary>
	/// <returns></returns>
	const virtual bool GetInLight() const;
};