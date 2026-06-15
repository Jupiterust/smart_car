//=============================================================
// 雷达距离接收模块 (TC264)
//=============================================================
#include "cheju.h"

// 对外状态
volatile uint16_t radar_distance   = 9999;  // 初值给大，防上电误触发
volatile uint8_t  radar_confidence = 0;
volatile bool     radar_arrived    = false;
volatile uint32_t radar_byte_cnt  = 0;
volatile uint32_t radar_frame_cnt = 0;
// 内部状态
static volatile uint8_t radar_updated = 0;  // 收到合法新帧标志
static uint8_t radar_rx_state = 0;
static uint8_t radar_buf[4];
static uint8_t radar_cnt = 0;
static uint8_t radar_below_cnt = 0;
//volatile uint32_t uart2_isr_cnt = 0;
//-------------------------------------------------------------
// 帧解析，逐字节喂入。帧格式 BB | dL | dH | conf | xor | 0D
//-------------------------------------------------------------
void Radar_Feed_Byte(uint8_t b)
{
    switch (radar_rx_state)
    {
        case 0:                              // 找帧头 BB
            if (b == 0xBB) { radar_cnt = 0; radar_rx_state = 1; }
            break;
        case 1:                              // 收 dL dH conf xor 共4字节
            radar_buf[radar_cnt++] = b;
            if (radar_cnt >= 4) radar_rx_state = 2;
            break;
        case 2:                              // 验帧尾 0D + 异或校验
            if (b == 0x0D)
            {
                uint8_t chk = radar_buf[0] ^ radar_buf[1] ^ radar_buf[2];
                if (chk == radar_buf[3])     // 校验过才更新，错帧丢掉
                {
                    radar_distance   = (uint16_t)(radar_buf[0] | (radar_buf[1] << 8));
                    radar_confidence = radar_buf[2];
                    radar_updated    = 1;
                }
            }
            radar_rx_state = 0;              // 不管对错都回去找帧头
            break;
        default:
            radar_rx_state = 0;
            break;
    }
}

//-------------------------------------------------------------
// 距离判断，主循环周期调用
// 防抖(连续N帧) + 回滞(到达/离开两套阈值) + 锁存(只触发一次)
//-------------------------------------------------------------
void Radar_Distance_Judge(void)
{
    if (!radar_updated) return;             // 没新数据不处理
    radar_updated = 0;

    uint16_t d = radar_distance;
    uint8_t  c = radar_confidence;
    if (c < RADAR_CONF_MIN) return;         // 数据不可信，跳过

    if (!radar_arrived)
    {
        if (d < RADAR_TARGET_MM)
        {
            if (++radar_below_cnt >= RADAR_STABLE_CNT)
            {
                radar_arrived   = true;
                radar_below_cnt = 0;
                // ========= 距离到达，执行任务(只触发一次) =========
                // 这里跑在主循环上下文，放重逻辑也安全
                // 例：停车 / 启动机械臂 / 切状态机
                // ===============================================
            }
        }
        else radar_below_cnt = 0;           // 中途超阈值，重新计数
    }
    else
    {
        if (d > RADAR_RELEASE_MM) radar_arrived = false;  // 回升复位，允许再次触发
    }
}
