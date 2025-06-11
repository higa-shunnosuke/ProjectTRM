#pragma once

#include "../Singleton.h"
#include "../Vector2D.h"
#include "../../Objects/Character/Player/Oracle/Oracle.h"

class Camera : public Singleton<Camera>
{
private:
	Vector2D location;	// カメラ座標
	Vector2D size;		// スクリーンサイズ
	Oracle* oracle;		// 巫女のポインタ
	bool tracking_flag;	// 追跡フラグ

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

	/// <summary>
	/// スクリーンサイズ取得処理
	/// </summary>
	/// <returns>スクリーンサイズ</returns>
	Vector2D GetScreeenSize();

	/// <summary>
	/// 巫女情報設定処理
	/// </summary>
	/// <param name="oracle"></param>
	void SetPlayer(Oracle* oracle);
};

