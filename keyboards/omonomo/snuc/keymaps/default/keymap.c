#include "main_keymap.h"
#include "main_code_layer.h"

#include "ini_config.h"
#include "ini_keymap.h"
#include "sub_counter.h"
#include "sub_flag_mode.h"
#include "sub_key_mod.h"
#include "sub_layer.h"
#include "sub_mouse.h"
#include "sub_romazi.h"
#include "sub_other.h"
#include "sub_override.h"

// フラグ・変数 #############################################
static global_s global_val = {
	WIN, // operating_system
	NO_OFF, // romazi_mode
	0, // last2_keycode
	0, // last_keycode
	0, // now_keycode
	0, // p_mod_flag
	0, // p_mod_flag_ot
	0  // various_flag
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {LAYER_KEYMAP}; // 各レイヤーのキーマップ

// 関数プロトタイプ宣言 #############################################
static global_s *global_val_p(void);

// 補助関数 |||||||||||||||||||||||||||||||||||||||||||||

// グローバル変数ポインタ関数 #############################################
static global_s *global_val_p(void) {
	return &global_val;
}

// 英数切換キーコード出力関数 #############################################
uint16_t kk_eisu(global_s *global) {
	return (!IS_WIN ? KC_LNG2 :
	  (IS_LEGACY_EJ ? A(KC_GRV) :
	        (IS_JIS ? CK_MHEN : KC_LNG2)));
}

// かな切換キーコード出力関数 #############################################
uint16_t kk_kana(global_s *global) {
	return (!IS_WIN ? KC_LNG1 :
	  (IS_LEGACY_EJ ? A(KC_GRV) :
	        (IS_JIS ? CK_HENK : KC_LNG1)));
}

// 実行命令 |||||||||||||||||||||||||||||||||||||||||||||

// ファームウェア起動時実行命令 #############################################
void keyboard_post_init_user(void){
	global_s *global = global_val_p();

	#ifdef DEBUG_KEYMAP
		debug_enable = true;
		debug_matrix = true;
//	debug_keyboard = true;
//	debug_mouse = true;
	#endif

	#ifdef WIN_BOOT
		WIN_DEFAULT;
	#endif
	#ifdef MAC_BOOT
		MAC_DEFAULT;
	#endif
	#ifdef IOS_BOOT
		IOS_DEFAULT;
	#endif
	#ifdef GAME_BOOT
		GAME_DEFAULT;
	#endif
}

// 常時バックグランド実行命令 #############################################
void matrix_scan_user(void) {
	global_s *global = global_val_p();

	ms_mouse_key(global); // マウス移動実行
	ms_repeat_macro(global); // 繰り返しマクロ実行
	ms_key_override(); // キーオーバーライド シフトアップ時対応
	ms_wakeup(global); // 不眠モード実行
	ms_countdown(); // カウンター
	return;
}

// レイヤー状態変更時実行命令 #############################################
layer_state_t layer_state_set_user(layer_state_t state) {
	global_s *global = global_val_p();

	return ls_change(state, global);
}

// キー入力状態変更時実行命令 #############################################
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	global_s *global = global_val_p();

	pr_p_mod_flag(keycode, record, global); // MOD押下フラグ(最初に置くこと)
	pr_cancel (keycode, record, global); // モッドタップ等解除
	pr_layer_change(global); // レイヤー切り替え
	pr_change_en(keycode, record, global); // 入力時英数切り替え
	pr_windows(keycode, record, global); // Windows対応
	if (!pr_mod_replace(keycode, record, global)) return false; // MOD置き換え(キーオーバーライドより前に置くこと)
	if (!pr_mouse_key(keycode, record, global)) return false; // マウスキー(METキーシフトより前に置くこと)
	if (!pr_second_prefix(keycode, record, global)) return false; // 2次プレフィクス(METキーシフトより前に置くこと)
	if (!pr_metkey(keycode, record, global)) return false; // METキーシフト(モッドタップより前に置くこと)
	if (!pr_layer_one_shot(keycode, record, global)) return false; // ワンショットレイヤー
// 以上、処理後return trueあり
	if (!pr_romazi_output(keycode, record, global)) return false; // ローマ字出力用
	if (!pr_romazi_input(keycode, record, global)) return false; // ローマ字入力用(キーオーバーライドより前に置くこと)
	if (!pr_numeric_hyper(keycode, record, global)) return false; // テンキー・Hyper(キーオーバーライドより前に置くこと)
	if (!pr_key_override(keycode, record, global)) return false; // キーオーバーライド(モッドタップより前に置くこと、処理後return trueあり)
	if (!pr_layer_tap(keycode, record)) return false; // レイヤータップ
	if (!pr_mod_tap_dance(keycode, record, global)) return false; // モッドタップ関係
	if (!pr_mode_change(keycode, record, global)) return false; // モード変更

	return true;
}

// End |||||||||||||||||||||||||||||||||||||||||||||
