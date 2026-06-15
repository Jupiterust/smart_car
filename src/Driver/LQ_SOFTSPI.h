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
#ifndef __LQ_Soft_SPI_H_
#define __LQ_Soft_SPI_H_
#include "include.h"

//                   单片机    龙邱模块
#define Soft_SPI_SCK_PIN      P15_8     //SCK
#define Soft_SPI_MISO_PIN     P15_5     //单片机MISO接模块的--SDO
#define Soft_SPI_MOSI_PIN     P15_7     //单片机MOSI接模块的--SDI
#define Soft_SPI_CS_PIN       P15_2     //CS

#define Soft_SPI_SCK_OUT_H    PIN_Write(Soft_SPI_SCK_PIN, 1)        //配置输出高电平
#define Soft_SPI_SCK_OUT_L    PIN_Write(Soft_SPI_SCK_PIN, 0)        //配置输出低电平

#define Soft_SPI_MISO_IN      PIN_Read(Soft_SPI_MISO_PIN)         //读取引脚上的引脚状态

#define Soft_SPI_MOSI_OUT_H   PIN_Write(Soft_SPI_MOSI_PIN, 1)       //配置输出高电平
#define Soft_SPI_MOSI_OUT_L   PIN_Write(Soft_SPI_MOSI_PIN, 0)       //配置输出低电平

#define Soft_SPI_CS_OUT_H     PIN_Write(Soft_SPI_CS_PIN, 1)         //配置输出高电平
#define Soft_SPI_CS_OUT_L     PIN_Write(Soft_SPI_CS_PIN, 0)         //配置输出低电平

void Soft_SPI_Init(void);
void Soft_SPI_ReadWriteNbyte(unsigned char *lqbuff, unsigned char len);
void LQ_SPI_Read(unsigned char reg, unsigned short len, unsigned char* buf);
void LQ_SPI_Write(unsigned char reg, unsigned char value);
#endif

















