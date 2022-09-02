#include "reg52.h"
#include "lcd.h"

void delay1ms(void) //��ʱ����1ms ��� 0us
{
    unsigned char a, b;
    for (b = 199; b > 0; b--)
        for (a = 1; a > 0; a--)
            ;
}
void delay5ms(void) //��ʱ����5ms ��� 0us
{
    unsigned char a, b;
    for (b = 185; b > 0; b--)
        for (a = 12; a > 0; a--)
            ;
}

void Lcd1602_Write_com(uchar com) //����д�뺯��
{
    RW = 0;     //ѡ��д����ȡLCD������ѡ��д��
    RS = 0;     //ѡ��д����������ݣ�����ѡ������
    E = 0;      //ʹ��
    IO = com;   // IO�൱��P0  �������P0
    delay1ms(); //�ȴ������ȶ����ȴ������������
    E = 1;      // e��ߣ�IO��P0�����ݿ�ʼд��
    delay5ms(); //�ȴ�д�����
    E = 0;      // E�ع�������д�����
}

void Lcd1602_Write_dat(uchar dat) //����д�뺯��
{
    RW = 0;     //ѡ��д����ȡLCD������ѡ��д��
    RS = 1;     //ѡ��д����������ݣ�����ѡ������
    E = 0;      //ʹ��
    IO = dat;   // IO�൱��P0  �������ݵ�P0
    delay1ms(); //�ȴ������ȶ����ȴ������������
    E = 1;      // e��ߣ�IO��P0�����ݿ�ʼд��
    delay5ms(); //�ȴ�д�����
    E = 0;      // E�ع�������д�����
}

void Lcd_Init() // LCD��ʾ����ʼ������
{
    Lcd1602_Write_com(0x38); //����ʾ
    Lcd1602_Write_com(0x0c); //����ʾ����ʾ���
    Lcd1602_Write_com(0x06); //дһ��ָ���1
    Lcd1602_Write_com(0x01); //����
    Lcd1602_Write_com(0x80); //��������ָ�����Ͻ����
}
