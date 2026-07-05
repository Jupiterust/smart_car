/*
 * IR_sensor.c
 *
 *  Created on: Jun 24, 2026
 *      Author: TX
 */


#include "IR_sensor.h"

    //PIN_InitConfig(P15_1, IfxPort_Mode_inputNoPullDevice,0);
    // P10_2 P11_10 P11_6 P11_3 P13_3 P13_2 P13_0 P15_1

const uint16_t ir_sensor1_pins[8] = {sr1_pin1, sr1_pin2, sr1_pin3, sr1_pin4, sr1_pin5, sr1_pin6, sr1_pin7};
const uint16_t ir_sensor2_pins[6] = {sr2_pin1, sr2_pin2, sr2_pin3, sr2_pin4, sr2_pin5, sr2_pin5};

void sensor_number_one_init(void){
    PIN_InitConfig(ir_sensor1_pins[0], sr1_pin_mode,0);
    PIN_InitConfig(ir_sensor1_pins[1], sr1_pin_mode,0);
    PIN_InitConfig(ir_sensor1_pins[2], sr1_pin_mode,0);
    PIN_InitConfig(ir_sensor1_pins[3], sr1_pin_mode,0);

    PIN_InitConfig(ir_sensor1_pins[4], sr1_pin_mode,0);
    PIN_InitConfig(ir_sensor1_pins[5], sr1_pin_mode,0);
    PIN_InitConfig(ir_sensor1_pins[6], sr1_pin_mode,0);
    PIN_InitConfig(ir_sensor1_pins[7], sr1_pin_mode,0);
}

void sensor_number_two_init(void){
    PIN_InitConfig(ir_sensor2_pins[0], sr1_pin_mode,0);
    PIN_InitConfig(ir_sensor2_pins[1], sr1_pin_mode,0);
    PIN_InitConfig(ir_sensor2_pins[2], sr1_pin_mode,0);
    PIN_InitConfig(ir_sensor2_pins[3], sr1_pin_mode,0);

    PIN_InitConfig(ir_sensor2_pins[4], sr1_pin_mode,0);
    PIN_InitConfig(ir_sensor2_pins[5], sr1_pin_mode,0);
}

void ir_sensor_init(void){
    sensor_number_one_init();
    sensor_number_two_init();
}

void get_ir_pins_state(uint8_t ir_sensorx, uint8_t* state){

    if(ir_sensorx == 1){
        for(uint8_t i = 0; i < 8; i++){
            state[i] = PIN_Read(ir_sensor1_pins[i]);
        }
    }
    else if(ir_sensorx == 2){
        for(uint8_t i = 0; i < 6; i++){
            state[i] = PIN_Read(ir_sensor2_pins[i]);
        }
    }
}

uint8_t get_ir_pins_state_num(uint8_t ir_sensorx){
    if(ir_sensorx == 1){
        uint8_t count = 0;
        for(uint8_t i = 0; i < 8; i++){
            if(PIN_Read(ir_sensor1_pins[i]) == 1){;
                count++;
            }
        }
        return count;
    }
    else if(ir_sensorx == 2){
        uint8_t count = 0;
        for(uint8_t i = 0; i < 8; i++){
            if(PIN_Read(ir_sensor2_pins[i]) == 0){
                count++;
            }
        }
        return 0;
    }
}



