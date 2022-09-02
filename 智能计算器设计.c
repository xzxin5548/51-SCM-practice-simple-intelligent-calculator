#include "reg52.h"
#include "lcd.h"
#define Key_Net P1
typedef unsigned char u8;
uchar code number[] = {1, 2, 3, 0x2b - 0x30, 4, 5, 6, 0x2d - 0x30, 7, 8, 9, 0x2a - 0x30, 0, 0x01 - 0x30, 0x3d - 0x30, 0x2f - 0x30}; //输入字模
uchar value;																														// value储存键入的键值
uchar fuhao, vip, check;																											// fuhao 储存+，-，*，/，clr，=；
long Table_A, Table_B;																												// Table_A,Table_B 储存输入的两个数字
long SUM, remainder;																												// SUM储存结果，remainder储存除法余数
u8 Disp1[] = " Error Check /0  ";																									//预备输入的字模
u8 Disp2[] = " Incomplete input";																									//预备输入的字模
void delay10ms()																													//延时函数，误差 0us
{
	unsigned char a, b, c;
	for (c = 5; c > 0; c--)
		for (b = 4; b > 0; b--)
			for (a = 248; a > 0; a--)
				;
}

void Init() // main中用到的初始化
{
	check = 0;
	vip = 0;
	fuhao = 0;
	Table_A = 0;
	Table_B = 0;
}
void Keydown()
{
	char a, i; // a用于长按的判定，i用于做for循环的参数
	i = 0;
	a = 0;
	Key_Net = 0x0f;
	if (Key_Net != 0x0f)
	{
		delay10ms();
		if (Key_Net != 0x0f) //配合前面几句来消除抖动，确保是人按下按键
		{
			Key_Net = 0x0f; // 行列扫描 先进行列扫描，确定是那一列按下
			switch (Key_Net)
			{
			case (0x07):
				value = 0;
				break; //第一列
			case (0x0b):
				value = 1;
				break; //第二列
			case (0x0d):
				value = 2;
				break; //第三列
			case (0x0e):
				value = 3;
				break; //第四列
			}

			Key_Net = 0xf0;
			delay10ms();
			switch (Key_Net) // 行列扫描 再进行行扫描，确定是那一行按下
			{
			case (0x70):
				value = value + 0;
				break; //第一行 就是0 1 2 3个按键按下
			case (0xb0):
				value = value + 4;
				break; //第二行
			case (0xd0):
				value = value + 8;
				break; //第三行
			case (0xe0):
				value = value + 12;
				break; //第四行 这里确定的是哪一个按键按下，还没对按键赋值
			}
			while ((a < 100) && (Key_Net != 0xf0)) //判断按键长按时间，如果超过一秒继续输入
			{
				delay10ms();
				a++;
			}
			a = 0;
		}
		if (vip == 1) // vip变量在写入等号时会改为1，这里可以将等号后键入的按键全改为13号（重置按键）
		{
			value = 13;
		}
		if ((0 <= value) && (value <= 2) || (4 <= value) && (value <= 6) || (8 <= value) && (value <= 10) || value == 12) //根据自己的键盘设计的数据这一条语句 对应的是0-9这几个数子对应的按键
		{																												  //判断是否是数字按键
			if (fuhao == 0)																								  //如果没有键入符号
			{
				Table_A = Table_A * 10 + number[value]; //按下一位储存在个位，再按下一位原来的数*10加上刚按下的值
			}
			if ((fuhao > 0)) //如果键入符号
			{
				Table_B = Table_B * 10 + number[value]; //当有符号位按下后面的值储存在Table_B中
				check = 1;
			}
			Lcd1602_Write_dat(0x30 + number[value]); //显示0-9中按下的值
		}

		if (fuhao == 0) //如果没有按过符号按键
		{
			if (value == 3) //  也就是数组number中的第三位    键盘按下+
			{
				fuhao = 1;								 // +号标志
				Lcd1602_Write_dat(0x30 + number[value]); //显示+号 value==3 就是数组中的 0x2b-0x30 在LCD中0x2b就是+号
			}

			if (value == 7) //  也就是数组number中的第7位     键盘按下-
			{
				fuhao = 2;								 // -号标志
				Lcd1602_Write_dat(0x30 + number[value]); //同理显示-号
			}

			if (value == 11) // 也就是数组number中的第11位   键盘按下*
			{
				fuhao = 3;								 //  *号标志
				Lcd1602_Write_dat(0x30 + number[value]); //同理显示*号
			}

			if (value == 15) //  键盘按下/
			{
				fuhao = 4;								 //  /号标志
				Lcd1602_Write_dat(0x30 + number[value]); //同理显示/号
			}
		}

		if (value == 13) //  键盘按下CLR
		{
			vip = 0; //变量置零
			check = 0;
			Lcd1602_Write_com(0x30 + number[value]);
			fuhao = 0;
			Table_A = 0;
			Table_B = 0;
		}

		if (value == 14) //  键盘按下= 下面直接对各种特殊符号对应的情况进行处理
		{
			if (check == 1)
			{
				if (fuhao == 1) //加法
				{
					SUM = Table_A + Table_B; //计算SUM
					Lcd1602_Write_com(0Xcf); //从第二行最右边开始显示
					Lcd1602_Write_com(0X04); //控制指针左移
					if (SUM == 0)			 //如果得数是0
					{
						Lcd1602_Write_dat(0x30 + 0); //输出0
					}
					else
					{
						while (SUM != 0) //一位一位显示 先从num的最低位开始
						{
							Lcd1602_Write_dat(0x30 + SUM % 10); // num的最后一位在0x4f的位置也就是最右边
							SUM = SUM / 10;						//再取一位的结果数据
						}
					}
					Lcd1602_Write_dat(0x3d); //写等于号
				}

				if (fuhao == 2) //减法
				{
					if (Table_A > Table_B) //先判断A B大小
					{
						SUM = Table_A - Table_B;
					}
					else
					{
						SUM = Table_B - Table_A;
					}
					Lcd1602_Write_com(0Xcf); //从第二行最右边开始显示
					Lcd1602_Write_com(0X04); //控制指针左移
					if (SUM == 0)			 //如果得数是0
					{
						Lcd1602_Write_dat(0x30 + 0); //输出0
					}
					else
					{
						while (SUM != 0) //一位一位显示 先从num的最低位开始
						{
							Lcd1602_Write_dat(0x30 + SUM % 10); //显示结果的最后一位在0x4f的位置
							SUM = SUM / 10;						//取前面的结果数据
						}
						if (Table_A < Table_B) //如果A<B 在结果显示完后需要给一个负号
						{
							Lcd1602_Write_dat(0x2d); // lcd显示负号
						}
					}
					Lcd1602_Write_dat(0x3d); //写等于号
				}

				if (fuhao == 3) //乘法
				{
					SUM = (Table_A) * (Table_B); //算得数
					Lcd1602_Write_com(0Xcf);	 //第二行最后一位显示
					Lcd1602_Write_com(0X04);	 //指针左移
					if (SUM == 0)				 //如果得数是0
					{
						Lcd1602_Write_dat(0x30 + 0); // LCD输出0
					}
					else //如果得数不是0
					{
						while (SUM != 0) //一位一位显示
						{
							Lcd1602_Write_dat(0x30 + SUM % 10); //显示结果的最后一位
							SUM = SUM / 10;						//取前面的结果数据
						}
					}
					Lcd1602_Write_dat(0x3d); //写等于号
				}

				if (fuhao == 4) //除法
				{
					Lcd1602_Write_com(0Xcf); //第二行最后一位显示
					Lcd1602_Write_com(0X04); //指针左移
					if (Table_B == 0)		 //如果除数为0
					{
						for (i = 16; i > 0; i--) // for循环逐个打出Disp字符串里面的字母
						{
							Lcd1602_Write_dat(Disp1[i]); // LCD输出第i位字母
						}
					}
					else //如果除数不为0
					{
						remainder = (Table_A % Table_B); //算余数
						SUM = (Table_A / Table_B);		 //算得数
						if (remainder == 0)				 //如果余数为0（就不需要输出余数了）
						{
							if (SUM == 0) //如果得数为0
							{
								Lcd1602_Write_dat(0x30 + 0); // lcd输出0
							}
							else //如果得数不为0
							{
								while (SUM != 0) //一位一位显示
								{
									Lcd1602_Write_dat(0x30 + SUM % 10); //显示结果的最后一位
									SUM = SUM / 10;						//取前面的结果数据
								}
							}
							Lcd1602_Write_dat(0x3d); //写等于号
						}
						else //如果余数不为0
						{
							while (remainder != 0) //一位一位显示余数
							{
								Lcd1602_Write_dat(0x30 + remainder % 10); //显示结果的最后一位
								remainder = remainder / 10;				  //取前面的结果数据
							}
							Lcd1602_Write_dat(0x2D);
							Lcd1602_Write_dat(0x2D); //打两个杠，分隔开得数和余数
							if (SUM == 0)			 //如果得数为0
							{
								Lcd1602_Write_dat(0x30 + 0); // LCD输出0
							}
							else //如果得数不为0
							{
								while (SUM != 0) //一位一位显示
								{
									Lcd1602_Write_dat(0x30 + SUM % 10); //显示结果的最后一位在0x4f的位置
									SUM = SUM / 10;						//取前面的结果数据
								}
							}
							Lcd1602_Write_dat(0x3d); //写等于号
						}
					}
				}
				vip = 1;
			}
			else
			{
				Lcd1602_Write_com(0Xcf); //第二行最后一位显示
				Lcd1602_Write_com(0X04); //指针左移
				for (i = 16; i > 0; i--)
				{
					Lcd1602_Write_dat(Disp2[i]);
				}
				vip = 1; //将后面的按键全改为13号（重置按键），配合前面if语句
			}
		}
	}
}

void main()
{
	Lcd_Init(); //初始化LCD
	while (1)	//在以下函数中循环
	{
		Keydown();
	}
}
