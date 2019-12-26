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
 ALIENTEK STM32F103开发板 扩展实验5
 ATK-RM04 WIFI模块测试实验 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/


 int main(void)
 {	 
	//u8 key,fontok=0; 	    

	delay_init();	    	 //延时函数初始化	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
 	//usmart_dev.init(72);		//初始化USMART		
 	LED_Init();		  			//初始化与LED连接的硬件接口
	Adc_Init();
	usart3_init(115200);		//初始化串口3 
	LCD_Init();
	POINT_COLOR=RED;//设置字体为红色 
	LCD_ShowString(10,50,200,16,16,"STM32 Program");	
	LCD_ShowString(10,70,200,16,16,"WIFI_State:");	
	LCD_ShowString(10,90,200,16,16,"Output_State:");
	LCD_ShowString(10,110,200,16,16,"AD_State:");
	LCD_ShowString(10,130,200,16,16,"Temperature_Alarm_State:");
	 LCD_ShowString(82,110,200,16,16,"ON");
	 LCD_ShowString(202,130,200,16,16,"OFF");
	 LCD_ShowString(114,90,200,16,16,"OFF");
	//显示提示信息
	POINT_COLOR=BLUE;//设置字体为蓝色
	LCD_ShowString(10,150,200,16,16,"ADC_CH0_VAL:");	      
	LCD_ShowString(10,170,200,16,16,"ADC_CH0_VOL:0.000V");
	atk_8266_test();		//进入ATK_ESP8266测试
}


















