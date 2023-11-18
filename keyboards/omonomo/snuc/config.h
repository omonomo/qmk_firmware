#pragma once

/* #include "config_common.h" */

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#undef LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
#undef LOCKING_RESYNC_ENABLE

/* key combination for command */
/*
#define IS_COMMAND() ( \
    get_mods() == MOD_MASK_SHIFT \
)
*/

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
//#define NO_DEBUG
//#define DEBUG_MATRIX_SCAN_RATE // キースキャンにかかる時間を表示

/* disable print */
//#define NO_PRINT

/* disable action features */
//#define NO_ACTION_LAYER
//#define LAYER_STATE_8BIT
#define LAYER_STATE_16BIT

#define NO_ACTION_TAPPING
#define NO_ACTION_ONESHOT

/* disable music mode */
#define NO_MUSIC_MODE

/* Tap setting */
//#define TAPPING_TERM 300
#define TAP_CODE_DELAY 15
//#define PERMISSIVE_HOLD // TAPPING_TERM経過前に他のキーを押すとHOLD(離した時に出力)
//#define HOLD_ON_OTHER_KEY_PRESS // TAPPING_TERM経過前に他のキーを押すとHOLD(押した時に出力) ON推奨
//#define IGNORE_MOD_TAP_INTERRUPT // MODキーと通常キーを高速で押すと両方TAP
//#define TAPPING_FORCE_HOLD // MODキーをTAP後にすぐHOLDするとHOLD、押しっぱなしでリピート無しでHOLD ON推奨
//#define RETRO_TAPPING // TAPPING_TERMを経過しても他のキーを押していなければTAP
//#define TAPPING_TERM_PER_KEY

/* Combo setting */
//#define COMBO_TERM 500

/* Debounce Term */
#define DEBOUNCE 15 // デバウンス時間

/* MouseKey setting */
//#define MOUSEKEY_DELAY 200 // キーを押してからポインタが動き出すまでの時間
//#define MOUSEKEY_INTERVAL 20 // ポインタ移動間の時間
//#define MOUSEKEY_MAX_SPEED 4 // 最大のポインタ速度
//#define MOUSEKEY_TIME_TO_MAX 40 // ポインタの加速時間
//#define MOUSEKEY_WHEEL_DELAY 200 // キーを押してからホイールが動き出すまでの時間
//#define MOUSEKEY_WHEEL_INTERVAL 100 // ホイールの動きの間の時間
//#define MOUSEKEY_WHEEL_MAX_SPEED 8 // 最大のスクロールステップ数
//#define MOUSEKEY_WHEEL_TIME_TO_MAX 40 // スクロールの加速時間
