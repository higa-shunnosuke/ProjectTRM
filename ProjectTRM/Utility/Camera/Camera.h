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
	float zoom = 1.0f;	// ズーム倍率

	// ズーム倍率の範囲
	const float ZOOM_MIN = 1.0f;
	const float ZOOM_MAX = 2.0f;

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
	/// スクリーン描画
	/// </summary>
	/// <param name="back_buffer">表示する画面</param>
	void Draw(int back_buffer);

	/// <summary>
	/// ズーム処理
	/// </summary>
	void Zoom();

	/// <summary>
	/// スクロール処理
	/// </summary>
	void Scroll();

	/// <summary>
	///	カメラ座標指定処理
	/// </summary>
	/// <param name="location">指定座標</param>
	void SetCameraPos(Vector2D location);

	/// <summary>
	/// 巫女情報設定処理
	/// </summary>
	/// <param name="oracle"></param>
	void SetPlayer(Oracle* oracle);

	/// <summary>
	/// カメラ拡大率指定処理
	/// </summary>
	/// <param name="zoom"></param>
	void SetZoom(float zoom);

	/// <summary>
	/// カメラ拡大率取得処理
	/// </summary>
	/// <returns></returns>
	float GetZoom();
};

