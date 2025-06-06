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
#define BOX_SIZE		(60)	// ブロックサイズ
#define STAGE_SIZE_X	(22)	// ステージの横幅（ブロック数）
#define STAGE_SIZE_Y	(15)	// ステージの縦幅（ブロック数）


/**********************************************************
* プロトタイプ宣言
***********************************************************/
// エラー内容を出力する
int ErrorThrow(std::string error_log);

class ProjectConfig
{
public:
	static bool DEBUG;
};