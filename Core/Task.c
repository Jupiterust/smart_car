/*
 * Task.c
 *
 *  Created on: Mar 25, 2026
 *      Author: TX
 */

#include "Task.h"

volatile bool following_correct_flag1 = false;
volatile bool following_correct_flag2 = false;

void following_correct_by_icm(void){
    static float last_yaw = 0.0f;
    static volatile float current_yaw =0.0f;current_yaw =  wheel_asix.yaw;
    static volatile uint16_t correct_cnt1 = 0;
    static volatile uint16_t correct_cnt2 = 0;
    static volatile bool flag1_end = false;
    if(wheel_system_tick.tick_in_100_ms > 80 && flag1_end != true){
        if(current_yaw > -1 && current_yaw < 1){
            following_correct_flag1 = true;
        }
        correct_cnt1++;
        if(correct_cnt1 >50){
            correct_cnt1 = 0;
            correct_cnt2 = 0;
            following_correct_flag1 = false;
            flag1_end = true;
        }
    }
    if(wheel_system_tick.tick_in_100_ms > 80){
        ;
    }


}




















