#ifndef _LCD_H //��ֹͷ�ļ����ظ������ͱ���
#define _LCD_H

typedef unsigned char uchar; // char����һ������uchar
typedef unsigned int uint;   // int����һ������uint

#define IO P0 //����P0��ΪIO
sbit RW = P2 ^ 5;
sbit RS = P2 ^ 6;
sbit E = P2 ^ 7;

void Lcd1602_Write_com(uchar com);
void Lcd1602_Write_dat(uchar dat);
void Lcd_Init();

#endif
