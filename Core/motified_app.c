/*
 * motified_app.c
 *
 *  Created on: Mar 25, 2026
 *      Author: TX
 */

#include "motified_app.h"

//    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)          //读PB1输入寄存器的状态，如果为0，则代表按键1按下
//    {
//        Delay_ms(20);                                           //延时消抖
//        while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0);  //等待按键松手
//        Delay_ms(20);                                           //延时消抖
//        KeyNum = 1;                                             //置键码为1
//    }

u8 blocking_key_read(void){
    u8 key_value = 0;
    if(PIN_Read(KEY0p) == 0){
        delayms(15);
        while(PIN_Read(KEY0p) == 0);
        delayms(15);
        key_value = 1;
    }
    if(PIN_Read(KEY1p) == 0){
        delayms(15);
        while(PIN_Read(KEY1p) == 0);
        delayms(15);
        key_value = 2;
    }
    if(PIN_Read(KEY2p) == 0){
        delayms(15);
        while(PIN_Read(KEY2p) == 0);
        delayms(15);
        key_value = 3;
    }
    return key_value;
}


