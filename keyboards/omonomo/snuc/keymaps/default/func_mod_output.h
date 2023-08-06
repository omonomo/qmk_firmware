#pragma once

#include QMK_KEYBOARD_H
#include "main_keymap.h"

// MOD押下フラグ用 #############################################
enum p_mod_flag_bit {
	LCTL, RCTL, // CTRL  C
	ENT,        // CTRL_ENTER T
	LSFT, RSFT, // SHIFT S
	LSPC, RSPC, // SHIFT_SPACE E
	LALT, RALT, // ALT  A
	LGUI, RGUI, // GUI  G
	APP,        // APPS P
	LFN, RFN,   // FN   F
	MET,        // MET  M
	TAB         // LAYER_TAB  B
};

#define P_MOD_FLAG_KEY_G /* フラグに対応するキーコード(gui2ctlオフ時 register_p_mods用) */ \
	KC_LCTL, KC_RCTL,\
	KC_RCTL,\
	KC_LSFT, KC_RSFT,\
	KC_LSFT, KC_RSFT,\
	KC_LALT, KC_RALT,\
	KC_LGUI, KC_RGUI,\
	KC_APP,\
	0, 0,\
	MET_KEY,\
	0

#define P_MOD_FLAG_KEY_C /* フラグに対応するキーコード(gui2ctlオン時 register_p_mods用) */ \
	KC_LGUI, KC_RGUI,\
	KC_RCTL,\
	KC_LSFT, KC_RSFT,\
	KC_LSFT, KC_RSFT,\
	KC_LALT, KC_RALT,\
	KC_LCTL, KC_RCTL,\
	KC_APP,\
	0, 0,\
	MET_KEY,\
	0

#define P_MOD_KEYCODE /* 操作するキー(code_to_p_mod_flag用) */ \
	KC_LCTL, DT_LCTL_HYPR,\
	KC_RCTL, DT_RCTL_HYPR,\
	MT_RCTL_ENT,\
	KC_LSFT, DT_LSFT_HYPR,\
	KC_RSFT, DT_RSFT_HYPR,\
	MT_LSFT_SPC,\
	MT_RSFT_SPC,\
	KC_LALT,\
	KC_RALT,\
	KC_LGUI, MT_LGUI_EISU,\
	KC_RGUI, MT_RGUI_KANA,\
	KC_APP,\
	LT_LFNC_ESC,\
	LT_RFNC_ESC,\
	CK_MET,\
	LT_CRSR_TAB

#define P_MOD_FLAG /* 変化するフラグ(code_to_p_mod_flag用) */ \
	LCTL, LCTL,\
	RCTL, RCTL,\
	ENT,\
	LSFT, LSFT,\
	RSFT, RSFT,\
	LSPC,\
	RSPC,\
	LALT,\
	RALT,\
	LGUI, LGUI,\
	RGUI, RGUI,\
	APP,\
	LFN,\
	RFN,\
	MET,\
	TAB
// (追加・削除する場合はフラグの変更も忘れないこと)

// モッド押下フラグ判定用 #############################################
#define _LC | (1 << LCTL)
#define _RC | (1 << RCTL)
#define  _C _LC _RC
#define  _T | (1 << ENT)
#define _LS | (1 << LSFT)
#define _RS | (1 << RSFT)
#define  _S _LS _RS
#define _LE | (1 << LSPC)
#define _RE | (1 << RSPC)
#define  _E _LE _RE
#define _LA | (1 << LALT)
#define _RA | (1 << RALT)
#define  _A _LA _RA
#define _LG | (1 << LGUI)
#define _RG | (1 << RGUI)
#define  _G _LG _RG
#define  _P | (1 << APP)
#define _LF | (1 << LFN)
#define _RF | (1 << RFN)
#define  _F _LF _RF
#define  _M | (1 << MET)
#define  _B | (1 << TAB)
#define _ALL | (~0)

#define MOD_MASK(bit)      (0 bit) // 指定したMOD
#define MOD_MASK_EX(bit) (~(0 bit)) // 指定した以外のMOD
#define IS_MOD_PRESS(bit)    (IS_ANY_MOD_PRESS & MOD_MASK(bit)) // 指定したMODが押されているか
#define IS_MOD_PRESS_EX(bit) (IS_ANY_MOD_PRESS & MOD_MASK_EX(bit)) // 指定した以外のMODが押されているか
#define IS_MOD_PRESS_ONLY(bit) (IS_MOD_PRESS(bit) && !IS_MOD_PRESS_EX(bit)) // 指定したMODのいずれかのみ押されているか
#define IS_MOD_PRESS_COMP(bit) (IS_ANY_MOD_PRESS == MOD_MASK(bit)) // 指定したMODと相違が全くないか

#define IS_OTHER_MOD_PRESS(bit)    (IS_ANY_OTHER_MOD_PRESS & MOD_MASK(bit)) // 押したMODキーを除いて指定したMODが押されているか
#define IS_OTHER_MOD_PRESS_EX(bit) (IS_ANY_OTHER_MOD_PRESS & MOD_MASK_EX(bit)) // 押したMODキーを除いて指定した以外のMODが押されているか
#define IS_OTHER_MOD_PRESS_ONLY(bit) (IS_OTHER_MOD_PRESS(bit) && !IS_OTHER_MOD_PRESS_EX(bit)) // 押したMODキーを除いて指定したMODのいずれかのみ押されているか
#define IS_OTHER_MOD_PRESS_COMP(bit) (IS_ANY_OTHER_MOD_PRESS == MOD_MASK(bit)) // 押したMODキーを除いて指定したMODと相違が全くないか

// キー置き替え・マクロ用 #############################################
#define CLEAR_MODS                      UNREGISTER_P_MODS(MOD_MASK(_C _S _A _G))
#define UNREGISTER_MODS(bit)            UNREGISTER_P_MODS(MOD_MASK(bit))
#define UNREGISTER_MODS_IF_RELEASE(bit) UNREGISTER_P_MODS(~IS_MOD_PRESS(bit) & MOD_MASK(bit))

#define REGISTER_MODS(bit)          REGISTER_P_MODS(MOD_MASK(bit))
#define REGISTER_MODS_IF_PRESS(bit) REGISTER_P_MODS(IS_MOD_PRESS(bit))
#define REGISTER_MODS_IF_KEY_RELEASE(bit) \
	if (!IS_KEY_PRESS) {\
		REGISTER_MODS_IF_PRESS(bit);\
	}
#define REGISTER_OTHER_MODS_IF_PRESS(bit) REGISTER_P_MODS(IS_OTHER_MOD_PRESS(bit))

// get_mods()用のフラグ取得 #############################################
#define CODE_TO_GET_MODS(mod) (mod & 0x07)
#define PAIR_CODE_TO_GET_MODS(mod) (CODE_TO_GET_MODS(mod) ^ 0x04)

// 関数置き替え #############################################
#define P_MOD_FLAG_FILTER(flag) p_mod_flag_filter(flag, global)

#define UNREGISTER_P_MODS(flag) unregister_p_mods(flag, global)
#define REGISTER_P_MODS(flag)     register_p_mods(flag, global)

#define UNREGISTER_P_MOD_IF_ALONE(code, flag) unregister_p_mod_if_alone(code, flag, global)
#define REGISTER_P_MOD_IF_ALONE(code, flag)     register_p_mod_if_alone(code, flag, global)

#define FIND_P_MOD(code, flag) find_p_mod(code, flag, global)
#define P_MOD_FLAG_TO_CODE(flag) p_mod_flag_to_code(flag, global)
#define INVALID_ONE_SHOT(mod) invalid_one_shot(mod, global)

// 関数プロトタイプ宣言 #############################################
void p_mod_flag_filter(uint16_t *target_flag, global_s *global);

void register_p_mods(uint16_t reg_flag, global_s *global);
void unregister_p_mods(uint16_t reg_flag, global_s *global);

void register_p_mod_if_alone(uint8_t reg_mod, uint16_t target_flag, global_s *global);
void unregister_p_mod_if_alone(uint8_t reg_mod, uint16_t target_flag, global_s *global);

bool find_p_mod(uint8_t source_mod, uint16_t target_flag, global_s *global);
uint16_t code_to_p_mod_flag(uint16_t mod_key);
uint8_t p_mod_flag_to_code(uint16_t mod_flag, global_s *global);

bool invalid_one_shot(uint8_t reg_mod, global_s *global);

// End |||||||||||||||||||||||||||||||||||||||||||||
