#pragma once

// デバッグ用 #############################################
//#define DEBUG_KEYMAP

// デフォルトモード #############################################
#define WIN_BOOT
//#define MAC_BOOT
//#define IOS_BOOT
//#define GAME BOOT

// 設定値 #############################################
#define MOD_TAP_TH  400 // モッドタップ有効期間
#define CHANGE_OVERRIDE_DELAY 400 // キーオーバーライド移行後待ち時間

#define TAP_AFTER_DELAY_S  20 // タップ後待ち時間(ms)
#define TAP_AFTER_DELAY_L 100 // カット・ペースト後待ち時間(iOS用・ms)

#define CURSOR_MVS  10 // キャレット移動回数(少)
#define CURSOR_MVL  30 // キャレット移動回数(多)
#define MACRO_TH  160 // マクロ繰り返し間隔

#define WHEEL_MV  (IS_MAC ?   1 :   1) // ホイール移動量
#define WHEEL_THH (IS_MAC ?  30 :  20) // ホイール移動インターバル(高速)
#define WHEEL_THL (IS_MAC ? 180 : 120) // ホイール移動インターバル(低速)
#define WHEEL_THM (IS_MAC ? 150 :  60) // ホイール移動インターバル(固定)
#define WHEEL_AC  (IS_MAC ?  30 :  20) // ホイール加速度
#define WHEEL_AD  (IS_MAC ?   6 :   4) // ホイール加速減少度

#define POINTER_MV  (IS_MAC ?  1 :  3) // ポインタ移動量
#define POINTER_THH (IS_MAC ?  0 :  0) // ポインタ移動インターバル(高速)
#define POINTER_THL (IS_MAC ? 40 :120) // ポインタ移動インターバル(低速)
#define POINTER_THM (IS_MAC ?  4 : 30) // ポインタ移動インターバル(固定)
#define POINTER_AC  (IS_MAC ? 20 : 60) // ポインタ加速度
#define POINTER_AD  (IS_MAC ?  8 : 25) // ポインタ加速減少度

#define MOUSE_ON_DELAY 15 // マウスキーオンディレイ時間(シフト誤出力防止・ms)
#define POINTER_MVE    20 // ポインタエスケープ移動量
#define OFF_DELAY_TH   70 // マウスキーオフディレイ

#define WAKEUP_TH 65000 // たたき起こし間隔

// End |||||||||||||||||||||||||||||||||||||||||||||
