/*
 * Servo.h
 *
 *  Created on: Apr 17, 2026
 *      Author: TX
 */

#ifndef CORE_SERVO_H_
#define CORE_SERVO_H_

#include "fashion_star_uart_servo_examples.h"
#include "fashion_star_uart_servo.h"
#include "user_uart.h"
#include "include.h"
#include <stdbool.h>

#define SERVO1_ADDR     0X00
#define SERVO2_ADDR     0X01
#define SERVO3_ADDR     0X02
#define SERVO4_ADDR     0X03
#define SERVO5_ADDR     0X04


typedef enum{
    PUMP_IDLE = 0,
    PUMP_ON = 1,
    PUMP_OFF = 2,
}AIR_PUMP_STATE;

typedef struct{
        float angle[5];
        u16 interval[5];
        u8 id[5];
        ServoData servo_data[5];
        AIR_PUMP_STATE pump_state;
        FSUS_sync_servo sync_servo[5];
}dummy_struct;

extern volatile dummy_struct my_dummy;
extern float angle_sequence[20][5];


void Servo_angle_move_Syn(float* angle, u16* interval);
void Servo_angle_read(float* angle);
void Air_pump_pwm_init();
void air_pump_pick_up(void);

#define AIR_PUMP_PWM1               IfxGtm_ATOM2_0_TOUT32_P33_10_OUT
#define AIR_PUMP_CRL_PWM2           IfxGtm_ATOM2_5_TOUT35_P33_13_OUT

#define SERVO_PERIOD_F      50
#define PUMP_OFF_CCR        500
#define PWMP_ON_CCR         1300


void DUMMY_INIT(void);

void TASK1_PICK_OBJECT_UP(void);

void TASK1_PUT_OBJECT(void);

extern volatile bool is_task1_wheels_moving_to_next_point;
extern volatile bool is_task1_wheels_moving_to_last_point;



void TASK1_PICK_OBJECT_UP_SYN(void);
void TASK1_PUT_OBJECT_DOWN_SYN(void);


extern volatile  bool this_function_only_runs_once1 ;
extern volatile u8 TASK2_DROP_COUNT;
extern volatile bool does_task_work_flow_start;
extern volatile bool task2_finish_half;
extern volatile uint8_t task2_all_done;
extern  volatile bool temp_task2_to_next_point[4];
extern volatile bool does_start_moving_three_water_in_one;
extern volatile bool does_start_moving_three_water_in_two;
extern volatile bool task3_arrive_three_ball_done_in_one;
extern volatile bool task3_arrive_three_ball_done_in_two;

extern volatile bool task2_go_back_to_in_one;
extern volatile bool task2_go_back_to_in_two;
typedef enum
{
    TASK2_IDLE = 0,

    TASK2_CALIBRATION,

    TASK2_ARM,

    TASK2_GO_NEXT_HALF,

    TASK2_SECOND_CALIBRATION, // 4

    TASK2_SECOND_ARM,

    TASK2_FINISH, // 6

}TASK2_STATE;


extern volatile TASK2_STATE task2_current_state;

void TASK2_WATCH_DROP_WATER_NUMBER(void);
void TASK2_WATCH_DROP_WATER(void);

void TASK2_PICK_AND_PUT_DROP_WATER_WORKFLOW(void);





// task3
// �ƶ�������꣨�� WORKFLOW ��λ���жϼ�⣩
extern volatile bool task3_shoot_move_s[4];

// ����ȷ����꣨�� dis_idle/dis_end ��λ��WORKFLOW ��ѯ��
extern volatile bool task3_shoot_arrive_s[4];

void TASK3_WORKFLOW(volatile uint8_t* worm_record_array);

void TASK3_WATCH_WORM(void);
void TASK4_WATCH_BALL(void);



#endif /* CORE_SERVO_H_ */
