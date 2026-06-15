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
#ifndef SRC_APPSW_TRICORE_MAIN_MAIN_H_
#define SRC_APPSW_TRICORE_MAIN_MAIN_H_

#include "../../Libraries/iLLD/TC26B/Tricore/Cpu/Std/IfxCpu.h"
#include "../../Libraries/iLLD/TC26B/Tricore/Cpu/Std/Platform_Types.h"
#include "../Driver/include.h"//各个模块的头文件

extern App_Cpu0 g_AppCpu0; // brief CPU 0 global data
extern IfxCpu_mutexLock mutexCpu0InitIsOk;   /** CPU0 初始化完成标志位  */
extern volatile char mutexCpu0TFTIsOk;       /** TFT18使用标志位  */
/**
 * 定时器 5ms和50ms标志位
 */
extern volatile uint8 cpu1Flage5ms;
extern volatile uint8 cpu1Flage50ms;

/* 期望速度  */
extern volatile sint16 targetSpeed;

/* 避障标志位 */
extern volatile uint8 evadibleFlage;


extern volatile uint8_t System_run;

int core0_main (void);



#define TFT_WORK_ON_ONE_CPU      0


#endif /* SRC_APPSW_TRICORE_MAIN_MAIN_H_ */
