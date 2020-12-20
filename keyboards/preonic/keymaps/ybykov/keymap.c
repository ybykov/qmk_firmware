/* Copyright 2015-2017 Jack Humbert
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
#include "muse.h"

enum preonic_layers {
  _COLEMAK,
  _QWERTY,
  _RU,
  _LOWER,
  _RAISE,
  _ADJUST
};

enum preonic_keycodes {
  COLEMAK = SAFE_RANGE,
  QWERTY,
  RU,
  EMAIL_1,
  EMAIL_2,
  EMAIL_3,
};

#define LOWER MO(_LOWER)
#define RAISE MO(_RAISE)

/* #define UND_LGU MT(MOD_LGUI, KC_UNDS) */
#define PLS_LCT MT(MOD_LCTL, KC_PPLS)
#define EQL_LCT MT(MOD_LCTL, KC_PEQL)
#define EQL_LAL MT(MOD_LALT, KC_EQL)
#define NML_LGU MT(MOD_LGUI, KC_NLCK)
#define MIN_RCT MT(MOD_RCTL, KC_MINS)
#define F11_RSU MT(MOD_RGUI, KC_F11)

#define ESC_RSE LT(_RAISE, KC_ESC)
#define SPC_RSE LT(_RAISE, KC_SPC)
#define DEL_LWR LT(_LOWER, KC_DEL)
#define BSPC_LWR LT(_LOWER, KC_BSPC)

// tapdance keycodes https://github.com/qmk/qmk_firmware/blob/master/docs/feature_tap_dance.md#example-5-using-tap-dance-for-advanced-mod-tap-and-layer-tap-keys
enum td_keycodes {
  UND_LGU // `LGUI` when held, `_` when tapped.
};

// define a type containing as many tapdance states as you need
typedef enum {
  SINGLE_TAP,
  SINGLE_HOLD,
  DOUBLE_SINGLE_TAP
} td_state_t;

// create a global instance of the tapdance state type
static td_state_t td_state;

// declare your tapdance functions:

// function to determine the current tapdance state
int cur_dance (qk_tap_dance_state_t *state);

// `finished` and `reset` functions for each tapdance keycode
void altlp_finished (qk_tap_dance_state_t *state, void *user_data);
void altlp_reset (qk_tap_dance_state_t *state, void *user_data);

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Colemak
 * ,-----------------------------------------------------------------------------------.
 * |   `  |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  |  ]   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Super|   Q  |   W  |   F  |   P  |   G  |   J  |   L  |   U  |   Y  |   ;  | Super|
 * |   `  |      |      |      |      |      |      |      |      |      |      |  [   |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Ctrl |   A  |   R  |   S  |   T  |   D  |   H  |   N  |   E  |   I  |   O  | Ctrl |
 * |      |      |      |      |      |      |      |      |      |      |      |  '   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * | Alt  |   Z  |   X  |   C  |   V  |   B  |   K  |   M  |   ,  |   .  |   /  | Alt  |
 * |      |      |      |      |      |      |      |      |      |      |      |  \   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      | Shift|Raise | Lower| BSpc | Space|Enter |      |  RU  |Clmk  |
 * `-----------------------------------------------------------------------------------'
 */
[_COLEMAK] = LAYOUT_preonic_grid( \
  KC_GRV,         KC_1,    KC_2,    KC_3,           KC_4,    KC_5,    KC_6,     KC_7,    KC_8,           KC_9,    KC_0,    KC_RBRACKET, \
  LGUI_T(KC_GRV), KC_Q,    KC_W,    KC_F,           KC_P,    KC_G,    KC_J,     KC_L,    KC_U,           KC_Y,    KC_SCLN, RGUI_T(KC_LBRACKET), \
  KC_LCTL,        KC_A,    KC_R,    KC_S,           KC_T,    KC_D,    KC_H,     KC_N,    KC_E,           KC_I,    KC_O,    RCTL_T(KC_QUOT), \
  KC_LALT,        KC_Z,    KC_X,    KC_C,           KC_V,    KC_B,    KC_K,     KC_M,    KC_COMM,        KC_DOT,  KC_SLSH, RALT_T(KC_BSLASH),  \
  _______,        _______, _______, LSFT_T(KC_TAB), ESC_RSE, DEL_LWR, BSPC_LWR, SPC_RSE, RSFT_T(KC_ENT), _______, RU,      COLEMAK  \
),


/* Qwerty
 * ,-----------------------------------------------------------------------------------.
 * |   `  |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  |  ]   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Super|   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | Super|
 * |   `  |      |      |      |      |      |      |      |      |      |      |  [   |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Ctrl |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  | Ctrl |
 * |      |      |      |      |      |      |      |      |      |      |      |  '   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * | Alt  |   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  | Alt  |
 * |      |      |      |      |      |      |      |      |      |      |      |  \   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      | Shift|Raise | Lower| BSpc | Space|Enter |      |  RU  |Clmk  |
 * `-----------------------------------------------------------------------------------'
 */
[_QWERTY] = LAYOUT_preonic_grid( \
  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  \
  _______, KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    RGUI_T(KC_LBRACKET),  \
  _______, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, RCTL_T(KC_QUOT), \
  _______, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, RALT_T(KC_BSLASH),  \
  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______  \
),

/* RU
 * ,-----------------------------------------------------------------------------------.
 * |   `  |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  | Del  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  |  [   |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Esc  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  |  "   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |  I3  |   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  |  ]   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Ctrl | Alt  | GUI  |Shift |Raise |Lower | BSpc |Space |Enter |AltGr |  RU  |Clmk  |
 * `-----------------------------------------------------------------------------------'
 */
[_RU] = LAYOUT_preonic_grid( \
  KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_DEL, \
  KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,  \
  KC_ESC,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, \
  _______, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RBRC,  \
  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______  \
),

/* Lower
 * ,-----------------------------------------------------------------------------------.
 * |   `  |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  |  F12 |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Super|  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |  F7  |  F8  |  F9  | F10  | Super|
 * |   _  |      |      |      |      |      |      |      |      |      |      |  F11 |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Ctrl |   !  |   @  |   #  |   $  |   %  |   ^  |   &  |   *  |   (  |   )  | Ctrl |
 * |   +  |      |      |      |      |      |      |      |      |      |      |  -   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * | Alt  |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  | Alt  |
 * |   =  |      |      |      |      |      |      |      |      |      |      |  \   |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |      |      |      |      |QWERTY|
 * `-----------------------------------------------------------------------------------'
 */
[_LOWER] = LAYOUT_preonic_grid( \
  KC_UNDS, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_F12, \
  TD(UND_LGU), KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  F11_RSU,
  PLS_LCT, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, MIN_RCT, \
  EQL_LAL, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______, \
  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, QWERTY  \
),

/* Raise
 * ,-----------------------------------------------------------------------------------.
 * |   `  | Mute | Vol- | Vol+ | Br-  | Br+  |      |EMAIL1|EMAIL2|EMAIL3|      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |Super |   /  |   7  |   8  |   9  |  -   | Vol+ | PgUp |  Up  | PgDwn|PrnScr|Super |
 * |NumLk |      |      |      |      |      |      |      |      |      |      |  [   |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Ctrl |   *  |   4  |   5  |   6  |  +   | Vol- | Left | Down | Right|Caps  | Ctrl |
 * |  =   |      |      |      |      |      |      |      |      |      |      |  '   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * | Alt  |   0  |   1  |   2  |   3  |  .   | Mute | Home | End  |Insert|Scroll| Alt  |
 * |      |      |      |      |      |      |      |      |      |      |      |  \   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |      |      |      |      |QWERTY|
 * `-----------------------------------------------------------------------------------'
 */
[_RAISE] = LAYOUT_preonic_grid( \
  _______, KC_MUTE, KC_VOLD, KC_VOLU, KC_BRID, KC_BRIU, _______, EMAIL_1, EMAIL_2, EMAIL_3, _______, _______, \
  NML_LGU, KC_PSLS, KC_P7,   KC_P8,   KC_P9,   KC_PMNS, KC_VOLU, KC_PGUP, KC_UP,   KC_PGDN, KC_PSCR, _______, \
  EQL_LCT, KC_PAST, KC_P4,   KC_P5,   KC_P6,   KC_PPLS, KC_VOLD, KC_LEFT, KC_DOWN, KC_RGHT, KC_CAPS, _______, \
  _______, KC_P0,   KC_P1,   KC_P2,   KC_P3,   KC_PCMM, KC_MUTE, KC_HOME, KC_END,  KC_INS,  KC_SLCK, _______, \
  _______, _______, _______, _______, _______, _______, _______,  _______, _______, _______, _______, QWERTY  \
),

/* ADJUST (LOWER + RAISE)
 * ,-----------------------------------------------------------------------------------.
 * |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      | RESET|      |      |      |      |      |      |      |      |      |  DEL |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * |      |      |      |AUD ON|AUDOFF|AGNORM|AGSWAP|QWERTY|COLEMK|      |      |      |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |VOICE-|VOICE+|MUS ON|MUSOFF|MIDION|MIDOFF|      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_ADJUST] = LAYOUT_preonic_grid( \
  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12, \
  _______, RESET,   DEBUG,   _______, _______, _______, _______, TERM_ON, TERM_OFF,_______, _______, KC_DEL,  \
  _______, _______, MU_MOD,  AU_ON,   AU_OFF,  AG_NORM, AG_SWAP, QWERTY,  COLEMAK, _______, _______, _______, \
  _______, MUV_DE,  MUV_IN,  MU_ON,   MU_OFF,  MI_ON,   MI_OFF,  _______, _______, _______, _______, _______, \
  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______  \
)
};


uint32_t layer_state_set_user(uint32_t state) {
  return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
        case QWERTY:
          if (record->event.pressed) {
            set_single_persistent_default_layer(_QWERTY);
            SEND_STRING(SS_TAP(X_F11));
          }
          return false;
          break;
        case RU:
          if (record->event.pressed) {
            set_single_persistent_default_layer(_QWERTY);
            SEND_STRING(SS_TAP(X_F12));
          }
          return false;
          break;
        case COLEMAK:
          if (record->event.pressed) {
            set_single_persistent_default_layer(_COLEMAK);
            SEND_STRING(SS_TAP(X_F11));
          }
          return false;
          break;
        case EMAIL_1:
          if (record->event.pressed) {
            SEND_STRING("yury.bykov@gmail.com");
          }
          return false;
          break;
        case EMAIL_2:
          if (record->event.pressed) {
            SEND_STRING("lobator@gmail.com");
          }
          return false;
          break;
        case EMAIL_3:
          if (record->event.pressed) {
            SEND_STRING("yury.bykov@oysterlab.ch");
          }
          return false;
          break;
      }
    return true;
};


bool muse_mode = false;
uint8_t last_muse_note = 0;
uint16_t muse_counter = 0;
uint8_t muse_offset = 70;
uint16_t muse_tempo = 50;

void encoder_update_user(uint8_t index, bool clockwise) {
  if (muse_mode) {
    if (IS_LAYER_ON(_RAISE)) {
      if (clockwise) {
        muse_offset++;
      } else {
        muse_offset--;
      }
    } else {
      if (clockwise) {
        muse_tempo+=1;
      } else {
        muse_tempo-=1;
      }
    }
  } else {
    if (clockwise) {
      register_code(KC_PGDN);
      unregister_code(KC_PGDN);
    } else {
      register_code(KC_PGUP);
      unregister_code(KC_PGUP);
    }
  }
}

void dip_update(uint8_t index, bool active) {
  switch (index) {
    case 0:
      if (active) {
        layer_on(_ADJUST);
      } else {
        layer_off(_ADJUST);
      }
      break;
    case 1:
      if (active) {
        muse_mode = true;
      } else {
        muse_mode = false;
        #ifdef AUDIO_ENABLE
          stop_all_notes();
        #endif
      }
   }
}

void matrix_scan_user(void) {
  #ifdef AUDIO_ENABLE
    if (muse_mode) {
      if (muse_counter == 0) {
        uint8_t muse_note = muse_offset + SCALE[muse_clock_pulse()];
        if (muse_note != last_muse_note) {
          stop_note(compute_freq_for_midi_note(last_muse_note));
          play_note(compute_freq_for_midi_note(muse_note), 0xF);
          last_muse_note = muse_note;
        }
      }
      muse_counter = (muse_counter + 1) % muse_tempo;
    }
  #endif
}

bool music_mask_user(uint16_t keycode) {
  switch (keycode) {
    case RAISE:
    case LOWER:
      return false;
    default:
      return true;
  }
}

// determine the tapdance state to return
int cur_dance (qk_tap_dance_state_t *state) {
  if (state->count == 1) {
    if (state->interrupted || !state->pressed) { return SINGLE_TAP; }
    else { return SINGLE_HOLD; }
  }
  if (state->count == 2) { return DOUBLE_SINGLE_TAP; }
  else { return 3; } // any number higher than the maximum state value you return above
}

// handle the possible states for each tapdance keycode you define:

void guiund_finished (qk_tap_dance_state_t *state, void *user_data) {
  td_state = cur_dance(state);
  switch (td_state) {
    case SINGLE_TAP:
      register_code16(KC_UNDS);
      break;
    case SINGLE_HOLD:
      register_mods(MOD_BIT(KC_LGUI)); // for a layer-tap key, use `layer_on(_MY_LAYER)` here
      break;
    case DOUBLE_SINGLE_TAP: // allow nesting of 2 `__` within tapping term
      tap_code16(KC_UNDS);
      register_code16(KC_UNDS);
  }
}

void guiund_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (td_state) {
    case SINGLE_TAP:
      unregister_code16(KC_UNDS);
      break;
    case SINGLE_HOLD:
      unregister_mods(MOD_BIT(KC_LGUI)); // for a layer-tap key, use `layer_off(_MY_LAYER)` here
      break;
    case DOUBLE_SINGLE_TAP:
      unregister_code16(KC_UNDS);
  }
}

// define `ACTION_TAP_DANCE_FN_ADVANCED()` for each tapdance keycode, passing in `finished` and `reset` functions
qk_tap_dance_action_t tap_dance_actions[] = {
  [UND_LGU] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, guiund_finished, guiund_reset)
};
