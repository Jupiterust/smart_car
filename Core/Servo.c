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
    FSUS_SetServoAngle(servo_usart, 4, -75.0f,  100, 0);
}

// pick up details
//2:-100 3:-5 4:60 5:47
//3:53 3:-57 4: 40 5:83
float task1_idle_angle[4] ={0.0f, 126.50f,-75.0f, 75.0f};
// -87  -16 86
float task1_pick_up_angle[4] = {0.0f, -87.0f, -16.0f, 86.0f};
// -101 -29  -79
// 36 -49  87
//27   -52 76
float task1_turning_angle1[4] = {180.0f, 27.0f, -52.0f, 76.0f};
// -60    -26  85
// -79 -14 88
float task1_put_angle[4] = {180.0f, -75.0f, -14.0f, 88.0f};

float task1_turning_angle2[4] = {-90.0f,-180.0f,-30.0f,-75.0f};

float task3_watch_angle[4] = {180.0f,  180.0f, -132.0f,-15.0f };

volatile bool is_task1_wheels_moving_to_next_point = false;
volatile bool is_task1_wheels_moving_to_last_point = false;
uint8_t run_only_once_in_servo[10] = {0};
u8 sync_mode = 1;
u8 sync_count = 4;

void TASK1_PICK_OBJECT_UP_SYN(void){
    if(pick_or_put != servo_pick_up_from_camera){
        return ;
    }
    // 正常启动气泵
    my_dummy.pump_state = PUMP_ON;
    air_pump_pick_up();
    // 转动抓取
    SyncArray[0].id = 1;SyncArray[0].power = 0; SyncArray[0].interval_single = 0;   SyncArray[0].angle = task1_pick_up_angle[0];
    SyncArray[1].id = 2;SyncArray[1].power = 0; SyncArray[1].interval_single = 1500;SyncArray[1].angle = task1_pick_up_angle[1];
    SyncArray[2].id = 3;SyncArray[2].power = 0; SyncArray[2].interval_single = 50;  SyncArray[2].angle = task1_pick_up_angle[2];
    SyncArray[3].id = 4;SyncArray[3].power = 0; SyncArray[3].interval_single = 50;  SyncArray[3].angle = task1_pick_up_angle[3];
    FSUS_SyncCommand(servo_usart, sync_count, sync_mode, SyncArray);
    delayms(1800);
//    my_dummy.pump_state = PUMP_IDLE;
//    air_pump_pick_up();
//    is_task1_wheels_moving_to_next_point = true;
//    pick_times = 1;
//    task1_cy_id = task1_cylinder_id_small;
    // 可以正常移动到下一点
    is_task1_wheels_moving_to_next_point =true;
    SyncArray[0].interval_single = 100; SyncArray[0].angle = task1_turning_angle1[0];
    SyncArray[1].interval_single = 1;   SyncArray[1].angle = task1_turning_angle1[1];
    SyncArray[2].interval_single = 1000; SyncArray[2].angle = task1_turning_angle1[2];
    SyncArray[3].interval_single = 1000; SyncArray[3].angle = task1_turning_angle1[3];
    FSUS_SyncCommand(servo_usart, sync_count, sync_mode, SyncArray);
    delayms(10);
    is_task1_wheels_moving_to_next_point =true;
    delayms(1000);
    // 同时启动y轴校准以及摄像头接收数据正常
    pick_or_put = servo_from_camera_idle;
    task1_y_correct_start2 = true;
    task1_start_yaw_correction = false;

}

void TASK1_PUT_OBJECT_DOWN_SYN(void){
    if(pick_or_put != servo_put_down_from_camera){
            return ;
    }
    // 正常放下
    SyncArray[0].interval_single = 1000;SyncArray[0].angle = task1_put_angle[0];
    SyncArray[1].interval_single = 1200;SyncArray[1].angle = task1_put_angle[1];
    SyncArray[2].interval_single = 100; SyncArray[2].angle = task1_put_angle[2];
    SyncArray[3].interval_single = 100; SyncArray[3].angle = task1_put_angle[3];
    FSUS_SyncCommand(servo_usart, sync_count, sync_mode, SyncArray);
    delayms(1600);
    my_dummy.pump_state = PUMP_OFF;
    air_pump_pick_up();
    delayms(600);
    // 1先单独动一下
    // 机械臂和小车都回到原来的位置
    if(put_times == 3){
        SyncArray[0].angle = 180;
    }
    else{
        SyncArray[0].angle = task1_idle_angle[0];
    }
    SyncArray[0].interval_single = 1600;
    SyncArray[1].interval_single = 10;   SyncArray[1].angle = task1_idle_angle[1];
    SyncArray[2].interval_single = 500;  SyncArray[2].angle = task1_idle_angle[2];
    SyncArray[3].interval_single = 500;  SyncArray[3].angle = task1_idle_angle[3];
    FSUS_SyncCommand(servo_usart, sync_count, sync_mode, SyncArray);
    delayms(10);

    is_task1_wheels_moving_to_last_point = true;
    delayms(400);
    if(put_times ==  3){
        task1_y_correct_start1 = false;
        SyncArray[0].interval_single = 1600; SyncArray[0].angle = task3_watch_angle[0];
        SyncArray[1].interval_single = 10;   SyncArray[1].angle = task3_watch_angle[1];
        SyncArray[2].interval_single = 500;  SyncArray[2].angle = task3_watch_angle[2];
        SyncArray[3].interval_single = 500;  SyncArray[3].angle = task3_watch_angle[3];
        FSUS_SyncCommand(servo_usart, sync_count, sync_mode, SyncArray);
    }
    else{
        task1_y_correct_start1 = true;
    }
    pick_or_put = servo_from_camera_idle;
}

// task2

volatile u8 TASK2_DROP_COUNT = 0;
volatile bool does_task_work_flow_start = false;
//
volatile float task2_watch_drop_number_state[4] = {0,180, -115,30};

volatile float task2_watch_drop_state[4] = {180,-19,-92,-52};

volatile float task2_pick_drop_one_up[4] = {180,-39,-39, 85};
volatile float task2_pick_drop_two_up[4] = {180,-89,6, 107};

volatile float task2_middle_state[4] = {0, 126, -122, -18};

volatile float task2_put_drop_one_down[4] = {0, -19, -92, -52};
volatile float task2_put_drop_two_down[4] = {0, 8, -85 , -41};
volatile float task2_put_drop_three_down[4] = {0, 48, -74, -16};


void TASK2_WATCH_DROP_WATER(void){
    static bool this_function_only_runs_once = true;
    if(TASK2_DROP_COUNT != 0 && this_function_only_runs_once == true){
        this_function_only_runs_once = false;
        SyncArray[0].angle =  task2_watch_drop_state[0];  SyncArray[0].interval_single = 1500;

        SyncArray[1].angle =  task2_watch_drop_state[1];  SyncArray[0].interval_single = 1500;

        SyncArray[2].angle =  task2_watch_drop_state[2];  SyncArray[0].interval_single = 500;

        SyncArray[2].angle =  task2_watch_drop_state[3];  SyncArray[0].interval_single = 500;
        FSUS_SyncCommand(servo_usart, sync_count, sync_mode, SyncArray);
        delayms(1500);
    }
}

void TASK2_PICK_AND_PUT_DROP_WATER_WORKFLOW(void){
    if(does_task_work_flow_start == false){
        return;
    }
    if(TASK2_DROP_COUNT == 1){
        my_dummy.pump_state = PUMP_ON;
        air_pump_pick_up();
        SyncArray[0].angle =  task2_watch_drop_state[0];  SyncArray[0].interval_single = 10;

        SyncArray[1].angle =  task2_watch_drop_state[1];  SyncArray[0].interval_single = 500;

        SyncArray[2].angle =  task2_watch_drop_state[2];  SyncArray[0].interval_single = 10;

        SyncArray[2].angle =  task2_watch_drop_state[3];  SyncArray[0].interval_single = 10;
        FSUS_SyncCommand(servo_usart, sync_count, sync_mode, SyncArray);

        delayms(1700);
        // 抓取完毕，进入中间状态
        SyncArray[0].angle =  task2_middle_state[0];  SyncArray[0].interval_single = 1200;

        SyncArray[1].angle =  task2_middle_state[1];  SyncArray[0].interval_single = 10;

        SyncArray[2].angle =  task2_middle_state[2];  SyncArray[0].interval_single = 900;

        SyncArray[2].angle =  task2_middle_state[3];  SyncArray[0].interval_single = 900;
        FSUS_SyncCommand(servo_usart, sync_count, sync_mode, SyncArray);
        delayms(800);

        // 放下
        SyncArray[0].angle =  task2_put_drop_one_down[0];  SyncArray[0].interval_single = 10;

        SyncArray[1].angle =  task2_put_drop_one_down[1];  SyncArray[0].interval_single = 500;

        SyncArray[2].angle =  task2_put_drop_one_down[2];  SyncArray[0].interval_single = 10;

        SyncArray[2].angle =  task2_put_drop_one_down[3];  SyncArray[0].interval_single = 10;
        FSUS_SyncCommand(servo_usart, sync_count, sync_mode, SyncArray);
        delayms(1400);
        my_dummy.pump_state = PUMP_OFF;
        air_pump_pick_up();
        // 放下
        delayms(400);
        // 抓取完毕，进入看水滴状态
        SyncArray[0].angle =  task2_watch_drop_state[0];  SyncArray[0].interval_single = 500;

        SyncArray[1].angle =  task2_watch_drop_state[1];  SyncArray[0].interval_single = 500;

        SyncArray[2].angle =  task2_watch_drop_state[2];  SyncArray[0].interval_single = 500;

        SyncArray[2].angle =  task2_watch_drop_state[3];  SyncArray[0].interval_single = 500;
        FSUS_SyncCommand(servo_usart, sync_count, sync_mode, SyncArray);
        delayms(400);
        does_task_work_flow_start = false;
        TASK2_DROP_COUNT = 0;
    }
    else if(TASK2_DROP_COUNT == 2){
        my_dummy.pump_state = PUMP_ON;
        air_pump_pick_up();
        SyncArray[0].angle =  task2_watch_drop_state[0];  SyncArray[0].interval_single = 10;

        SyncArray[1].angle =  task2_watch_drop_state[1];  SyncArray[0].interval_single = 500;

        SyncArray[2].angle =  task2_watch_drop_state[2];  SyncArray[0].interval_single = 10;

        SyncArray[2].angle =  task2_watch_drop_state[3];  SyncArray[0].interval_single = 10;
        FSUS_SyncCommand(servo_usart, sync_count, sync_mode, SyncArray);

        delayms(1700);
        // 抓取完毕，进入中间状态
        SyncArray[0].angle =  task2_middle_state[0];  SyncArray[0].interval_single = 10;

        SyncArray[1].angle =  task2_middle_state[1];  SyncArray[0].interval_single = 500;

        SyncArray[2].angle =  task2_middle_state[2];  SyncArray[0].interval_single = 10;

        SyncArray[2].angle =  task2_middle_state[3];  SyncArray[0].interval_single = 10;
        FSUS_SyncCommand(servo_usart, sync_count, sync_mode, SyncArray);
        delayms(1400);

        // 放下
        SyncArray[0].angle =  task2_put_drop_one_down[0];  SyncArray[0].interval_single = 10;

        SyncArray[1].angle =  task2_put_drop_one_down[1];  SyncArray[0].interval_single = 500;

        SyncArray[2].angle =  task2_put_drop_one_down[2];  SyncArray[0].interval_single = 10;

        SyncArray[2].angle =  task2_put_drop_one_down[3];  SyncArray[0].interval_single = 10;
        FSUS_SyncCommand(servo_usart, sync_count, sync_mode, SyncArray);
        delayms(1400);
        my_dummy.pump_state = PUMP_OFF;
        air_pump_pick_up();
        // 放下
        delayms(400);
        // 抓取完毕，进入中间状态
        SyncArray[0].angle =  task2_middle_state[0];  SyncArray[0].interval_single = 10;

        SyncArray[1].angle =  task2_middle_state[1];  SyncArray[0].interval_single = 500;

        SyncArray[2].angle =  task2_middle_state[2];  SyncArray[0].interval_single = 100;

        SyncArray[2].angle =  task2_middle_state[3];  SyncArray[0].interval_single = 100;
        FSUS_SyncCommand(servo_usart, sync_count, sync_mode, SyncArray);
        delayms(400);
        SyncArray[0].angle = 180;

        SyncArray[1].angle =  task2_middle_state[1];  SyncArray[0].interval_single = 500;

        SyncArray[2].angle =  task2_middle_state[2];  SyncArray[0].interval_single = 10;

        SyncArray[2].angle =  task2_middle_state[3];  SyncArray[0].interval_single = 10;
        FSUS_SyncCommand(servo_usart, sync_count, sync_mode, SyncArray);
        delayms(400);

        // 开始抓取第二个
        my_dummy.pump_state = PUMP_ON;
        air_pump_pick_up();
        SyncArray[0].angle =  task2_middle_state[0];  SyncArray[0].interval_single = 10;

        SyncArray[1].angle =  task2_middle_state[1];  SyncArray[0].interval_single = 500;

        SyncArray[2].angle =  task2_middle_state[2];  SyncArray[0].interval_single = 10;

        SyncArray[2].angle =  task2_middle_state[3];  SyncArray[0].interval_single = 10;
        FSUS_SyncCommand(servo_usart, sync_count, sync_mode, SyncArray);
        delayms(1800);

        // 第二次抓取完毕，进入中间状态
        SyncArray[0].angle =  task2_middle_state[0];  SyncArray[0].interval_single = 1200;

        SyncArray[1].angle =  task2_middle_state[1];  SyncArray[0].interval_single = 10;

        SyncArray[2].angle =  task2_middle_state[2];  SyncArray[0].interval_single = 900;

        SyncArray[2].angle =  task2_middle_state[3];  SyncArray[0].interval_single = 900;
        FSUS_SyncCommand(servo_usart, sync_count, sync_mode, SyncArray);
        delayms(1400);

        // 放下
        SyncArray[0].angle =  task2_put_drop_two_down[0];  SyncArray[0].interval_single = 10;

        SyncArray[1].angle =  task2_put_drop_two_down[1];  SyncArray[0].interval_single = 500;

        SyncArray[2].angle =  task2_put_drop_two_down[2];  SyncArray[0].interval_single = 10;

        SyncArray[2].angle =  task2_put_drop_two_down[3];  SyncArray[0].interval_single = 10;
        FSUS_SyncCommand(servo_usart, sync_count, sync_mode, SyncArray);
        delayms(1400);
        my_dummy.pump_state = PUMP_OFF;
        air_pump_pick_up();
        // 放下
        delayms(400);

        // 抓取完毕，进入看水滴状态
        SyncArray[0].angle =  task2_watch_drop_state[0];  SyncArray[0].interval_single = 500;

        SyncArray[1].angle =  task2_watch_drop_state[1];  SyncArray[0].interval_single = 500;

        SyncArray[2].angle =  task2_watch_drop_state[2];  SyncArray[0].interval_single = 500;

        SyncArray[2].angle =  task2_watch_drop_state[3];  SyncArray[0].interval_single = 500;
        FSUS_SyncCommand(servo_usart, sync_count, sync_mode, SyncArray);
        delayms(400);
        does_task_work_flow_start = false;
        TASK2_DROP_COUNT = 0;
    }
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



void angle_all_read(void){
    ServoData temp_servo[1] = {0};
     static float temp_angles[4] = {0};
     char temp_buffer[20] = {20};
     for(u8 i = 1; i <= 4; i++){
         int ret = FSUS_ServoMonitor(servo_usart, i, temp_servo);
         if (ret == 0) {
             temp_angles[i-1] = temp_servo[0].angle;  // 修正下标
         } else {
             printf("servo %d read fail\r\n", i);
         }
     }
     sprintf(temp_buffer,"angles: %0.2f %0.2f %0.2f %0.2f \r\n", temp_angles[0], temp_angles[1], temp_angles[2], temp_angles[3]);
     UART_PutStr(UART1,temp_buffer);
     printf("angles: %0.2f %0.2f %0.2f %0.2f %u\r\n", temp_angles[0], temp_angles[1], temp_angles[2], temp_angles[3], rx_irq_count);
}


