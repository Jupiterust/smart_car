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
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef _LQ_INCLUDE_H_
#define _LQ_INCLUDE_H_

/////////////c标准库和底层库文件//////////////////////////////////////////////////
#include "Platform_Types.h"
#include <CompilerTasking.h>
#include <IfxQspi_PinMap.h>
#include <IfxQspi_SpiMaster.h>
#include <IfxCpu.h>
#include <IfxScuCcu.h>
#include <IfxScuWdt.h>
#include <IfxStm.h>
#include <IfxStm_reg.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>

/////////////////////龙邱库文件////////////////////////////////////////////
/**************APP*******************/
#include "LQ_CCU6_Timer.h"
#include "LQ_GPIO_KEY.h"
#include "LQ_GPIO_LED.h"
#include "LQ_I2C_9AX.h"
#include "LQ_MPU6050_DMP.h"
#include "LQ_I2C_VL53.h"
#include "LQ_ICM20602.h"
#include "LQ_IIC_Gyro.h"
#include "LQ_MPU6050_DMP.h"
#include "LQ_STM_Timer.h"
#include "LQ_TFT18.h"
#include "LQ_TFT2.h"
#include "LQ_LSM6DSR.h"
#include "LQ_IPSLCD.h"
#include "LQ_OLED096.h"
#include "LQ_Atom_Motor.h"
#include "LQ_Tom_Servo.h"


/**************Drv*******************/
#include "LQ_ADC.h"
#include "LQ_CCU6.h"
#include "LQ_EEPROM.h"
#include "LQ_GPIO.h"
#include "LQ_GPSR.h"
#include "LQ_GPT12_ENC.h"
#include "LQ_GTM.h"
#include "LQ_QSPI.h"
#include "LQ_SOFTI2C.h"
#include "LQ_SOFTSPI.h"
#include "LQ_SPI.h"
#include "LQ_STM.h"
#include "LQ_UART.h"

/**************User******************/
#include "LQ_MotorServo.h"

////////////////////////////////////////////////////////////////////////////
typedef struct
{
    float32 sysFreq;  //Actual SPB frequency
    float32 cpuFreq;  //Actual CPU frequency
    float32 pllFreq;  //Actual PLL frequency
    float32 stmFreq;  //Actual STM frequency
} AppInfo;



// Application information
typedef struct
{
    AppInfo info;     //Info object
} App_Cpu0;

#define     __IO    volatile
#define     __O     volatile
#define     __I     volatile
typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef const int32_t sc32;
typedef const int16_t sc16;
typedef const int8_t sc8;

typedef __IO int32_t  vs32;
typedef __IO int16_t  vs16;
typedef __IO int8_t   vs8;

typedef __I int32_t vsc32;
typedef __I int16_t vsc16;
typedef __I int8_t vsc8;

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef const uint32_t uc32;
typedef const uint16_t uc16;
typedef const uint8_t uc8;

typedef __IO uint32_t  vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;

typedef __I uint32_t vuc32;
typedef __I uint16_t vuc16;
typedef __I uint8_t vuc8;
#endif




