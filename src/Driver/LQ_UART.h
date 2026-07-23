/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨�������������ܿƼ�TC264DA���İ�
����    д��chiusir
��E-mail��chiusir@163.com
������汾��V1.2 ��Ȩ���У���λʹ��������ϵ��Ȩ
�������¡�2023��6��6��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://longqiu.taobao.com
------------------------------------------------
��dev.env.��AURIX Development Studio1.6�汾
��Target �� TC264DA/TC264D
��Crystal�� 20.000Mhz
��SYS PLL�� 200MHz
________________________________________________________________
����iLLD_1_0_1_11_0�ײ����,

ʹ�����̵�ʱ�򣬽������û�пո��Ӣ��·����
����CIFΪTC264DA�����⣬�����Ĵ������TC264D
����Ĭ�ϳ�ʼ����EMEM��512K������û�ʹ��TC264D��ע�͵�EMEM_InitConfig()��ʼ��������
������\Libraries\iLLD\TC26B\Tricore\Cpu\CStart\IfxCpu_CStart0.c��164�����ҡ�
ASC�첽����ͨ�ţ�������ΪUART������LIN����ʹ�ã�
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

/** UART0�����жϷ��������ȼ�   ��Χ��1-255   ����Խ�� ���ȼ�Խ��  ע�����ȼ���Ҫ�ظ� */
#define  UART0_RX_PRIORITY    120

/** UART0�����жϷ��������ȼ�   ��Χ��1-255   ����Խ�� ���ȼ�Խ��  ע�����ȼ���Ҫ�ظ� */
#define  UART0_TX_PRIORITY    121

/** UART0�����жϷ��������ȼ�   ��Χ��1-255   ����Խ�� ���ȼ�Խ��  ע�����ȼ���Ҫ�ظ� */
#define  UART0_ER_PRIORITY    122

/** UART0�жϷ��������ȼ� �жϹ��ĸ��ں˹��� ��Χ��0��CPU0   1��CPU1   3��DMA*/
#define  UART0_VECTABNUM    0


/** UART1�����жϷ��������ȼ�   ��Χ��1-255   ����Խ�� ���ȼ�Խ��  ע�����ȼ���Ҫ�ظ� */
#define  UART1_RX_PRIORITY    130

/** UART1�����жϷ��������ȼ�   ��Χ��1-255   ����Խ�� ���ȼ�Խ��  ע�����ȼ���Ҫ�ظ� */
#define  UART1_TX_PRIORITY    131

/** UART1�����жϷ��������ȼ�   ��Χ��1-255   ����Խ�� ���ȼ�Խ��  ע�����ȼ���Ҫ�ظ� */
#define  UART1_ER_PRIORITY    132

/** UART1�жϷ��������ȼ� �жϹ��ĸ��ں˹��� ��Χ��0��CPU0   1��CPU1   3��DMA*/
#define  UART1_VECTABNUM    0


/** UART2�����жϷ��������ȼ�   ��Χ��1-255   ����Խ�� ���ȼ�Խ��  ע�����ȼ���Ҫ�ظ� */
#define  UART2_RX_PRIORITY    140

/** UART2�����жϷ��������ȼ�   ��Χ��1-255   ����Խ�� ���ȼ�Խ��  ע�����ȼ���Ҫ�ظ� */
#define  UART2_TX_PRIORITY    141

/** UART2�����жϷ��������ȼ�   ��Χ��1-255   ����Խ�� ���ȼ�Խ��  ע�����ȼ���Ҫ�ظ� */
#define  UART2_ER_PRIORITY    142

/** UART2�жϷ��������ȼ� �жϹ��ĸ��ں˹��� ��Χ��0��CPU0   1��CPU1   3��DMA*/
#define  UART2_VECTABNUM    0


/** UART3�����жϷ��������ȼ�   ��Χ��1-255   ����Խ�� ���ȼ�Խ��  ע�����ȼ���Ҫ�ظ� */
#define  UART3_RX_PRIORITY    150

/** UART3�����жϷ��������ȼ�   ��Χ��1-255   ����Խ�� ���ȼ�Խ��  ע�����ȼ���Ҫ�ظ� */
#define  UART3_TX_PRIORITY    151

/** UART3�����жϷ��������ȼ�   ��Χ��1-255   ����Խ�� ���ȼ�Խ��  ע�����ȼ���Ҫ�ظ� */
#define  UART3_ER_PRIORITY    152

/** UART3�жϷ��������ȼ� �жϹ��ĸ��ں˹��� ��Χ��0��CPU0   1��CPU1   3��DMA*/
#define  UART3_VECTABNUM    0

/**
  * @brief UART ö��
  */
typedef enum
{
	UART0 = 0,
	UART1,
	UART2,
	UART3
}UART_t;

//L.Q UART RX �ܽ�ö��
typedef enum
{
    UART0_RX_P14_1 = 0xB401, UART0_RX_P15_3 = 0xB503,

    UART1_RX_P02_3 = 0xA203, UART1_RX_P11_10= 0xB10A, UART1_RX_P15_1 = 0xB501, UART1_RX_P15_5 = 0xB505, UART1_RX_P20_9 = 0xC009, UART1_RX_P33_13= 0xD30D,

    UART2_RX_P02_0 = 0xA200, UART2_RX_P02_1 = 0xA201, UART2_RX_P10_6 = 0xB006, UART2_RX_P14_3 = 0xB403, UART2_RX_P33_8 = 0xD308,

    UART3_RX_P00_1 = 0xA001, UART3_RX_P15_7 = 0xB507, UART3_RX_P20_3 = 0xC003, UART3_RX_P21_6 = 0xC106

}UART_RX_t;


//L.Q UART TX �ܽ�ö��
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

// 4�ֽ� float �� unsigned char[4] ����ӳ��
typedef union {
    float f;               // ������
    unsigned char bytes[4];// 4���ֽ�
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
extern volatile float servo_angle[5];
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
extern volatile bool following_after_task1;

extern volatile uint8_t worm_record_array[4];
extern volatile bool task2_start;


typedef enum{
    IDLE_BALL = 0,
    BLUE_BALL = 1,
    YELLOW_BALL = 2,
}TASK4_BALL_STATE;

#define HEADER 0xAA                         /* ��ʼ�� */
#define device_address 0x00     /* �豸��ַ */
#define chunk_offset 0x00       /* ƫ�Ƶ�ַ���� */
#define PACK_GET_DISTANCE 0x02  /* ��ȡ������������ */
#define PACK_RESET_SYSTEM 0x0D  /* ��λ���� */
#define PACK_STOP 0x0F                /* ֹͣ�������ݴ������� */
#define PACK_ACK 0x10           /* Ӧ�������� */
#define PACK_VERSION 0x14       /* ��ȡ��������Ϣ���� */


typedef struct {
    int16_t distance;                       /* �������ݣ�����Ŀ����뵥λ mm */
    uint16_t noise;                             /* ������������ǰ���������µ��ⲿ����������Խ��˵������Խ�� */
    uint32_t peak;                              /* ����ǿ����Ϣ������Ŀ�귴��صĹ�ǿ�� */
    uint8_t confidence;                     /* ���Ŷȣ��ɻ��������ͽ���ǿ����Ϣ�ںϺ�Ĳ�����Ŀ��Ŷ� */
    uint32_t intg;                          /* ���ִ�������ǰ�����������Ļ��ִ��� */
    int16_t reftof;                         /* �¶ȱ���ֵ������оƬ�ڲ��¶ȱ仯����ֵ��ֻ��һ���¶ȱ仯���޷�����ʵ�¶ȶ�Ӧ */
}LidarPointTypedef;

struct AckResultData{
    uint8_t ack_cmd_id;                     /* �𸴵����� id */
    uint8_t result;                             /* 1��ʾ�ɹ�,0��ʾʧ�� */
};

struct LiManuConfig
{
    uint32_t version;                       /* ����汾�� */
    uint32_t hardware_version;      /* Ӳ���汾�� */
    uint32_t manufacture_date;      /* �������� */
    uint32_t manufacture_time;      /* ����ʱ�� */
    uint32_t id1;                               /* �豸 id1 */
    uint32_t id2;                               /* �豸 id2 */
    uint32_t id3;                               /* �豸 id3 */
    uint8_t sn[8];                              /* sn */
    uint16_t pitch_angle[4];            /* �Ƕ���Ϣ */
    uint16_t blind_area[2];             /* ä����Ϣ */
    uint32_t frequence;                     /* ���ݵ�Ƶ */
};

extern uint16_t receive_cnt;
extern uint8_t confidence;
extern uint16_t distance,noise,reftof;
extern uint32_t peak,intg;

extern volatile char task_start_signal_from_me;
extern IfxAsclin_Asc g_UartConfig[4];
extern volatile uint32_t rx_irq_count;

extern volatile TASK4_BALL_STATE task4_ball_current_state;
extern volatile bool does_task4_start_action;
extern volatile uint8_t blue_ball_number;

#endif /* 0_APPSW_TRICORE_APP_LQ_ASC_H_ */
