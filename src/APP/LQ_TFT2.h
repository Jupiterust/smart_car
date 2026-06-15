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
#ifndef __LQ_TFT2_TFT2SPI_H__
#define __LQ_TFT2_TFT2SPI_H__

#include "stdint.h"

/*******************接口定义******************************/
#define TFT20W        220
#define TFT20H        176

#define u16RED		0xf800
#define u16GREEN	0x07e0
#define u16BLUE	    0x001f
#define u16PURPLE	0xf81f
#define u16YELLOW	0xffe0
#define u16CYAN	    0x07ff 		//蓝绿色
#define u16ORANGE	0xfc08
#define u16BLACK	0x0000
#define u16WHITE	0xffff
#define     u16LightPink                0xFDB8     //24位0xFFB6C1        浅粉红
#define     u16Pink                     0xFE19     //24位0xFFC0CB        粉红
#define     u16Crimson                  0xD8A7     //24位0xDC143C        猩红
#define     u16LavenderBlush            0xFF9E     //24位0xFFF0F5        脸红的淡紫色
#define     u16PaleVioletRed            0xDB92     //24位0xDB7093        苍白的紫罗兰红色
#define     u16HotPink                  0xFB56     //24位0xFF69B4        热情的粉红
#define     u16DeepPink                 0xF8B2     //24位0xFF1493        深粉色
#define     u16MediumVioletRed          0xC0B0     //24位0xC71585        适中的紫罗兰红色
#define     u16Orchid                   0xDB9A     //24位0xDA70D6        兰花的紫色
#define     u16Thistle                  0xDDFB     //24位0xD8BFD8        蓟
#define     u16plum                     0xDD1B     //24位0xDDA0DD        李子
#define     u16Violet                   0xEC1D     //24位0xEE82EE        紫罗兰
#define     u16Magenta                  0xF81F     //24位0xFF00FF        洋红
#define     u16Fuchsia                  0xF81F     //24位0xFF00FF        灯笼海棠(紫红色)
#define     u16DarkMagenta              0x8811     //24位0x8B008B        深洋红色
#define     u16Purple                   0x8010     //24位0x800080        紫色
#define     u16MediumOrchid             0xBABA     //24位0xBA55D3        适中的兰花紫
#define     u16DarkVoilet               0xD81C     //24位0x9400D3        深紫罗兰色
#define     u16DarkOrchid               0x9999     //24位0x9932CC        深兰花紫
#define     u16Indigo                   0x4810     //24位0x4B0082        靛青
#define     u16BlueViolet               0x895C     //24位0x8A2BE2        深紫罗兰的蓝色
#define     u16MediumPurple             0x939B     //24位0x9370DB        适中的紫色
#define     u16MediumSlateBlue          0x7B5D     //24位0x7B68EE        适中的板岩暗蓝灰色
#define     u16SlateBlue                0x6AD9     //24位0x6A5ACD        板岩暗蓝灰色
#define     u16DarkSlateBlue            0x49F1     //24位0x483D8B        深岩暗蓝灰色
#define     u16Lavender                 0xE73F     //24位0xE6E6FA        熏衣草花的淡紫色
#define     u16GhostWhite               0xFFDF     //24位0xF8F8FF        幽灵的白色
#define     u16Blue                     0x001F     //24位0x0000FF        纯蓝
#define     u16MediumBlue               0x0019     //24位0x0000CD        适中的蓝色
#define     u16MidnightBlue             0x18CE     //24位0x191970        午夜的蓝色
#define     u16DarkBlue                 0x0011     //24位0x00008B        深蓝色
#define     u16Navy                     0x0010     //24位0x000080        海军蓝
#define     u16RoyalBlue                0x435C     //24位0x4169E1        皇军蓝
#define     u16CornflowerBlue           0x64BD     //24位0x6495ED        矢车菊的蓝色
#define     u16LightSteelBlue           0xB63B     //24位0xB0C4DE        淡钢蓝
#define     u16LightSlateGray           0x7453     //24位0x778899        浅石板灰
#define     u16SlateGray                0x7412     //24位0x708090        石板灰
#define     u16DoderBlue                0xD700     //24位0x1E90FF        道奇蓝
#define     u16AliceBlue                0xF7DF     //24位0xF0F8FF        爱丽丝蓝
#define     u16SteelBlue                0x4416     //24位0x4682B4        钢蓝
#define     u16LightSkyBlue             0x867F     //24位0x87CEFA        淡蓝色
#define     u16SkyBlue                  0x867D     //24位0x87CEEB        天蓝色
#define     u16DeepSkyBlue              0x05FF     //24位0x00BFFF        深天蓝
#define     u16LightBLue                0xAEDC     //24位0xADD8E6        淡蓝
#define     u16PowDerBlue               0xB71C     //24位0xB0E0E6        火药蓝
#define     u16CadetBlue                0x5CF4     //24位0x5F9EA0        军校蓝
#define     u16Azure                    0xF7FF     //24位0xF0FFFF        蔚蓝色
#define     u16LightCyan                0xE7FF     //24位0xE1FFFF        淡青色
#define     u16PaleTurquoise            0xAF7D     //24位0xAFEEEE        苍白的绿宝石
#define     u16Cyan                     0x07FF     //24位0x00FFFF        青色
#define     u16Aqua                     0x07FF     //24位0x00FFFF        水绿色
#define     u16DarkTurquoise            0x067A     //24位0x00CED1        深绿宝石
#define     u16DarkSlateGray            0x2A69     //24位0x2F4F4F        深石板灰
#define     u16DarkCyan                 0x0451     //24位000x8B8B        深青色
#define     u16Teal                     0x0410     //24位0x008080        水鸭色
#define     u16MediumTurquoise          0x4E99     //24位0x48D1CC        适中的绿宝石
#define     u16LightSeaGreen            0x2595     //24位0x20B2AA        浅海洋绿
#define     u16Turquoise                0x471A     //24位0x40E0D0        绿宝石
#define     u16Auqamarin                0xA500     //24位0x7FFFAA        绿玉\碧绿色
#define     u16MediumAquamarine         0x6675     //24位0x00FA9A        适中的碧绿色
#define     u16MediumSpringGreen        0x07D3     //24位0xF5FFFA        适中的春天的绿色
#define     u16MintCream                0xF7FF     //24位0x00FF7F        薄荷奶油
#define     u16SpringGreen              0x07EF     //24位0x3CB371        春天的绿色
#define     u16SeaGreen                 0x2C4A     //24位0x2E8B57        海洋绿
#define     u16Honeydew                 0xF7FE     //24位0xF0FFF0        蜂蜜
#define     u16LightGreen               0x9772     //24位0x90EE90        淡绿色
#define     u16PaleGreen                0x9FD3     //24位0x98FB98        苍白的绿色
#define     u16DarkSeaGreen             0x8DF1     //24位0x8FBC8F        深海洋绿
#define     u16LimeGreen                0x3666     //24位0x32CD32        酸橙绿
#define     u16Lime                     0x07E0     //24位0x00FF00        酸橙色
#define     u16ForestGreen              0x2444     //24位0x228B22        森林绿
#define     u16Green                    0x0400     //24位0x008000        纯绿
#define     u16DarkGreen                0x0320     //24位0x006400        深绿色
#define     u16Chartreuse               0x7FE0     //24位0x7FFF00        查特酒绿
#define     u16LawnGreen                0x7FE0     //24位0x7CFC00        草坪绿
#define     u16GreenYellow              0xAFE5     //24位0xADFF2F        绿黄色
#define     u16OliveDrab                0x6C64     //24位0x556B2F        橄榄土褐色
#define     u16Beige                    0xF7BB     //24位0x6B8E23        米色(浅褐色)
#define     u16LightGoldenrodYellow     0xFFDA     //24位0xFAFAD2        浅秋麒麟黄
#define     u16Ivory                    0xFFFE     //24位0xFFFFF0        象牙
#define     u16LightYellow              0xFFFC     //24位0xFFFFE0        浅黄色
#define     u16Yellow                   0xFFE0     //24位0xFFFF00        纯黄
#define     u16Olive                    0x8400     //24位0x808000        橄榄
#define     u16DarkKhaki                0xBDAD     //24位0xBDB76B        深卡其布
#define     u16LemonChiffon             0xFFD9     //24位0xFFFACD        柠檬薄纱
#define     u16PaleGodenrod             0xA060     //24位0xEEE8AA        灰秋麒麟
#define     u16Khaki                    0xF731     //24位0xF0E68C        卡其布
#define     u16Gold                     0xFEA0     //24位0xFFD700        金
#define     u16Cornislk                 0xC000     //24位0xFFF8DC        玉米色
#define     u16GoldEnrod                0xDD24     //24位0xDAA520        秋麒麟
#define     u16FloralWhite              0xFFDE     //24位0xFFFAF0        花的白色
#define     u16OldLace                  0xFFBC     //24位0xFDF5E6        老饰带
#define     u16Wheat                    0xF6F6     //24位0xF5DEB3        小麦色
#define     u16Moccasin                 0xFF36     //24位0xFFE4B5        鹿皮鞋
#define     u16Orange                   0xFD20     //24位0xFFA500        橙色
#define     u16PapayaWhip               0xFF7A     //24位0xFFEFD5        番木瓜
#define     u16BlanchedAlmond           0xFF59     //24位0xFFEBCD        漂白的杏仁
#define     u16NavajoWhite              0xFEF5     //24位0xFFDEAD        Navajo白
#define     u16AntiqueWhite             0xFF5A     //24位0xFAEBD7        古代的白色
#define     u16Tan                      0xD5B1     //24位0xD2B48C        晒黑
#define     u16BrulyWood                0xB000     //24位0xDEB887        结实的树
#define     u16Bisque                   0xFF38     //24位0xFFE4C4        (浓汤)乳脂,番茄等
#define     u16DarkOrange               0xFC60     //24位0xFF8C00        深橙色
#define     u16Linen                    0xFF9C     //24位0xFAF0E6        亚麻布
#define     u16Peru                     0xCC27     //24位0xCD853F        秘鲁
#define     u16PeachPuff                0xFED7     //24位0xFFDAB9        桃色
#define     u16SandyBrown               0xF52C     //24位0xF4A460        沙棕色
#define     u16Chocolate                0xD343     //24位0xD2691E        巧克力
#define     u16SaddleBrown              0x8A22     //24位0x8B4513        马鞍棕色
#define     u16SeaShell                 0xFFBD     //24位0xFFF5EE        海贝壳
#define     u16Sienna                   0xA285     //24位0xA0522D        黄土赭色
#define     u16LightSalmon              0xFD0F     //24位0xFFA07A        浅鲜肉(鲑鱼)色
#define     u16Coral                    0xFBEA     //24位0xFF7F50        珊瑚
#define     u16OrangeRed                0xFA20     //24位0xFF4500        橙红色
#define     u16DarkSalmon               0xECAF     //24位0xE9967A        深鲜肉(鲑鱼)色
#define     u16Tomato                   0xFB08     //24位0xFF6347        番茄
#define     u16MistyRose                0xFF3C     //24位0xFFE4E1        薄雾玫瑰
#define     u16Salmon                   0xFC0E     //24位0xFA8072        鲜肉(鲑鱼)色
#define     u16Snow                     0xFFDF     //24位0xFFFAFA        雪
#define     u16LightCoral               0xF410     //24位0xF08080        淡珊瑚色
#define     u16RosyBrown                0xBC71     //24位0xBC8F8F        玫瑰棕色
#define     u16IndianRed                0xCAEB     //24位0xCD5C5C        印度红
#define     u16Red                      0xF800     //24位0xFF0000        纯红
#define     u16Brown                    0xA145     //24位0xA52A2A        棕色
#define     u16FireBrick                0xB104     //24位0xB22222        耐火砖
#define     u16DarkRed                  0x8800     //24位0x8B0000        深红色
#define     u16Maroon                   0x8000     //24位0x800000        栗色
#define     u16White                    0xFFFF     //24位0xFFFFFF        纯白
#define     u16WhiteSmoke               0xF7BE     //24位0xF5F5F5        白烟
#define     u16Gainsboro                0xDEFB     //24位0xDCDCDC        Gainsboro
#define     u16LightGrey                0xD69A     //24位0xD3D3D3        浅灰色
#define     u16Silver                   0xC618     //24位0xC0C0C0        银白色
#define     u16DarkGray                 0xAD55     //24位0xA9A9A9        深灰色
#define     u16Gray                     0x8410     //24位0x808080        灰色
#define     u16DimGray                  0x6B4D     //24位0x696969        暗淡的灰色

/*!
  * @brief    TFT2初始化
  *
  * @param    type ： 0:横屏  1：竖屏
  *
  * @return   无
  *
  * @note     如果修改管脚 需要修改初始化的管脚
  *
  * @see      TFT2SPI_Init(1);
  *
  * @date     2019/6/13 星期四
  */
void TFT2SPI_Init(unsigned char type);

/*!
  * @brief    写命令
  *
  * @param    cmd ：命令
  *
  * @return   无
  *
  * @note     内部调用
  *
  * @see      TFT2SPI_Write_Cmd(0xb7); //LCD Driveing control
  *
  * @date     2019/6/13 星期四
  */
void TFT2SPI_Write_Cmd(unsigned char cmd);

/*!
  * @brief    写字节
  *
  * @param    dat ：数据
  *
  * @return   无
  *
  * @note     内部调用
  *
  * @see      TFT2SPI_Write_Byte(0x00);	//CRL=0
  *
  * @date     2019/6/13 星期四
  */
void TFT2SPI_Write_Byte(unsigned char dat);


/*!
  * @brief    写半字
  *
  * @param    dat ：数据
  *
  * @return   无
  *
  * @note     无
  *
  * @see      TFT2SPI_Write_Word(0xFFFF);
  *
  * @date     2019/6/13 星期四
  */
void TFT2SPI_Write_Word(unsigned short dat);


/*!
  * @brief    重新定位输入信息位置
  *
  * @param    xs ：起始x
  * @param    ys ：起始y
  * @param    xe ：结束x
  * @param    ys ：结束y
  *
  * @return   无
  *
  * @note     内部调用
  *
  * @see      TFT2SPI_Set_Pos(10, 20, 30, 40);
  *
  * @date     2019/6/13 星期四
  */
void TFT2SPI_Set_Pos(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye);

/*!
  * @brief    填充指定区域
  *
  * @param    xs ：起始x
  * @param    ys ：起始y
  * @param    xe ：结束x
  * @param    ys ：结束y
  * @param    color ：填充的颜色
  *
  * @return   无
  *
  * @note     起始、终止横坐标(0-127)，纵坐标(0-159),显示颜色uint16
  *
  * @see      TFT2SPI_Fill_Area(10, 20, 30, 40, u16YELLOW);
  *
  * @date     2019/6/13 星期四
  */
void TFT2SPI_Fill_Area(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned short color);

/*!
  * @brief    全屏显示单色画面
  *
  * @param    color ：填充的颜色
  *
  * @return   无
  *
  * @note     起始、终止横坐标(0-127)，纵坐标(0-159),显示颜色uint16
  *
  * @see      TFT2SPI_CLS(u16YELLOW);
  *
  * @date     2019/6/13 星期四
  */
void TFT2SPI_CLS(unsigned short color);


/*!
  * @brief    填充矩形区域
  *
  * @param    xs ：起始x
  * @param    ys ：起始y
  * @param    xe ：结束x
  * @param    ys ：结束y
  * @param    color_dat ：填充的颜色
  *
  * @return   无
  *
  * @note     起始、终止横坐标(0-127)，纵坐标(0-159),显示颜色uint16
  *
  * @see      TFT2SPI_Draw_Part(10, 20, 30, 40, u16YELLOW);
  *
  * @date     2019/6/13 星期四
  */
void TFT2SPI_Draw_Part(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned short color_dat);

/*!
  * @brief    画矩形边框
  *
  * @param    xs ：起始x
  * @param    ys ：起始y
  * @param    xe ：结束x
  * @param    ys ：结束y
  * @param    color_dat ：颜色
  *
  * @return   无
  *
  * @note     起始、终止横坐标(0-127)，纵坐标(0-159),显示颜色uint16
  *
  * @see      TFT2SPI_Draw_Rectangle(10, 20, 30, 40, u16YELLOW);
  *
  * @date     2019/6/13 星期四
  */
void TFT2SPI_Draw_Rectangle(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned short color_dat);

/*!
  * @brief    画圆
  *
  * @param    x ：圆心x   (0-127)
  * @param    y ：圆心y   (0-159)
  * @param    r ：半径    (0-128)
  * @param    color_dat ：颜色
  *
  * @return   无
  *
  * @note     圆心坐标不要超出屏幕范围
  *
  * @see      TFT2SPI_Draw_Circle(50, 50, 30, u16YELLOW);
  *
  * @date     2019/6/13 星期四
  */
void TFT2SPI_Draw_Circle(unsigned char x,unsigned char y,unsigned char r,unsigned short color_dat);

/*!
  * @brief    画线
  *
  * @param    xs ：起始x
  * @param    ys ：起始y
  * @param    xe ：结束x
  * @param    ys ：结束y
  * @param    color_dat ：颜色
  *
  * @return   无
  *
  * @note     起始、终止横坐标(0-127)，纵坐标(0-159),显示颜色uint16
  *
  * @see      TFT2SPI_Draw_Line(10, 20, 30, 40, u16YELLOW);
  *
  * @date     2019/6/13 星期四
  */
void TFT2SPI_Draw_Line(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned short color_dat);

/*!
  * @brief    画点
  *
  * @param    x ：x
  * @param    y ：y
  * @param    color_dat ：颜色
  *
  * @return   无
  *
  * @note     起始、终止横坐标(0-127)，纵坐标(0-159),显示颜色uint16
  *
  * @see      TFT2SPI_Draw_Dot(10, 20, u16YELLOW);
  *
  * @date     2019/6/13 星期四
  */
void TFT2SPI_Draw_Dot(unsigned char x,unsigned char y,unsigned short color_dat);

/*!
  * @brief    显示图片
  *
  * @param    xs ：起始x
  * @param    ys ：起始y
  * @param    xe ：结束x
  * @param    ys ：结束y
  * @param    ppic ：图片数据
  *
  * @return   无
  *
  * @note     起始、终止横坐标(0-127)，纵坐标(0-159),显示颜色uint16
  *
  * @see
  *
  * @date     2019/6/13 星期四
  */
void TFT2SPI_Show_Pic(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,const unsigned char *ppic);

/*!
  * @brief    液晶字符串输出(6*8字体)
  *
  * @param    x: 0 - 20 (行)
  * @param    y: 0 - 19 (列)
  * @param    word_color: 字体颜色
  * @param    back_color: 背景颜色
  *
  * @return   无
  *
  * @note     无
  *
  * @see      TFT2SPI_P6X8Str(1, 1, "123456", u16YELLOW, u16RED);
  *
  * @date     2019/6/13 星期四
  */
void TFT2SPI_P6X8Str(unsigned char x, unsigned char y, char *s_dat,unsigned short word_color,unsigned short back_color);

/*!
  * @brief    液晶字符串输出(8*8字体)
  *
  * @param    x:0 - 15 (行)
  * @param    y:0 - 19 (列)
  * @param    word_color: 字体颜色
  * @param    back_color: 背景颜色
  *
  * @return   无
  *
  * @note     无
  *
  * @see      TFT2SPI_P8X8Str(1, 1, "123456", u16YELLOW, u16RED);
  *
  * @date     2019/6/13 星期四
  */
void TFT2SPI_P8X8Str(unsigned char x, unsigned char y, char *s_dat,unsigned short word_color,unsigned short back_color);

/*!
  * @brief    液晶字符串输出(8*16字体)
  *
  * @param    x: x: 0 -15   (行)
  * @param    y: y: 0 -9  	 (列)
  * @param    word_color: 字体颜色
  * @param    back_color: 背景颜色
  *
  * @return   无
  *
  * @note     无
  *
  * @see      TFT2SPI_P8X16Str(1, 1, "123456", u16YELLOW, u16RED);
  *
  * @date     2019/6/13 星期四
  */
void TFT2SPI_P8X16Str(unsigned char x, unsigned char y, char *s_dat,unsigned short word_color,unsigned short back_color);


/*!
  * @brief    液晶汉字字符串输出(16*16字体)
  *
  * @param    x: 0 - 7	(行)
  * @param    y: 0 - 9	(列)
  * @param    word_color: 字体颜色
  * @param    back_color: 背景颜色
  *
  * @return   无
  *
  * @note     汉字只能是字库里的 字库没有的需要自行添加
  *
  * @see      TFT2SPI_P16x16Str(1, 1, "123456", u16YELLOW, u16RED);
  *
  * @date     2019/6/13 星期四
  */
void TFT2SPI_P16x16Str(unsigned char x, unsigned char y, unsigned char *s_dat,unsigned short word_color,unsigned short back_color);


/*!
  * @brief    显示图片
  *
  * @param    xs ：起始x
  * @param    ys ：起始y
  * @param    xe ：结束x
  * @param    ys ：结束y
  * @param    ppic ：图片数据
  *
  * @return   无
  *
  * @note     起始、终止横坐标(0-127)，纵坐标(0-159),显示颜色uint16
  *
  * @see
  *
  * @date     2019/6/13 星期四
  */
void TFT2SPI_Show_Pic2(unsigned char xs,unsigned char ys,unsigned char w,unsigned char h,const unsigned char *ppic) ;


/*!
  * @brief    显示电池图标
  *
  * @param    无
  *
  * @return   无
  *
  * @note     无
  *
  * @see      TFT2SPI_Show_Battery_Icon();
  *
  * @date     2019/6/13 星期四
  */
void TFT2SPI_Show_Battery_Icon(void);

/*!
  * @brief    显示标签
  *
  * @param    无
  *
  * @return   无
  *
  * @note     无
  *
  * @see      TFT2SPI_Show_Title();
  *
  * @date     2019/6/13 星期四
  */
void TFT2SPI_Show_Title(void);

/*!
  * @brief    显示龙邱LOG
  *
  * @param    无
  *
  * @return   无
  *
  * @note     无
  *
  * @see      TFT2SPI_Show_Logo();
  *
  * @date     2019/6/13 星期四
  */
void TFT2SPI_Show_Logo(unsigned char xs,unsigned char ys);

/*!
  * @brief    TFT2屏 unsigned char 灰度数据显示
  *
  * @param    high_start ： 显示图像开始位置
  * @param    wide_start ： 显示图像开始位置
  * @param    high ： 显示图像高度
  * @param    wide ： 显示图像宽度
  * @param    Pixle： 显示图像数据地址
  *
  * @return   无
  *
  * @note     注意 屏幕左上为 （0，0）
  *
  * @see
  *
  * @date     2019/12/3 星期二
  */
void TFT2SPI_Road(unsigned char high_start, unsigned char wide_start, unsigned char high, unsigned char wide, unsigned char *Pixle);

/*!
  * @brief    TFT2屏 unsigned char 二值化数据显示
  *
  * @param    high_start ： 显示图像开始位置
  * @param    wide_start ： 显示图像开始位置
  * @param    high ： 显示图像高度
  * @param    wide ： 显示图像宽度
  * @param    Pixle： 显示图像数据地址
  *
  * @return   无
  *
  * @note     注意 屏幕左上为 （0，0）
  *
  * @see
  *
  * @date     2019/12/3 星期二
  */
void TFT2SPI_BinRoad(unsigned char high_start, unsigned char wide_start, unsigned char high, unsigned char wide, unsigned char *Pixle);
void write_command(unsigned char LCD_Reg,unsigned int LCD_RegValue) ;

void Test_TFT2(void);


#endif /*SGP18T_ILI9163B.h*/
