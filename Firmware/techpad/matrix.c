// Copyright 2026 Aarav bhatt(MoonRock)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "matrix.h" 
#include"gpio.h"
#include "wait.h"
static const pin_t l_pins[3] = {GP28,GP29,GP0};
static const pin_t m_pins[3] = {GP2,GP4,GP3};

void matrix_init_custom(void) {

    for (uint8_t i = 0;i<3;i++){
        gpio_set_pin_input_high(l_pins[i]);
        gpio_set_pin_input_high(m_pins[i]);
        //resets all pins to there resting configurations before reading (high)
    }

}

bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    bool matrix_has_changed = false;
//creating new matrix for comparison and identifing
//passA
    matrix_row_t new_matrix[MATRIX_ROWS]={0,0,0};
    for (uint8_t i= 0;i<3;i++){
        gpio_set_pin_output(l_pins[i]);     // stop listening, start "talking"
        gpio_write_pin_low(l_pins[i]);      
        wait_us(30);
        for (uint8_t a=0;a<3;a++){
            if (!gpio_read_pin(m_pins[a])){
                new_matrix[i] |= (MATRIX_ROW_SHIFTER <<a);
            }
        }
        gpio_set_pin_input_high(l_pins[i]);   //quietly listening, gently resting at HIGH
    }
    //PassB
    for (uint8_t i= 0;i<2;i++){// for identifing the pressed keys m->l in the duplix matrix
        gpio_set_pin_output(m_pins[i]);     // stop listening, start "talking"
        gpio_write_pin_low(m_pins[i]);     
        wait_us(30);
        for (uint8_t a=0;a<3;a++){
            if (!gpio_read_pin(l_pins[a])){
                new_matrix[a] |= (MATRIX_ROW_SHIFTER <<(i+3));
            }
        }
        gpio_set_pin_input_high(m_pins[i]);   //quietly listening, gently resting at HIGH


    }
    for (uint8_t row=0;row<MATRIX_ROWS;row++){
        if (current_matrix[row]!=new_matrix[row]){
            current_matrix[row]=new_matrix[row];
            matrix_has_changed=true;
        }//checks if the matrix has changed(any keys pressed) and then copies the new changes as the current
    }
    return matrix_has_changed; //returns as the name suggests
}