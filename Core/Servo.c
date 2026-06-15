/*
 * Servo.c
 *
 *  Created on: Apr 17, 2026
 *      Author: TX
 */


#include "Servo.h"
#include "motified_app.h"

#define low_interval        500
#define midium_interval     200
#define high_interval       100
#define fast_interval       50

volatile dummy_struct my_dummy = {.id = {1,2,3,4,5}};

float dummy_angle_sequence[20][5] = {0.0f};
u16 dummy_angle_interval[20][5] = {0};

void Air_pump_pwm_init(){
    ATOM_PWM_InitConfig(AIR_PUMP_PWM1, PUMP_OFF, SERVO_PERIOD_F);  //舵机频率为100HZ，初始值为1.5ms中值
    ATOM_PWM_InitConfig(AIR_PUMP_CRL_PWM2, PUMP_OFF, SERVO_PERIOD_F);  //舵机理论范围为：0.5ms--2.5ms，大多舵机实际比这个范围小

}

void air_pump_pick_up(void){
    switch(my_dummy.pump_state){
        case PUMP_IDLE:
            ATOM_PWM_InitConfig(AIR_PUMP_PWM1,     PUMP_OFF_CCR, SERVO_PERIOD_F);
            ATOM_PWM_InitConfig(AIR_PUMP_CRL_PWM2, PUMP_OFF_CCR, SERVO_PERIOD_F);
            break;
        case PUMP_ON:
            ATOM_PWM_InitConfig(AIR_PUMP_PWM1,     PWMP_ON_CCR, SERVO_PERIOD_F);
            ATOM_PWM_InitConfig(AIR_PUMP_CRL_PWM2, PUMP_OFF_CCR, SERVO_PERIOD_F);
            break;
        case PUMP_OFF:
            ATOM_PWM_InitConfig(AIR_PUMP_PWM1,     PUMP_OFF_CCR, SERVO_PERIOD_F);
            ATOM_PWM_InitConfig(AIR_PUMP_CRL_PWM2, PWMP_ON_CCR, SERVO_PERIOD_F);
            my_dummy.pump_state = PUMP_IDLE;
            break;
    }
}



void Servo_angle_move_Syn(float* angle, u16* interval) {
    FSUS_BeginAsync(&FSUS_Usart);
    for (u8 i=0; i<5; i++) {
        my_dummy.sync_servo[i].angle = angle[i];
        my_dummy.sync_servo[i].interval_single = interval[i];
        my_dummy.sync_servo[i].power = 0;

    }
    FSUS_SyncCommand(&FSUS_Usart, 5, MODE_SET_SERVO_ANGLE, SyncArray);
}

void Servo_angle_read(float* angle) {
    // 按id从小到大存储舵机的返回值
    FSUS_SyncServoMonitor(&FSUS_Usart,5,(ServoData *)my_dummy.servo_data);
    for(u8 i = 0; i < 5; i++){
        my_dummy.angle[i] =my_dummy.servo_data[i].angle;
    }
}


void TASK1_DEFAULT_STATE(void){

    FSUS_SetServoAngle(servo_usart, 1, -90.0f,  100, 0);
    FSUS_SetServoAngle(servo_usart, 2, -180.0f, 100, 0);
    FSUS_SetServoAngle(servo_usart, 3, -30.0f,  100, 0);
    FSUS_SetServoAngle(servo_usart, 4, -75.0f, 100, 0);
}

// pick up details
float task1_idle_angle[4] = {-90.0f,-180.0f,-30.0f,-75.0f};
float task1_pick_up_angle[4] = {-90.0f, 114.0f, 40.0f, -67.0f};
float task1_pick_up_middle[4] = {-90.0f, -126.0f, -20.0f, -60.0f};
float task1_turning_angle1[4] = {90.0f,  -126.0f, -20.0f, -60.0f};
float task1_put_angle[4] = {90.0f, 131.0f, 19.0f, -60.0f};
float task1_turning_angle2[4] = {-90.0f,-180.0f,-30.0f,-75.0f};
volatile bool is_task1_wheels_moving_to_next_point = false;
volatile bool is_task1_wheels_moving_to_last_point = false;
uint8_t run_only_once_in_servo[10] = {0};
u8 sync_mode = 1;
u8 sync_count = 4;

void TASK1_PICK_OBJECT_UP_SYN(void){
    SyncArray[0].id = 1;SyncArray[0].power = 0; SyncArray[0].interval_single = 100;SyncArray[0].angle = task1_pick_up_angle[0];
    SyncArray[0].id = 2;SyncArray[0].power = 0; SyncArray[0].interval_single = 100;SyncArray[0].angle = task1_pick_up_angle[1];
    SyncArray[0].id = 3;SyncArray[0].power = 0; SyncArray[0].interval_single = 100;SyncArray[0].angle = task1_pick_up_angle[2];
    SyncArray[0].id = 4;SyncArray[0].power = 0; SyncArray[0].interval_single = 100;SyncArray[0].angle = task1_pick_up_angle[3];
    FSUS_SyncCommand(servo_usart, sync_count, sync_mode, SyncArray);

}

void TASK1_PICK_OBJECT_UP(void){

//    static float temp_midiem_angle[4] = {0};
    // 到达拿去位置
    uint8_t i = 0;
    my_dummy.pump_state = PUMP_ON;
    air_pump_pick_up();
    //
    SyncArray[0].id = 1;SyncArray[0].power = 0; SyncArray[0].interval_single = 100;SyncArray[0].angle = task1_pick_up_angle[0];
    SyncArray[0].id = 2;SyncArray[0].power = 0; SyncArray[0].interval_single = 100;SyncArray[0].angle = task1_pick_up_angle[1];
    SyncArray[0].id = 3;SyncArray[0].power = 0; SyncArray[0].interval_single = 100;SyncArray[0].angle = task1_pick_up_angle[2];
    SyncArray[0].id = 4;SyncArray[0].power = 0; SyncArray[0].interval_single = 100;SyncArray[0].angle = task1_pick_up_angle[3];
    FSUS_SyncCommand(servo_usart, sync_count, sync_mode, SyncArray);
    FSUS_SetServoAngle(servo_usart, 1, task1_pick_up_angle[0], 10, 0);
    FSUS_SetServoAngle(servo_usart, 3, task1_pick_up_angle[2], 10, 0);
    FSUS_SetServoAngle(servo_usart, 4, task1_pick_up_angle[3], 10, 0);
    delayms(500);
    FSUS_SetServoAngle(servo_usart, 2, task1_pick_up_angle[1], 1000, 0);
    delayms(3000);
    //
    my_dummy.pump_state = PUMP_IDLE;
    air_pump_pick_up();
//
//    for(i = 0; i < 4; i++)
//        FSUS_SetServoAngle(servo_usart, (i+1),task1_pick_up_middle[i],  10, 0);
//    delayms(500);
    // 转动

//    float temp_medium_angle[4] = {0};
//    for(i = 0; i < 4; i++){
//        temp_medium_angle[i] = task1_turning_angle1[i] + task1_pick_up_middle[i];
//        FSUS_SetServoAngle(servo_usart, (i+1), temp_medium_angle[i], 10, 0);
//    }
//    delayms(10);
//
    is_task1_wheels_moving_to_next_point = true;

    for(i = 0; i< 4;i++){
        FSUS_SetServoAngle(servo_usart, (i+1), task1_turning_angle1[i], 1500, 0);
    }

}
// put details

void TASK1_PUT_OBJECT(void){
    my_dummy.pump_state = PUMP_OFF;
    u8 i = 0;
    // 移动到目标点
    for(i = 0; i < 4; i++){
        FSUS_SetServoAngle(servo_usart, (i+1), task1_put_angle[i], 100, 0);
    }
    delayms(2000);
    // 放下
    air_pump_pick_up();
    delayms(500);
    //
    is_task1_wheels_moving_to_last_point = true;

    for(i = 0; i< 4;i++){
        FSUS_SetServoAngle(servo_usart, (i+1), task1_idle_angle[i], 1500, 0);
    }
    delayms(10);
}

void TASK1_FLOW(void){
    if(does_dummy_run == false)
        return;
    switch(pick_or_put){
        case servo_from_camera_idle:
            break;
        case servo_pick_up_from_camera:
            TASK1_PICK_OBJECT_UP();
            does_dummy_run = false;
            break;
        case servo_put_down_from_camera:
            TASK1_PUT_OBJECT();
            does_dummy_run = false;
            break;
    }
}


