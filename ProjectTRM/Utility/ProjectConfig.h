#pragma once

#include <string>

/**********************************************************
* マクロ定義
***********************************************************/
#define	D_SUCCESS		(0)		// 成功
#define	D_FAILURE		(-1)	// 失敗
#define D_WIN_MAX_X		(1280)	// スクリーンサイズ（幅）
#define D_WIN_MAX_Y		(720)	// スクリーンサイズ（高さ）
#define D_COLOR_BIT		(32)	// カラービット

/**********************************************************
* プロトタイプ宣言
***********************************************************/
// エラー内容を出力する
int ErrorThrow(std::string error_log);

// プロジェクト設定クラス
class ProjectConfig
{
public:
	static bool DEBUG;			// デバックモードの切り替え用変数
	static int  STAGE_WIDTH;	// ステージの横幅
	static int  STAGE_HEIGHT;	// ステージの高さ
};