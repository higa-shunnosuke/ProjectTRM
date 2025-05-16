#pragma once

#include "../Utility/Singleton.h"
#include "../Utility/LightMapManager.h"
#include "SceneBase.h"

class SceneManager : public Singleton<SceneManager>
{
private:
	SceneBase* current_scene;	// 現在のシーン情報

public:
	// コンストラクタ
	SceneManager();
	// デストラクタ
	~SceneManager();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="delta_second">1フレームあたりの時間</param>
	void Update(float delta_second);

	/// <summary>
	/// 終了時処理
	/// </summary>
	void Finalize();

private:
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() const;

	/// <summary>
	/// シーン切り替え処理
	/// </summary>
	/// <param name="next_type">次のシーンタイプ</param>
	void ChangeScene(eSceneType next_type);

	/// <summary>
	/// 当たり判定確認処理
	/// </summary>
	/// <param name="target">1つ目のゲームオブジェクト</param>
	/// <param name="partner">2つ目のゲームオブジェクト</param>
	void CheckCollision(GameObject* target, GameObject* partner);

	/// <summary>
	/// 攻撃範囲検知処理
	/// </summary>
	/// <param name="target">1つ目のゲームオブジェクト</param>
	/// <param name="partner">2つ目のゲームオブジェクト</param>
	void CheckHitBox(GameObject* target, GameObject* partner);

	/// <summary>
	/// 明暗検知処理
	/// </summary>
	/// <param name="target">1つ目のゲームオブジェクト</param>
	/// <param name="partner">2つ目のゲームオブジェクト</param>
	void CheckLightRange(GameObject* target, GameObject* partner);
};