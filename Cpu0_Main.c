/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技TC264DA核心板
【编    写】chiusir
【E-mail】chiusir@163.com
【软件版本】V1.2 版权所有，单位使用请先联系授权
【最后更新】2023年11月15日
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
=================================================================
程序配套视频地址：https://space.bilibili.com/95313236
=================================================================
摄像头接口                  龙邱神眼或者OV7725模块
● 数据端口：P02.0-P02.7口，共8位，接摄像头的数据端口；
● 时钟像素：外部中断第0组：P00_4；
● 场信号：外部中断第3组：P15_1；
-----------------------------------------------------------------
推荐GPT12模块，共可以实现5路正交解码增量编码器（兼容带方向编码器）信号采集，任意选择四路即可；
P33_7, P33_6   龙邱TC母板编码器1
P02_8, P33_5   龙邱TC母板编码器2 有问题
P10_3, P10_1   龙邱TC母板编码器3
P20_3, P20_0   龙邱TC母板编码器4
-----------------------------------------------------------------
电感电压采集模块或者麦克风模块
推荐使用AN0-7，共八路ADC，可以满足chirp声音信号及电磁车电感电压采集；
AN0-3          龙邱TC接四个麦克风模块或者电感
-----------------------------------------------------------------
默认电机接口
使用GTM模块，ATOM四个通道可产生4*8共32路PWM，而且各自频率和占空比可调，推荐使用ATOM0的0-7通道；
第一组双路全桥驱动
P23_1         龙邱TC母板MOTOR1_P
P32_4         龙邱TC母板MOTOR1_N
P21_2         龙邱TC母板MOTOR2_P
P22_3         龙邱TC母板MOTOR2_N
第二组双路全桥驱动
P21_4         龙邱TC母板MOTOR3_P
P21_3         龙邱TC母板MOTOR3_N
P20_8         龙邱TC母板MOTOR4_P
P21_5         龙邱TC母板MOTOR4_N
-----------------------------------------------------------------
默认舵机接口
使用GTM模块，ATOM四个通道可产生4*8共32路PWM，而且各自频率和占空比可调，推荐使用ATOM2；
P33_10        龙邱TC母板舵机1
P33_13        龙邱TC母板舵机2
-----------------------------------------------------------------
 默认屏幕显示接口，用户可以使用TFT或者OLED模块
TFTSPI_CS     P20_14     龙邱TC母板 CS管脚 默认拉低，可以不用
TFTSPI_SCK    P20_11     龙邱TC母板 SPI SCK管脚
TFTSPI_SDI    P20_10     龙邱TC母板 SPI MOSI管脚
TFTSPI_DC     P20_12     龙邱TC母板 D/C管脚
TFTSPI_RST    P20_13     龙邱TC母板 RESET管脚
-----------------------------------------------------------------
OLED_CK       P20_14     龙邱TC母板OLED CK管脚
OLED_DI       P20_11     龙邱TC母板OLED DI管脚
OLED_RST      P20_10     龙邱TC母板OLED RST管脚
OLED_DC       P20_12     龙邱TC母板OLED DC管脚
OLED_CS       P20_13     龙邱TC母板OLED CS管脚 默认拉低，可以不用
----------------------------------------------------------------
默认按键接口
KEY0p         P22_0      龙邱TC母板上按键0
KEY1p         P22_1      龙邱TC母板上按键1
KEY2p         P22_2      龙邱TC母板上按键2
DSW0p         P33_9      龙邱TC母板上拨码开关0
DSW1p         P33_11     龙邱TC母板上拨码开关1
-----------------------------------------------------------------
默认LED接口
LED0p         P10_6      龙邱TC母板核心板上LED0 翠绿
LED1p         P10_5      龙邱TC母板核心板上LED1 蓝灯
LED2p         P20_6      龙邱TC母板上LED0
LED3p         P20_7      龙邱TC母板上LED1
-----------------------------------------------------------------
默认蜂鸣器接口
BEEPp         P33_8      龙邱TC母板上蜂鸣器接口
-----------------------------------------------------------------
定时器
有两个CCU6模块  每个模块有两个独立定时器  触发定时器中断 定时器中断服务函数在LQ_CCU6.c文件中
推荐使用CCU6模块，STM用作系统时钟或者延时；


功能说明
编码器测试，编码器连接好之后，编译烧录程序，转动编码器，屏幕显示值，说明编码器正常。
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include <IfxCpu.h>
#include <IfxScuCcu.h>
#include <IfxScuWdt.h>
#include <IfxStm.h>
#include <IfxStm_reg.h>
#include <stdio.h>

#include "include.h"
App_Cpu0 g_AppCpu0;                       // brief CPU 0 global data
IfxCpu_mutexLock mutexCpu0InitIsOk = 1;   // CPU0 初始化完成标志位
volatile char mutexCpu0TFTIsOk=0;         // CPU1 0占用/1释放 TFT
#include "Motor.h"
#include "Main.h"
#include "motified_app.h"
//#include "user_uart.h"
#include "Servo.h"
#include "LQ_SOFTI2C.h"

#include "LQ_CCU6.h"
#include "cheju.h"
#include "IR_sensor.h"
/*************************************************************************
*  函数名称：int core0_main (void)
*  功能说明：CPU0主函数
*  参数说明：无
*  函数返回：无
*  修改时间：2023年11月15日
*  备    注：
*************************************************************************/




u32 encoder1 = 0;
u32 encoder2 = 0;
u32 encoder3 = 0;
u32 encoder4 = 0;
u32 encoder5 = 0;
void Encoder_get(void){
    encoder1 = MODULE_GPT120.T2.U;
    encoder2 = MODULE_GPT120.T3.U;
    encoder3 = MODULE_GPT120.T4.U;
    encoder4 = MODULE_GPT120.T5.U;
    encoder5 = MODULE_GPT120.T6.U;

}

volatile unsigned int last_encoder = 0;
int function_only_once = 0;
//IFX_INTERRUPT

#define TFT_2_0      0
#define TFT_1_8      1
#define TFT_VERSION  TFT_1_8
#define DEBUG_MODE   0

// 所有的中断函数
/*
 * CCU60_CH0_IRQHandler
 * CCU60_CH1_IRQHandler
 * CCU61_CH0_IRQHandler
 * CCU61_CH1_IRQHandler
 * */


int ECPULSE5 = 0;
int speed_examine[4] = {0,0,0,0};
int speed_value[4] = {0};
volatile float  exam_angles[5] = {0};
const char angle_hint_str[5][11] = {"servo idle","index add","index sub","angle add","angle sub"};

// float angle1 = -90
// float angle2 = -180
// float angle3 = -30
// float angle4 = -75 // -135
// DEFALUT ANGLE

int core0_main (void)
{

	// 关闭CPU总中断
	IfxCpu_disableInterrupts();

	// 关闭看门狗，如果不设置看门狗喂狗需要关闭
	IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
	IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());

	// 读取总线频率
	g_AppCpu0.info.pllFreq = IfxScuCcu_getPllFrequency();
	g_AppCpu0.info.cpuFreq = IfxScuCcu_getCpuFrequency(IfxCpu_getCoreIndex());
	g_AppCpu0.info.sysFreq = IfxScuCcu_getSpbFrequency();
	g_AppCpu0.info.stmFreq = IfxStm_getFrequency(&MODULE_STM0);

	// 按键初始化
	GPIO_KEY_Init();



	UART_InitConfig(UART0_RX_P14_1, UART0_TX_P14_0, 115200); // 和 jetson nano 上位机通信

    UART_InitConfig(UART1_RX_P02_3, UART1_TX_P02_2, 115200); // 和 蓝牙通信

    UART_InitConfig(UART2_RX_P02_1, UART2_TX_P02_0, 115200); // 和 servo bus 通信

    UART_InitConfig(UART3_RX_P00_1, UART3_TX_P00_0, 115200); // 和激光测距通信

    User_Uart_Init(&g_UartConfig[2]);
    // LED灯所用P10.6和P10.5初始化
	GPIO_LED_Init();
//	UART_PutStr(UART0, "1hello\r\n");
	// 开启CPU总中断
	IfxCpu_enableInterrupts();
//	UART_PutStr(UART0, "2hello\r\n");
	// 通知CPU1，CPU0初始化完成
	IfxCpu_releaseMutex(&mutexCpu0InitIsOk);
//	UART_PutStr(UART0, "3hello\r\n");
	// 切记CPU0,CPU1...不可以同时开启屏幕显示，否则冲突不显示
	mutexCpu0TFTIsOk = 1;         // CPU1： 0占用/1释放 TFT

	char txt[32];

#if TFT_VERSION
	TFTSPI_Init(1);        //LCD初始化  0:横屏  1：竖屏
	TFTSPI_CLS(u16BLACK);   //蓝色屏幕
#else
	UART_PutStr(UART0, "hello\r\n");
    TFT2SPI_Init(0);               // TFT2.0初始化  0：竖屏0度   1:横屏90度  2：倒立10度  3：270度

    UART_PutStr(UART0, "hello\r\n");
    TFT2SPI_CLS(u16BLACK);         // 清屏
    UART_PutStr(UART0, "hello\r\n");
    TFT2SPI_Show_Logo(0,37);       // 显示龙邱LOGO
    UART_PutStr(UART0, "hello\r\n");
    TFT2SPI_P16x16Str(0,0,(unsigned char*)"北京龙邱智能科技",u16RED,u16BLUE);// 字符串显示
#endif
    IIC_Init();
    delayms(2000);
    unsigned char res = Gyro_Chose();
    printf("%d\r\n",res);

    uint16_t  interval = 1000;
    uint16_t power = 0;
    FSUS_SetServoAngle(servo_usart, 1, 0.0f, interval, power);
    delayms(20);
    FSUS_SetServoAngle(servo_usart, 2, 126.50f, 2000, power);//-169
    delayms(20);
    FSUS_SetServoAngle(servo_usart, 3, -75.0f, interval, power);//-108
    delayms(20);
    FSUS_SetServoAngle(servo_usart, 4, 75.0f, interval, power);//76
    delayms(20);
    my_dummy.pump_state = PUMP_ON;
    air_pump_pick_up();
    delayms(500);
    my_dummy.pump_state = PUMP_OFF;
    air_pump_pick_up();


    delayms(300);
    MPU6050_Init();
    delayms(500);

    get_real_gyro_error2();
    //ICM20602_Init();
    // encoder init
    EncInit();
    Reed_Init();
    MotorInit();

    ir_sensor_init();
    //2 4 3 1
    position_init();

#if 0
    for(;;){
        if(KEY_Read(KEY0p) == 0){
            break;
        }
    }
    delayms(500);
    System_run = 1;
#endif
    encoder_clear();
    PID_init();
    LED_Ctrl(LEDALL, RVS);        //电平翻转,LED闪烁

    CCU6_InitConfig(CCU60,CCU6_Channel0,   5000);       // 5000us = 5ms //  速度环 位置环
    CCU6_InitConfig(CCU60,CCU6_Channel1, 100000);       // 100ms        //  led闪烁state system clock
    CCU6_InitConfig(CCU61,CCU6_Channel0,   1000);       // 1000us = 1ms //  icm 积分z
    task1_get_put_position_loop_init();
    task1_get_pick_position_loop_init();

    task1_all_position_loop_init();
    task2_all_position_loop_init();

    delayms(600);

    delayms(300);
    DUMMY_INIT();

    PIN_InitConfig(P15_1, IfxPort_Mode_inputNoPullDevice, 0);
    // P10_2 P11_10 P11_6 P11_3 P13_3 P13_2 P13_0 P15_1
    //TASK1_PICK_OBJECT_UP_SYN();

  //  delayms(5000);
    //TASK1_PICK_OBJECT_UP_SYN();
    //  0
    //  30

    // -58
    // 35
    //Servo angle:Servo angle:{0.0f, 125.0f,-75.0f, 75.0f,19.0f};

    //delayms(3000);
  //  TASK1_PICK_OBJECT_UP_SYN();
    // 是否开启 角度修正
    //task1_start_yaw_correction = true;
  //  interval = 2000;


    //FSUS_SetServoAngle(servo_usart, 2, -169.0f, interval, power);//-169

//////
//    delayms(2000);
    //TASK1_PUT_OBJECT();
    static volatile bool temp_flag1 = false;
    wheel_system_tick.does_tick_start = true;
    //task2_start_correct = true;
    following_flow_start = false;
    task1_y_correct_start1  = false;
    task1_start_yaw_correction = false; // yaw矫正还未开启

    uint8_t temp_ir_record[8] = {0};
    volatile bool look_at_worm = true; //task2_start_correct = true;= true;
    volatile bool send_task2_signal_once = true;
    volatile u8 did_i_sen = 0;
    static bool runs_only_once2 = true;
    does_task_work_flow_start = false;
//    is_task1_wheels_moving_to_next_point = true;
//    pick_times = 1;
//    task1_cy_id = task1_cylinder_id_small;
//    delayms(5000);
//    is_position_loop_done = false;
//    is_task1_wheels_moving_to_last_point = true;
//    put_times = 1;
//    task1_cy_id = task1_cylinder_id_small;

//    FSUS_SetServoAngle(servo_usart, 1, 0.0f, interval, power);
//    delayms(20);
//    FSUS_SetServoAngle(servo_usart, 2, 101.50f, 400, power);//-169
//    delayms(20);
//    FSUS_SetServoAngle(servo_usart, 3, -116.0f, 1000, power);//-108
//    delayms(20);
//    FSUS_SetServoAngle(servo_usart, 4, 10.0f, 1000, power);//76
//    delayms(20);

    ////////////////////////////////////////////// task2
//    TASK2_WATCH_DROP_WATER_NUMBER();
//    delayms(2000);
//    TASK2_DROP_COUNT = 2;
//    TASK2_WATCH_DROP_WATER();
//    delayms(1000);
//    does_task_work_flow_start = true;
//    TASK2_PICK_AND_PUT_DROP_WATER_WORKFLOW();

    while (1)	//主循环
    {
#if TFT_VERSION
//        get_ir_pins_state(1,temp_ir_record);
//        for(uint8_t i = 0; i< 8; i++){
//            printf("%d ",temp_ir_record[i]);
//        }
//        printf("\r\n");
        //

        // task2
        if(task2_current_state == TASK2_SECOND_CALIBRATION){
            temp_test_flag = 0;

            task2_start = false;
        }
        if(task2_prepare_correct == true){
            task2_prepare_correct = false;
            delayms(50);
            task2_start_correct = true;
        }
        if(temp_task2_to_next_point[2] == true){
            task2_start = false;
        }

        // task3
        if(look_at_worm == true && is_waiting_for_task_record == true){
            look_at_worm = false;
            did_i_sen = 1;
            UART_PutChar(UART0,task_start_signal_from_me);
        }
        if(temp_flag1 == false && following_flow_start == false){
            temp_flag1 =true;
        }
        if(does_task2_dummy_move == true && runs_only_once2 == true){
            runs_only_once2 = false;
            tast2_start_test_distance = true;
            TASK2_WATCH_DROP_WATER_NUMBER();
        }
        if(task4_prepare_correct == true){
            task4_prepare_correct = false;
            delayms(50);
            task4_start_correct = true;
            TASK4_WATCH_BALL();
        }
        TASK1_PICK_OBJECT_UP_SYN();
        TASK1_PUT_OBJECT_DOWN_SYN();

        TASK2_WATCH_DROP_WATER();
        TASK2_PICK_AND_PUT_DROP_WATER_WORKFLOW();
        // 显示 巡线速度
        sprintf(txt,"%d %d %d %d ",(int)following_speed[0],(int)following_speed[1], (int)following_speed[2],(int)following_speed[3]);
        TFTSPI_P8X16Str(1, 0, txt, u16WHITE, u16BLACK);

        // 确认串口是否接收是否被读取
        sprintf(txt,"t2s:%d t2st:%d    ", task2_start, task2_current_state);
        TFTSPI_P8X16Str(1, 1, txt, u16WHITE, u16BLACK);

        sprintf(txt, "enc_f:%d %d %d %d  ", motor_pid.encoder_value[0], motor_pid.encoder_value[1], motor_pid.encoder_value[2], motor_pid.encoder_value[3]);
        TFTSPI_P8X16Str(1, 2, txt, u16WHITE, u16BLACK);

        sprintf(txt, " %d %d  pwm   ",motor_pid.pwm_out[0], motor_pid.pwm_out[1]);
        TFTSPI_P8X16Str(1, 3, txt, u16WHITE, u16BLACK);
        sprintf(txt, " %d %d    ",motor_pid.pwm_out[2], motor_pid.pwm_out[3]);
        TFTSPI_P8X16Str(1, 4, txt, u16WHITE, u16BLACK);

        int temp_encoder[4] = {0};
        get_encoder_directly(temp_encoder);
        sprintf(txt, "enc_b %d %d %d %d     ", temp_encoder[0],temp_encoder[1],temp_encoder[2], temp_encoder[3]);
        TFTSPI_P8X16Str(1, 5, txt, u16WHITE, u16BLACK);
//        BT_flag

        if(line_record1 == true){
            line_record1 = false;
            //sprintf(txt, "%d\r\n",(int)crossing_line_only_total_path);
            // UART_PutStr(UART1,txt);
        }
        if(BT_flag_set){
            if(BT_flag_set == 1){
                FSUS_SetServoAngle(servo_usart, 1, servo_angle[0], interval, power);
                FSUS_SetServoAngle(servo_usart, 2, servo_angle[1], interval, power);
                FSUS_SetServoAngle(servo_usart, 3, servo_angle[2], interval, power);
                FSUS_SetServoAngle(servo_usart, 4, servo_angle[3], interval, power);
            }
            else if(BT_flag_set == 2){
                 FSUS_SetServoAngle(servo_usart, 1, servo_angle[0], 1500, power);
                 FSUS_SetServoAngle(servo_usart, 2, servo_angle[1], 1500, power);
                 FSUS_SetServoAngle(servo_usart, 3, servo_angle[2], 1500, power);
                 FSUS_SetServoAngle(servo_usart, 4, servo_angle[3], 1500, power);
            }
            BT_flag_set =0;
        }
        sprintf(txt, "db: %d %d %d %d ", did_this_work, task2_current_state, task2_finish_half, task2_all_done);
        TFTSPI_P8X16Str(1, 6, txt, u16WHITE, u16BLACK);
        Radar_Distance_Judge(&radar1);
        Radar_Distance_Judge(&radar2);
        // 第一路（前雷达，0xBB）
        if (radar1.distance < RADAR_TARGET_MM) {
            sprintf(txt, "R:%4dmm OK ", radar1.distance);
            TFTSPI_P8X16Str(1, 7, txt, u16RED, u16BLACK);
        } else {
            sprintf(txt, "R:%4dmm    ", radar1.distance);
            TFTSPI_P8X16Str(1, 7, txt, u16WHITE, u16BLACK);
        }

        // 第二路（侧雷达，0xCC）
        if (radar2.distance < RADAR_TARGET_MM) {
            sprintf(txt, "L:%4dmm OK ", radar2.distance);
            TFTSPI_P8X16Str(1, 8, txt, u16RED, u16BLACK);
        } else {
            sprintf(txt, "L:%4dmm    ", radar2.distance);
            TFTSPI_P8X16Str(1, 8, txt, u16WHITE, u16BLACK);
        }

        if(flag_from_irq){
            flag_from_irq = 0;
        }

//        get_real_gyro((float *)&wheel_asix.gx,(float *)&wheel_asix.gy,(float *)&wheel_asix.gz);

//        printf("%d %d %d \r\n",  (int)wheel_asix.gx, (int)wheel_asix.gy, (int)wheel_asix.gz);
//        printf("%f %f %f \r\n",  wheel_asix.roll,wheel_asix.pitch,wheel_asix.yaw);
//        signed short ax12,ay12,az12,gx12,gy12,gz12;
//        MPU_Get_Raw_data(&ax12,&ay12,&az12,&gx12,&gy12,&gz12);
//        MPU_Get_Gyroscope(&gx12,&gy12,&gz12);
//        printf("%d %d %d \r\n",  gx12,gy12,gz12);

        sprintf(txt, "A:%d %d %d %d %d", (int)wheel_asix.roll,(int)wheel_asix.pitch,(int)wheel_asix.yaw,pick_times, put_times);
        //UART_PutStr(UART1,txt);
        TFTSPI_P8X16Str(1, 9, txt, u16WHITE, u16BLACK);

#if DEBUG_MODE
        //static signed int value[4] = {0};
        encoder_value[0] = (signed int)MODULE_GPT120.T5.U;
        MODULE_GPT120.T5.U = 0;
        encoder_value[1] = (signed int)MODULE_GPT120.T4.U;
        MODULE_GPT120.T4.U = 0;
        encoder_value[2] = (signed int)MODULE_GPT120.T2.U;
        MODULE_GPT120.T2.U = 0;
        encoder_value[3] = (signed int)MODULE_GPT120.T6.U;
        MODULE_GPT120.T6.U = 0;
        sprintf(txt, "Enc1: %05d;", encoder_value[0]);
        TFTSPI_P8X16Str(1, 1, txt, u16WHITE, u16BLACK);       //字符串显示
        sprintf(txt, "Enc2: %05d;", encoder_value[1]);
        TFTSPI_P8X16Str(1, 2, txt, u16WHITE, u16BLACK);       //字符串显示
        sprintf(txt, "Enc3: %05d;", encoder_value[2]);
        TFTSPI_P8X16Str(1, 3, txt, u16WHITE, u16BLACK);       //字符串显示
        sprintf(txt, "Enc4: %05d;", encoder_value[3]);
        TFTSPI_P8X16Str(1, 4, txt, u16WHITE, u16BLACK);       //字符串显示
        sprintf(txt, "Roll=%.1f   ", Roll_1);
        TFTSPI_P8X16Str(1, 5, txt, u16WHITE, u16BLACK);
        sprintf(txt, "Pitch=%.1f  ",Pitch_1);
        TFTSPI_P8X16Str(1, 6, txt, u16WHITE, u16BLACK);
        sprintf(txt, "Yaw=%.1f  ",Yaw_1);
        TFTSPI_P8X16Str(1, 7, txt, u16WHITE, u16BLACK);
#endif


#else
        sprintf(txt, "Enc1: %05d;", ECPULSE1);
        TFT2SPI_P8X16Str(0, 3, txt, u16WHITE, u16BLACK);       //字符串显示
        sprintf(txt, "Enc2: %05d;", ECPULSE2);
        TFT2SPI_P8X16Str(0, 5, txt, u16WHITE, u16BLACK);       //字符串显示
        sprintf(txt, "Enc3: %05d;", ECPULSE3);
        TFT2SPI_P8X16Str(0, 7, txt, u16WHITE, u16BLACK);       //字符串显示
        sprintf(txt, "Enc4: %05d;", ECPULSE4);
        TFT2SPI_P8X16Str(0, 9, txt, u16WHITE, u16BLACK);       //字符串显示
#endif

    }
}





