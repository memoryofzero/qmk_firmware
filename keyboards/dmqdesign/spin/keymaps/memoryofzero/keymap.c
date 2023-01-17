#include QMK_KEYBOARD_H

enum layers {
  _ARROW,
  _NUMPAD,
  _MACRO
};

//Update TO(_layer) accordingly if changing layers
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_ARROW] = LAYOUT(/* Arrows & Nav Block */
                KC_INS,  KC_HOME, KC_PGUP, TO(1),
                KC_DEL,  KC_END, KC_PGDN, KC_MPLY,
                KC_SPC,  KC_UP,   LSFT(KC_N),   KC_F24,
                KC_LEFT, KC_DOWN, KC_RGHT
                ),

    [_NUMPAD] = LAYOUT(/* Numpad */
                KC_P7, KC_P8, KC_P9, TO(2),
                KC_P4, KC_P5, KC_P6, KC_TRNS,
                KC_P1, KC_P2, KC_P3, KC_TRNS, /* for audio device switch */
                KC_P0, KC_PDOT, KC_PENT
                ),

    [_MACRO] = LAYOUT(/* Fn Keys */
                KC_F13, KC_F14, KC_F15, TO(0),
                KC_F16, KC_F17, KC_F18, KC_TRNS,
                KC_F19, KC_F20, KC_F21, KC_TRNS,
                KC_F22, KC_F23, KC_F24
                )
};

void keyboard_post_init_user(void) {
  // Call the post init code.
  rgblight_enable_noeeprom(); // enables Rgb, without saving settings
  rgblight_sethsv_noeeprom(180, 0, 0); // sets LEDs to white
}

//ENCODER_RESOLUTIONS { 4, 12, 4 }
void encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) { /* First encoder */
      switch (get_highest_layer(layer_state)) {
        default: /* Media FFWD/RWD */
          if (clockwise) {
            tap_code(KC_MFFD);
          } else {
            tap_code(KC_MRWD);
          }
          break;
      }
    } else if (index == 1) { /* Second encoder */
        switch (get_highest_layer(layer_state)) {
            default: /* Media Control */
                if (clockwise) {
                  tap_code(KC_MNXT);
                } else {
                  tap_code(KC_MPRV);
                }
                break;
        }
    } else if (index == 2) { /* Third encoder */
        switch (get_highest_layer(layer_state)) {
            default: /* Volume Control */
                if (clockwise) {
                    tap_code(KC_VOLU);
                } else {
                    tap_code(KC_VOLD);
                }
                break;
        }
    }
}

#ifdef OLED_DRIVER_ENABLE

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
#ifdef LEFT_HAND
    return OLED_ROTATION_180;
#else
    return OLED_ROTATION_0;
#endif
}

void oled_task_user(void) {
    // Layer Status
    oled_write_P(PSTR("Layer: "), false);

    switch (get_highest_layer(layer_state)) {
        case _NUMPAD:
            oled_write_P(PSTR("Numpad\n"), false);
            break;
        case _ARROW:
            oled_write_P(PSTR("Arrow\n"), false);
            break;
        case _MACRO:
            oled_write_P(PSTR("Macro\n"), false);
            break;
        default:
            // Or use the write_ln shortcut over adding '\n' to the end of your string
            oled_write_ln_P(PSTR("Undefined"), false);
    }

    led_t led_state = host_keyboard_led_state();
    oled_write_P(PSTR("Caps: "), false);
    oled_write_P(led_state.caps_lock ? PSTR("On  ") : PSTR("Off "), false);
    oled_write_P(PSTR("Num: "), false);
    oled_write_ln_P(led_state.num_lock ? PSTR("On") : PSTR("Off"), false);

    oled_write_ln_P(PSTR("Top Encoder: "), false);
    switch (get_highest_layer(layer_state)) {
        default:
            oled_write_P(PSTR("Fast-foward/Rewind"), false);
    }
}
#endif
