/* Copyright 2019 Brandon Schlack
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H
#include "brandonschlack.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Base Layer
 * ┌───┬───┬───┬───┬───┬───┬─┬───┬───┬───┬───┬─┬───┬───┬───┬───┬─┬───┬───┬───┐
 * │Esc│   │ F1│ F2│ F3│ F4│ │ F5│ F6│ F7│ F8│ │ F9│F10│F11│F12│ │F13│F14│F15│
 * ├───┘   └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┤
 * ├───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┐ ┌───┬───┬───┤
 * │~ `│! 1│@ 2│# 3│$ 4│% 5│^ 6│& 7│* 8│( 9│) 0│_ -│+ =│ Bckspc│ │Ins│Hom│PgU│
 * ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┤ ├───┼───┼───┤
 * │Tab  │ Q │ W │ E │ R │ T │ Y │ U │ I │ O │ P │{ [│} ]│|   \│ │Del│End│PgD│
 * ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┤ └───┴───┴───┤
 * │HyCaps│ A │ S │ D │ F │ G │ H │ J │ K │ L │: ;│" '│   Enter│             │
 * ├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────────┤     ┌───┐   │
 * │Shift   │ Z │ X │ C │ V │ B │ N │ M │< ,│> .│? /│     Shift│     │ Up│   │
 * ├────┬───┴┬──┴─┬─┴───┴───┴───┴───┴───┴──┬┴───┼───┴┬────┬────┤ ┌───┼───┼───┤
 * │Ctrl│ Opt│ Cmd│          Space         │Cmd │Opt │Fn  │Ctrl│ │Lef│Dow│Rig│
 * └────┴────┴────┴────────────────────────┴────┴────┴────┴────┴─┴───┴───┴───┘
 */
[_BASE] = LAYOUT_tkl_ansi(
    KC_ESC,           KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,           KC_F13,  KC_F14,  KC_F15,
    KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,          KC_INS,  KC_HOME, KC_PGUP,
    KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,          KC_DEL,  KC_END,  KC_PGDN,
    HY_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,
    KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT,                   KC_UP,
    KC_LCTL, KC_LOPT, KC_LCMD,                           KC_SPC,                              RWD_CMD, FFD_OPT, PLY_FN1, KC_RCTL,          KC_LEFT, KC_DOWN, KC_RGHT
),
/* Function Layer
 * ┌───┬───┬───┬───┬───┬───┬─┬───┬───┬───┬───┬─┬───┬───┬───┬───┬─┬───┬───┬───┐
 * │Mke│   │BrD│BrU│Msn│LHP│ │RV-│RV+│   │   │ │   │   │   │   │ │   │   │Mut│
 * ├───┘   └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┤
 * ├───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┐ ┌───┬───┬───┤
 * │   │   │   │   │   │   │   │   │   │   │   │   │   │   Del │ │Ply│Slp│VlU│
 * ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┤ ├───┼───┼───┤
 * │RMod │RH+│RS+│RV+│AS+│   │   │   │   │   │   │   │   │     │ │Prv│Nxt│VlD│
 * ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┤ └───┴───┴───┤
 * │RTgl  │RH-│RS-│RV-│AS-│   │   │   │   │   │   │   │        │             │
 * ├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────────┤     ┌───┐   │
 * │        │LYR│Thm│   │   │RST│   │   │   │   │   │          │     │PgU│   │
 * ├────┬───┴┬──┴─┬─┴───┴───┴───┴───┴───┴──┬┴───┼───┴┬────┬────┤ ┌───┼───┼───┤
 * │    │    │    │                        │    │    │    │    │ │Hom│PgD│End│
 * └────┴────┴────┴────────────────────────┴────┴────┴────┴────┴─┴───┴───┴───┘
 */
[_FN1] = LAYOUT_tkl_ansi(
    QM_MAKE,          KC_BRMD, KC_BRMU, MC_MSSN, MC_LHPD, RGB_VAD, RGB_VAI, _______, _______, _______, _______, _______, _______,          _______, _______, KC_MUTE,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_DEL,           KC_MPLY, MC_SLPD, KC_VOLU,
    RGB_MOD, RGB_HUI, RGB_SAI, RGB_VAI, RGB_SPI, _______, _______, _______, _______, _______, _______, _______, _______, _______,          KC_MPRV, KC_MNXT, KC_VOLD,
    RGB_TOG, RGB_HUD, RGB_SAD, RGB_VAD, RGB_SPD, _______, _______, _______, _______, _______, _______, _______,          _______,
    _______,          RGB_LYR, RGB_THM, _______, _______, QK_BOOT, _______, _______, _______, _______, _______,          _______,                   KC_PGUP,
    _______, _______, _______,                           _______,                             _______, _______, _______, _______,          KC_HOME, KC_PGDN, KC_END
)
/* Blank Template
 * ┌───┬───┬───┬───┬───┬───┬─┬───┬───┬───┬───┬─┬───┬───┬───┬───┬─┬───┬───┬───┐
 * │   │   │   │   │   │   │ │   │   │   │   │ │   │   │   │   │ │   │   │   │
 * ├───┘   └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┤
 * ├───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┐ ┌───┬───┬───┤
 * │   │   │   │   │   │   │   │   │   │   │   │   │   │       │ │   │   │   │
 * ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┤ ├───┼───┼───┤
 * │     │   │   │   │   │   │   │   │   │   │   │   │   │     │ │   │   │   │
 * ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┤ └───┴───┴───┤
 * │      │   │   │   │   │   │   │   │   │   │   │   │        │             │
 * ├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────────┤     ┌───┐   │
 * │        │   │   │   │   │   │   │   │   │   │   │          │     │   │   │
 * ├────┬───┴┬──┴─┬─┴───┴───┴───┴───┴───┴──┬┴───┼───┴┬────┬────┤ ┌───┼───┼───┤
 * │    │    │    │                        │    │    │    │    │ │   │   │   │
 * └────┴────┴────┴────────────────────────┴────┴────┴────┴────┴─┴───┴───┴───┘
 */
/*
[BLANK] = LAYOUT_tkl_ansi(
    _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______,
    _______, _______, _______,_______,  _______, _______, _______, _______, _______, _______, _______, _______,          _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,                   _______,
    _______, _______, _______,                           _______,                             _______, _______, _______, _______,          _______, _______, _______
)
*/
};

bool process_record_keymap(uint16_t keycode, keyrecord_t *record) {
    return true;
}
