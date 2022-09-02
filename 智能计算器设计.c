#include "reg52.h"
#include "lcd.h"
#define Key_Net P1
typedef unsigned char u8;
uchar code number[] = {1, 2, 3, 0x2b - 0x30, 4, 5, 6, 0x2d - 0x30, 7, 8, 9, 0x2a - 0x30, 0, 0x01 - 0x30, 0x3d - 0x30, 0x2f - 0x30}; //������ģ
uchar value;																														// value�������ļ�ֵ
uchar fuhao, vip, check;																											// fuhao ����+��-��*��/��clr��=��
long Table_A, Table_B;																												// Table_A,Table_B �����������������
long SUM, remainder;																												// SUM��������remainder�����������
u8 Disp1[] = " Error Check /0  ";																									//Ԥ���������ģ
u8 Disp2[] = " Incomplete input";																									//Ԥ���������ģ
void delay10ms()																													//��ʱ��������� 0us
{
	unsigned char a, b, c;
	for (c = 5; c > 0; c--)
		for (b = 4; b > 0; b--)
			for (a = 248; a > 0; a--)
				;
}

void Init() // main���õ��ĳ�ʼ��
{
	check = 0;
	vip = 0;
	fuhao = 0;
	Table_A = 0;
	Table_B = 0;
}
void Keydown()
{
	char a, i; // a���ڳ������ж���i������forѭ���Ĳ���
	i = 0;
	a = 0;
	Key_Net = 0x0f;
	if (Key_Net != 0x0f)
	{
		delay10ms();
		if (Key_Net != 0x0f) //���ǰ�漸��������������ȷ�����˰��°���
		{
			Key_Net = 0x0f; // ����ɨ�� �Ƚ�����ɨ�裬ȷ������һ�а���
			switch (Key_Net)
			{
			case (0x07):
				value = 0;
				break; //��һ��
			case (0x0b):
				value = 1;
				break; //�ڶ���
			case (0x0d):
				value = 2;
				break; //������
			case (0x0e):
				value = 3;
				break; //������
			}

			Key_Net = 0xf0;
			delay10ms();
			switch (Key_Net) // ����ɨ�� �ٽ�����ɨ�裬ȷ������һ�а���
			{
			case (0x70):
				value = value + 0;
				break; //��һ�� ����0 1 2 3����������
			case (0xb0):
				value = value + 4;
				break; //�ڶ���
			case (0xd0):
				value = value + 8;
				break; //������
			case (0xe0):
				value = value + 12;
				break; //������ ����ȷ��������һ���������£���û�԰�����ֵ
			}
			while ((a < 100) && (Key_Net != 0xf0)) //�жϰ�������ʱ�䣬�������һ���������
			{
				delay10ms();
				a++;
			}
			a = 0;
		}
		if (vip == 1) // vip������д��Ⱥ�ʱ���Ϊ1��������Խ��Ⱥź����İ���ȫ��Ϊ13�ţ����ð�����
		{
			value = 13;
		}
		if ((0 <= value) && (value <= 2) || (4 <= value) && (value <= 6) || (8 <= value) && (value <= 10) || value == 12) //�����Լ��ļ�����Ƶ�������һ����� ��Ӧ����0-9�⼸�����Ӷ�Ӧ�İ���
		{																												  //�ж��Ƿ������ְ���
			if (fuhao == 0)																								  //���û�м������
			{
				Table_A = Table_A * 10 + number[value]; //����һλ�����ڸ�λ���ٰ���һλԭ������*10���ϸհ��µ�ֵ
			}
			if ((fuhao > 0)) //����������
			{
				Table_B = Table_B * 10 + number[value]; //���з���λ���º����ֵ������Table_B��
				check = 1;
			}
			Lcd1602_Write_dat(0x30 + number[value]); //��ʾ0-9�а��µ�ֵ
		}

		if (fuhao == 0) //���û�а������Ű���
		{
			if (value == 3) //  Ҳ��������number�еĵ���λ    ���̰���+
			{
				fuhao = 1;								 // +�ű�־
				Lcd1602_Write_dat(0x30 + number[value]); //��ʾ+�� value==3 ���������е� 0x2b-0x30 ��LCD��0x2b����+��
			}

			if (value == 7) //  Ҳ��������number�еĵ�7λ     ���̰���-
			{
				fuhao = 2;								 // -�ű�־
				Lcd1602_Write_dat(0x30 + number[value]); //ͬ����ʾ-��
			}

			if (value == 11) // Ҳ��������number�еĵ�11λ   ���̰���*
			{
				fuhao = 3;								 //  *�ű�־
				Lcd1602_Write_dat(0x30 + number[value]); //ͬ����ʾ*��
			}

			if (value == 15) //  ���̰���/
			{
				fuhao = 4;								 //  /�ű�־
				Lcd1602_Write_dat(0x30 + number[value]); //ͬ����ʾ/��
			}
		}

		if (value == 13) //  ���̰���CLR
		{
			vip = 0; //��������
			check = 0;
			Lcd1602_Write_com(0x30 + number[value]);
			fuhao = 0;
			Table_A = 0;
			Table_B = 0;
		}

		if (value == 14) //  ���̰���= ����ֱ�ӶԸ���������Ŷ�Ӧ��������д���
		{
			if (check == 1)
			{
				if (fuhao == 1) //�ӷ�
				{
					SUM = Table_A + Table_B; //����SUM
					Lcd1602_Write_com(0Xcf); //�ӵڶ������ұ߿�ʼ��ʾ
					Lcd1602_Write_com(0X04); //����ָ������
					if (SUM == 0)			 //���������0
					{
						Lcd1602_Write_dat(0x30 + 0); //���0
					}
					else
					{
						while (SUM != 0) //һλһλ��ʾ �ȴ�num�����λ��ʼ
						{
							Lcd1602_Write_dat(0x30 + SUM % 10); // num�����һλ��0x4f��λ��Ҳ�������ұ�
							SUM = SUM / 10;						//��ȡһλ�Ľ������
						}
					}
					Lcd1602_Write_dat(0x3d); //д���ں�
				}

				if (fuhao == 2) //����
				{
					if (Table_A > Table_B) //���ж�A B��С
					{
						SUM = Table_A - Table_B;
					}
					else
					{
						SUM = Table_B - Table_A;
					}
					Lcd1602_Write_com(0Xcf); //�ӵڶ������ұ߿�ʼ��ʾ
					Lcd1602_Write_com(0X04); //����ָ������
					if (SUM == 0)			 //���������0
					{
						Lcd1602_Write_dat(0x30 + 0); //���0
					}
					else
					{
						while (SUM != 0) //һλһλ��ʾ �ȴ�num�����λ��ʼ
						{
							Lcd1602_Write_dat(0x30 + SUM % 10); //��ʾ��������һλ��0x4f��λ��
							SUM = SUM / 10;						//ȡǰ��Ľ������
						}
						if (Table_A < Table_B) //���A<B �ڽ����ʾ�����Ҫ��һ������
						{
							Lcd1602_Write_dat(0x2d); // lcd��ʾ����
						}
					}
					Lcd1602_Write_dat(0x3d); //д���ں�
				}

				if (fuhao == 3) //�˷�
				{
					SUM = (Table_A) * (Table_B); //�����
					Lcd1602_Write_com(0Xcf);	 //�ڶ������һλ��ʾ
					Lcd1602_Write_com(0X04);	 //ָ������
					if (SUM == 0)				 //���������0
					{
						Lcd1602_Write_dat(0x30 + 0); // LCD���0
					}
					else //�����������0
					{
						while (SUM != 0) //һλһλ��ʾ
						{
							Lcd1602_Write_dat(0x30 + SUM % 10); //��ʾ��������һλ
							SUM = SUM / 10;						//ȡǰ��Ľ������
						}
					}
					Lcd1602_Write_dat(0x3d); //д���ں�
				}

				if (fuhao == 4) //����
				{
					Lcd1602_Write_com(0Xcf); //�ڶ������һλ��ʾ
					Lcd1602_Write_com(0X04); //ָ������
					if (Table_B == 0)		 //�������Ϊ0
					{
						for (i = 16; i > 0; i--) // forѭ��������Disp�ַ����������ĸ
						{
							Lcd1602_Write_dat(Disp1[i]); // LCD�����iλ��ĸ
						}
					}
					else //���������Ϊ0
					{
						remainder = (Table_A % Table_B); //������
						SUM = (Table_A / Table_B);		 //�����
						if (remainder == 0)				 //�������Ϊ0���Ͳ���Ҫ��������ˣ�
						{
							if (SUM == 0) //�������Ϊ0
							{
								Lcd1602_Write_dat(0x30 + 0); // lcd���0
							}
							else //���������Ϊ0
							{
								while (SUM != 0) //һλһλ��ʾ
								{
									Lcd1602_Write_dat(0x30 + SUM % 10); //��ʾ��������һλ
									SUM = SUM / 10;						//ȡǰ��Ľ������
								}
							}
							Lcd1602_Write_dat(0x3d); //д���ں�
						}
						else //���������Ϊ0
						{
							while (remainder != 0) //һλһλ��ʾ����
							{
								Lcd1602_Write_dat(0x30 + remainder % 10); //��ʾ��������һλ
								remainder = remainder / 10;				  //ȡǰ��Ľ������
							}
							Lcd1602_Write_dat(0x2D);
							Lcd1602_Write_dat(0x2D); //�������ܣ��ָ�������������
							if (SUM == 0)			 //�������Ϊ0
							{
								Lcd1602_Write_dat(0x30 + 0); // LCD���0
							}
							else //���������Ϊ0
							{
								while (SUM != 0) //һλһλ��ʾ
								{
									Lcd1602_Write_dat(0x30 + SUM % 10); //��ʾ��������һλ��0x4f��λ��
									SUM = SUM / 10;						//ȡǰ��Ľ������
								}
							}
							Lcd1602_Write_dat(0x3d); //д���ں�
						}
					}
				}
				vip = 1;
			}
			else
			{
				Lcd1602_Write_com(0Xcf); //�ڶ������һλ��ʾ
				Lcd1602_Write_com(0X04); //ָ������
				for (i = 16; i > 0; i--)
				{
					Lcd1602_Write_dat(Disp2[i]);
				}
				vip = 1; //������İ���ȫ��Ϊ13�ţ����ð����������ǰ��if���
			}
		}
	}
}

void main()
{
	Lcd_Init(); //��ʼ��LCD
	while (1)	//�����º�����ѭ��
	{
		Keydown();
	}
}
