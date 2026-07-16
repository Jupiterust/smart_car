/*
 * Motor.c
 *
 *  Created on: Mar 23, 2026
 *      Author: Jupiter
 */

#include "Motor.h"
#include "LQ_IIC_Gyro.h"

// 先写底层驱动 以我们会使用的外设的方法，不仅只是为了熟悉基本函数操作，而且还是为了遇到问题时知道可能的所在地
// 然后写调用底层驱动的算法 先就这两步 会#define现实中车的长度和宽度，以及轮子直径，方便算法映射现实
// motor1
//
    //2 4 3
#define code_is_wrong1  0

// 用于传入速度环里的全局速度变量






void motor_all_run(int* pwm_arr){
   if(pwm_arr[0] < 0){
#if code_is_wrong1
       IfxPort_setPinLow(&MODULE_P21, 3);
#else
       Set_Motor1_BW();
#endif
       ATOM_PWM_SetDuty(MOTOR1_PWM, (0-pwm_arr[0]), MOTOR_FREQUENCY);
   }
   else{
#if code_is_wrong1
       IfxPort_setPinHigh(&MODULE_P21, 3);
#else
       Set_Motor1_FW();
#endif
       ATOM_PWM_SetDuty(MOTOR1_PWM, pwm_arr[0], MOTOR_FREQUENCY);
   }

   if(pwm_arr[1] < 0){
#if code_is_wrong1
       IfxPort_setPinLow(&MODULE_P32, 4);
#else
       Set_Motor2_BW();
#endif
       ATOM_PWM_SetDuty(MOTOR2_PWM, (0-pwm_arr[1]), MOTOR_FREQUENCY);
   }
   else{
#if code_is_wrong1
       IfxPort_setPinHigh(&MODULE_P32, 4);
#else
       Set_Motor2_FW();
#endif
       ATOM_PWM_SetDuty(MOTOR2_PWM, pwm_arr[1], MOTOR_FREQUENCY);
   }

   if(pwm_arr[2] < 0){
#if code_is_wrong1
       IfxPort_setPinHigh(&MODULE_P21, 5);
#else
       Set_Motor3_BW();
#endif
       ATOM_PWM_SetDuty(MOTOR3_PWM, (0-pwm_arr[2]), MOTOR_FREQUENCY);
   }
   else{
#if code_is_wrong1
       IfxPort_setPinLow(&MODULE_P21, 5);
#else
       Set_Motor3_FW();
#endif
       ATOM_PWM_SetDuty(MOTOR3_PWM, pwm_arr[2], MOTOR_FREQUENCY);
   }

   if(pwm_arr[3] < 0){
#if code_is_wrong1
        IfxPort_setPinHigh(&MODULE_P22, 3);
#else
        Set_Motor4_BW();
#endif
        ATOM_PWM_SetDuty(MOTOR4_PWM, (0-pwm_arr[3]), MOTOR_FREQUENCY);
    }
    else{
#if code_is_wrong1
        IfxPort_setPinLow(&MODULE_P22, 3);
#else
        Set_Motor4_FW();
#endif
        ATOM_PWM_SetDuty(MOTOR4_PWM, pwm_arr[3], MOTOR_FREQUENCY);
    }
}

void motor_all_stop(void){

    int temp_speed[4] = {0};
    motor_all_run(temp_speed);
    encoder_clear();
}
// 3  2  1 4
// ECPULSE1 = ENC_GetCounter(ENC2_InPut_P33_7);   // 02
// ECPULSE2 = ENC_GetCounter(ENC4_InPut_P02_8);   // 04
// ECPULSE3 = ENC_GetCounter(ENC5_InPut_P10_3);   // 05
// ECPULSE4 = ENC_GetCounter(ENC6_InPut_P20_3);   // 06

void encoder_clear(void){
    MODULE_GPT120.T5.U = 0;
    MODULE_GPT120.T4.U = 0;
    MODULE_GPT120.T2.U = 0;
    MODULE_GPT120.T6.U = 0;
}


void get_encoder(int* value){
    value[0] = (signed short)MODULE_GPT120.T5.U;
    MODULE_GPT120.T5.U = 0;
    value[1] = (signed short)MODULE_GPT120.T4.U;
    MODULE_GPT120.T4.U = 0;
    value[2] = (signed short)MODULE_GPT120.T2.U;
    MODULE_GPT120.T2.U = 0;
    value[3] = (signed short)MODULE_GPT120.T6.U;
    MODULE_GPT120.T6.U = 0;

}

void get_encoder_directly(int* value){
     value[0] = (signed short)MODULE_GPT120.T5.U;
     value[1] = (signed short)MODULE_GPT120.T4.U;
     value[2] = (signed short)MODULE_GPT120.T2.U;
     value[3] = (signed short)MODULE_GPT120.T6.U;
}

// 速度环
// 目标速度计算

#define RADIUS      2.92f       // 半径 cm
#define MOTOR_RADIO 1.0f        // 减速比 一圈电机也转一圈
#define MOTOR_T     0.005f      // 5ms一周期
#define MOTOR_T_    200.0f      //  周期倒数
#define MOTOR_PI    3.14159265f //
#define MOTOR_C     501.4f      //转一圈所需要的编码值 10圈的平均值
#define MOTOR_SPEED_PARA  ( (float)((RADIUS * 2.0f * MOTOR_PI * MOTOR_RADIO ) / (MOTOR_C * MOTOR_T )))

#define GET_SPEED(encoder)       ((encoder) * MOTOR_SPEED_PARA)

#define PID_MAX 10000
#define PID_MIN -10000


volatile Motor_pid_struct motor_pid = {{0}, {0}, {0}, {0},{0}, {0}, {0}, {0}, {0}, {0}};

void PID_init(void){
    motor_pid.k_p[0] = 4.6f;
    motor_pid.k_p[1] = 4.6f;
    motor_pid.k_p[2] = 4.6f;
    motor_pid.k_p[3] = 4.6f;

    motor_pid.k_i[0] = 20.951f;
    motor_pid.k_i[1] = 20.951f;
    motor_pid.k_i[2] = 20.951f;
    motor_pid.k_i[3] = 20.951f;

    motor_pid.k_d[0] = 1.0f;
    motor_pid.k_d[1] = 1.0f;
    motor_pid.k_d[2] = 1.0f;
    motor_pid.k_d[3] = 1.0f;

    // 其余均已在创立时清零
}
inline void get_real_speed(int * encoder_value, float * actual_speed){

    actual_speed[0] = (float)encoder_value[0] *  2.507f;
    actual_speed[1] = (float)encoder_value[1] *  2.507f;
    actual_speed[2] = (float)encoder_value[2] *  2.507f;
    actual_speed[3] = (float)encoder_value[3] *  2.507f;

}


char debug_massage[40] = {0};


void all_error_clear(void){
    for(u8 i = 0; i < 4; i++){
        motor_pid.error[i] = 0;
        motor_pid.last_error[i] = 0;
    }
}

void motor_speed_loop(float *target_s){
    u8 i = 0;
    // 赋值 目标速度
    for(i = 0; i < 4; i++){
        motor_pid.target_speed[i] = target_s[i];
    }
    // 获取 编码值
    get_encoder((int*)motor_pid.encoder_value);
    // 由公式换算成cm/s的单位
//    motor_pid.actual_speed[0] = GET_SPEED(motor_pid.encoder_value[0]);
//    motor_pid.actual_speed[1] = GET_SPEED(motor_pid.encoder_value[1]);
//    motor_pid.actual_speed[2] = GET_SPEED(motor_pid.encoder_value[2]);
//    motor_pid.actual_speed[3] = GET_SPEED(motor_pid.encoder_value[3]);
//    get_real_speed((int *)motor_pid.encoder_value,(float *)motor_pid.actual_speed);
    motor_pid.actual_speed[0] = (float)motor_pid.encoder_value[0] *  3.507f;
    motor_pid.actual_speed[1] = (float)motor_pid.encoder_value[1] *  3.507f;
    motor_pid.actual_speed[2] = (float)motor_pid.encoder_value[2] *  3.507f;
    motor_pid.actual_speed[3] = (float)motor_pid.encoder_value[3] *  3.507f;
//    printf("%d %d %d %d \r\n", motor_pid.encoder_value[0], motor_pid.encoder_value[1], motor_pid.encoder_value[2], motor_pid.encoder_value[3]);
//
//sprintf(debug_massage,"%f %f %f %f \r\n", motor_pid.actual_speed[0], motor_pid.actual_speed[1], motor_pid.actual_speed[2], motor_pid.actual_speed[3]);
//    printf("%s",debug_massage);

    // 临时存储增量 pwm value
    float delta_pid[4] = {0};
    for(i = 0; i < 4; i++){
        motor_pid.error[i] =  motor_pid.target_speed[i] -  motor_pid.actual_speed[i];
        // k_p是用来预测的 k_i是速度积分
        delta_pid[i] = ( motor_pid.error[i] - motor_pid.last_error[i] ) * motor_pid.k_p[i] + motor_pid.error[i] * motor_pid.k_i[i];
        motor_pid.pwm_out[i] += (int)delta_pid[i];

        motor_pid.last_error[i] = motor_pid.error[i];
        if(motor_pid.pwm_out[i] > PID_MAX){
            motor_pid.pwm_out[i] = PID_MAX;
        }
        if(motor_pid.pwm_out[i] < PID_MIN){
            motor_pid.pwm_out[i] = PID_MIN;
        }
    }
    int pwm_out[4] = {(int)motor_pid.pwm_out[0], (int)motor_pid.pwm_out[1], (int)motor_pid.pwm_out[2],(int)motor_pid.pwm_out[3]};
    motor_all_run((int *)motor_pid.pwm_out);
}
// 全局变量angle由外界设置
// 所以在设置角度的时候需要注意设置后就不能对angle这个角度做出任何赋值等操作
// angle > 0 --> 右转
// angle < 0 --> 左转

// 左右转参数
#define angle_derease_para    0.2438f
// 这个参数只是测试而已
//  右转
// 2000,-2000,2000,-2000
//  左转
// -2000,2000,2000,-2000




volatile float angle = 0.0f;
volatile u8 angle_dir = 0;
volatile int turning_speed[4] = {2200,-2000,2000,-2000};
void turning_any_angle(void){
    if(angle == 0.0f)
        return;
    else if(angle > 0){
        angle -= angle_derease_para;
        motor_all_run((int *)turning_speed);
        if(angle < 0){
            angle = 0.0f;
        }
    }
}

inline void get_ang_vel(float* all_vel,float * all_angle_vel){
    all_vel[0] *=0.83333f;
    all_vel[1] *=0.83333f;
    all_vel[2] *=0.83333f;
    all_vel[3] *=0.83333f;
}

// 下面的这个函数完全是我的兴趣爱好而写的
// 既然我们的速度环单位是cm/s
// 那我就有信息我们可以完成由坐标1到达坐标2的位置环
//

volatile u16 point_number = 0;
volatile coordinate_struct Points[20] = {0};



//void position_init(void){
//    ;
//}

// 初始话时需要自己判断方向 功能测试
void position_init(void){

    //
    point_number = 5;

    Points[0].delta_distance.x_y_path = 10;
    // LF=-V，RF=+V，LB=+V，RB=-V（V 为速度值） 1 2 3 4

    Points[0].target_speed.x_y[0] = 14;
    Points[0].target_speed.x_y[1] = 14;
    Points[0].target_speed.x_y[2] = 14;
    Points[0].target_speed.x_y[3] = 14;
    Points[0].flag = dis_start_y; //这个和速度决定了行驶的路线


    Points[1].delta_distance.x_y_path = 10;
    // LF=-V，RF=+V，LB=+V，RB=-V（V 为速度值） 1 2 3 4

    Points[1].target_speed.x_y[0] = -14;
    Points[1].target_speed.x_y[1] = -14;
    Points[1].target_speed.x_y[2] = -14;
    Points[1].target_speed.x_y[3] = -14;
    Points[1].flag = dis_start_y;

    Points[2].delta_distance.x_y_path = 10;
    // LF=-V，RF=+V，LB=+V，RB=-V（V 为速度值） 1 2 3 4
    Points[2].target_speed.x_y[0] = -14;
    Points[2].target_speed.x_y[1] = -14;
    Points[2].target_speed.x_y[2] = -14;
    Points[2].target_speed.x_y[3] = -14;
    Points[2].flag = dis_start_y;

    Points[3].delta_distance.x_y_path = 30;
    // LF=-V，RF=+V，LB=+V，RB=-V（V 为速度值） 1 2 3 4

    Points[3].target_speed.x_y[0] = 14;
    Points[3].target_speed.x_y[1] = 14;
    Points[3].target_speed.x_y[2] = 14;
    Points[3].target_speed.x_y[3] = 14;
    Points[3].flag = dis_start_y;
}



volatile bool is_position_loop_done = false;

volatile coordinate_struct task1_points_to_put[1] = {0};
volatile coordinate_struct task1_points_to_pick[3] = {0};

// task1 测试
void task1_get_put_position_loop_init(void){
    task1_points_to_put[0].delta_distance.x_y_path = 7;

    task1_points_to_put[0].target_speed.x_y[0] =  -10;
    task1_points_to_put[0].target_speed.x_y[1] =   10;
    task1_points_to_put[0].target_speed.x_y[2] =   10;
    task1_points_to_put[0].target_speed.x_y[3] =  -10;
    task1_points_to_put[0].flag = dis_start_x;

}

void task1_get_pick_position_loop_init(void){
    task1_points_to_pick[0].delta_distance.x_y_path = 10;

    task1_points_to_pick[0].target_speed.x_y[0] =  -10;
    task1_points_to_pick[0].target_speed.x_y[1] =   10;
    task1_points_to_pick[0].target_speed.x_y[2] =   10;
    task1_points_to_pick[0].target_speed.x_y[3] =  -10;
    task1_points_to_pick[0].flag = dis_start_x;

    task1_points_to_pick[1].delta_distance.x_y_path = 20;

    task1_points_to_pick[1].target_speed.x_y[0] =  -10;
    task1_points_to_pick[1].target_speed.x_y[1] =  -10;
    task1_points_to_pick[1].target_speed.x_y[2] =  -10;
    task1_points_to_pick[1].target_speed.x_y[3] =  -10;
    task1_points_to_pick[1].flag = dis_start_y;


    task1_points_to_pick[2].delta_distance.x_y_path = 30;

    task1_points_to_pick[2].target_speed.x_y[0] =  10;
    task1_points_to_pick[2].target_speed.x_y[1] =  10;
    task1_points_to_pick[2].target_speed.x_y[2] =  10;
    task1_points_to_pick[2].target_speed.x_y[3] =  10;
    task1_points_to_pick[2].flag = dis_start_y;

}


volatile coordinate_struct task1_encounter_small_cy_in_one_S[2] = {0};
volatile coordinate_struct task1_back_to_two_from_small_cy_S[2] = {0};

volatile coordinate_struct task1_encounter_medium_cy_in_one_S[2] = {0};
volatile coordinate_struct task1_back_to_two_from_medium_cy_S[2] = {0};

volatile coordinate_struct task1_encounter_large_cy_in_one_S[2] = {0};
volatile coordinate_struct task1_back_to_two_from_large_cy_S[2] = {0};

// state;
volatile float task1_walk_sideway = 8.6f;

volatile float task1_walk_sideway2 = 8.4f;
volatile float walk_vertically1 = 0;
volatile float walk_vertically2 = 0;
volatile float walk_vertically3 = 0;
void task1_encounter_small_cy_in_one(void){
    point_number = 3;
    task1_encounter_small_cy_in_one_S[0].target_speed.x_y[0] =  -17;
    task1_encounter_small_cy_in_one_S[0].target_speed.x_y[1] =   17;
    task1_encounter_small_cy_in_one_S[0].target_speed.x_y[2] =   24.3;
    task1_encounter_small_cy_in_one_S[0].target_speed.x_y[3] =  -24.9;

    task1_encounter_small_cy_in_one_S[0].flag = dis_start_x;

    task1_encounter_small_cy_in_one_S[0].delta_distance.x_y_path = (task1_walk_sideway + 1);
    task1_encounter_small_cy_in_one_S[0].next_flag = dis_start_y;



    task1_encounter_small_cy_in_one_S[1].target_speed.x_y[0] =   19;
    task1_encounter_small_cy_in_one_S[1].target_speed.x_y[1] =   19;
    task1_encounter_small_cy_in_one_S[1].target_speed.x_y[2] =   19;
    task1_encounter_small_cy_in_one_S[1].target_speed.x_y[3] =   19;

    task1_encounter_small_cy_in_one_S[1].flag = dis_start_y;

    task1_encounter_small_cy_in_one_S[1].delta_distance.x_y_path = 15.6f;
    task1_encounter_small_cy_in_one_S[1].next_flag = dis_end;     // [1]完成后期望结束
}

void task1_back_to_two_from_small_cy(void){
    point_number = 3;
    task1_back_to_two_from_small_cy_S[0].target_speed.x_y[0] =   18;
    task1_back_to_two_from_small_cy_S[0].target_speed.x_y[1] =  -18;
    task1_back_to_two_from_small_cy_S[0].target_speed.x_y[2] =  -21;
    task1_back_to_two_from_small_cy_S[0].target_speed.x_y[3] =   21.5;

    task1_back_to_two_from_small_cy_S[0].flag = dis_start_x;

    task1_back_to_two_from_small_cy_S[0].delta_distance.x_y_path = (task1_walk_sideway2 + 3);
    task1_back_to_two_from_small_cy_S[0].next_flag = dis_start_y;


    task1_back_to_two_from_small_cy_S[1].target_speed.x_y[0] =   -18;
    task1_back_to_two_from_small_cy_S[1].target_speed.x_y[1] =   -18;
    task1_back_to_two_from_small_cy_S[1].target_speed.x_y[2] =   -18;
    task1_back_to_two_from_small_cy_S[1].target_speed.x_y[3] =   -18;

    task1_back_to_two_from_small_cy_S[1].flag = dis_start_y;

    task1_back_to_two_from_small_cy_S[1].delta_distance.x_y_path = 11.1f;
    task1_back_to_two_from_small_cy_S[1].next_flag = dis_end;     // [1]完成后期望结束

}




//
void task1_encounter_medium_cy_in_one(void){
    point_number = 3;
    task1_encounter_medium_cy_in_one_S[0].target_speed.x_y[0] =  -19;
    task1_encounter_medium_cy_in_one_S[0].target_speed.x_y[1] =   19;
    task1_encounter_medium_cy_in_one_S[0].target_speed.x_y[2] =   25.5;
    task1_encounter_medium_cy_in_one_S[0].target_speed.x_y[3] =  -26.1;

    task1_encounter_medium_cy_in_one_S[0].flag = dis_start_x;

    task1_encounter_medium_cy_in_one_S[0].delta_distance.x_y_path =  (task1_walk_sideway + 3);
    task1_encounter_medium_cy_in_one_S[0].next_flag = dis_start_y;


    task1_encounter_medium_cy_in_one_S[1].target_speed.x_y[0] =   18;
    task1_encounter_medium_cy_in_one_S[1].target_speed.x_y[1] =   18;
    task1_encounter_medium_cy_in_one_S[1].target_speed.x_y[2] =   18;
    task1_encounter_medium_cy_in_one_S[1].target_speed.x_y[3] =   18;

    task1_encounter_medium_cy_in_one_S[1].flag = dis_start_y;

    task1_encounter_medium_cy_in_one_S[1].delta_distance.x_y_path = 9.1f;
    task1_encounter_medium_cy_in_one_S[1].next_flag = dis_end;

}

void task1_back_to_two_from_medium_cy(void){
    task1_back_to_two_from_medium_cy_S[0].target_speed.x_y[0] =   18;
    task1_back_to_two_from_medium_cy_S[0].target_speed.x_y[1] =  -18;
    task1_back_to_two_from_medium_cy_S[0].target_speed.x_y[2] =  -24;
    task1_back_to_two_from_medium_cy_S[0].target_speed.x_y[3] =   24.5;

    task1_back_to_two_from_medium_cy_S[0].flag = dis_start_x;
    task1_back_to_two_from_medium_cy_S[0].next_flag = dis_start_y;
    task1_back_to_two_from_medium_cy_S[0].delta_distance.x_y_path = (task1_walk_sideway2 + 2.6);

    task1_back_to_two_from_medium_cy_S[1].target_speed.x_y[0] =   9;
    task1_back_to_two_from_medium_cy_S[1].target_speed.x_y[1] =   9;
    task1_back_to_two_from_medium_cy_S[1].target_speed.x_y[2] =   9;
    task1_back_to_two_from_medium_cy_S[1].target_speed.x_y[3] =   9;

    task1_back_to_two_from_medium_cy_S[1].flag = dis_start_y;

    task1_back_to_two_from_medium_cy_S[1].delta_distance.x_y_path = 0.003;



}

// 2 - s
// 3 -mid
//
void task1_encounter_large_cy_in_one(void){
    task1_encounter_large_cy_in_one_S[0].target_speed.x_y[0] =  -18;
    task1_encounter_large_cy_in_one_S[0].target_speed.x_y[1] =   18;
    task1_encounter_large_cy_in_one_S[0].target_speed.x_y[2] =   23.1;
    task1_encounter_large_cy_in_one_S[0].target_speed.x_y[3] =  -23.2;

    task1_encounter_large_cy_in_one_S[0].flag = dis_start_x;
    task1_encounter_large_cy_in_one_S[0].next_flag = dis_start_y;
    task1_encounter_large_cy_in_one_S[0].delta_distance.x_y_path = (task1_walk_sideway + 3.5);

    task1_encounter_large_cy_in_one_S[1].target_speed.x_y[0] =  9;
    task1_encounter_large_cy_in_one_S[1].target_speed.x_y[1] =  9;
    task1_encounter_large_cy_in_one_S[1].target_speed.x_y[2] =  9;
    task1_encounter_large_cy_in_one_S[1].target_speed.x_y[3] =  9;

    task1_encounter_large_cy_in_one_S[1].flag = dis_start_y;

    task1_encounter_large_cy_in_one_S[1].delta_distance.x_y_path = 1.8;

}

void task1_back_to_two_from_large_cy(void){
    task1_back_to_two_from_large_cy_S[0].target_speed.x_y[0] =   18;
    task1_back_to_two_from_large_cy_S[0].target_speed.x_y[1] =  -18;
    task1_back_to_two_from_large_cy_S[0].target_speed.x_y[2] =  -24;
    task1_back_to_two_from_large_cy_S[0].target_speed.x_y[3] =   24.5;

    task1_back_to_two_from_large_cy_S[0].flag = dis_start_x;

    task1_back_to_two_from_large_cy_S[0].delta_distance.x_y_path = (task1_walk_sideway2 + 3);
    task1_back_to_two_from_large_cy_S[0].next_flag = dis_start_y;


    task1_back_to_two_from_large_cy_S[1].target_speed.x_y[0] =   18;
    task1_back_to_two_from_large_cy_S[1].target_speed.x_y[1] =   18;
    task1_back_to_two_from_large_cy_S[1].target_speed.x_y[2] =   18;
    task1_back_to_two_from_large_cy_S[1].target_speed.x_y[3] =   18;

    task1_back_to_two_from_large_cy_S[1].flag = dis_start_y;

    task1_back_to_two_from_large_cy_S[1].delta_distance.x_y_path = 6.31f;
    task1_back_to_two_from_large_cy_S[1].next_flag = dis_end;
}


volatile coordinate_struct task1_encounter_small_cy_in_two_S[2] = {0};
volatile coordinate_struct task1_back_to_three_from_small_cy_S[2] = {0};

volatile coordinate_struct task1_encounter_medium_cy_in_two_S[2] = {0};
volatile coordinate_struct task1_back_to_three_from_medium_cy_S[2] = {0};

volatile coordinate_struct task1_encounter_large_cy_in_two_S[2] = {0};
volatile coordinate_struct task1_back_to_three_from_large_cy_S[2] = {0};

void task1_encounter_small_cy_in_two(void){
    task1_encounter_small_cy_in_two_S[0].target_speed.x_y[0] =  -18;
    task1_encounter_small_cy_in_two_S[0].target_speed.x_y[1] =   18;
    task1_encounter_small_cy_in_two_S[0].target_speed.x_y[2] =   24;
    task1_encounter_small_cy_in_two_S[0].target_speed.x_y[3] =  -24.4;

    task1_encounter_small_cy_in_two_S[0].flag = dis_start_x;

    task1_encounter_small_cy_in_two_S[0].delta_distance.x_y_path = (task1_walk_sideway + 2);
    task1_encounter_small_cy_in_two_S[0].next_flag = dis_start_y;


    task1_encounter_small_cy_in_two_S[1].target_speed.x_y[0] =   18;
    task1_encounter_small_cy_in_two_S[1].target_speed.x_y[1] =   18;
    task1_encounter_small_cy_in_two_S[1].target_speed.x_y[2] =   18;
    task1_encounter_small_cy_in_two_S[1].target_speed.x_y[3] =   18;

    task1_encounter_small_cy_in_two_S[1].flag = dis_start_y;

    task1_encounter_small_cy_in_two_S[1].delta_distance.x_y_path = 9.6f;
    task1_encounter_small_cy_in_two_S[1].next_flag = dis_end;

}

void task1_back_to_three_from_small_cy(void){
    task1_back_to_three_from_small_cy_S[0].target_speed.x_y[0] =   18;
    task1_back_to_three_from_small_cy_S[0].target_speed.x_y[1] =  -18;
    task1_back_to_three_from_small_cy_S[0].target_speed.x_y[2] =  -22;
    task1_back_to_three_from_small_cy_S[0].target_speed.x_y[3] =   22.5;

    task1_back_to_three_from_small_cy_S[0].flag = dis_start_x;

    task1_back_to_three_from_small_cy_S[0].delta_distance.x_y_path = (task1_walk_sideway + 3);

    task1_back_to_three_from_small_cy_S[1].target_speed.x_y[0] =  -10;
    task1_back_to_three_from_small_cy_S[1].target_speed.x_y[1] =  -10;
    task1_back_to_three_from_small_cy_S[1].target_speed.x_y[2] =  -10;
    task1_back_to_three_from_small_cy_S[1].target_speed.x_y[3] =  -10;

    task1_back_to_three_from_small_cy_S[1].flag = dis_start_y;

    task1_back_to_three_from_small_cy_S[1].delta_distance.x_y_path = 6.41;
}


void task1_encounter_medium_cy_in_two(void){
    task1_encounter_medium_cy_in_two_S[0].target_speed.x_y[0] =  -17;
    task1_encounter_medium_cy_in_two_S[0].target_speed.x_y[1] =   17;
    task1_encounter_medium_cy_in_two_S[0].target_speed.x_y[2] =   24.8;
    task1_encounter_medium_cy_in_two_S[0].target_speed.x_y[3] =  -25.6;

    task1_encounter_medium_cy_in_two_S[0].flag = dis_start_x;

    task1_encounter_medium_cy_in_two_S[0].delta_distance.x_y_path = (task1_walk_sideway+2);
    task1_encounter_medium_cy_in_two_S[0].next_flag = dis_start_y;


    task1_encounter_medium_cy_in_two_S[1].target_speed.x_y[0] =  10;
    task1_encounter_medium_cy_in_two_S[1].target_speed.x_y[1] =  10;
    task1_encounter_medium_cy_in_two_S[1].target_speed.x_y[2] =  10;
    task1_encounter_medium_cy_in_two_S[1].target_speed.x_y[3] =  10;

    task1_encounter_medium_cy_in_two_S[1].flag = dis_start_y;

    task1_encounter_medium_cy_in_two_S[1].delta_distance.x_y_path = 0.41;
}


void task1_back_to_three_from_medium_cy(void){
    task1_back_to_three_from_medium_cy_S[0].target_speed.x_y[0] =   18;
    task1_back_to_three_from_medium_cy_S[0].target_speed.x_y[1] =  -18;
    task1_back_to_three_from_medium_cy_S[0].target_speed.x_y[2] =  -24.5;
    task1_back_to_three_from_medium_cy_S[0].target_speed.x_y[3] =   25;

    task1_back_to_three_from_medium_cy_S[0].flag = dis_start_x;

    task1_back_to_three_from_medium_cy_S[0].delta_distance.x_y_path =  (task1_walk_sideway + 2.6);
    task1_back_to_three_from_medium_cy_S[0].next_flag = dis_start_y;

    task1_back_to_three_from_medium_cy_S[1].target_speed.x_y[0] =   18;
    task1_back_to_three_from_medium_cy_S[1].target_speed.x_y[1] =   18;
    task1_back_to_three_from_medium_cy_S[1].target_speed.x_y[2] =   18;
    task1_back_to_three_from_medium_cy_S[1].target_speed.x_y[3] =   18;

    task1_back_to_three_from_medium_cy_S[1].flag = dis_start_y;

    task1_back_to_three_from_medium_cy_S[1].delta_distance.x_y_path = 6.9f;
    task1_back_to_three_from_medium_cy_S[1].next_flag = dis_end;
}


void task1_encounter_large_cy_in_two(void){

    task1_encounter_large_cy_in_two_S[0].target_speed.x_y[0] =  -18;
    task1_encounter_large_cy_in_two_S[0].target_speed.x_y[1] =   18;
    task1_encounter_large_cy_in_two_S[0].target_speed.x_y[2] =   26.7;
    task1_encounter_large_cy_in_two_S[0].target_speed.x_y[3] =  -27.1;

    task1_encounter_large_cy_in_two_S[0].flag = dis_start_x;

    task1_encounter_large_cy_in_two_S[0].delta_distance.x_y_path =  (task1_walk_sideway + 3.66);
    task1_encounter_large_cy_in_two_S[0].next_flag = dis_start_y;

    task1_encounter_large_cy_in_two_S[1].target_speed.x_y[0] =   -9;
    task1_encounter_large_cy_in_two_S[1].target_speed.x_y[1] =   -9;
    task1_encounter_large_cy_in_two_S[1].target_speed.x_y[2] =   -9;
    task1_encounter_large_cy_in_two_S[1].target_speed.x_y[3] =   -9;

    task1_encounter_large_cy_in_two_S[1].flag = dis_start_y;

    task1_encounter_large_cy_in_two_S[1].delta_distance.x_y_path = 6.13f;

    task1_encounter_large_cy_in_two_S[1].next_flag = dis_end;
}

volatile int task1_bug_flag = 0;

void task1_back_to_three_from_large_cy(void){
    task1_back_to_three_from_large_cy_S[0].target_speed.x_y[0] =   18;
    task1_back_to_three_from_large_cy_S[0].target_speed.x_y[1] =  -18;
    task1_back_to_three_from_large_cy_S[0].target_speed.x_y[2] =  -24;
    task1_back_to_three_from_large_cy_S[0].target_speed.x_y[3] =   24.4;

    task1_back_to_three_from_large_cy_S[0].flag = dis_start_x;

    task1_back_to_three_from_large_cy_S[0].delta_distance.x_y_path = (task1_walk_sideway + 3);
    task1_back_to_three_from_large_cy_S[0].next_flag = dis_start_y;

    task1_back_to_three_from_large_cy_S[1].target_speed.x_y[0] =   20;
    task1_back_to_three_from_large_cy_S[1].target_speed.x_y[1] =   20;
    task1_back_to_three_from_large_cy_S[1].target_speed.x_y[2] =   20;
    task1_back_to_three_from_large_cy_S[1].target_speed.x_y[3] =   20;

    task1_back_to_three_from_large_cy_S[1].flag = dis_start_y;

    task1_back_to_three_from_large_cy_S[1].delta_distance.x_y_path = 12.6f;
    task1_back_to_three_from_large_cy_S[1].next_flag = dis_end;
    task1_bug_flag =1;
}

volatile coordinate_struct task1_encounter_small_cy_in_three_S[2] = {0};
volatile coordinate_struct task1_back_to_following_from_small_cy_S[2] = {0};

volatile coordinate_struct task1_encounter_medium_cy_in_three_S[2] = {0};
volatile coordinate_struct task1_back_to_following_from_medium_cy_S[2] = {0};

volatile coordinate_struct task1_encounter_large_cy_in_three_S[2] = {0};
volatile coordinate_struct task1_back_to_following_from_large_cy_S[2] = {0};
void task1_encounter_small_cy_in_three(void){
    task1_encounter_small_cy_in_three_S[0].target_speed.x_y[0] =  -18;
    task1_encounter_small_cy_in_three_S[0].target_speed.x_y[1] =   18;
    task1_encounter_small_cy_in_three_S[0].target_speed.x_y[2] =   23.7;
    task1_encounter_small_cy_in_three_S[0].target_speed.x_y[3] =   -24.1;
//    task1_encounter_large_cy_in_three_S[0].target_speed.x_y[2] =   23.3;
//    task1_encounter_large_cy_in_three_S[0].target_speed.x_y[3] =  -24.1;

    //
    task1_encounter_small_cy_in_three_S[0].flag = dis_start_x;

    task1_encounter_small_cy_in_three_S[0].delta_distance.x_y_path = (task1_walk_sideway + 3);
    task1_encounter_small_cy_in_three_S[0].next_flag = dis_start_y;


    task1_encounter_small_cy_in_three_S[1].flag = dis_start_y;
    task1_encounter_small_cy_in_three_S[1].target_speed.x_y[0] =  9;
    task1_encounter_small_cy_in_three_S[1].target_speed.x_y[1] =  9;
    task1_encounter_small_cy_in_three_S[1].target_speed.x_y[2] =  9;
    task1_encounter_small_cy_in_three_S[1].target_speed.x_y[3] =  9;

    task1_encounter_small_cy_in_three_S[1].delta_distance.x_y_path = 0.0003;

}

void task1_back_to_following_from_small_cy(void){
    task1_back_to_following_from_small_cy_S[0].target_speed.x_y[0] =   18;
    task1_back_to_following_from_small_cy_S[0].target_speed.x_y[1] =  -18;
    task1_back_to_following_from_small_cy_S[0].target_speed.x_y[2] =  -22;
    task1_back_to_following_from_small_cy_S[0].target_speed.x_y[3] =   23.5;

    task1_back_to_following_from_small_cy_S[0].flag = dis_start_x;

    task1_back_to_following_from_small_cy_S[0].next_flag = dis_start_y;
    task1_back_to_following_from_small_cy_S[0].delta_distance.x_y_path = (task1_walk_sideway  - 2);

    task1_back_to_following_from_small_cy_S[1].target_speed.x_y[0] =  24;
    task1_back_to_following_from_small_cy_S[1].target_speed.x_y[1] =  24;
    task1_back_to_following_from_small_cy_S[1].target_speed.x_y[2] =  24;
    task1_back_to_following_from_small_cy_S[1].target_speed.x_y[3] =  24;

    task1_back_to_following_from_small_cy_S[1].flag = dis_start_y;

    task1_back_to_following_from_small_cy_S[1].delta_distance.x_y_path = 5;


}


void task1_encounter_medium_cy_in_three(void){
    task1_encounter_medium_cy_in_three_S[0].target_speed.x_y[0] =  -18;
    task1_encounter_medium_cy_in_three_S[0].target_speed.x_y[1] =   18;
    task1_encounter_medium_cy_in_three_S[0].target_speed.x_y[2] =   24.8;
    task1_encounter_medium_cy_in_three_S[0].target_speed.x_y[3] =  -25;

    task1_encounter_medium_cy_in_three_S[0].flag = dis_start_x;

    task1_encounter_medium_cy_in_three_S[0].delta_distance.x_y_path =  (task1_walk_sideway + 3.3);
    task1_encounter_medium_cy_in_three_S[0].next_flag = dis_start_y;



    task1_encounter_medium_cy_in_three_S[1].target_speed.x_y[0] =    -10;
    task1_encounter_medium_cy_in_three_S[1].target_speed.x_y[1] =    -10;
    task1_encounter_medium_cy_in_three_S[1].target_speed.x_y[2] =    -10;
    task1_encounter_medium_cy_in_three_S[1].target_speed.x_y[3] =    -10;

    task1_encounter_medium_cy_in_three_S[1].flag = dis_start_y;

    task1_encounter_medium_cy_in_three_S[1].delta_distance.x_y_path = 5.85f;
    task1_encounter_medium_cy_in_three_S[1].next_flag = dis_end;
}

void task1_back_to_following_from_medium_cy(void){
    task1_back_to_following_from_medium_cy_S[0].target_speed.x_y[0] =  18;
    task1_back_to_following_from_medium_cy_S[0].target_speed.x_y[1] =  -18;
    task1_back_to_following_from_medium_cy_S[0].target_speed.x_y[2] =  -20;
    task1_back_to_following_from_medium_cy_S[0].target_speed.x_y[3] =  20.4;

    task1_back_to_following_from_medium_cy_S[0].flag = dis_start_x;

    task1_back_to_following_from_medium_cy_S[0].delta_distance.x_y_path = 5.8f;
    task1_back_to_following_from_medium_cy_S[0].next_flag = dis_start_y;

    task1_back_to_following_from_medium_cy_S[1].target_speed.x_y[0] = 24;
    task1_back_to_following_from_medium_cy_S[1].target_speed.x_y[1] = 24;
    task1_back_to_following_from_medium_cy_S[1].target_speed.x_y[2] = 24;
    task1_back_to_following_from_medium_cy_S[1].target_speed.x_y[3] = 24;

    task1_back_to_following_from_medium_cy_S[1].flag = dis_start_y;

    task1_back_to_following_from_medium_cy_S[1].delta_distance.x_y_path = 12.65f;
    task1_back_to_following_from_medium_cy_S[1].next_flag = dis_end;
}





void task1_encounter_large_cy_in_three(void){
    task1_encounter_large_cy_in_three_S[0].target_speed.x_y[0] =  -18;
    task1_encounter_large_cy_in_three_S[0].target_speed.x_y[1] =   18;
    task1_encounter_large_cy_in_three_S[0].target_speed.x_y[2] =   25.3;
    task1_encounter_large_cy_in_three_S[0].target_speed.x_y[3] =  -25.6;

    task1_encounter_large_cy_in_three_S[0].flag = dis_start_x;

    task1_encounter_large_cy_in_three_S[0].delta_distance.x_y_path = (task1_walk_sideway + 2.5);
    task1_encounter_large_cy_in_three_S[0].next_flag = dis_start_y;


    task1_encounter_large_cy_in_three_S[1].target_speed.x_y[0] =    -18;
    task1_encounter_large_cy_in_three_S[1].target_speed.x_y[1] =    -18;
    task1_encounter_large_cy_in_three_S[1].target_speed.x_y[2] =    -18;
    task1_encounter_large_cy_in_three_S[1].target_speed.x_y[3] =    -18;

    task1_encounter_large_cy_in_three_S[1].flag = dis_start_y;

    task1_encounter_large_cy_in_three_S[1].delta_distance.x_y_path = 14.2f;
    task1_encounter_large_cy_in_three_S[1].next_flag = dis_end;
}

void task1_back_to_following_from_large_cy(void){
    task1_back_to_following_from_large_cy_S[0].target_speed.x_y[0] =  18;
    task1_back_to_following_from_large_cy_S[0].target_speed.x_y[1] = -18;
    task1_back_to_following_from_large_cy_S[0].target_speed.x_y[2] = -20;
    task1_back_to_following_from_large_cy_S[0].target_speed.x_y[3] =  20.5;

    task1_back_to_following_from_large_cy_S[0].flag = dis_start_x;

    task1_back_to_following_from_large_cy_S[0].delta_distance.x_y_path = 6.4f;
    task1_back_to_following_from_large_cy_S[0].next_flag = dis_start_y;

    task1_back_to_following_from_large_cy_S[1].target_speed.x_y[0] =    22;
    task1_back_to_following_from_large_cy_S[1].target_speed.x_y[1] =    22;
    task1_back_to_following_from_large_cy_S[1].target_speed.x_y[2] =    22;
    task1_back_to_following_from_large_cy_S[1].target_speed.x_y[3] =    22;

    task1_back_to_following_from_large_cy_S[1].flag = dis_start_y;

    task1_back_to_following_from_large_cy_S[1].delta_distance.x_y_path = 19.45f;
    task1_back_to_following_from_large_cy_S[1].next_flag = dis_end;
}




volatile coordinate_struct following_flow1[4] = {0};

void following_flow1_init(void){
    point_number = 5;
    following_flow1[0].target_speed.x_y[0] =  27;
    following_flow1[0].target_speed.x_y[1] =  27;
    following_flow1[0].target_speed.x_y[2] =  27;
    following_flow1[0].target_speed.x_y[3] =  27;

    following_flow1[0].flag = dis_start_y;

    following_flow1[0].delta_distance.x_y_path = 18.929f;
    following_flow1[0].next_flag = dis_start_angle;

    following_flow1[1].delta_distance.angle = 31.01f;
    following_flow1[1].flag = dis_start_angle;
    following_flow1[1].next_flag = dis_start_y;

    following_flow1[2].target_speed.x_y[0] =  27;
    following_flow1[2].target_speed.x_y[1] =  27;
    following_flow1[2].target_speed.x_y[2] =  27;
    following_flow1[2].target_speed.x_y[3] =  27;

    following_flow1[2].flag = dis_start_y;



    following_flow1[2].delta_distance.x_y_path = 21.6;
    following_flow1[2].next_flag = dis_start_x;

    following_flow1[3].flag = dis_start_x;

    following_flow1[3].target_speed.x_y[0] =  15;
    following_flow1[3].target_speed.x_y[1] =  -15;
    following_flow1[3].target_speed.x_y[2] =  -15;
    following_flow1[3].target_speed.x_y[3] =  15;

// { 10,-10.-10. 10}
    following_flow1[3].delta_distance.x_y_path = 9.1;
    following_flow1[3].next_flag = dis_idle;
}

void task1_all_position_loop_init(void){
    ;
    // state;
    task1_encounter_small_cy_in_one();
    task1_back_to_two_from_small_cy();//

    task1_encounter_medium_cy_in_one();
    task1_back_to_two_from_medium_cy();

    task1_encounter_large_cy_in_one();
    task1_back_to_two_from_large_cy();


    task1_encounter_small_cy_in_two();
    task1_back_to_three_from_small_cy();

    task1_encounter_medium_cy_in_two();
    task1_back_to_three_from_medium_cy();

    task1_encounter_large_cy_in_two();
    task1_back_to_three_from_large_cy();


    task1_encounter_small_cy_in_three();
    task1_back_to_following_from_small_cy();

    task1_encounter_medium_cy_in_three();
    task1_back_to_following_from_medium_cy();

    task1_encounter_large_cy_in_three();
    task1_back_to_following_from_large_cy();

    following_flow1_init();
    point_number = 5;
}

volatile coordinate_struct task2_to_next_half_S[3] = {0};

volatile coordinate_struct task2_to_correct_S[2] = {0};
void TASK2_CORRECT_INIT(void){
    task2_to_correct_S[0].target_speed.x_y[0] =  -12;
    task2_to_correct_S[0].target_speed.x_y[1] =  -12;
    task2_to_correct_S[0].target_speed.x_y[2] =  -12;
    task2_to_correct_S[0].target_speed.x_y[3] =  -12;

    task2_to_correct_S[0].flag = dis_start_x;

    task2_to_correct_S[0].delta_distance.x_y_path = 2.929f;

    task2_to_correct_S[0].next_flag = dis_start_x;
    // {-10,+10,-10,+10}
    task2_to_correct_S[1].target_speed.x_y[0] =  12;
    task2_to_correct_S[1].target_speed.x_y[1] =  -12;
    task2_to_correct_S[1].target_speed.x_y[2] =  12;
    task2_to_correct_S[1].target_speed.x_y[3] =  -12;

    task2_to_correct_S[1].flag = dis_start_x;

    task2_to_correct_S[1].delta_distance.x_y_path = 0.529f;

    task2_to_correct_S[1].next_flag = dis_idle;

}

void TASK2_TO_NEXT_HALF_INIT(void){
    task2_to_next_half_S[0].target_speed.x_y[0] =  15;
    task2_to_next_half_S[0].target_speed.x_y[1] =  -15;
    task2_to_next_half_S[0].target_speed.x_y[2] =  -15;
    task2_to_next_half_S[0].target_speed.x_y[3] =  15;

    task2_to_next_half_S[0].flag = dis_start_x;

    task2_to_next_half_S[0].delta_distance.x_y_path = 4.929f;

    task2_to_next_half_S[0].next_flag = dis_start_y;

    task2_to_next_half_S[1].target_speed.x_y[0] =  25;
    task2_to_next_half_S[1].target_speed.x_y[1] =  25;
    task2_to_next_half_S[1].target_speed.x_y[2] =  25;
    task2_to_next_half_S[1].target_speed.x_y[3] =  25;

    task2_to_next_half_S[1].flag = dis_start_y;
    task2_to_next_half_S[1].delta_distance.x_y_path = 15.729f;
    task2_to_next_half_S[1].next_flag = dis_idle;

}



void task2_all_position_loop_init(void){
    TASK2_TO_NEXT_HALF_INIT();
    TASK2_CORRECT_INIT();
    point_number = 5;
}


volatile coordinate_struct TASK3_SHOOT_IN_ONE_DIS_S1[1] = {0};

volatile coordinate_struct TASK3_SHOOT_IN_ONE_DIS_S2[1] = {0};

volatile coordinate_struct TASK3_SHOOT_IN_ONE_DIS_S3[1] = {0};

volatile coordinate_struct TASK3_SHOOT_IN_ONE_DIS_S4[1] = {0};


volatile coordinate_struct TASK3_SHOOT_IN_TWO_DIS_S1[1] = {0};

volatile coordinate_struct TASK3_SHOOT_IN_TWO_DIS_S2[1] = {0};


volatile coordinate_struct TASK3_SHOOT_IN_THREE_DIS_S[1] = {0};


volatile coordinate_struct TASK3_SHOOT_IN_FOUR_DIS_S[1] = {0};


volatile coordinate_struct TASK3_CORRECT_POS_S[2] = {0};


volatile bool does_task3_yaw_correct = false;
void task3_correct_pos_init(void){
    TASK3_CORRECT_POS_S[0].target_speed.x_y[0] =  -12;
    TASK3_CORRECT_POS_S[0].target_speed.x_y[1] =  -12;
    TASK3_CORRECT_POS_S[0].target_speed.x_y[2] =  -12;
    TASK3_CORRECT_POS_S[0].target_speed.x_y[3] =  -12;

    TASK3_CORRECT_POS_S[0].flag = dis_start_y;

    TASK3_CORRECT_POS_S[0].delta_distance.x_y_path = 2.629f;

    TASK3_CORRECT_POS_S[0].next_flag = dis_start_x;
    TASK3_CORRECT_POS_S[1].target_speed.x_y[0] =  -12;
    TASK3_CORRECT_POS_S[1].target_speed.x_y[1] =  12;
    TASK3_CORRECT_POS_S[1].target_speed.x_y[2] =  -12;
    TASK3_CORRECT_POS_S[1].target_speed.x_y[3] =  12;

    TASK3_CORRECT_POS_S[1].flag = dis_start_x;

    TASK3_CORRECT_POS_S[1].delta_distance.x_y_path = 0.529f;

    TASK3_CORRECT_POS_S[1].next_flag = dis_idle;
}

void task3_shoot_in_one_dis_init(void){
    TASK3_SHOOT_IN_ONE_DIS_S1[0].target_speed.x_y[0] =  15;
    TASK3_SHOOT_IN_ONE_DIS_S1[0].target_speed.x_y[1] =  15;
    TASK3_SHOOT_IN_ONE_DIS_S1[0].target_speed.x_y[2] =  15;
    TASK3_SHOOT_IN_ONE_DIS_S1[0].target_speed.x_y[3] =  15;

    TASK3_SHOOT_IN_ONE_DIS_S1[0].flag = dis_start_x;

    TASK3_SHOOT_IN_ONE_DIS_S1[0].delta_distance.x_y_path = 4.929f;

    TASK3_SHOOT_IN_ONE_DIS_S1[0].next_flag = dis_idle;


    TASK3_SHOOT_IN_ONE_DIS_S2[0].target_speed.x_y[0] =  15;
    TASK3_SHOOT_IN_ONE_DIS_S2[0].target_speed.x_y[1] =  15;
    TASK3_SHOOT_IN_ONE_DIS_S2[0].target_speed.x_y[2] =  15;
    TASK3_SHOOT_IN_ONE_DIS_S2[0].target_speed.x_y[3] =  15;

    TASK3_SHOOT_IN_ONE_DIS_S2[0].flag = dis_start_x;

    TASK3_SHOOT_IN_ONE_DIS_S2[0].delta_distance.x_y_path = 4.929f;

    TASK3_SHOOT_IN_ONE_DIS_S2[0].next_flag = dis_idle;


    TASK3_SHOOT_IN_ONE_DIS_S3[0].target_speed.x_y[0] =  15;
    TASK3_SHOOT_IN_ONE_DIS_S3[0].target_speed.x_y[1] =  15;
    TASK3_SHOOT_IN_ONE_DIS_S3[0].target_speed.x_y[2] =  15;
    TASK3_SHOOT_IN_ONE_DIS_S3[0].target_speed.x_y[3] =  15;

    TASK3_SHOOT_IN_ONE_DIS_S3[0].flag = dis_start_x;

    TASK3_SHOOT_IN_ONE_DIS_S3[0].delta_distance.x_y_path = 4.929f;

    TASK3_SHOOT_IN_ONE_DIS_S3[0].next_flag = dis_idle;


    TASK3_SHOOT_IN_ONE_DIS_S4[0].target_speed.x_y[0] =  15;
    TASK3_SHOOT_IN_ONE_DIS_S4[0].target_speed.x_y[1] =  15;
    TASK3_SHOOT_IN_ONE_DIS_S4[0].target_speed.x_y[2] =  15;
    TASK3_SHOOT_IN_ONE_DIS_S4[0].target_speed.x_y[3] =  15;

    TASK3_SHOOT_IN_ONE_DIS_S4[0].flag = dis_start_x;

    TASK3_SHOOT_IN_ONE_DIS_S4[0].delta_distance.x_y_path = 4.929f;

    TASK3_SHOOT_IN_ONE_DIS_S4[0].next_flag = dis_idle;
}

void task3_shoot_in_two_dis_init(void){
    TASK3_SHOOT_IN_TWO_DIS_S1[0].target_speed.x_y[0] =  15;
    TASK3_SHOOT_IN_TWO_DIS_S1[0].target_speed.x_y[1] =  15;
    TASK3_SHOOT_IN_TWO_DIS_S1[0].target_speed.x_y[2] =  15;
    TASK3_SHOOT_IN_TWO_DIS_S1[0].target_speed.x_y[3] =  15;

    TASK3_SHOOT_IN_TWO_DIS_S1[0].flag = dis_start_x;

    TASK3_SHOOT_IN_TWO_DIS_S1[0].delta_distance.x_y_path = 4.929f;

    TASK3_SHOOT_IN_TWO_DIS_S1[0].next_flag = dis_idle;


    TASK3_SHOOT_IN_TWO_DIS_S2[0].target_speed.x_y[0] =  15;
    TASK3_SHOOT_IN_TWO_DIS_S2[0].target_speed.x_y[1] =  15;
    TASK3_SHOOT_IN_TWO_DIS_S2[0].target_speed.x_y[2] =  15;
    TASK3_SHOOT_IN_TWO_DIS_S2[0].target_speed.x_y[3] =  15;

    TASK3_SHOOT_IN_TWO_DIS_S2[0].flag = dis_start_x;

    TASK3_SHOOT_IN_TWO_DIS_S2[0].delta_distance.x_y_path = 4.929f;

    TASK3_SHOOT_IN_TWO_DIS_S2[0].next_flag = dis_idle;
}

void task3_shoot_in_three_dis_init(void){

    TASK3_SHOOT_IN_THREE_DIS_S[0].target_speed.x_y[0] =  15;
    TASK3_SHOOT_IN_THREE_DIS_S[0].target_speed.x_y[1] =  15;
    TASK3_SHOOT_IN_THREE_DIS_S[0].target_speed.x_y[2] =  15;
    TASK3_SHOOT_IN_THREE_DIS_S[0].target_speed.x_y[3] =  15;

    TASK3_SHOOT_IN_THREE_DIS_S[0].flag = dis_start_x;

    TASK3_SHOOT_IN_THREE_DIS_S[0].delta_distance.x_y_path = 4.929f;

    TASK3_SHOOT_IN_THREE_DIS_S[0].next_flag = dis_start_y;

}

void task3_shoot_in_four_dis_init(void){
    TASK3_SHOOT_IN_FOUR_DIS_S[0].target_speed.x_y[0] =  15;
    TASK3_SHOOT_IN_FOUR_DIS_S[0].target_speed.x_y[1] =  15;
    TASK3_SHOOT_IN_FOUR_DIS_S[0].target_speed.x_y[2] =  15;
    TASK3_SHOOT_IN_FOUR_DIS_S[0].target_speed.x_y[3] =  15;

    TASK3_SHOOT_IN_FOUR_DIS_S[0].flag = dis_start_x;

    TASK3_SHOOT_IN_FOUR_DIS_S[0].delta_distance.x_y_path = 4.929f;

    TASK3_SHOOT_IN_FOUR_DIS_S[0].next_flag = dis_start_y;

}
void task3_correct_position_before_shoot_init(void){
    task3_correct_pos_init();

    task3_shoot_in_one_dis_init();
    task3_shoot_in_two_dis_init();
    task3_shoot_in_three_dis_init();
    task3_shoot_in_four_dis_init();
}







void angle_stable(float angle){
    float angle_dev = wheel_asix.yaw - angle;
    if(angle_dev > -0.8f && angle_dev < 0.8f){
        angle_dev = 0;
    }
    static float last_dev = 0;
    static float angle_Kp = 2;
    static float angle_Kd = 0.5;
    float d = angle_dev - last_dev;
    last_dev = angle_dev;
    angle_dev = angle_Kp  * angle_dev + angle_Kd * d;
    float yaw_correction_speed[4] = {angle_dev, -angle_dev, angle_dev,-angle_dev};
    for(u8 i = 0; i < 4; i++){
        if(yaw_correction_speed[i]> 25){
            yaw_correction_speed[i] = 25;
        }
        if(yaw_correction_speed[i] < -25){
            yaw_correction_speed[i] = -25;
        }
    }
    motor_speed_loop(yaw_correction_speed);
}


void angle_correct(float angle, float limit){
    float angle_dev = wheel_asix.yaw - angle;
    if(angle_dev > -0.8f && angle_dev < 0.8f){
        angle_dev = 0;
        if(task1_start_yaw_correction == true){
            task1_start_yaw_correction = false;
        }
    }
    static float last_dev = 0;
    static float Kp = 2.4;
    static float Kd = 0.5;
    float d = angle_dev - last_dev;
    last_dev = angle_dev;
    angle_dev = Kp * angle_dev + Kd * d;
    float yaw_correction_speed[4] = {angle_dev, -angle_dev, angle_dev,-angle_dev};
    for(u8 i = 0; i < 4; i++){
        if(yaw_correction_speed[i]> limit){
            yaw_correction_speed[i] = limit;
        }
        if(yaw_correction_speed[i] < -limit){
            yaw_correction_speed[i] = -limit;
        }
    }
    following_speed[0] = following_speed[0] + yaw_correction_speed[0];
    following_speed[1] = following_speed[1] + yaw_correction_speed[1];
    following_speed[2] = following_speed[2] + yaw_correction_speed[2];
    following_speed[3] = following_speed[3] + yaw_correction_speed[3];

   // motor_speed_loop(yaw_correction_speed);
}

void position_loop(coordinate_struct *Points){
    // 记录当前 位置环行驶的点数
    static volatile u16 index = 0;
    //

    //
    static volatile u16 last_index = 0;
    // 因为是增量是pwm, 提取设置速度附加的pwm来防止 变化太快产生影响
    static volatile int tem_pwm[4] = {0};
    // 仅用来第一次设置pwm,来避免速度环误差产生太快影响
    static volatile int tem_pwm_flag[6] = {0};
    if(is_position_loop_done == true){
        return;
    }
    switch(Points[index].flag){
        case dis_idle:
            if(tem_pwm_flag[0] == 0){
                tem_pwm[0] = 0;tem_pwm[1] = 0;tem_pwm[2] = 0;tem_pwm[3] = 0;motor_all_run((int *)tem_pwm);
                tem_pwm_flag[0] = 1;
            }
            index = 0;
            is_position_loop_done = true;
            for(u8 i = 0; i < 6; i++){
                tem_pwm_flag[i] = 0;
            }

            if(is_task1_wheels_moving_to_next_point == true){
                is_task1_wheels_moving_to_next_point = false;
//                task1_start_yaw_correction = false;
            }
            if(is_task1_wheels_moving_to_last_point == true){
                is_task1_wheels_moving_to_last_point = false;
//                task1_start_yaw_correction = false;
            }
            if(following_flow_start == true){
                following_flow_start = false;
            }
            if(task1_y_correct_start1 == false && Points[0].delta_distance.x_y_path ==  15.929f){
                task1_y_correct_start1 = true;

            }
//            if( task2_finish_half == true){
//                task2_all_done = 2;
//                task2_finish_half = false;
//                does_task_work_flow_start = false;
//            }
            if(task2_start_correct == true){
                task2_start_correct = false;
            }
            if(tast3_start_to_correct == true){
                tast3_start_to_correct = false;
            }
            break;
            /*
//            Points[index].flag = dis_start_y;
//            Points[index].delta_distance.x = Points[index].target_point.x; // cm
//            Points[index].delta_distance.y = Points[index].target_point.y;
//            if(Points[index].target_point.y > 0)
//            {
//                Points[index].speed[0] = 10;
//                Points[index].speed[1] = 10;
//                Points[index].speed[2] = 10;
//                Points[index].speed[3] = 10;sa
//            }
//            else{
//                Points[index].speed[0] = -10;
//                Points[index].speed[1] = -10;
//                Points[index].speed[2] = -10;
//                Points[index].speed[3] = -10;
//            }
//            Points[index].position.x = 0;
//            Points[index].position.y = 0;
//            break;
 */
        case dis_start_y:
            if(tem_pwm_flag[1] == 0){ // 提前设置速度，因为这是增量式速度环
                all_error_clear(); // 清理所有error
                motor_pid.pwm_out[0] = Points[index].target_speed.x_y[0] * 207;
                motor_pid.pwm_out[1] = Points[index].target_speed.x_y[1] * 207;
                motor_pid.pwm_out[2] = Points[index].target_speed.x_y[2] * 207;
                motor_pid.pwm_out[3] = Points[index].target_speed.x_y[3] * 207;
                motor_all_run((int *)tem_pwm);
                tem_pwm_flag[1] = 1;
            }
            motor_speed_loop((float *)Points[index].target_speed.x_y);
//            Points[index].clock.ms+=5;
            // 0.005 / 4 * sum(all_speed) 积分成速度
            Points[index].position.x_y_path += 0.00125f *( motor_pid.actual_speed[0] + motor_pid.actual_speed[1] + motor_pid.actual_speed[2]+ motor_pid.actual_speed[3]);
            if(fabs(Points[index].delta_distance.x_y_path) <= fabs(Points[index].position.x_y_path)){
                last_index = index;
                index++;
                if(index >= point_number)
                {
                    index = point_number - 1;
                    Points[index].flag = dis_end;
                    break;
                }
                if(Points[last_index].next_flag == Points[index].flag){
                    // 修改提前提前调整速度flag
                    tem_pwm_flag[(Points[index].flag)] = 0;
                    break;
                    // 进入下一个状态
                }
                else{
                    // 赋值环节出现问题 进入空闲状态
                    Points[index].flag = dis_idle;
                }

            }
            break;
        case dis_start_x:
            if(tem_pwm_flag[2] == 0){
                motor_all_stop();
                all_error_clear(); // 清理所以error // 无需清空 encoder 因为 这里设置了速度，下一环节 get_encoder 可以读取到已经设置过的编码值，而且每次循环均有将编码器值清除这种行为
                motor_pid.pwm_out[0] = Points[index].target_speed.x_y[0] * 207;
                motor_pid.pwm_out[1] = Points[index].target_speed.x_y[1] * 207;
                motor_pid.pwm_out[2] = Points[index].target_speed.x_y[2] * 207;
                motor_pid.pwm_out[3] = Points[index].target_speed.x_y[3] * 207;
                motor_all_run((int *)tem_pwm);
                tem_pwm_flag[2] = 1;
            }
            motor_speed_loop((float *)Points[index].target_speed.x_y);
            // L = 0.005 / 4 * abs(sum(all_speed))
            // 转化为 cm/s的单位
            Points[index].position.x_y_path +=  0.00125f * (fabs(motor_pid.actual_speed[0]) +fabs(motor_pid.actual_speed[1])+ fabs(motor_pid.actual_speed[2])+ fabs(motor_pid.actual_speed[3]));
            // 总路程 和 积分的路程比较
            if(fabs(Points[index].delta_distance.x_y_path) <= fabs(Points[index].position.x_y_path)){
                last_index = index;
                index++;
                if(index >= point_number)
                {
                    index = point_number - 1;
                    Points[index].flag = dis_end;
                    break;
                }
                if(Points[last_index].next_flag == Points[index].flag){
                    tem_pwm_flag[(Points[index].flag)] = 0;
                    break;
                }
                else{
                    Points[index].flag = dis_idle;
                }
            }
            break;
        case dis_start_x_y:
            if(tem_pwm_flag[3] == 0){
                all_error_clear(); // 清理所以error // 无需清空 encoder 因为 这里设置了速度，下一环节 get_encoder 可以读取到已经设置过的编码值，而且每次循环均有将编码器值清除这种行为
                motor_pid.pwm_out[0] = Points[index].target_speed.x_y[0] * 207;
                motor_pid.pwm_out[1] = Points[index].target_speed.x_y[1] * 207;
                motor_pid.pwm_out[2] = Points[index].target_speed.x_y[2] * 207;
                motor_pid.pwm_out[3] = Points[index].target_speed.x_y[3] * 207;
                motor_all_run((int *)tem_pwm);
                tem_pwm_flag[3] = 1;
            }
            motor_speed_loop((float *)Points[index].target_speed.x_y);
            // L = 0.005 / 4 * abs(sum(all_speed))
            // 转化为 cm/s的单位
            Points[index].position.x_y_path +=  0.00125f * (fabs(motor_pid.actual_speed[0]) +fabs(motor_pid.actual_speed[1])+ fabs(motor_pid.actual_speed[2])+ fabs(motor_pid.actual_speed[3]));
            if(fabs(Points[index].delta_distance.x_y_path) <= fabs(Points[index].position.x_y_path)){
                last_index = index;
                index++;
                if(index >= point_number)
                {
                    index = point_number - 1;
                    Points[index].flag = dis_end;
                    break;
                }
                if(Points[last_index].next_flag == Points[index].flag){
                    tem_pwm_flag[(Points[index].flag)] = 0;
                    break;
                }
                else{
                    Points[index].flag = dis_idle;
                }
            }
            break;
        case dis_start_angle:
            if(tem_pwm_flag[4] == 0){
                all_error_clear(); // 清理所以error
                motor_all_stop();
                tem_pwm_flag[4] = 1;
            }
            {
                 // delta_distance.angle 存目标偏航角（绝对角度，度）
                 float target_yaw = Points[index].delta_distance.angle;
                 float angle_dev = wheel_asix.yaw - target_yaw;


                 if(angle_dev > -0.8f && angle_dev < 0.8f){
                     angle_dev = 0;
                 }

                 static float last_dev = 0;
                 static float angle_Kp = 2;
                 static float angle_Kd = 0.5;
                 float d = angle_dev - last_dev;
                 last_dev = angle_dev;
                 float correction = angle_Kp * angle_dev + angle_Kd * d;

                 float yaw_correction_speed[4] = {correction, -correction, correction, -correction};
                 for(u8 i = 0; i < 4; i++){
                     if(yaw_correction_speed[i] > 20)  yaw_correction_speed[i] = 20;
                     if(yaw_correction_speed[i] < -20) yaw_correction_speed[i] = -20;
                 }

                 motor_speed_loop(yaw_correction_speed);

                 if(angle_dev == 0){
                     last_dev = 0; // 复位静态变量，供下一个角度点使用

                     last_index = index;
                     index++;
                     if(index >= point_number){
                         index = point_number - 1;
                         Points[index].flag = dis_end;
                         break;
                     }
                     if(Points[last_index].next_flag == Points[index].flag){
                         tem_pwm_flag[(Points[index].flag)] = 0;
                         break;
                     }
                     else{
                         Points[index].flag = dis_idle;
                     }
                 }
             }
             break;
        case dis_end:
            index = 0;
            is_position_loop_done = true;
            for(u8 i = 0; i < 6; i++){
                tem_pwm_flag[i] = 0;
            }
            tem_pwm[0] = 0;
            tem_pwm[1] = 0;
            tem_pwm[2] = 0;
            tem_pwm[3] = 0;

            motor_all_run((int *)tem_pwm);
            if(is_task1_wheels_moving_to_next_point == true){
                is_task1_wheels_moving_to_next_point = false;
            }
            if(is_task1_wheels_moving_to_last_point == true){
                is_task1_wheels_moving_to_last_point = false;
            }
            if(following_flow_start == true){
                following_flow_start = false;
            }
            if(task1_y_correct_start1 == false && Points[0].delta_distance.x_y_path == 15.929f){
                task1_y_correct_start1 = true;

            }
            if(task2_start_correct == true){
                task2_start_correct = false;
                does_task2_send_a_signal =true;
            }
//            if(temp_test_flag == 1){
//                does_task_work_flow_start = false;
//                task2_current_state = TASK2_SECOND_CALIBRATION;
//                temp_test_flag = 0;
//                task2_start = true;
//            }
//            if( task2_finish_half == true){
//                task2_all_done = 2;
//                task2_finish_half = false;
//                does_task_work_flow_start = false;
//            }
            break;
    }
//    for(u16 i = 0; i < Point_length; i++){
//    }
//
}




volatile MOTOR_ICM_TRACKING_STRUCT motor_icm_track = {0};



void ICM_READ_REAL_GYRO(MY_ICM_STRUCT* icm_para){
    unsigned char  buf[14],res;
    res=MPU_Read_Len(MPU6050_ADDR,0X1F,12,buf);
    icm_para->ax=((uint16)buf[0]<<8)|buf[1];
    icm_para->ay=((uint16)buf[2]<<8)|buf[3];
    icm_para->az=((uint16)buf[4]<<8)|buf[5];
    icm_para->gx=((uint16)buf[6]<<8)|buf[7];
    icm_para->gy=((uint16)buf[8]<<8)|buf[9];
    icm_para->gz=((uint16)buf[10]<<8)|buf[11];

    icm_para->gx /= 32.8f;
    icm_para->gy /= 32.8f;
    icm_para->gz /= 32.8f;

    icm_para->ax/= 4096.0f;
    icm_para->ay/= 4096.0f;
    icm_para->az/= 4096.0f;
}

volatile cross_line_state_enum cross_state = CROSS_WAIT_FIRST;
volatile bool is_crossing_line1 = false;
volatile bool is_crossing_line2 = false;
volatile bool line_record1 = false;
volatile bool is_waiting_for_task_record = false;
volatile bool does_task3_start_to_count = false;

volatile float crossing_line_only_total_path = 0;
#define CROSS_LINE_DURATION_CNT1 360
#define CROSS_LINE_DURATION_CNT2 290
// 在5ms速度环里调用
void crossing_line_handle(void){
    float yaw = wheel_asix.yaw;
    static u16 cross_timer = 0;     // 计时，用于"处理窗口"持续时间
    static float yaw_dev_threshold1 = 5.0f; // yaw判定容差

    static float yaw_dev_threshold = 37.0f; // yaw判定容差
    static u16 record_pointer_distance = 0;  // 打印到第几个10c
    crossing_line_only_total_path += 0.00125f * (motor_pid.actual_speed[0] + motor_pid.actual_speed[1]
                               + motor_pid.actual_speed[2] + motor_pid.actual_speed[3]);
    if(crossing_line_only_total_path > record_pointer_distance * 10){
        record_pointer_distance++;
        line_record1 = true;
    }
    switch(cross_state){
        case CROSS_WAIT_FIRST:
            // 检测是否接近 yaw = 0
            if(fabs(yaw - 0.0f) < yaw_dev_threshold){
                cross_state = CROSS_HANDLING_FIRST;
                cross_timer = 0;
                // 进入第一个cross line的特殊处理（比如忽略视觉、直行）
                is_crossing_line1 = true;   // 标志位，给视觉处理逻辑用，告诉它"现在别用视觉修正"
            }
            break;

        case CROSS_HANDLING_FIRST:
            cross_timer++;
            // 假设小车以一定速度通过路口需要约 N 个5ms周期
            // 比如车速10cm/s，路口宽度5cm，需要 0.5s = 100个周期
            if(cross_timer >= CROSS_LINE_DURATION_CNT1){
                is_crossing_line1 = false;  // 恢复视觉修正
                cross_state = CROSS_WAIT_SECOND;
                is_waiting_for_task_record = true;
                does_task3_start_to_count = true;
                following_speed[0] = 0;
                following_speed[1] = 0;
                following_speed[2] = 0;
                following_speed[3] = 0;
            }
            // 这段时间内可以让速度环按直行PWM跑（不依赖视觉偏角）
            break;

        case CROSS_WAIT_SECOND:
            // 检测是否接近 yaw = 270
            if(fabs(yaw - 270.0f) < yaw_dev_threshold1){
                cross_state = CROSS_HANDLING_SECOND;
                cross_timer = 0;
                is_crossing_line2 = true;

            }
            break;

        case CROSS_HANDLING_SECOND:
            cross_timer++;
            if(cross_timer >= CROSS_LINE_DURATION_CNT2){
                is_crossing_line2 = false;
                following_after_task1 = false;
                cross_state = CROSS_DONE;

            }
            break;

        case CROSS_DONE:
            // 两次cross line都处理完了，后续巡线正常进行
            break;
        default:
            break;
    }
}
