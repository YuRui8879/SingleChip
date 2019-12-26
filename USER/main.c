#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h" 		 	 
#include "lcd.h"  
#include "key.h"     
#include "usmart.h" 
#include "malloc.h"
#include "sdio_sdcard.h"  
#include "w25qxx.h"    
#include "ff.h"  
#include "exfuns.h"   
#include "text.h"	
#include "touch.h"		
#include "usart3.h"
#include "common.h" 
#include "adc.h"
#include "timer.h"
 
/************************************************
 ALIENTEK STM32F103������ ��չʵ��5
 ATK-RM04 WIFIģ�����ʵ�� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/


 int main(void)
 {	 
	//u8 key,fontok=0; 	    

	delay_init();	    	 //��ʱ������ʼ��	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
 	//usmart_dev.init(72);		//��ʼ��USMART		
 	LED_Init();		  			//��ʼ����LED���ӵ�Ӳ���ӿ�
	Adc_Init();
	usart3_init(115200);		//��ʼ������3 
	LCD_Init();
	POINT_COLOR=RED;//��������Ϊ��ɫ 
	LCD_ShowString(10,50,200,16,16,"STM32 Program");	
	LCD_ShowString(10,70,200,16,16,"WIFI_State:");	
	LCD_ShowString(10,90,200,16,16,"Output_State:");
	LCD_ShowString(10,110,200,16,16,"AD_State:");
	LCD_ShowString(10,130,200,16,16,"Temperature_Alarm_State:");
	 LCD_ShowString(82,110,200,16,16,"ON");
	 LCD_ShowString(202,130,200,16,16,"OFF");
	 LCD_ShowString(114,90,200,16,16,"OFF");
	//��ʾ��ʾ��Ϣ
	POINT_COLOR=BLUE;//��������Ϊ��ɫ
	LCD_ShowString(10,150,200,16,16,"ADC_CH0_VAL:");	      
	LCD_ShowString(10,170,200,16,16,"ADC_CH0_VOL:0.000V");
	atk_8266_test();		//����ATK_ESP8266����
}


















