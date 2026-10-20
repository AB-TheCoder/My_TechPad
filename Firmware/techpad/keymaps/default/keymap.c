// Copyright 2026 Aarav bhatt(MoonRock)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum level{
    _DEV,
    _MEDIA,
    _LIGHTS
};// Name giving to numbers :/
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_DEV] = LAYOUT(
                         TO(_MEDIA),
    C(KC_C), C(KC_V), C(KC_X), C(KC_Z),
    C(KC_S), C(KC_F), C(KC_SLASH), C(S(KC_P)),
    C(KC_GRV), A(KC_TAB), G(KC_L),KC_MUTE 
),
    [_MEDIA] = LAYOUT(
            TO(_LIGHTS),
    KC_MPRV, KC_MPLY, KC_MNXT, KC_MPLY,
    KC_VOLD, KC_VOLU, KC_BRID, KC_BRIU,
    KC_CALC, KC_WHOM, G(KC_E), KC_MUTE
),
    [_LIGHTS] = LAYOUT(
                TO(_DEV),
    RM_TOGG, RM_NEXT, RM_VALD, RM_VALU,
    RM_HUED, RM_HUEU, RM_SPDD, RM_SPDU,
    KC_NO, KC_NO, KC_NO, QK_BOOT
)

};
#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_DEV] = {ENCODER_CCW_CW(KC_VOLD,KC_VOLU)},
    [_MEDIA] = {ENCODER_CCW_CW(KC_VOLD,KC_VOLU)},
    [_LIGHTS] = {ENCODER_CCW_CW(RM_VALD,RM_VALU)}
};
#endif

#ifdef OLED_ENABLE
bool oled_task_user(void){
    uint8_t layer=get_highest_layer(layer_state);
    uint8_t wpm = get_current_wpm();

    switch (layer){
        case _DEV:
            oled_write_ln("</>DEVELOPER MODE</>",false);
            break;
        case _MEDIA:
            oled_write_ln("</>MEDIA MODE</>",false);
            break;
        case _LIGHTS:
            oled_write_ln("</>RGB MODE</>",false);
            break;
        default:
            oled_write_ln("</>?UNKNOWN?</>",false);
            break;
    }

    oled_set_cursor(0,2);
    oled_write( "WPM: " , false);
    oled_write( get_u8_str( wpm , ' ') , false);
     
    oled_set_cursor(0,3);
    oled_write("</> TECHPAD",false);
    return false;
}
#endif