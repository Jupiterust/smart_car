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
#ifndef _LQ_ASC_H_
#define _LQ_ASC_H_

#include <Platform_Types.h>
#include <stdint.h>
#include <Ifx_Fifo.h>
#include <Ifx_Types.h>
#include <IfxAsclin.h>
#include <IfxAsclin_Asc.h>
#include <IfxAsclin_PinMap.h>
#include <IfxCpu.h>
#include <IfxCpu_Irq.h>
#include <IfxPort.h>
#include <IfxPort_regdef.h>
#include <stdbool.h>

/** UART0接收中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  UART0_RX_PRIORITY    120

/** UART0发送中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  UART0_TX_PRIORITY    121

/** UART0错误中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  UART0_ER_PRIORITY    122

/** UART0中断服务函数优先级 中断归哪个内核管理？ 范围：0：CPU0   1：CPU1   3：DMA*/
#define  UART0_VECTABNUM    0


/** UART1接收中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  UART1_RX_PRIORITY    130

/** UART1发送中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  UART1_TX_PRIORITY    131

/** UART1错误中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  UART1_ER_PRIORITY    132

/** UART1中断服务函数优先级 中断归哪个内核管理？ 范围：0：CPU0   1：CPU1   3：DMA*/
#define  UART1_VECTABNUM    0


/** UART2接收中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  UART2_RX_PRIORITY    140

/** UART2发送中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  UART2_TX_PRIORITY    141

/** UART2错误中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  UART2_ER_PRIORITY    142

/** UART2中断服务函数优先级 中断归哪个内核管理？ 范围：0：CPU0   1：CPU1   3：DMA*/
#define  UART2_VECTABNUM    0


/** UART3接收中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  UART3_RX_PRIORITY    150

/** UART3发送中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  UART3_TX_PRIORITY    151

/** UART3错误中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  UART3_ER_PRIORITY    152

/** UART3中断服务函数优先级 中断归哪个内核管理？ 范围：0：CPU0   1：CPU1   3：DMA*/
#define  UART3_VECTABNUM    0

/**
  * @brief UART 枚举
  */
typedef enum
{
	UART0 = 0,
	UART1,
	UART2,
	UART3
}UART_t;

//L.Q UART RX 管脚枚举
typedef enum
{
    UART0_RX_P14_1 = 0xB401, UART0_RX_P15_3 = 0xB503,

    UART1_RX_P02_3 = 0xA203, UART1_RX_P11_10= 0xB10A, UART1_RX_P15_1 = 0xB501, UART1_RX_P15_5 = 0xB505, UART1_RX_P20_9 = 0xC009, UART1_RX_P33_13= 0xD30D,

    UART2_RX_P02_0 = 0xA200, UART2_RX_P02_1 = 0xA201, UART2_RX_P10_6 = 0xB006, UART2_RX_P14_3 = 0xB403, UART2_RX_P33_8 = 0xD308,

    UART3_RX_P00_1 = 0xA001, UART3_RX_P15_7 = 0xB507, UART3_RX_P20_3 = 0xC003, UART3_RX_P21_6 = 0xC106

}UART_RX_t;


//L.Q UART TX 管脚枚举
typedef enum
{
    UART0_TX_P14_0 = 0xB400, UART0_TX_P14_1 = 0xB401, UART0_TX_P15_2 = 0xB502, UART0_TX_P15_3 = 0xB503,

    UART1_TX_P02_2 = 0xA202, UART1_TX_P11_12= 0xB10C, UART1_TX_P15_0 = 0xB500, UART1_TX_P15_1 = 0xB501, UART1_TX_P15_4 = 0xB504, UART1_TX_P15_5 = 0xB505, UART1_TX_P20_10= 0xC00A, UART1_TX_P33_12= 0xD30C, UART1_TX_P33_13 = 0xD30D,

    UART2_TX_P02_0 = 0xA200, UART2_TX_P10_5 = 0xB005, UART2_TX_P14_2 = 0xB402, UART2_TX_P14_3 = 0xB403, UART2_TX_P33_8 = 0xD308, UART2_TX_P33_9 = 0xD309,

    UART3_TX_P00_0 = 0xA000, UART3_TX_P00_1 = 0xA001, UART3_TX_P15_6 = 0xB506, UART3_TX_P15_7 = 0xB507, UART3_TX_P20_0 = 0xC000, UART3_TX_P20_3 = 0xC003, UART3_TX_P21_7 = 0xC107

}UART_TX_t;


void UART_InitConfig(UART_RX_t RxPin, UART_TX_t TxPin, unsigned long baudrate);
void UART_PutChar(UART_t  uratn, char ch);
void UART_PutStr(UART_t  uratn, char *str);
void UART_PutBuff(UART_t  uratn, unsigned char *buff, unsigned long len);
uint8 UART_GetCount(UART_t  uratn);
char UART_GetChar(UART_t  uratn);
char UART_GetBuff(UART_t  uratn, unsigned char *data, unsigned char len);



//#define UART0_FORWARD    1
//#define UART0_LEFT_TURN  2
//#define UART0_RIGHT_TURN 3
//
//#define UART_LEFTWARD   5
//#define UART_RIGHTWARD  6
//#define UART_BACKWARD   2


//typedef enum{
//    keep_straight = 0,
//    first_crossing_line = 1,
//    first_crossing_line_end = 2,
//    first_turning_left_line = 3,
//    first_circle_line = 4,
//    second_turning_left_line = 5,
//    third_same_turning_left_line = 6,
//    first_turning_right_line = 7,
//    second_turning_right_line = 8,
//    fourth_turning_left_line = 9,
//    stop_line = 10,
//
//}tracking_state_enum;

//
//#define    last_is_straight     1
//#define    last_is_cross_line       2
//#define    last_is_cross_line_end   3
//#define    last_is_first_turning_left_line      4
//#define    last_is_circle_line      5
//#define    last_is_second__turning_left_line    6
//#define    last_is_third_same_turning_left_line 7
//#define    last_is_first_turning_right_line     8
//#define    last_is_second_turning_right_line    9
//#define    last_is_fourth_turning_left_line     10
//#define    last_is_stop_line    11

// 4字节 float 与 unsigned char[4] 互相映射
typedef union {
    float f;               // 浮点数
    unsigned char bytes[4];// 4个字节
} FloatByteUnion;

// extern volatile FloatByteUnion deviation;
extern volatile char uart_send_data;

extern unsigned char uart0_buffer[30];
extern float Roll_1 ,Pitch_1 ,Yaw_1 ;
extern volatile short dis_error;
extern volatile int tracking_state;
extern volatile int speed_error_from_camera;

extern volatile uint8_t BT_flag;
extern volatile uint8_t BT_flag_set;
extern volatile float servo_angle[4];
// extern volatile blue tooth variables

typedef enum{
    servo_from_camera_idle = 0,
    servo_pick_up_from_camera = 1,
    servo_put_down_from_camera = 2,
}SERVO_ACTION_ENUM;

typedef enum{
    task1_cylinder_id_idle = 0,

    task1_cylinder_id_small = 1,
    task1_cylinder_id_medium = 2,
    task1_cylinder_id_large = 3,
}task1_cylinder_enum;

extern volatile SERVO_ACTION_ENUM pick_or_put;
extern volatile task1_cylinder_enum task1_cy_id ;
extern volatile bool does_dummy_run;
extern volatile uint8_t pick_times;
extern volatile uint8_t put_times;

#define HEADER 0xAA                         /* 起始符 */
#define device_address 0x00     /* 设备地址 */
#define chunk_offset 0x00       /* 偏移地址命令 */
#define PACK_GET_DISTANCE 0x02  /* 获取测量数据命令 */
#define PACK_RESET_SYSTEM 0x0D  /* 复位命令 */
#define PACK_STOP 0x0F                /* 停止测量数据传输命令 */
#define PACK_ACK 0x10           /* 应答码命令 */
#define PACK_VERSION 0x14       /* 获取传感器信息命令 */


typedef struct {
    int16_t distance;                       /* 距离数据：测量目标距离单位 mm */
    uint16_t noise;                             /* 环境噪声：当前测量环境下的外部环境噪声，越大说明噪声越大 */
    uint32_t peak;                              /* 接收强度信息：测量目标反射回的光强度 */
    uint8_t confidence;                     /* 置信度：由环境噪声和接收强度信息融合后的测量点的可信度 */
    uint32_t intg;                          /* 积分次数：当前传感器测量的积分次数 */
    int16_t reftof;                         /* 温度表征值：测量芯片内部温度变化表征值，只是一个温度变化量无法与真实温度对应 */
}LidarPointTypedef;

struct AckResultData{
    uint8_t ack_cmd_id;                     /* 答复的命令 id */
    uint8_t result;                             /* 1表示成功,0表示失败 */
};

struct LiManuConfig
{
    uint32_t version;                       /* 软件版本号 */
    uint32_t hardware_version;      /* 硬件版本号 */
    uint32_t manufacture_date;      /* 生产日期 */
    uint32_t manufacture_time;      /* 生产时间 */
    uint32_t id1;                               /* 设备 id1 */
    uint32_t id2;                               /* 设备 id2 */
    uint32_t id3;                               /* 设备 id3 */
    uint8_t sn[8];                              /* sn */
    uint16_t pitch_angle[4];            /* 角度信息 */
    uint16_t blind_area[2];             /* 盲区信息 */
    uint32_t frequence;                     /* 数据点频 */
};

extern uint16_t receive_cnt;
extern uint8_t confidence;
extern uint16_t distance,noise,reftof;
extern uint32_t peak,intg;


extern IfxAsclin_Asc g_UartConfig[4];
extern volatile uint32_t rx_irq_count;

#endif /* 0_APPSW_TRICORE_APP_LQ_ASC_H_ */
