/*
 * cheju.h
 *
 *  Created on: 2026年6月14日
 *      Author: WSS
 */

#ifndef CORE_CHEJU_H_
#define CORE_CHEJU_H_

//=============================================================
// 雷达距离接收模块 (TC264)
// 收STM32转发的6字节帧: BB | dist_L | dist_H | conf | xor | 0D
// 串口2中断里调 Radar_Feed_Byte()，主循环调 Radar_Distance_Judge()
//=============================================================
#ifndef __RADAR_DIST_H
#define __RADAR_DIST_H

#include <stdint.h>
#include <stdbool.h>

// 距离判断参数，按场地调
#define RADAR_TARGET_MM     300   // 小于它算"到达"
#define RADAR_RELEASE_MM    350   // 回滞，大于它才算"离开"
#define RADAR_CONF_MIN      30    // 置信度门限，低于不信
#define RADAR_STABLE_CNT    3     // 连续N帧满足才确认

// 对外状态，其他文件可直接读
extern volatile uint16_t radar_distance;    // 当前距离 mm
extern volatile uint8_t  radar_confidence;  // 当前置信度
extern volatile bool     radar_arrived;     // 是否已到达目标距离(带回滞锁存)
extern volatile uint32_t radar_byte_cnt ;   // 进来多少字节
extern volatile uint32_t radar_frame_cnt ;   // 成功解析多少帧
// 接口
void Radar_Feed_Byte(uint8_t b);   // 每收到1字节喂进来(串口2中断里调)
void Radar_Distance_Judge(void);   // 主循环周期调用，判距+触发任务
extern volatile uint32_t uart3_isr_cnt;
#endif




#endif /* CORE_CHEJU_H_ */
