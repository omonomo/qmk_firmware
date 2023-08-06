#pragma once

#include QMK_KEYBOARD_H
#include "main_code_layer.h"

// 配列要素数計算 #############################################
#define ARRAY_NUM(array) sizeof(array) / sizeof(array[0])

// if文用 #############################################
#define IS_KEY_PRESS (record->event.pressed) // キーを押した時か

// ファームウェア起動時実行用 #############################################
#define WIN_DEFAULT \
OS_WIN;\
KEYBOARD_JIS;\
LAYOUT_COLEMAK;\
GAME_MODE_OFF;\
ROMAZI_NO_OFF;\
MODERN_EJ;\
GUI_TO_CTL;\
TEXT_EDIT_ON;\
HALF_SYMBOL_ON

#define MAC_DEFAULT \
OS_MAC;\
KEYBOARD_US;\
LAYOUT_COLEMAK;\
GAME_MODE_OFF;\
ROMAZI_NO_OFF;\
MODERN_EJ;\
GUI_TO_GUI;\
TEXT_EDIT_OFF;\
HALF_SYMBOL_OFF

#define IOS_DEFAULT \
OS_IOS;\
KEYBOARD_JIS;\
LAYOUT_COLEMAK;\
GAME_MODE_OFF;\
ROMAZI_OUT_OFF;\
MODERN_EJ;\
GUI_TO_GUI;\
TEXT_EDIT_ON;\
HALF_SYMBOL_ON

#define GAME_DEFAULT \
OS_WIN;\
KEYBOARD_JIS;\
LAYOUT_QWERTY;\
GAME_MODE_ON;\
ROMAZI_NO_OFF;\
MODERN_EJ;\
GUI_TO_GUI;\
TEXT_EDIT_ON;\
HALF_SYMBOL_OFF

// 押下キー履歴用 #############################################
#define LAST2_KEYCODE (global->last2_keycode)
#define LAST_KEYCODE  (global->last_keycode)
#define NOW_KEYCODE   (global->now_keycode)

// 各種フラグ用 #############################################
enum various {
	JIS_KEYBOARD, // JIS/ANSIIキーボード
	QWERTY_LAYOUT, // QWERTY/COLEMAK配列
	ALT_GRV, // 入力切り替え方法(Windows用 ALT-GRV/LANGまたは変換(LANGと変換はキーボードの種類で切り替え))
	GAME_MODE, // ゲームモード
	GUI2CTL, // GUIとCTLを入れ替え
	TEXT_EDITING, // テキスト編集機能(DefaultKeyBingingsを使わない場合)
	HALF_SYMBOL, // 全・半角切り替え
	MOUSE_MODE, // マウスモード
	MOD_DIFFERENT, // 押しているMODキーとレジストされているMODが異なる
	KEY_PRESS_AFTER_MOD // MODを押した後に別のキーが押されたか(Windows用)
};

// モード判定用 #############################################
#define IS_WIN (global->operating_system == WIN)
#define IS_MAC (global->operating_system == MAC)
#define IS_IOS (global->operating_system == IOS)

#define IS_ROMAZI_OFF  (global->romazi_mode % 3 == 0)
#define IS_ROMAZI_TEMP (global->romazi_mode % 3 == 1)
#define IS_ROMAZI_ON   (global->romazi_mode % 3 == 2)

#define IS_ROMAZI_NO_ON  (global->romazi_mode == NO_ON)
#define IS_ROMAZI_OUT_ON (global->romazi_mode == OUT_ON)
#define IS_ROMAZI_IN_ON  (global->romazi_mode == IN_ON)

#define IS_ANY_MOD_PRESS       (global->p_mod_flag)
#define IS_ANY_OTHER_MOD_PRESS (global->p_mod_flag_ot)
#define ANY_MOD_PRESS          IS_ANY_MOD_PRESS
#define ANY_OTHER_MOD_PRESS    IS_ANY_OTHER_MOD_PRESS

#define IS_JIS                 (global->various_flag & (1 << JIS_KEYBOARD))
#define IS_QWERTY              (global->various_flag & (1 << QWERTY_LAYOUT))
#define IS_LEGACY_EJ           (global->various_flag & (1 << ALT_GRV))
#define IS_GAME_MODE           (global->various_flag & (1 << GAME_MODE))
#define GUI_IS_CTL             (global->various_flag & (1 << GUI2CTL))
#define IS_TEXT_EDIT           (global->various_flag & (1 << TEXT_EDITING))
#define IS_HALF_SYMBOL         (global->various_flag & (1 << HALF_SYMBOL))
#define IS_MOUSE_MODE          (global->various_flag & (1 << MOUSE_MODE))
#define IS_MOD_DIFFERENT       (global->various_flag & (1 << MOD_DIFFERENT))
#define IS_KEY_PRESS_AFTER_MOD (global->various_flag & (1 << KEY_PRESS_AFTER_MOD))

// モード切り替え用 #############################################
// レイヤー変更前にフラグを更新すること
#define OS_WIN global->operating_system = WIN
#define OS_MAC global->operating_system = MAC
#define OS_IOS global->operating_system = IOS

#define ROMAZI_NO_OFF  global->romazi_mode = NO_OFF;  tap_code(KK_EISU)
#define ROMAZI_NO_ON   global->romazi_mode = NO_ON;   tap_code(KK_KANA)
#define ROMAZI_OUT_OFF global->romazi_mode = OUT_OFF; tap_code(KK_EISU)
#define ROMAZI_OUT_ON  global->romazi_mode = OUT_ON;  tap_code(KK_KANA); layer_on(_MIZU)
#define ROMAZI_IN_OFF  global->romazi_mode = IN_OFF;  tap_code(KK_EISU)
#define ROMAZI_IN_ON   global->romazi_mode = IN_ON;   tap_code(KK_KANA)

#define ROMAZI_OFF  global->romazi_mode -= global->romazi_mode % 3
#define ROMAZI_TEMP global->romazi_mode += 1 - (global->romazi_mode % 3)
#define ROMAZI_ON   global->romazi_mode += 2 - (global->romazi_mode % 3)

#define KEYBOARD_US  global->various_flag &= ~(1 << JIS_KEYBOARD)
#define KEYBOARD_JIS global->various_flag |=  (1 << JIS_KEYBOARD)

#define LAYOUT_COLEMAK global->various_flag &= ~(1 << QWERTY_LAYOUT);
#define LAYOUT_QWERTY  global->various_flag |=  (1 << QWERTY_LAYOUT);  layer_on(_QWERTY)

#define GAME_MODE_OFF global->various_flag &= ~(1 << GAME_MODE); layer_off(_GAME)
#define GAME_MODE_ON  global->various_flag |=  (1 << GAME_MODE);  layer_on(_GAME)

#define MODERN_EJ global->various_flag &= ~(1 << ALT_GRV)
#define LEGACY_EJ global->various_flag |=  (1 << ALT_GRV)

#define GUI_TO_GUI global->various_flag &= ~(1 << GUI2CTL)
#define GUI_TO_CTL global->various_flag |=  (1 << GUI2CTL)

#define TEXT_EDIT_OFF global->various_flag &= ~(1 << TEXT_EDITING)
#define TEXT_EDIT_ON  global->various_flag |=  (1 << TEXT_EDITING)

#define HALF_SYMBOL_OFF global->various_flag &= ~(1 << HALF_SYMBOL)
#define HALF_SYMBOL_ON  global->various_flag |=  (1 << HALF_SYMBOL)

#define MOUSE_MODE_OFF global->various_flag &= ~(1 << MOUSE_MODE)
#define MOUSE_MODE_ON  global->various_flag |=  (1 << MOUSE_MODE)

#define MOD_DIFFERENT_OFF global->various_flag &= ~(1 << MOD_DIFFERENT)
#define MOD_DIFFERENT_ON  global->various_flag |=  (1 << MOD_DIFFERENT)

#define KEY_PRESS_AFTER_MOD_OFF global->various_flag &= ~(1 << KEY_PRESS_AFTER_MOD)
#define KEY_PRESS_AFTER_MOD_ON  global->various_flag |=  (1 << KEY_PRESS_AFTER_MOD)

// 型定義 #############################################
typedef enum {
	WIN, MAC, IOS
} system_e; // OSモード

typedef enum {
	NO_OFF,  NO_TEMP,  NO_ON,
	OUT_OFF, OUT_TEMP, OUT_ON,
	IN_OFF,  IN_TEMP,  IN_ON
} romazi_e; // ローマ字モード

typedef struct {
	system_e operating_system; // 使用OS Windows/macOS/iOS
	romazi_e romazi_mode; // ローマ字モード(不使用-OFF/不使用-ON/出力-OFF/出力-ON/入力-OFF/入力-ON)

	uint16_t last2_keycode; // 前々回に押したキーコード
	uint16_t last_keycode; // 前回押したキーコード
	uint16_t now_keycode; // 今回押したキーコード

	uint16_t p_mod_flag; // MOD押下フラグ
	uint16_t p_mod_flag_ot; // 押したキーを除いたMOD押下フラグ(離した時の判定はp_mod_flagを利用すること)

	uint16_t various_flag; // 各種フラグ
} global_s;

// 関数プロトタイプ宣言 #############################################
//global_s *global_val_p(void);
uint16_t kk_eisu(global_s *global);
uint16_t kk_kana(global_s *global);

// End |||||||||||||||||||||||||||||||||||||||||||||
