	#include "includes.h"
	/******************************************************
	* Name�� main����
	* Function��  ��ģ�����ʵ��
	* Description�� 
	* Author��  sun
	******************************************************/	

	int main()
	{
	u8 hour,min,sec,ampm;
	u8 year,month,date,week;
	u8 tbuf[40];
	u8 t=0;  
		Usart1_Init(9600);
		SPI_Init();
		LED_Init();
		LED_ALLOFF();
    LCD_Init();
		RTC_Init();		 			//��ʼ��RTC
	  RTC_Set_WakeUp(4,0);		//����WAKE UP�ж�,1�����ж�һ�� 
		while(1) 
	{		
		t++;
		if((t%10)==0)	//ÿ100ms����һ����ʾ����
		{
			RTC_Get_Time(&hour,&min,&sec,&ampm);
			sprintf((char*)tbuf,"Time:%02d:%02d:%02d",hour,min,sec); 
			LCD_ShowString(30,100,RED,WHITE,tbuf);	
			RTC_Get_Date(&year,&month,&date,&week);
			sprintf((char*)tbuf,"Date:20%02d-%02d-%02d",year,month,date); 
			LCD_ShowString(30,150,RED,WHITE,tbuf);	
			sprintf((char*)tbuf,"Week:%d",week); 
			LCD_ShowString(30,200,RED,WHITE,tbuf);
		} 
		if((t%20)==0)
		LED1_ON;
		LED1_OFF;
		Delay_ms(10);
	}	 

	}
					 




