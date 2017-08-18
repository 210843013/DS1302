#include "includes.h"
/*************************************
��������DS1302_IO_init()
������������
��������ֵ����
�������ܣ�
��������:
CLK   PD6
DAT   PD7
RST   PG15
**************************************/
void RST_out(void)
{
		//������ʱ��
	RCC->AHB1ENR |=(1<<6);
		/**PG15����Ϊ����ģʽ**/
	//�˿�ģʽ�Ĵ���
	GPIOG->MODER &=~(0x3ul<<30);  //����
	GPIOG->MODER |=(1ul<<30);     //����Ϊͨ�����ģʽ
	//�������
	GPIOG->OTYPER &=~(1<<15);   //�������
	//�������
	GPIOG->OSPEEDR &=~(0x3ul<<30); //����
	GPIOG->OSPEEDR |=(0x2ul<<30); //�������50M
	//������
	GPIOG->PUPDR &=~(0x3ul<<30);  //����
	GPIOG->PUPDR |=1ul<<30;
}
void CLK_out(void)
{
	//������ʱ��
	RCC->AHB1ENR |=(1<<3);
	/**PD6����Ϊ����ģʽ**/
	//�˿�ģʽ�Ĵ���
	GPIOD->MODER &=~(0x3<<12);  //����
	GPIOD->MODER |=(1<<12);     //����Ϊͨ�����ģʽ
	//�������
	GPIOD->OTYPER &=~(1<<6);   //�������
	//�������
	GPIOD->OSPEEDR &=~(0x3<<12); //����
	GPIOD->OSPEEDR |=(0x2<<12); //�������50M
	//������
	GPIOD->PUPDR &=~(0x3<<12);  //����
	GPIOD->PUPDR |=1ul<<12;
	
}

void SDA_out(void)
{
	//������ʱ��
	RCC->AHB1ENR |=(1<<3);
	/**PD7����Ϊ����ģʽ**/
	//�˿�ģʽ�Ĵ���
	GPIOD->MODER &=~(0x3<<14);  //����
	GPIOD->MODER |=(1<<14);     //����Ϊͨ�����ģʽ
	//�������
	GPIOD->OTYPER &=~(1<<7);   //�������
	//�������
	GPIOD->OSPEEDR &=~(0x3<<14); //����
	GPIOD->OSPEEDR |=(0x2<<14); //�������50M
	//������
	GPIOD->PUPDR &=~(0x3<<14);  //����	
	GPIOD->PUPDR |=1ul<<14;
}
void SDA_int(void)
{
	//������ʱ��
	RCC->AHB1ENR |=(1<<3);
	//�˿�ģʽ�Ĵ���
	GPIOD->MODER &=~(0x3<<14);  //����
	//������
	GPIOD->PUPDR &=~(0x3<<14);  //����	
}

/************************************************
*����:DS1302Read()
*˵��: ��д��ַ���������
*����: ��cmd��Ӧ��ַ�ж�ȡһ���ֽڵ�����
*����:DS1302WriteByte(),DS1302ReadByte()
*����:cmd:Ҫд��Ŀ����ֽ�
*���:dat:��ȡ������
*************************************************/
u8 DS1302_Read(u8 cmd)
{
	u8 dat;
	RST_out();
	CLK_out();
	RSTL;//��ʼCE����Ϊ0
	SCLKL;//��ʼʱ������Ϊ0
	RSTH;//��ʼCE��Ϊ1�����俪ʼ
	DS1302_WriteByte(cmd);//���������֣�Ҫ��ȡ��ʱ��/������ַ
	dat=DS1302_ReadByte();//��ȡҪ�õ���ʱ��/����
	SCLKH;//ʱ��������
	RSTL;//��ȡ������ CE��Ϊ0���������ݵĴ���
	return dat;//���صõ���ʱ��/����
}
/************************************************
*����:DS1302Write
*˵��: ��д��ַ����д����
*����: ��cmd��Ӧ��ַ��дһ���ֽڵ�����
*����:DS1302WriteByte()
*����:cmd:Ҫд��Ŀ�����,dat:Ҫд�������
*���: ��
*************************************************/
void DS1302_Write(u8 cmd,u8 dat)
{
	RST_out();
	CLK_out();
	RSTL;//��ʼCE����Ϊ0
	SCLKL;//��ʼʱ������Ϊ0
	RSTH;//��ʼCE��Ϊ1�����俪ʼ
	Delay_us(2);
	DS1302_WriteByte(cmd);//���������֣�Ҫд���ʱ��/������ַ
	DS1302_WriteByte(dat);//д��Ҫ�޸ĵ�ʱ��/����
	SCLKH;//ʱ��������
	RSTL;//��ȡ������ CE��Ϊ0���������ݵĴ���
}
/************************************************
*����:DS1302WriteByte
*˵��: ��
*����: д��8bit����
*����:delayus()
*����:dat:Ҫд�������
*���: ��
*************************************************/
void DS1302_WriteByte(u8 dat)
{
	u8 i;
	SDA_out();
	CLK_out();
	SCLKL;
	Delay_us(2);
	for(i=0;i<8;i++)
	{
		
	  if((dat&(0x01<<i))) //ע��DS1302�����ݺ͵�ַ���Ǵ����λ��ʼ�����
		{
	    SDAH;
		}
		else
		{
			SDAL;
		}
		Delay_us(2);
		SCLKH;
		Delay_us(2);
		SCLKL;
	  
	}
	 
}
/************************************************
*����:DS1302ReadByte()
*˵��: ��
*����: ��ȡ8bit������
*����:delayus()
*����: ��
*���:dat:��ȡ������
*************************************************/
u8 DS1302_ReadByte(void)
{
	u8 i,dat=0;
	SDA_int();
	CLK_out();
	Delay_us(2);
	for(i=0;i<8;i++)
	{ 
		
		if((GPIOD->IDR&(1<<7)))
		{
			dat|=0x01<<i;
	  }
		else
	  {
		   dat&=~(0x01<<i);
	  }
		SCLKH;
		Delay_us(2);
		SCLKL;
		Delay_us(2);
		}
	return dat;
}



/************************************************
*����:void DS1302_init(void)
*˵��: ��1302д��һ����ʼ��ֵ
*����: д�����ڣ���ʱ�ӵ�ֵ
*����:DS1302Write����
*����: ��
*���: ��
*************************************************/
void DS1302_init(void)
{
	DS1302_Write(0x8e,0x00);//д������
	DS1302_Write(DS1302_SECOND_WRITE,0x00); 
	DS1302_Write(DS1302_MINUTE_WRITE,0x29);
	DS1302_Write(DS1302_HOUR_WRITE,0x11);
	DS1302_Write(DS1302_WEEK_WRITE,0x05);
	DS1302_Write(DS1302_DAY_WRITE,0x18); 
	DS1302_Write(DS1302_MONTH_WRITE,0x08);
	DS1302_Write(DS1302_YEAR_WRITE,0x17);
	DS1302_Write(0x90,0x01); //���
	DS1302_Write(0xc0,0xf0); //��ʼ��һ�α�ʾ
	DS1302_Write(0x8e,0x80);
}
/************************************************
��������DisplayTime(void)
������������
��������ֵ����
�������ܣ�
��������: 
*************************************************/
void DisplayTime(void)
{
	u8 str1[20];
	u8 str2[20];
	u8 str3[20];
	u8 year,month,day,hour,minute,second,week;
	u8 temp_year,temp_month,temp_day,temp_hour,temp_minute,temp_second;
	LCD_Init();
	while(1)
	{
	 
  temp_year=DS1302_Read(DS1302_YEAR_READ);
	temp_month=DS1302_Read(DS1302_MONTH_READ);
	temp_day=DS1302_Read(DS1302_DAY_READ);
  temp_hour=DS1302_Read(DS1302_HOUR_READ);
	temp_minute=DS1302_Read(DS1302_MINUTE_READ );
	temp_second=DS1302_Read(DS1302_SECOND_READ);
	week=DS1302_Read(DS1302_WEEK_READ);	
	year=(temp_year/16)*10+temp_year%16;
  month=(temp_month/16)*10+temp_month%16;
  day=(temp_day/16)*10+temp_day%16;
  hour=(temp_hour/16)*10+temp_hour%16;
  minute=(temp_minute/16)*10+temp_minute%16;		
	second=	(temp_second/16)*10+temp_second%16;		
	sprintf((char*)str1,"20%02d��%02d��%02d��",year,month,day);
	sprintf((char*)str2,"%02d:%02d:%02d",hour,(char)minute,second);	
	sprintf((char*)str3,"����%d",(char)week);
	LCD_ShowString(110,140,RED,WHITE,"ʵʱʱ��");
  LCD_ShowString(48,180,RED,WHITE,str1);
	LCD_ShowString(80,212,RED,WHITE,str2);
	LCD_ShowString(104,244,RED,WHITE,str3);
  }
}

