#include "reg52.h"
#include "lcd.h"

void delay1ms(void) //延时函数1ms 误差 0us
{
    unsigned char a, b;
    for (b = 199; b > 0; b--)
        for (a = 1; a > 0; a--)
            ;
}
void delay5ms(void) //延时函数5ms 误差 0us
{
    unsigned char a, b;
    for (b = 185; b > 0; b--)
        for (a = 12; a > 0; a--)
            ;
}

void Lcd1602_Write_com(uchar com) //命令写入函数
{
    RW = 0;     //选择写入或读取LCD，这里选择写入
    RS = 0;     //选择写入命令或数据，这里选择命令
    E = 0;      //使能
    IO = com;   // IO相当于P0  放入命令到P0
    delay1ms(); //等待数据稳定（等待放入命令完成
    E = 1;      // e提高，IO（P0）数据开始写入
    delay5ms(); //等待写入完成
    E = 0;      // E回归正常，写入完成
}

void Lcd1602_Write_dat(uchar dat) //数据写入函数
{
    RW = 0;     //选择写入或读取LCD，这里选择写入
    RS = 1;     //选择写入命令或数据，这里选择数据
    E = 0;      //使能
    IO = dat;   // IO相当于P0  放入数据到P0
    delay1ms(); //等待数据稳定（等待放入数据完成
    E = 1;      // e提高，IO（P0）数据开始写入
    delay5ms(); //等待写入完成
    E = 0;      // E回归正常，写入完成
}

void Lcd_Init() // LCD显示屏初始化函数
{
    Lcd1602_Write_com(0x38); //开显示
    Lcd1602_Write_com(0x0c); //开显示不显示光标
    Lcd1602_Write_com(0x06); //写一个指针加1
    Lcd1602_Write_com(0x01); //清屏
    Lcd1602_Write_com(0x80); //设置数据指针右上角起点
}
