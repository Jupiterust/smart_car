/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技TC264DA核心板
【编    写】chiusir
【E-mail】chiusir@163.com
【软件版本】V1.2 版权所有，单位使用请先联系授权
【最后更新】2023年6月6日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://longqiu.taobao.com
------------------------------------------------
【dev.env.】AURIX Development Studio1.6版本
【Target 】 TC264DA/TC264D
【Crystal】 20.000Mhz
【SYS PLL】 200MHz
 ________________________________________________________________
 基于iLLD_1_0_1_11_0底层程序,

 使用例程的时候，建议采用没有空格的英文路径，
 除了CIF为TC264DA独有外，其它的代码兼容TC264D
 本库默认初始化了EMEM：512K，如果用户使用TC264D，注释掉EMEM_InitConfig()初始化函数。
 工程下\Libraries\iLLD\TC26B\Tricore\Cpu\CStart\IfxCpu_CStart0.c第164行左右。
 ________________________________________________________________

 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ
 *  备    注：TC264 有两个CCU6模块  每个模块有两个独立定时器  触发定时器中断
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include "LQ_CCU6.h"
#include <CompilerTasking.h>

#include "LQ_GPIO_LED.h"
#include "LQ_GPT12_ENC.h"
#include "motor.h"
#include "Servo.h"
#include "Task.h"
//#include "Motor.h"

volatile int flag_from_irq = 0;
IFX_INTERRUPT(CCU60_CH0_IRQHandler, CCU60_VECTABNUM, CCU60_CH0_PRIORITY);
IFX_INTERRUPT(CCU60_CH1_IRQHandler, CCU60_VECTABNUM, CCU60_CH1_PRIORITY);
IFX_INTERRUPT(CCU61_CH0_IRQHandler, CCU61_VECTABNUM, CCU61_CH0_PRIORITY);
IFX_INTERRUPT(CCU61_CH1_IRQHandler, CCU61_VECTABNUM, CCU61_CH1_PRIORITY);

/** CCU6中断CPU标号 */
const uint8 Ccu6IrqVectabNum[2] = {CCU60_VECTABNUM, CCU61_VECTABNUM};

/** CCU6中断优先级 */
const uint8 Ccu6IrqPriority[4] = {CCU60_CH0_PRIORITY, CCU60_CH1_PRIORITY, CCU61_CH0_PRIORITY, CCU61_CH1_PRIORITY};

/** CCU6中断服务函数地址 */
const void *Ccu6IrqFuncPointer[4] = {&CCU60_CH0_IRQHandler, &CCU60_CH1_IRQHandler, &CCU61_CH0_IRQHandler,
        &CCU61_CH1_IRQHandler};

/***********************************************************************************************/
/********************************CCU6外部中断  服务函数******************************************/
/***********************************************************************************************/

/*************************************************************************
 *  函数名称：void CCU60_CH0_IRQHandler(void)
 *  功能说明：
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2020年3月30日
 *  备    注：CCU60_CH0使用的中断服务函数
 *************************************************************************/

float target_speed[4] = {5, 5, 5, 5};
volatile float following_speed[4] = {0};
volatile bool task1_start_yaw_correction = false;
//1- 2+   3+ 4-
//{-10, 10, 10,-10}; 左
//{ 10,-10.-10. 10}  右
//  10, 10, 10, 10
// {-10,+10,-10,+10} 顺时针
// { 10,-10, 10,-10}
volatile int tem1_speed[4] = {0};
volatile int error_clear_count[10] = {0};
void CCU60_CH0_IRQHandler(void)
{
    /* 开启CPU中断  否则中断不可嵌套 */
//   IfxCpu_enableInterrupts();
// 清除中断标志
    IfxCcu6_clearInterruptStatusFlag(&MODULE_CCU60, IfxCcu6_InterruptSource_t12PeriodMatch);

    /* 用户代码 */
    // 任务1

    //angle_stable(0);

    if(is_task1_wheels_moving_to_next_point == true){
        is_position_loop_done = false;
        if(pick_times == 1){
            if(task1_cy_id == task1_cylinder_id_small){
                position_loop((coordinate_struct *)task1_encounter_small_cy_in_one_S);
            }
            else if(task1_cy_id == task1_cylinder_id_medium){
                position_loop((coordinate_struct *)task1_encounter_medium_cy_in_one_S);
            }
            else if(task1_cy_id == task1_cylinder_id_large){
                position_loop((coordinate_struct *)task1_encounter_large_cy_in_one_S);
            }
        }
        else if(pick_times == 2){
            if(task1_cy_id == task1_cylinder_id_small){
                position_loop((coordinate_struct *)task1_encounter_small_cy_in_two_S);
            }
            else if(task1_cy_id == task1_cylinder_id_medium){
                position_loop((coordinate_struct *)task1_encounter_medium_cy_in_two_S);
            }
            else if(task1_cy_id == task1_cylinder_id_large){
                position_loop((coordinate_struct *)task1_encounter_large_cy_in_two_S);
            }
        }
        else if(pick_times == 3){
            if(task1_cy_id == task1_cylinder_id_small){
                position_loop((coordinate_struct *)task1_encounter_small_cy_in_three_S);
            }
            else if(task1_cy_id == task1_cylinder_id_medium){
                position_loop((coordinate_struct *)task1_encounter_medium_cy_in_three_S);
            }
            else if(task1_cy_id == task1_cylinder_id_large){
                position_loop((coordinate_struct *)task1_encounter_large_cy_in_three_S);
            }
        }
        return;
    }
    if(is_task1_wheels_moving_to_last_point){
        is_position_loop_done = false;
        if(put_times == 1){
            if(task1_cy_id == task1_cylinder_id_small){
                position_loop((coordinate_struct *)task1_back_to_two_from_small_cy_S);
            }
            else if(task1_cy_id == task1_cylinder_id_medium){
                position_loop((coordinate_struct *)task1_back_to_two_from_medium_cy_S);
            }
            else if(task1_cy_id == task1_cylinder_id_large){
                position_loop((coordinate_struct *)task1_back_to_two_from_large_cy_S);
            }
        }
        else if(put_times == 2){
            if(task1_cy_id == task1_cylinder_id_small){
                position_loop((coordinate_struct *)task1_back_to_three_from_small_cy_S);
            }
            else if(task1_cy_id == task1_cylinder_id_medium){
                position_loop((coordinate_struct *)task1_back_to_three_from_medium_cy_S);
            }
            else if(task1_cy_id == task1_cylinder_id_large){
                position_loop((coordinate_struct *)task1_back_to_three_from_large_cy_S);
            }
        }
        else if(put_times == 3){
            if(task1_cy_id == task1_cylinder_id_small){
                position_loop((coordinate_struct *)task1_back_to_following_from_small_cy_S);
            }
            else if(task1_cy_id == task1_cylinder_id_medium){
                position_loop((coordinate_struct *)task1_back_to_following_from_medium_cy_S);
            }
            else if(task1_cy_id == task1_cylinder_id_large){
                position_loop((coordinate_struct *)task1_back_to_following_from_large_cy_S);
            }
        }
        return;
    }
    if(task1_start_yaw_correction == true){
        angle_stable(0);
    }
    //angle_correct(0);
    //following_correct_by_icm();
    if(following_correct_flag1 == true){
        following_speed[0] = 22;following_speed[1] = 22;following_speed[2] = 22;following_speed[3] = 22;
    }
    if(following_correct_flag2 == true){
        following_speed[0] = 22;following_speed[1] = 22;following_speed[2] = 22;following_speed[3] = 22;
    }
    //循迹任务
   // angle_correct(0);
    motor_speed_loop((float *)following_speed);


    following_speed[0] = 0;
    following_speed[1] = 0;
    following_speed[2] = 0;

    following_speed[3] = 0;

    // InductorGetSample();
    // 读取按键
//    turning_any_angle();
//    if(angle == 0.0f){
//        switch(final_tracking_state){
//            case keep_straight:
//                break;
//            case first_crossing_line:
//                error_clear_count[0]++;
//                if(error_clear_count[0] == 1){
//                    all_error_clear();  // {1300,2000,1500,2000};
//                    tem1_speed[0] = 1300;
//                    tem1_speed[1] = 2000;
//                    tem1_speed[2] = 1500;
//                    tem1_speed[3] = 2000;
//                }
//                if(error_clear_count[0] < 5){
//                    motor_all_run((int*)tem1_speed);
//                    encoder_clear();
//                }
//                else if(error_clear_count[0] >= 5){
//                    motor_speed_loop((float *)following_speed);
//                    if(error_clear_count[0] == 800){
//                        uart_send_data = last_is_cross_line;
//                        UART_PutChar(UART0,(char)uart_send_data);
//                    }
//                }
//                break;
//            case first_crossing_line_end:
//                error_clear_count[1]++;
//                if(error_clear_count[1] == 1){
//                    all_error_clear();
//                    tem1_speed[0] = 1600;
//                    tem1_speed[1] = 1600;
//                    tem1_speed[2] = 1600;
//                    tem1_speed[3] = 1600;
//                }
//                if(error_clear_count[1] < 5){
//                    motor_all_run((int*)tem1_speed);
//                    encoder_clear();
//                }
//                else if(error_clear_count[1] >= 5){
//                    motor_speed_loop((float *)following_speed);
//                }
//                break;
//            case first_turning_left_line:
//                break;
//            case first_circle_line:
//                break;
//            case second_turning_left_line:
//                break;
//            case third_same_turning_left_line:
//                break;
//            case first_turning_right_line:
//                break;
//            case second_turning_right_line:
//                break;
//            case fourth_turning_left_line:
//                break;
//            case stop_line:
//                break;
//
//
//        }
//    }
    // position_loop();

//    LED_Ctrl(LED1, RVS);        // 电平翻转,LED闪烁
}

/*************************************************************************
 *  函数名称：void CCU60_CH1_IRQHandler(void)
 *  功能说明：
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2020年3月30日
 *  备    注：CCU60_CH1使用的中断服务函数
 *************************************************************************/

#define TASK1_EVENT_MIN     200
#define TASK1_EVENT_MAX     300


volatile Wheel_system_tick_struct wheel_system_tick = {0};


void CCU60_CH1_IRQHandler (void)
{
    /* 开启CPU中断  否则中断不可嵌套 */
    IfxCpu_enableInterrupts();
    // 清除中断标志
    IfxCcu6_clearInterruptStatusFlag(&MODULE_CCU60, IfxCcu6_InterruptSource_t13PeriodMatch);

    if(wheel_system_tick.does_tick_start == true){
        wheel_system_tick.tick_in_100_ms++;
    }
    /* 用户代码 */
    LED_Ctrl(LEDALL, RVS);      //电平翻转,LED闪烁

}

/*************************************************************************
 *  函数名称：void CCU61_CH0_IRQHandler(void)
 *  功能说明：
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2020年3月30日
 *  备    注：CCU61_CH0使用的中断服务函数
 *************************************************************************/
volatile wheel_xyz_asix wheel_asix = {0};

void CCU61_CH0_IRQHandler (void)
{
    /* 开启CPU中断  否则中断不可嵌套 */
    IfxCpu_enableInterrupts();

    // 清除中断标志
    IfxCcu6_clearInterruptStatusFlag(&MODULE_CCU61, IfxCcu6_InterruptSource_t12PeriodMatch);

    /* 用户代码 */
    /* 获取编码器值 */
    get_real_gyro((float *)&wheel_asix.gx,(float *)&wheel_asix.gy,(float *)&wheel_asix.gz);
    wheel_asix.roll  += wheel_asix.gx * 0.001;
    wheel_asix.pitch += wheel_asix.gy * 0.001;
    wheel_asix.yaw   += wheel_asix.gz * 0.001;
    flag_from_irq = 1;
//    ECPULSE1 = ENC_GetCounter(ENC2_InPut_P33_7); // 左电机 母板上编码器1，小车前进为负值
//    ECPULSE2 = ENC_GetCounter(ENC4_InPut_P02_8); // 右电机 母板上编码器2，小车前进为正值
//    RAllPulse += ECPULSE2;                       //
}
/*************************************************************************
 *  函数名称：void CCU61_CH1_IRQHandler(void)
 *  功能说明：
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2020年3月30日
 *  备    注：CCU61_CH1使用的中断服务函数
 *************************************************************************/
void CCU61_CH1_IRQHandler (void)
{
    /* 开启CPU中断  否则中断不可嵌套 */
    IfxCpu_enableInterrupts();

    // 清除中断标志
    IfxCcu6_clearInterruptStatusFlag(&MODULE_CCU61, IfxCcu6_InterruptSource_t13PeriodMatch);

    /* 用户代码 */
    LED_Ctrl(LED0, RVS);        // 电平翻转,LED闪烁
}

/*************************************************************************
 *  函数名称：CCU6_InitConfig CCU6
 *  功能说明：定时器周期中断初始化
 *  参数说明：ccu6    ： ccu6模块            CCU60 、 CCU61
 *  参数说明：channel ： ccu6模块通道  CCU6_Channel0 、 CCU6_Channel1
 *  参数说明：us      ： ccu6模块  中断周期时间  单位us
 *  函数返回：无
 *  修改时间：2020年3月30日
 *  备    注：    CCU6_InitConfig(CCU60, CCU6_Channel0, 100);  // 100us进入一次中断
 *************************************************************************/
void CCU6_InitConfig (CCU6_t ccu6, CCU6_Channel_t channel, uint32 us)
{
    IfxCcu6_Timer_Config timerConfig;

    Ifx_CCU6 * module = IfxCcu6_getAddress((IfxCcu6_Index) ccu6);

    uint8 Index = ccu6 * 2 + channel;

    uint32 period = 0;

    uint64 clk = 0;

    /* 关闭中断 */
    boolean interrupt_state = disableInterrupts();

    IfxCcu6_Timer_initModuleConfig(&timerConfig, module);

    clk = IfxScuCcu_getSpbFrequency();

    /* 设置时钟频率  */
    uint8 i = 0;
    while (i++ < 16)
    {
        period = (uint32) (clk * us / 1000000);
        if (period < 0xffff)
        {
            break;
        }
        else
        {
            clk = clk / 2;
        }
    }
    switch (channel)
    {
        case CCU6_Channel0 :
            timerConfig.timer = IfxCcu6_TimerId_t12;
            timerConfig.interrupt1.source = IfxCcu6_InterruptSource_t12PeriodMatch;
            timerConfig.interrupt1.serviceRequest = IfxCcu6_ServiceRequest_1;
            timerConfig.base.t12Frequency = (float) clk;
            timerConfig.base.t12Period = period;                                  // 设置定时中断
            timerConfig.clock.t12countingInputMode = IfxCcu6_CountingInputMode_internal;
            timerConfig.timer12.counterValue = 0;
            timerConfig.interrupt1.typeOfService = Ccu6IrqVectabNum[ccu6];
            timerConfig.interrupt1.priority = Ccu6IrqPriority[Index];
            break;

        case CCU6_Channel1 :
            timerConfig.timer = IfxCcu6_TimerId_t13;
            timerConfig.interrupt2.source = IfxCcu6_InterruptSource_t13PeriodMatch;
            timerConfig.interrupt2.serviceRequest = IfxCcu6_ServiceRequest_2;
            timerConfig.base.t13Frequency = (float) clk;
            timerConfig.base.t13Period = period;
            timerConfig.clock.t13countingInputMode = IfxCcu6_CountingInputMode_internal;
            timerConfig.timer13.counterValue = 0;
            timerConfig.interrupt2.typeOfService = Ccu6IrqVectabNum[ccu6];
            timerConfig.interrupt2.priority = Ccu6IrqPriority[Index];
            break ;
    }

    timerConfig.trigger.t13InSyncWithT12 = FALSE;

    IfxCcu6_Timer Ccu6Timer;

    IfxCcu6_Timer_initModule(&Ccu6Timer, &timerConfig);

    IfxCpu_Irq_installInterruptHandler((void*) Ccu6IrqFuncPointer[Index], Ccu6IrqPriority[Index]);          // 配置中断函数和中断号

    restoreInterrupts(interrupt_state);

    IfxCcu6_Timer_start(&Ccu6Timer);
}

/*************************************************************************
 *  函数名称：CCU6_DisableInterrupt(CCU6_t ccu6, CCU6_Channel_t channel)
 *  功能说明：停止CCU6通道中断
 *  参数说明：ccu6    ： ccu6模块            CCU60 、 CCU61
 *  参数说明：channel ： ccu6模块通道  CCU6_Channel0 、 CCU6_Channel1
 *  函数返回：无
 *  修改时间：2020年5月6日
 *  备    注：
 *************************************************************************/
void CCU6_DisableInterrupt (CCU6_t ccu6, CCU6_Channel_t channel)
{
    Ifx_CCU6 * module = IfxCcu6_getAddress((IfxCcu6_Index) ccu6);
    IfxCcu6_clearInterruptStatusFlag(module, (IfxCcu6_InterruptSource) (7 + channel * 2));
    IfxCcu6_disableInterrupt(module, (IfxCcu6_InterruptSource) (7 + channel * 2));

}

/*************************************************************************
 *  函数名称：CCU6_DisableInterrupt(CCU6_t ccu6, CCU6_Channel_t channel)
 *  功能说明：使能CCU6通道中断
 *  参数说明：ccu6    ： ccu6模块            CCU60 、 CCU61
 *  参数说明：channel ： ccu6模块通道  CCU6_Channel0 、 CCU6_Channel1
 *  函数返回：无
 *  修改时间：2020年5月6日
 *  备    注：
 *************************************************************************/
void CCU6_EnableInterrupt (CCU6_t ccu6, CCU6_Channel_t channel)
{
    Ifx_CCU6 * module = IfxCcu6_getAddress((IfxCcu6_Index) ccu6);
    IfxCcu6_clearInterruptStatusFlag(module, (IfxCcu6_InterruptSource) (7 + channel * 2));
    IfxCcu6_enableInterrupt(module, (IfxCcu6_InterruptSource) (7 + channel * 2));

}

