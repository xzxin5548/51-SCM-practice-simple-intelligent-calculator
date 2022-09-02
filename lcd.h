#ifndef _LCD_H //防止头文件的重复包含和编译
#define _LCD_H

typedef unsigned char uchar; // char定义一个别名uchar
typedef unsigned int uint;   // int定义一个别名uint

#define IO P0 //定义P0口为IO
sbit RW = P2 ^ 5;
sbit RS = P2 ^ 6;
sbit E = P2 ^ 7;

void Lcd1602_Write_com(uchar com);
void Lcd1602_Write_dat(uchar dat);
void Lcd_Init();

#endif
