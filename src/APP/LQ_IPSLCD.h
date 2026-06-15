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
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef __LQ_IPSLCD_H__
#define __LQ_IPSLCD_H__

#include "stdint.h"

/*屏幕型号选择，四选一，使用哪款屏幕则取消哪款屏幕的注释*/
#define IPS114
//#define IPS130
//#define IPS154
//#define IPS200

/*******************接口定义******************************/

//龙邱IPS 模块管脚定义
#define IPS_CS        P20_13     //IPS CS管脚
#define IPS_CK        P20_14     //IPS CK管脚
#define IPS_DI	      P20_11     //IPS DI管脚
#define IPS_RST       P20_10     //IPS RST管脚
#define IPS_DC	      P20_12     //IPS DC管脚

//管脚操作宏定义
#define IPS_DC_H  	PIN_Write(IPS_DC, 1)
#define IPS_DC_L  	PIN_Write(IPS_DC, 0)
#define IPS_RST_H 	PIN_Write(IPS_RST,1)
#define IPS_RST_L 	PIN_Write(IPS_RST,0)
#define IPS_DI_H  	PIN_Write(IPS_DI, 1)
#define IPS_DI_L  	PIN_Write(IPS_DI, 0)
#define IPS_CK_H  	PIN_Write(IPS_CK, 1)
#define IPS_CK_L  	PIN_Write(IPS_CK, 0)
#define IPS_CS_H    PIN_Write(IPS_CS, 1)
#define IPS_CS_L    PIN_Write(IPS_CS, 0)

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE           	 0x001F  
#define BRED             0XF81F
#define GRED 			       0XFFE0
#define GBLUE			       0X07FF
#define RED_IPS           	 0xF800 //红色
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			     0XBC40 //棕色
#define BRRED 			     0XFC07 //棕红色
#define GRAY  			     0X8430 //灰色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 			     0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

/*写入命令*/
void LCD_Write_Cmd(unsigned char cmd);
/*写入8位数据*/
void LCD_Write_Byte(unsigned char dat);
/*写入16位数据*/
void LCD_Write_Word(unsigned int dat);
/*设置起始和结束地址*/
void LCD_Address_Set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);

/*LQ IPS 1.14寸屏幕初始化*/
void LCD_Init(void);
/*在指定位置画点*/
void LCD_DrawPoint(unsigned int x,unsigned int y,unsigned int color);
/*指定位置填充指定颜色*/
void LCD_Fill(unsigned int xsta,unsigned int ysta,unsigned int xend,unsigned int yend,unsigned int color);
/*清屏*/
void LCD_CLS(unsigned int color);
/*画线*/
void LCD_Draw_Line(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned short color_dat);
/*画矩形框*/
void LCD_Draw_Rectangle(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned short color_dat);
/*画圆*/
void LCD_Draw_Circle(unsigned char x,unsigned char y,unsigned char r,unsigned short color_dat);
/*显示图片*/
void LCD_ShowPicture(unsigned int x, unsigned int y, unsigned int length, unsigned int width,const unsigned char pic[]);

/*显示单个字符*/
void LCD_ShowChar(unsigned int x,unsigned int y,unsigned char num,unsigned int fc,unsigned int bc,unsigned char sizey,unsigned char mode);
/*显示字符串*/
void LCD_ShowString(unsigned int x,unsigned int y,char *p,unsigned int fc,unsigned int bc,unsigned char sizey,unsigned char mode);
/*显示12*6大小的字符串*/
void LCD_ShowStr_12(unsigned int x,unsigned int y, char *p,unsigned int fc,unsigned int bc);
/*显示16*8大小的字符串*/
void LCD_ShowStr_16(unsigned int x,unsigned int y, char *p,unsigned int fc,unsigned int bc);
/*显示24*12大小的字符串*/
void LCD_ShowStr_24(unsigned int x,unsigned int y, char *p,unsigned int fc,unsigned int bc);
/*显示32*16大小的字符串*/
void LCD_ShowStr_32(unsigned int x,unsigned int y, char *p,unsigned int fc,unsigned int bc);
/*显示汉字*/
void LCD_ShowChinese(unsigned int x,unsigned int y,char *s,unsigned int fc,unsigned int bc,unsigned char sizey);



void TFTSPI_Set_Pos(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye);
void TFTSPI_Fill_Area(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned short color);
void TFTSPI_CLS(unsigned short color);
void TFTSPI_Draw_Dot(unsigned char x,unsigned char y,unsigned short color_dat);
void TFTSPI_Draw_Part(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned short color_dat);
void TFTSPI_Draw_Rectangle(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned short color_dat);
void TFTSPI_Draw_Circle(unsigned char x,unsigned char y,unsigned char r,unsigned short color_dat);
void TFTSPI_Draw_Line(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned short color_dat);
void TFTSPI_Show_Pic(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,const unsigned char *ppic);
void TFTSPI_P6X8Str(unsigned char x, unsigned char y, char *s_dat,unsigned short word_color,unsigned short back_color);
void TFTSPI_P8X8Str(unsigned char x, unsigned char y, char *s_dat,unsigned short word_color,unsigned short back_color);
void TFTSPI_P8X16Str(unsigned char x, unsigned char y, char *s_dat,unsigned short word_color,unsigned short back_color);
void TFTSPI_P16x16Str(unsigned char x, unsigned char y, unsigned char *s_dat,unsigned short word_color,unsigned short back_color);
void TFTSPI_Show_Pic2(unsigned char xs,unsigned char ys,unsigned char w,unsigned char h,const unsigned char *ppic) ;
void TFTSPI_Show_Battery_Icon(void);
void TFTSPI_Show_Title(void);
void TFTSPI_Show_Logo(unsigned char xs,unsigned char ys);
void LCDSPI_Road( unsigned char wide_start, unsigned char high_start,unsigned char high, unsigned char wide, unsigned char *Pixle);
void LCDSPI_BinRoad( unsigned char wide_start, unsigned char high_start,unsigned char high, unsigned char wide, unsigned char *Pixle);
void TFTSPI_ShowBeacon(void );
void Test_IPSLCD(void);


#endif /*SGP18T_ILI9163B.h*/
