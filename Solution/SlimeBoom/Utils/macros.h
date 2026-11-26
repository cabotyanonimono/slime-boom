#pragma once
// プロジェクト共通マクロ。
// Directory.Build.props で強制インクルード (/FI) しているので、各ファイルで #include する必要はない。
// エンジンや外部ライブラリを含むすべての .cpp に入るので、
//   ・名前は必ず SB_ で始める
//   ・重いヘッダー（windows.h など）はここで include しないこと。

#ifndef SB_EDIT
#define SB_EDIT
#endif
