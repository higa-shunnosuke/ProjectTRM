#pragma once

#include "../Singleton.h"
#include "../Vector2D.h"

class Camera : public Singleton<Camera>
{
private:
	Vector2D location;	// カメラ座標
	Vector2D size;		// スクリーンサイズ

public:
	// コンストラクタ
	Camera();
	// デストラクタ
	~Camera();

public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	///  実行処理
	/// </summary>
	void Update();

	/// <summary>
	/// カメラ座標取得処理
	/// </summary>
	/// <returns>カメラ座標</returns>
	Vector2D GetCameraPos();

	/// <summary>
	///	カメラ座標指定処理
	/// </summary>
	/// <param name="location">指定座標</param>
	void SetCameraPos(Vector2D location);
};

