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
=================================================================
程序配套视频地址：https://space.bilibili.com/95313236
=================================================================
本程序测试MPU6050DMP功能，配置匿名地面站4.34显示波形
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef _LQ_MPU6050_DMP_
#define _LQ_MPU6050_DMP_

extern short   gyro[3], accel[3], sensors;
extern float   Pitch, Roll, Yaw;

unsigned short inv_row_2_scale(const signed char *row);

int LQ_DMP_Init(void);
int LQ_DMP_Read(void);
void Test_LQDMP(void);

extern int LQ_I2C_Write(unsigned char addr, unsigned char reg, unsigned int len, unsigned char *dat);
extern int LQ_I2C_Read(unsigned char addr, unsigned char reg, unsigned int len, unsigned char *buf);
//extern void delayms(u16 nms);
//extern void delayus(u16 nus);

// 初始化使用
int mpu_set_gyro_fsr(unsigned short fsr);
int mpu_set_accel_fsr(unsigned char fsr);
int mpu_set_lpf(unsigned short lpf);
int mpu_set_sample_rate(unsigned short rate);
int dmp_set_orientation(unsigned short orient);
int mpu_write_mem(unsigned short mem_addr, unsigned short length, unsigned char *lqda);
int mpu_read_mem(unsigned short mem_addr, unsigned short length, unsigned char *lqda);
int mpu_reset_fifo_in_dmp_enable_feature(void);
int mpu_reset_fifo_in_mpu_set_dmp_state(void);

// 读值使用
int dmp_read_fifo(short *gyro, short *accel, long *quat, short *sensors, unsigned char *more);

#endif
