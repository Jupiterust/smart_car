/*
 * IR_sensor.h
 *
 *  Created on: Jun 24, 2026
 *      Author: TX
 */

#ifndef IR_SENSOR_H_
#define IR_SENSOR_H_


#include "include.h"
#include "stdint.h"
// P10_2 P11_10 P11_6 P11_3 P13_3 P13_2 P13_0 P15_1
#define sr1_pin1 P10_2
#define sr1_pin2 P11_10
#define sr1_pin3 P11_6
#define sr1_pin4 P11_3
#define sr1_pin5 P13_3
#define sr1_pin6 P13_2
#define sr1_pin7 P13_0
#define sr1_pin8 P15_1

#define sr2_pin1 P15_5
#define sr2_pin2 P15_0
#define sr2_pin3 P15_7
#define sr2_pin4 P11_2
#define sr2_pin5 P11_9
#define sr2_pin6 P11_12

#define IR_SENSOR1 1
#define IR_SENSOR2 2

#define sr1_pin_mode IfxPort_Mode_inputNoPullDevice

#define sr2_pin_mode sr1_pin_mode

void ir_sensor_init(void);

void get_ir_pins_state(uint8_t ir_sensorx, uint8_t* state);

uint8_t get_ir_pins_state_num(uint8_t ir_sensorx);

#endif /* IR_SENSOR_H_ */
