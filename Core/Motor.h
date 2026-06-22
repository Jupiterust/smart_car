/*
 * Motor.h
 *
 *  Created on: Mar 23, 2026
 *      Author: TX
 */

#ifndef CORE_MOTOR_H_
#define CORE_MOTOR_H_

#include "include.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "stdbool.h"
#include "Servo.h"
#
//电机PWM 宏定义
#define MOTOR1_PWM          IfxGtm_ATOM0_2_TOUT55_P21_4_OUT
#define MOTOR1_DIR          IfxGtm_ATOM0_1_TOUT54_P21_3_OUT


#define MOTOR2_PWM          IfxGtm_ATOM0_6_TOUT42_P23_1_OUT
#define MOTOR2_DIR          IfxGtm_ATOM2_6_TOUT100_P11_11_OUT

#define MOTOR3_PWM          IfxGtm_ATOM0_7_TOUT64_P20_8_OUT
#define MOTOR3_DIR          IfxGtm_ATOM0_3_TOUT56_P21_5_OUT


#define MOTOR4_PWM          IfxGtm_ATOM0_0_TOUT53_P21_2_OUT
#define MOTOR4_DIR          IfxGtm_ATOM0_4_TOUT50_P22_3_OUT

//  设置电机转动的方向
//IfxPort_setPinState(port, pinIndex, IfxPort_State_low);
//port->OMR.U = action << pinIndex;
//
//MODULE_P21->OMR.U =
//#define Set_Motor1_FW()         (MODULE_P21->OMR.U = (IfxPort_State_high << 3))
//#define Set_Motor1_BW()         (MODULE_P21->OMR.U = (IfxPort_State_low  << 3))
//
//#define Set_Motor2_FW()         (MODULE_P32->OMR.U = (IfxPort_State_high << 4))
//#define Set_Motor2_BW()         (MODULE_P32->OMR.U = (IfxPort_State_low  << 4))
//
//#define Set_Motor3_FW()         (MODULE_P21->OMR.U = (IfxPort_State_low  << 5))
//#define Set_Motor3_BW()         (MODULE_P21->OMR.U = (IfxPort_State_high << 5))
//
//#define Set_Motor4_FW()         (MODULE_P22->OMR.U = (IfxPort_State_low  << 3))
//#define Set_Motor4_BW()         (MODULE_P22->OMR.U = (IfxPort_State_high << 3))

#define Set_Motor1_FW()   IfxPort_setPinState(&MODULE_P21, 3, IfxPort_State_high)
#define Set_Motor1_BW()   IfxPort_setPinState(&MODULE_P21, 3, IfxPort_State_low)
//
//#define Set_Motor2_FW()   IfxPort_setPinState(&MODULE_P32, 4, IfxPort_State_high)
//#define Set_Motor2_BW()   IfxPort_setPinState(&MODULE_P32, 4, IfxPort_State_low)
#define Set_Motor2_FW()   IfxPort_setPinState(&MODULE_P11, 11, IfxPort_State_high)
#define Set_Motor2_BW()   IfxPort_setPinState(&MODULE_P11, 11, IfxPort_State_low)


#define Set_Motor3_FW()   IfxPort_setPinState(&MODULE_P21, 5, IfxPort_State_high)
#define Set_Motor3_BW()    IfxPort_setPinState(&MODULE_P21, 5, IfxPort_State_low)


#define Set_Motor4_FW()   IfxPort_setPinState(&MODULE_P22, 3, IfxPort_State_high)
#define Set_Motor4_BW()    IfxPort_setPinState(&MODULE_P22, 3, IfxPort_State_low)
//编码器
typedef enum{
    MOTOR1_ENCONDER = 1,
    MOTOR2_ENCONDER = 2,
    MOTOR3_ENCONDER = 3,
    MOTOR4_ENCONDER = 4
}Motor_encoder;

void all_error_clear(void);
void motor_all_run(int* pwm_arr);
void encoder_clear(void);
void get_encoder(int* value);
void get_encoder_directly(int* value);
typedef struct{
        float k_p[4]; // 比例
        float k_i[4]; // 积分
        float k_d[4]; // 微分

        // 编码器读取
        int encoder_value[4];
        float target_speed[4]; // 单位cm/s
        float actual_speed[4];

        float error[4];
        float last_error[4];
        float last_last_error[4];

        int pwm_out[4];
}Motor_pid_struct;

extern volatile Motor_pid_struct motor_pid;

void PID_init(void);

void motor_speed_loop(float *target_s);

// 角度
#define LEFT_ANGLE_TURN     0
#define RIGHT_ANGLE_TURN    1

extern volatile u8 angle_dir;
extern volatile float angle;
extern volatile int turning_speed[4];
void turning_any_angle(void);


typedef struct{
        float x_y_path;
        float angle;
}coordinate_base_struct;

typedef enum{
    dis_idle = 0,
    dis_start_y = 1,        // tem_pwm_flag[1]
    dis_start_x = 2,        // tem_pwm_flag[2]
    dis_start_x_y = 3,      // tem_pwm_flag[3]
    dis_start_angle = 4,    // tem_pwm_flag[4]
    dis_end = 5,
}coordinate_flag_enum;
// 暂时路况仅需要
// dis_start_y 和 dis_start_angle
typedef struct{
        u16 ms;
        u16 s;
}coordinate_clock_struct;

typedef float coordinate_angle_type;

typedef struct{
        float w[4];              // 角速度 angle_vel
        float x_y[4];           // 无论是朝 x, y 或者 斜线 x_y 都是给这个变量赋值
}coordinate_speed_struct;

//typedef struct{
//        coordinate_base_struct target_points;
//        speed_struct target_speed;
//}coordinate_record_struct;



typedef struct{
    coordinate_base_struct delta_distance;      // 记录目标路程 仅支持走 一段沿 x, y的直线, 或者 x_y 的斜线
    coordinate_base_struct position;            // 记录现在自身移动的位置
    coordinate_speed_struct target_speed;       // 记录目标速度
    coordinate_clock_struct clock;              // 记录行驶的时间
    coordinate_flag_enum flag;                  // 记录状态
    coordinate_flag_enum next_flag;             // 记录当前状态走完后的下一个状态 进行校验
}coordinate_struct;



extern volatile coordinate_struct Points[20];
extern volatile u16 point_number;


typedef struct{
        float gx;
        float gy;
        float gz;

        float ax;
        float ay;
        float az;
}MY_ICM_STRUCT;

typedef enum{
    TRACKING_BY_ICM_OFF = 0,
    TRACKING_BY_ICM_ON = 1,
}ICM_TRACKING_FLAG;

typedef struct{
        MY_ICM_STRUCT icm_para;
        float speed[4];
        float target_angle;
        ICM_TRACKING_FLAG FLAG;
}MOTOR_ICM_TRACKING_STRUCT;

extern volatile MOTOR_ICM_TRACKING_STRUCT motor_icm_track;
extern volatile bool is_position_loop_done;



// extern volatile float following_speed[4];


void position_init(void);
void position_loop(coordinate_struct *Points);

void task1_get_put_position_loop_init(void);

void task1_get_pick_position_loop_init(void);

void task1_all_position_loop_init(void);
void angle_stable(float angle);
void angle_correct(float angle);
extern volatile coordinate_struct task1_points_to_put[1];
extern volatile coordinate_struct task1_points_to_pick[3];

extern volatile coordinate_struct task1_encounter_small_cy_in_one_S[3];
extern volatile coordinate_struct task1_back_to_two_from_small_cy_S[2];

extern volatile coordinate_struct task1_encounter_medium_cy_in_one_S[2];
extern volatile coordinate_struct task1_back_to_two_from_medium_cy_S[1];

extern volatile coordinate_struct task1_encounter_large_cy_in_one_S[1];
extern volatile coordinate_struct task1_back_to_two_from_large_cy_S[2];



extern volatile coordinate_struct task1_encounter_small_cy_in_two_S[2];
extern volatile coordinate_struct task1_back_to_three_from_small_cy_S[1];

extern volatile coordinate_struct task1_encounter_medium_cy_in_two_S[1];
extern volatile coordinate_struct task1_back_to_three_from_medium_cy_S[2];

extern volatile coordinate_struct task1_encounter_large_cy_in_two_S[2];
extern volatile coordinate_struct task1_back_to_three_from_large_cy_S[2];


extern volatile coordinate_struct task1_encounter_small_cy_in_three_S[1] ;
extern volatile coordinate_struct task1_back_to_following_from_small_cy_S[1];

extern volatile coordinate_struct task1_encounter_medium_cy_in_three_S[2];
extern volatile coordinate_struct task1_back_to_following_from_medium_cy_S[1];

extern volatile coordinate_struct task1_encounter_large_cy_in_three_S[2];
extern volatile coordinate_struct task1_back_to_following_from_large_cy_S[1];

extern volatile coordinate_struct following_flow1[4];

extern volatile bool is_crossing_line1;
extern volatile bool is_crossing_line2;
extern volatile bool line_record1;
extern volatile float crossing_line_only_total_path;
void crossing_line_handle(void);

#endif /* CORE_MOTOR_H_ */
