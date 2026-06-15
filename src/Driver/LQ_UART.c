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
ASC异步串行通信，可以作为UART和汽车LIN总线使用，
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include "LQ_UART.h"
#include "stdio.h"
#include <Compilers.h>
#include <CompilerTasking.h>
#include <stddef.h>
#include "stdio.h"
#include "LQ_GPIO.h"
#include "string.h"
#include "LQ_UART.h"
#include "Motor.h"
#include "stdbool.h"
#define ASC_TX_BUFFER_SIZE 64        //发送缓冲区长度
#define ASC_RX_BUFFER_SIZE 64        //接收缓冲区长度
#include "LQ_CCU6.h"
#include "fashion_star_uart_servo.h"
#include "cheju.h"
//串口通信结构体

IfxAsclin_Asc g_UartConfig[4];

static uint8 s_AscTxBuffer[4][ASC_TX_BUFFER_SIZE + sizeof(Ifx_Fifo) + 8];
static uint8 s_AscRxBuffer[4][ASC_TX_BUFFER_SIZE + sizeof(Ifx_Fifo) + 8];

/* UART中断 */
IFX_INTERRUPT(UART0_RX_IRQHandler, UART0_VECTABNUM, UART0_RX_PRIORITY);
IFX_INTERRUPT(UART1_RX_IRQHandler, UART1_VECTABNUM, UART1_RX_PRIORITY);
IFX_INTERRUPT(UART2_RX_IRQHandler, UART2_VECTABNUM, UART2_RX_PRIORITY);
IFX_INTERRUPT(UART3_RX_IRQHandler, UART3_VECTABNUM, UART3_RX_PRIORITY);
IFX_INTERRUPT(UART0_TX_IRQHandler, UART0_VECTABNUM, UART0_TX_PRIORITY);
IFX_INTERRUPT(UART1_TX_IRQHandler, UART1_VECTABNUM, UART1_TX_PRIORITY);
IFX_INTERRUPT(UART2_TX_IRQHandler, UART2_VECTABNUM, UART2_TX_PRIORITY);
IFX_INTERRUPT(UART3_TX_IRQHandler, UART3_VECTABNUM, UART3_TX_PRIORITY);
IFX_INTERRUPT(UART0_ER_IRQHandler, UART0_VECTABNUM, UART0_ER_PRIORITY);
IFX_INTERRUPT(UART1_ER_IRQHandler, UART1_VECTABNUM, UART1_ER_PRIORITY);
IFX_INTERRUPT(UART2_ER_IRQHandler, UART2_VECTABNUM, UART2_ER_PRIORITY);
IFX_INTERRUPT(UART3_ER_IRQHandler, UART3_VECTABNUM, UART3_ER_PRIORITY);


/** UART中断CPU标号 */
const uint8 UartIrqVectabNum[4] = {UART0_VECTABNUM, UART1_VECTABNUM, UART2_VECTABNUM, UART3_VECTABNUM};

/** UART中断优先级 */
const uint8 UartIrqPriority[12] = {UART0_RX_PRIORITY, UART0_TX_PRIORITY, UART0_ER_PRIORITY, UART1_RX_PRIORITY, UART1_TX_PRIORITY, UART1_ER_PRIORITY,
                                   UART2_RX_PRIORITY, UART2_TX_PRIORITY, UART2_ER_PRIORITY, UART3_RX_PRIORITY, UART3_TX_PRIORITY, UART3_ER_PRIORITY};

/** UART中断服务函数地址 */
const void *UartIrqFuncPointer[12] = {&UART0_RX_IRQHandler, &UART0_TX_IRQHandler, &UART0_ER_IRQHandler,
                                       &UART1_RX_IRQHandler, &UART1_TX_IRQHandler, &UART1_ER_IRQHandler,
                                       &UART2_RX_IRQHandler, &UART2_TX_IRQHandler, &UART2_ER_IRQHandler,
                                       &UART3_RX_IRQHandler, &UART3_TX_IRQHandler, &UART3_ER_IRQHandler,};

/*************************************************************************
*  函数名称：void UART0_RX_IRQHandler(void)
*  功能说明：UART0_RX_IRQHandler中断服务函数
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月30日
*  备    注：
*************************************************************************/


float Roll_1 = 0.0f, Pitch_1 = 0.0f , Yaw_1 = 0.0f;

// UART0 -- camera

// volatile u8 speed_motified_flag = 0;、
#define K_p_lateral     1.2f
#define K_p_yaw         0.5f

// 所有的状态
static uint8_t rx_state = 0; // 公共 state
    // 循迹
// 误差可视化
volatile int speed_error_from_camera = 0;

static uint8_t rx_count = 0;


static uint8_t temp_buffer[4];

static float   current_deviation = 0.0f;
    // task1
static uint8_t rx_count1 = 0;
static uint8_t temp_buffer1[6];

volatile SERVO_ACTION_ENUM pick_or_put;
volatile bool start_to_pick_or_put = false;
volatile bool does_dummy_run = false;
volatile task1_cylinder_enum task1_cy_id;
volatile u8 pick_times = 0;
volatile u8 put_times = 0;

void UART0_RX_IRQHandler(void)
{
    IfxAsclin_Asc_isrReceive(&g_UartConfig[0]);
    /* 用户代码 */
    uint8_t one_byte;
    Ifx_SizeT count;

    while (UART_GetCount(UART0) > 0)
    {
        count = 1;
        IfxAsclin_Asc_read(&g_UartConfig[0], &one_byte, &count, TIME_INFINITE);

        switch (rx_state)
        {
            case 0: // 寻找帧头1: 0xAA
                if (one_byte == 0xAA) rx_state = 1;
                break;
            case 1: // 寻找帧头2: 根据这个进行切换模式
                rx_count = 0;
                rx_count1 = 0;
                if      (one_byte == 0x01)  rx_state = 2;
                else if (one_byte == 0x02)  rx_state = 4;
                break;
// 巡线逻辑
            case 2: // 接收 4 字节数据
                temp_buffer[rx_count++] = one_byte;
                if (rx_count >= 4) rx_state = 3;
                break;

            case 3: // 验证帧尾: 0x0D
                if (one_byte == 0x0D)
                {
                    // 只有帧头、数据长度、帧尾全部匹配，才解析
                    speed_error_from_camera = (int32_t)((temp_buffer[0]) |
                            (temp_buffer[1] << 8) |
                            (temp_buffer[2] << 16) |
                            (temp_buffer[3] << 24));
                    //printf("hello %d\r\n",speed_error_from_camera);
                    current_deviation = (float)speed_error_from_camera / 10000.0f;
                    current_deviation *= 0.93;
                    // 取绝对值用于计算减速比
                    float abs_dev = (current_deviation > 0) ? current_deviation : -current_deviation;
                    // 在直道 (abs_dev < 5) 时，速度接近 22
                    // 在急弯 (abs_dev > 20) 时，速度大幅下降到 5 左右，甚至更低
                    float v_x = 24.0f - (abs_dev * 0.455f);
                    if (v_x < 15.0f) v_x = 15.0f; // 确保在极弯时还有一点微弱的前进动力

                    // 当偏差很大时，我们需要爆发性的转向力
                    // 使用 1.2 这个系数，当偏差为 30 时，W = 36，远超前进速度 3，实现原地自旋
                    float v_y = current_deviation * 0.115f;   // 减小平移比重，防止侧滑
                    float w = 0;
                    if (abs_dev < 8.0f) {
                        // 直道区间：低增益，保证平稳
                        w = current_deviation * 0.45f;
                    } else {
                        // 弯道区间：高增益，确保能转过来
                        // 使用 0.95 基础 + 差值补偿
                        w = current_deviation * 0.835f;
                    }
                    // 4. O型麦轮解算 (20x30 底盘标准公式)
                    following_speed[0] = v_x - v_y - w; // 左前
                    following_speed[1] = v_x + v_y + w; // 右前
                    following_speed[2] = v_x + v_y - w; // 左后
                    following_speed[3] = v_x - v_y + w; // 右后
                    rx_state = 0;
                    // 无论帧尾是否正确，都回到状态0重新寻找
                    rx_state = 0;
                }
                break;

// task1 抓取圆柱逻辑
            case 4:
                temp_buffer1[rx_count1++] = one_byte;
                if(rx_count1 >= 6) rx_state = 5;
                break;
            case 5:
                if(one_byte == 0x0D)
                {
                    if(pick_or_put != servo_from_camera_idle){
                        rx_state = 0;
                        return;
                    }
                    int16_t x_dev = (int16_t)((temp_buffer1[0]) | (temp_buffer1[1] << 8));
                    int16_t y_dev = (int16_t)((temp_buffer1[2]) | (temp_buffer1[3] << 8));

                    pick_or_put = (SERVO_ACTION_ENUM)temp_buffer1[4]; // typedef enum{pick = 1, put = 2};
                    task1_cy_id = (task1_cylinder_enum)temp_buffer1[5];
                    if(pick_or_put == servo_pick_up_from_camera){
                        does_dummy_run = true;
                        pick_times++;
                        following_speed[0] = 0;
                        following_speed[1] = 0;
                        following_speed[2] = 0;
                        following_speed[3] = 0;
                        task1_start_yaw_correction = false;
                    }
                    else if(pick_or_put == servo_put_down_from_camera){
                        does_dummy_run = true;
                        put_times++;
                        following_speed[0] = 0;
                        following_speed[1] = 0;
                        following_speed[2] = 0;
                        following_speed[3] = 0;
                        task1_start_yaw_correction = false;
                    }
                    else {
                        // 这里本来是位置环结果，但是因为帧率低，而放弃
                        // x_dev_f = cur_pos - target_pos
                        // y_dev_f = cur_suq - target_suq  < 0 -> right; >0 -> left;
                        // < 0 -> right
                        float x_dev_f = x_dev * 0.2; // 简单的p项
                        float y_dev_f = y_dev * 0.2;

                        if(x_dev_f >0){
                            x_dev_f = 11;
                        }
                        else {
                            x_dev_f = -11;
                        }

                        if(y_dev_f > 0){
                            y_dev_f = 11;
                        }
                        else{
                            y_dev_f = -11;
                        }
                        float temp_speed[4] = {0};
                        // 只有当机械臂抓到了然后把标志位换回来外 加上 下一个动作和上一个动作不一样时才再次启动摇车

                        // x 轴 与 y轴的 速度叠加
                        // 先判断x，再判断y
                        //  -------------
                        //  | c ->  t   | ->  x_dev > 0
                        //
                        //
                        //  {-10,10,10,-10};
                        //  >0 -> left;
                        // 速度叠加
                        temp_speed[0] = x_dev_f - y_dev_f;
                        temp_speed[1] = x_dev_f + y_dev_f;
                        temp_speed[2] = x_dev_f + y_dev_f;
                        temp_speed[3] = x_dev_f - y_dev_f;
                        for(u8 i =0; i < 4; i++ ){
                            if(temp_speed[i] > 15){
                                temp_speed[i] = 15;
                            }
                            else if(temp_speed[i] < -15){
                                temp_speed[i] = -15;
                            }
                        }
                        following_speed[0] = temp_speed[0];
                        following_speed[1] = temp_speed[1];
                        following_speed[2] = temp_speed[2];
                        following_speed[3] = temp_speed[3];
                    }
                }
                rx_state = 0;
                break;
            default:
                rx_state = 0;
                break;
        }
    }
}
//我


void UART0_TX_IRQHandler(void)
{
    IfxAsclin_Asc_isrTransmit(&g_UartConfig[0]);

    /* 用户代码 */
}

void UART0_ER_IRQHandler(void)
{
    IfxAsclin_Asc_isrError(&g_UartConfig[0]);

    /* 用户代码 */

}


typedef enum{
    servo_rx_idle = 0,
    servo_id_switch_increase = 1,
    servo_id_switch_decrease = 2,
    servo_angle_increase = 3,
    servo_angle_decrease = 4,
    servo_angle_check = 5,
    servo_angle_reedit = 6,
    task_pick_control = 7,
    task_put_control = 8,
    task_id_switch = 9,
    task_angle_angle = 10,
}bt_rx_enum;

volatile uint8_t BT_flag = 0;
volatile uint8_t BT_flag_set = 0;
volatile uint8_t servo_id = 0;
volatile float servo_angle[4] = {0.0f, -75.0f, 27.0f, -35.0f};//  0
//  30 25.0f}


// 35
// float angle1 = -90
// float angle2 = -180
// float angle3 = -30
// float angle4 = -75 // -135
// DEFALUT ANGLE
void UART1_RX_IRQHandler(void)
{
    IfxAsclin_Asc_isrReceive(&g_UartConfig[1]);

    /* 用户代码 */
    uint8_t one_byte;
    static  Ifx_SizeT count = 1;
    IfxAsclin_Asc_read(&g_UartConfig[1], &one_byte, &count, TIME_INFINITE);
    BT_flag = one_byte - '0';
    bt_rx_enum bt_flag = (bt_rx_enum)BT_flag;
    static char temp_str_buffer[30] = {0};
    switch(bt_flag){
        case servo_rx_idle:
            break;
        case servo_id_switch_increase:
            servo_id += 1;
            servo_id %= 4;
            UART_PutStr(UART1,"Servo index inc...\r\n");
            break;
        case servo_id_switch_decrease:
            servo_id -= 1;
            if(servo_id <= 0)
                servo_id =0;
            UART_PutStr(UART1,"Servo index dec...\r\n");
            break;
        case servo_angle_increase:
            servo_angle[servo_id]+=1;
            BT_flag_set = 1;
            UART_PutStr(UART1,"Servo angle inc...\r\n");
            break;
        case servo_angle_decrease:
            servo_angle[servo_id]-=1;
            BT_flag_set = 1;
            UART_PutStr(UART1,"Servo angle dec...\r\n");
            break;
        case servo_angle_check:{
            char temp_str_buffer[40] = {0};
            sprintf(temp_str_buffer,"Servo angle:{%d.0f, %d.0f, %d.0f, %d.0f};\r\n",(int)servo_angle[0],(int)servo_angle[1],(int)servo_angle[2],(int)servo_angle[3]);
            UART_PutStr(UART1,temp_str_buffer);
            break;
        }
        case servo_angle_reedit:{
            volatile static u8 index = 0;
            if(index == 0){
                float task1_idle_angle_in_idle[4] = {-90.0f,-180.0f,-30.0f,-75.0f};
                for(u8 i = 0; i < 4; i++){
                    servo_angle[i] = task1_idle_angle_in_idle[i];
                }
            }
            else if(index == 1){// :-84 157 37 -62
                float task1_pick_up_angle_in_bt[4] = {-90, 157,37,-62};
                for(u8 i = 0; i < 4; i++){
                    servo_angle[i] = task1_pick_up_angle_in_bt[i];
                }
            }
            else if(index == 2){
                float task1_turning_angle1_in_bt[4] = {90.0f,  -126.0f, -20.0f, -60.0f};
                for(u8 i = 0; i < 4; i++){
                    servo_angle[i] = task1_turning_angle1_in_bt[i];
                }
            }
            else if(index == 3){
                float task1_put_angle_in_bt[4] = {90.0f, 131.0f, 19.0f, -60.0f};
                for(u8 i = 0; i < 4; i++){
                    servo_angle[i] = task1_put_angle_in_bt[i];
                }
            }
            index++;
            index%=3;
            BT_flag_set = 2;
            break;
        }
        case task_pick_control:{
            task1_start_yaw_correction = false;
            is_task1_wheels_moving_to_next_point = true;
            pick_times++;
            break;
        }
        case task_put_control:{
            task1_start_yaw_correction = false;
            is_task1_wheels_moving_to_last_point = true;
            put_times++;
            break;
        }
        case task_id_switch:{
            task1_cy_id ++;
            task1_cy_id %=4;

            sprintf(temp_str_buffer,"task cy id %d %d %d \r\n",task1_cy_id,pick_times,put_times);
            UART_PutStr(UART1,temp_str_buffer);
            break;
        }
        case task_angle_angle:{
            static ServoData temp_servo[1] = {0};
            static float temp_angles[4] = {0};
            for(u8 i =1; i <= 4; i++){
                FSUS_ServoMonitor(servo_usart,i,temp_servo);
                temp_angles[i] = temp_servo[0].angle;
            }
            sprintf(temp_str_buffer,"angles: %0.2f %0.2f %0.2f %0.2f \r\n", temp_angles[0], temp_angles[1], temp_angles[2], temp_angles[3]);
            ;
            break;
        }
        default:
            break;
    }
}

void UART1_TX_IRQHandler(void)
{
    IfxAsclin_Asc_isrTransmit(&g_UartConfig[1]);

    /* 用户代码 */
}

void UART1_ER_IRQHandler(void)
{
    IfxAsclin_Asc_isrError(&g_UartConfig[1]);

    /* 用户代码 */
}


uint16_t point1 ;
LidarPointTypedef Pack_Data[12];/* 雷达接收的数据储存在这个变量之中 */
LidarPointTypedef Pack_sum;     /* 输出结果储存 */
uint16_t receive_cnt;
uint8_t confidence;
uint16_t distance,noise,reftof;
uint32_t peak,intg;


void data_process(void)/*数据处理函数，完成一帧之后可进行数据处理*/
{
        /* 计算距离 */
        u8 i;
        static u16 count = 0;
        LidarPointTypedef Pack_sum;
        for(i=0;i<12;i++)                                   /* 12个点取平均 */
        {
                if(Pack_Data[i].distance != 0)  /* 去除0的点 */
                {
                        count++;
                        Pack_sum.distance += Pack_Data[i].distance;
                        Pack_sum.noise += Pack_Data[i].noise;
                        Pack_sum.peak += Pack_Data[i].peak;
                        Pack_sum.confidence += Pack_Data[i].confidence;
                        Pack_sum.intg += Pack_Data[i].intg;
                        Pack_sum.reftof += Pack_Data[i].reftof;
                }
        }
        if(count !=0)
        {

                    distance = Pack_sum.distance/count;
                    noise = Pack_sum.noise/count;
                    peak = Pack_sum.peak/count;
                    confidence = Pack_sum.confidence/count;
                    intg = Pack_sum.intg/count;
                    reftof = Pack_sum.reftof/count;
                    Pack_sum.distance = 0;
                    Pack_sum.noise = 0;
                    Pack_sum.peak = 0;
                    Pack_sum.confidence = 0;
                    Pack_sum.intg = 0;
                    Pack_sum.reftof = 0;
                    count = 0;
        }
}


void UART2_RX_IRQHandler(void)
{
    IfxAsclin_Asc_isrReceive(&g_UartConfig[2]);

    uint8_t ucTemp = 0;
    Ifx_SizeT count = 1;
    IfxAsclin_Asc_read(&g_UartConfig[2], &ucTemp, &count, TIME_INFINITE);

    if (count > 0)
    {
        RingBuffer_Push(FSUS_Usart.recvBuf, ucTemp);
    }
}
void UART2_TX_IRQHandler(void)
{
    IfxAsclin_Asc_isrTransmit(&g_UartConfig[2]);

    /* 用户代码 */



}

void UART2_ER_IRQHandler(void)
{
    IfxAsclin_Asc_isrError(&g_UartConfig[2]);

    /* 用户代码 */
}

void UART3_RX_IRQHandler(void)
{
    IfxAsclin_Asc_isrReceive(&g_UartConfig[3]);

    /* 用户代码 */
    uint8_t one_byte;
    Ifx_SizeT count;
    while (UART_GetCount(UART3) > 0)
    {
        count = 1;
        IfxAsclin_Asc_read(&g_UartConfig[3], &one_byte, &count, TIME_INFINITE);
        Radar_Feed_Byte(one_byte);
    }
}

void UART3_TX_IRQHandler(void)
{
    IfxAsclin_Asc_isrTransmit(&g_UartConfig[3]);

    /* 用户代码 */
    uint8_t ucTemp = 0;
    Ifx_SizeT count = 1;
    IfxAsclin_Asc_read(&g_UartConfig[3], &ucTemp, &count, TIME_INFINITE);
    RingBuffer_Push(FSUS_Usart.recvBuf, ucTemp);// Put the received data into the buffer, no specific data processing in the interrupt

}

void UART3_ER_IRQHandler(void)
{
    IfxAsclin_Asc_isrError(&g_UartConfig[3]);

    /* 用户代码 */
}

/*************************************************************************
*  函数名称：void UART_InitConfig(UART_RX_t RxPin, UART_TX_t TxPin, unsigned long baudrate)
*  功能说明：串口模块初始化
*  参数说明：
  * @param    RxPin   ： 串口接收管脚
  * @param    TxPin   ： 串口发送管脚
  * @param    baudrate： 波特率
*  函数返回：字节
*  修改时间：2020年3月10日
*  备    注：UART_InitConfig(UART0_RX_P14_1,UART0_TX_P14_0, 115200);   //初始化串口0 波特率 115200 无奇偶校验 1停止位 使用管脚P14_0 P14_1
*************************************************************************/
void UART_InitConfig(UART_RX_t RxPin, UART_TX_t TxPin, unsigned long baudrate)
{
    int i,j;
        //关闭CPU中断
        IfxCpu_disableInterrupts();

        Ifx_P *portRx = PIN_GetModule(RxPin);
        uint8 pinIndexRx = PIN_GetIndex(RxPin);

        Ifx_P *portTx = PIN_GetModule(TxPin);
        uint8 pinIndexTx = PIN_GetIndex(TxPin);

        IfxAsclin_Rx_In  * IfxAsclin_Rx = NULL_PTR;
        IfxAsclin_Tx_Out * IfxAsclin_Tx = NULL_PTR;

        for( i = 0; i < IFXASCLIN_PINMAP_NUM_MODULES; i++)
        {
            for( j = 0; j < IFXASCLIN_PINMAP_RX_IN_NUM_ITEMS; j++)
            {
                if(IfxAsclin_Rx_In_pinTable[i][j] == NULL_PTR)
                {

                }
                else if((unsigned long)portRx == (unsigned long)(IfxAsclin_Rx_In_pinTable[i][j]->pin.port) && pinIndexRx == IfxAsclin_Rx_In_pinTable[i][j]->pin.pinIndex)
                {
                    IfxAsclin_Rx = IfxAsclin_Rx_In_pinTable[i][j];
                }
            }
            for(j = 0; j < IFXASCLIN_PINMAP_TX_OUT_NUM_ITEMS; j++)
            {
                if(IfxAsclin_Tx_Out_pinTable[i][j] == NULL_PTR)
                {

                }
                else if((unsigned long)portTx == (unsigned long)(IfxAsclin_Tx_Out_pinTable[i][j]->pin.port) && pinIndexTx == IfxAsclin_Tx_Out_pinTable[i][j]->pin.pinIndex)
                {
                    IfxAsclin_Tx = IfxAsclin_Tx_Out_pinTable[i][j];
                }
            }
        }
        if(IfxAsclin_Rx->module != IfxAsclin_Tx->module)
        {
    #pragma warning 557         // 屏蔽警告
            while (1);          // 检查 RX和TX 是否为同一UART
    #pragma warning default     // 打开警告
        }

        //新建串口配置结构体
        IfxAsclin_Asc_Config ascConfig;

        //初始化模块
        IfxAsclin_Asc_initModuleConfig(&ascConfig, IfxAsclin_Tx->module);
        if(RxPin == UART3_RX_P00_1)
        {
            /* Default values for baudrate */
            ascConfig.clockSource           = IfxAsclin_ClockSource_kernelClock;         /* kernel clock, fclc*/
            ascConfig.baudrate.prescaler    = 4;                                         /* default prescaler*/
            ascConfig.baudrate.baudrate     = 100000;                                    /* default baudrate (the fractional dividier setup will be calculated in initModule*/
            ascConfig.baudrate.oversampling = IfxAsclin_OversamplingFactor_8;            /* default oversampling factor*/

            /* Default Values for Bit Timings */
            ascConfig.bitTiming.medianFilter        = IfxAsclin_SamplesPerBit_three;       /* 每位一次采样*/
            ascConfig.bitTiming.samplePointPosition = IfxAsclin_SamplePointPosition_3;   /* 采样点位置为3*/
            /* Default Values for Frame Control */
            ascConfig.frame.idleDelay               = IfxAsclin_IdleDelay_0;             /* no idle delay*/
            ascConfig.frame.stopBit                 = IfxAsclin_StopBit_2;               /* one stop bit*/
            ascConfig.frame.frameMode               = IfxAsclin_FrameMode_asc;           /* ASC mode*/
            ascConfig.frame.shiftDir                = IfxAsclin_ShiftDirection_lsbFirst; /* shift diection LSB first*/
            ascConfig.frame.parityBit               = TRUE;                             /* disable parity*/
            ascConfig.frame.parityType              = IfxAsclin_ParityType_even;         /* even parity (if parity enabled)*/
            ascConfig.frame.dataLength              = IfxAsclin_DataLength_8;            /* number of bits per transfer 8*/

            /* Default Values for Fifo Control */
            ascConfig.fifo.inWidth              = IfxAsclin_TxFifoInletWidth_1;          /* 8-bit wide write*/
            ascConfig.fifo.outWidth             = IfxAsclin_RxFifoOutletWidth_1;         /* 8-bit wide read*/
            ascConfig.fifo.txFifoInterruptLevel = IfxAsclin_TxFifoInterruptLevel_0;      /* txFifoInterruptLevel = 0. optimised to write upto 16 bytes at a time */
            ascConfig.fifo.rxFifoInterruptLevel = IfxAsclin_RxFifoInterruptLevel_1;
            ascConfig.fifo.buffMode             = IfxAsclin_ReceiveBufferMode_rxFifo;    /* RxFIFO*/

            /* Default Values for Interrupt ascConfig */
            ascConfig.interrupt.rxPriority = UartIrqPriority[IfxAsclin_getIndex(IfxAsclin_Tx->module) * 3];
            ascConfig.interrupt.txPriority = UartIrqPriority[IfxAsclin_getIndex(IfxAsclin_Tx->module) * 3 + 1];
            ascConfig.interrupt.erPriority = UartIrqPriority[IfxAsclin_getIndex(IfxAsclin_Tx->module) * 3 + 2];
            ascConfig.interrupt.typeOfService = UartIrqVectabNum[IfxAsclin_getIndex(IfxAsclin_Tx->module)];

            /* Enable error flags */
            ascConfig.errorFlags.ALL = ~0;                                               /* all error flags enabled*/
            IfxAsclin_Asc_Pins pins =
            {
                NULL,                     IfxPort_InputMode_pullUp,        /* CTS pin not used */
                IfxAsclin_Rx,             IfxPort_InputMode_pullUp,        /* Rx pin */
                NULL,                     IfxPort_OutputMode_pushPull,     /* RTS pin not used */
                IfxAsclin_Tx,             IfxPort_OutputMode_pushPull,     /* Tx pin */
                IfxPort_PadDriver_cmosAutomotiveSpeed1
            };
            /* init pointers */
            ascConfig.pins        = &pins;                                         /* pins to null pointer*/
            ascConfig.txBuffer     = &s_AscTxBuffer[IfxAsclin_getIndex(IfxAsclin_Tx->module)][0];
            ascConfig.txBufferSize = ASC_TX_BUFFER_SIZE;
            ascConfig.rxBuffer     = &s_AscRxBuffer[IfxAsclin_getIndex(IfxAsclin_Tx->module)][0];
            ascConfig.rxBufferSize = ASC_RX_BUFFER_SIZE;

            IfxAsclin_Asc_initModule(&g_UartConfig[IfxAsclin_getIndex(IfxAsclin_Tx->module)], &ascConfig);
            IfxAsclin_Asc_initModuleConfig(&ascConfig, IfxAsclin_Tx->module);

            IfxCpu_Irq_installInterruptHandler((void*)UartIrqFuncPointer[IfxAsclin_getIndex(IfxAsclin_Tx->module) * 3],     UartIrqPriority[IfxAsclin_getIndex(IfxAsclin_Tx->module) * 3]);
            IfxCpu_Irq_installInterruptHandler((void*)UartIrqFuncPointer[IfxAsclin_getIndex(IfxAsclin_Tx->module) * 3 + 1], UartIrqPriority[IfxAsclin_getIndex(IfxAsclin_Tx->module) * 3 + 1]);
            IfxCpu_Irq_installInterruptHandler((void*)UartIrqFuncPointer[IfxAsclin_getIndex(IfxAsclin_Tx->module) * 3 + 2], UartIrqPriority[IfxAsclin_getIndex(IfxAsclin_Tx->module) * 3 + 2]);
        }
        else
        {
            ascConfig.baudrate.baudrate  = (float32)baudrate;        //波特率
            ascConfig.baudrate.prescaler    = 4;//数据帧模式
            ascConfig.baudrate.oversampling   = IfxAsclin_OversamplingFactor_8; //数据长度

            unsigned char uartNum = IfxAsclin_getIndex(IfxAsclin_Tx->module);

            //中断优先级配置
            ascConfig.interrupt.rxPriority = UartIrqPriority[uartNum * 3];
            ascConfig.interrupt.txPriority = UartIrqPriority[uartNum * 3 + 1];
            ascConfig.interrupt.erPriority = UartIrqPriority[uartNum * 3 + 2];
            ascConfig.interrupt.typeOfService = UartIrqVectabNum[uartNum];

            //接收和发送FIFO的配置
            ascConfig.txBuffer     = &s_AscTxBuffer[uartNum][0];
            ascConfig.txBufferSize = ASC_TX_BUFFER_SIZE;
            ascConfig.rxBuffer     = &s_AscRxBuffer[uartNum][0];
            ascConfig.rxBufferSize = ASC_RX_BUFFER_SIZE;

            IfxAsclin_Asc_Pins pins =
            {
                NULL,                     IfxPort_InputMode_pullUp,        /* CTS pin not used */
                IfxAsclin_Rx,             IfxPort_InputMode_pullUp,        /* Rx pin */
                NULL,                     IfxPort_OutputMode_pushPull,     /* RTS pin not used */
                IfxAsclin_Tx,             IfxPort_OutputMode_pushPull,     /* Tx pin */
                IfxPort_PadDriver_cmosAutomotiveSpeed1
            };
            ascConfig.pins = &pins;

            //调用上面结构体所预设的参数，完成模块的初始化
            IfxAsclin_Asc_initModule(&g_UartConfig[uartNum], &ascConfig);

            //接收，发送和错误中断配置
            IfxCpu_Irq_installInterruptHandler((void*)UartIrqFuncPointer[uartNum * 3],     UartIrqPriority[uartNum * 3]);
            IfxCpu_Irq_installInterruptHandler((void*)UartIrqFuncPointer[uartNum * 3 + 1], UartIrqPriority[uartNum * 3 + 1]);
            IfxCpu_Irq_installInterruptHandler((void*)UartIrqFuncPointer[uartNum * 3 + 2], UartIrqPriority[uartNum * 3 + 2]);
        }
        //开启CPU中断
        IfxCpu_enableInterrupts();
}

/*************************************************************************
*  函数名称：void UART_PutChar(UART_t  uratn, char ch)
*  功能说明：UART发送字节函数,使用前请先初始化对应串口
*  参数说明：uratn ： UART0 - UART3/ ch    ： 要打印的字符
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：UART_PutChar(UART0, 'a');  //打印字符a
*************************************************************************/
void UART_PutChar(UART_t  uratn, char ch)
{
    IfxAsclin_Asc_blockingWrite(&g_UartConfig[uratn], ch);
}
//IfxAsclin_Asc_blockingWrite(usart->ascHandle, value);
/*************************************************************************
*  函数名称：void UART_PutStr(UART_t  uratn, char *str)
*  功能说明：UART发送字符串函数(遇 NULL 停止发送),使用前请先初始化对应串口
*  参数说明：uratn ： UART0 - UART3/ str   ： 要打印的字符串地址
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：UART_PutStr(UART3, "123456789"); //发送9个字节
*************************************************************************/
void UART_PutStr(UART_t  uratn, char *str)
{
   while(*str)
    {
        UART_PutChar(uratn, *str++);
    }
}

/*************************************************************************
*  函数名称：void UART_PutBuff(UART_t  uratn, unsigned char *buff, unsigned long len)
*  功能说明：UART发送字节函数,使用前请先初始化对应串口
*  参数说明：
* @param    uratn ： UART0 - UART3
* @param    buff  ： 要打印的字符串地址
* @param    len   ： 要打印的长度
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：UART_PutBuff(UART4, "123456789",5);//实际发送5个字节‘1’‘2’‘3’‘4’‘5’
*************************************************************************/
void UART_PutBuff(UART_t  uratn, unsigned char *buff, unsigned long len)
{
    while(len--)
    {
        UART_PutChar(uratn, *buff);
        buff++;
    }
}

/*************************************************************************
*  函数名称：Ifx_SizeT UART_GetCount(UART_t  uratn)
*  功能说明：获取 接收缓冲区 存放当前接收数据个数
*  参数说明： uratn ： UART0 - UART3
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：
*************************************************************************/
uint8 UART_GetCount(UART_t  uratn)
{
    return (uint8)IfxAsclin_Asc_getReadCount(&g_UartConfig[uratn]);
}

/*************************************************************************
*  函数名称：char UART_GetChar(UART_t  uratn)
*  功能说明：UART读取字节 使用前请先初始化对应串口 调用前请确认有接收到数据 否则会等待数据到来
*  参数说明：uratn ： UART0 - UART3
*  函数返回：读取字节
*  修改时间：2020年3月10日
*  备    注：UART_GetChar(UART3); //接收一个字符
*************************************************************************/
char UART_GetChar(UART_t  uratn)
{
    uint8 data=0;
    Ifx_SizeT count = 1;

    /* 获取接收到的一个字节数据 注意如果没有接收到，会一直等待 */
    IfxAsclin_Asc_read(&g_UartConfig[uratn], &data, &count, TIME_INFINITE);

    return  data;
}

/*************************************************************************
*  函数名称：char UART_GetBuff(UART_t  uratn, unsigned char *data, unsigned char len)
*  功能说明：接收一定长度字符 使用前请先初始化对应串口 调用前请确认有接收到数据 否则会接受失败
*  参数说明：uratn ： UART0 - UART3
*  函数返回：0:读取正常  其他 读取失败
*  修改时间：2020年3月10日
*  备    注：ART_GetChar(UART0, data, 10); //接收10个字符
*************************************************************************/
char UART_GetBuff(UART_t  uratn, unsigned char *data, unsigned char len)
{
    Ifx_SizeT count = len;

    if(UART_GetCount(uratn) < len)
    {
        return 1;  //判断当前接收到的数据数量 不够读取长度
    }

    /* 获取接收到数据  */
    IfxAsclin_Asc_read(&g_UartConfig[uratn], data, &count, TIME_INFINITE);
    return  0;
}



/*******************************************************************************
* Function Name  : _write
* Description    : Support Printf Function
* Input          : *buf: UART send Data.
*                  size: Data length
* Return         : size: Data length
*******************************************************************************/
int _write(int fd, char *buf, int size)
{
  int i;

  for(i=0; i<size; i++)
  {
    UART_PutChar(UART0, *buf++);
  }
  return size;
}

//int _write(int fd, char *buf, int size)
//{
//  int i;
//
//  for(i=0; i<size; i++)
//  {
//#if (DEBUG == DEBUG_UART1)
//    while (USART_GetFlagStatus(UART1, USART_FLAG_TC) == RESET);
//    UART_PutChar(UART1, *buf++);
//#elif (DEBUG == DEBUG_UART2)
//    while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
//    UART_PutChar(USART2, *buf++);
//#elif (DEBUG == DEBUG_UART3)
//    while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
//    UART_PutChar(USART3, *buf++);
//#endif
//  }
//
//  return size;
//}






/////////////////////////////////////////////////////////////////////////////////////
