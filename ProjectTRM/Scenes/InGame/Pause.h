#pragma once

#include "../../Utility/Singleton.h"

class Pause:public Singleton<Pause>
{
private:
	bool is_pause;	// ポーズフラグ

public:
	// コンストラクタ
	Pause();
	// デストラクタ
	~Pause();

public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="delta_second">１フレーム当たりの時間</param>
	void Update(float delta_second);
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() const;

public:
	/// <summary>
	/// ポーズフラグ設定処理
	/// </summary>
	/// <param name="flag">true:ポーズ中、false:ポーズ解除</param>
	void SetPause(bool flag);
	/// <summary>
	/// ポーズフラグ取得処理
	/// </summary>
	/// <returns>ポーズフラグ</returns>
	bool GetPause();
};

