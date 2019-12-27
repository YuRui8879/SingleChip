#include "common.h"
#include "adc.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//ATK-ESP8266 WIFI模块 公用驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/4/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 


//usmart支持部分
//将收到的AT指令应答数据返回给电脑串口
//mode:0,不清零USART3_RX_STA;
//     1,清零USART3_RX_STA;
u8 atk_8266_at_response(u16 *puplimit, u16 *pdownlimit,u8 *plimitflag,u8 *state)
{
	u8 testopen[] = "TESTOPENOK";
	u8 testclose[] = "TESTCLOSEOK";
	u8 begin[] = "BEGINOK";
	u8 stop[] = "STOPOK";
	u8 limit[] = "LIMITOK";
	u8 unlimit[] = "UNLIMITOK";
	u8 testopenflag = 0;
	u8 testcloseflag = 0;
	static u8 beginflag;
	static u8 stopflag;
	static u8 printflag;
	u8 limitflag = 0;
	u8 unlimitflag = 0;
	u16 upbit = 0;
	u16 downbit = 0;
	u16 uplimit = 0;
	u16 downlimit = 0;
	u8 t = 0;
	
	if(USART3_RX_STA&0X8000)		//接收到一次数据了
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//添加结束符
		printf("%s",USART3_RX_BUF);	//发送到串口
		for(t = 0;t<10;t++)
		{
			if(USART3_RX_BUF[39+t]==testopen[t])
			{
				testopenflag = 1;
			}
			else if(USART3_RX_BUF[12+t]==testopen[t])
			{
				testopenflag = 1;
			}
			else if(USART3_RX_BUF[23+t]==testopen[t])
			{
				testopenflag = 1;
			}
			else
			{
				testopenflag = 0;
				break;
			}
		}
		for(t = 0;t<11;t++)
		{
			if(USART3_RX_BUF[39+t]==testclose[t])
			{
				testcloseflag = 1;
			}
			else if(USART3_RX_BUF[12+t]==testclose[t])
			{
				testcloseflag = 1;
			}
			else if(USART3_RX_BUF[23+t]==testclose[t])
			{
				testcloseflag = 1;
			}
			else
			{
				testcloseflag = 0;
				break;
			}
		}
		for(t =0;t<7;t++)
		{
			if(USART3_RX_BUF[38+t]==begin[t])
			{
				beginflag = 1;
			}
			else if(USART3_RX_BUF[22+t]==begin[t])
			{
				beginflag = 1;
			}
			else if(USART3_RX_BUF[11+t]==begin[t])
			{
				beginflag = 1;
			}
			else
			{
				beginflag = 0;
				break;
			}
		}
		for(t = 0;t<6;t++)
		{
			if(USART3_RX_BUF[38+t]==stop[t])
			{
				stopflag = 1;
			}
			else if(USART3_RX_BUF[22+t]==stop[t])
			{
				stopflag = 1;
			}
			else if(USART3_RX_BUF[11+t]==stop[t])
			{
				stopflag = 1;
			}
			else
			{
				stopflag = 0;
				break;
			}
		}
		for(t = 0;t<7;t++)
		{
			if(USART3_RX_BUF[39+t]==limit[t])
			{
				limitflag = 1;
			}
			else if(USART3_RX_BUF[23+t]==limit[t])
			{
				limitflag = 1;
			}
			else
			{
				limitflag = 0;
				break;
			}
		}
		for(t = 0;t<9;t++)
		{
			if(USART3_RX_BUF[38+t]==unlimit[t])
			{
				unlimitflag = 1;
			}
			else if(USART3_RX_BUF[22+t]==unlimit[t])
			{
				unlimitflag = 1;
			}
			else if(USART3_RX_BUF[11+t]==unlimit[t])
			{
				unlimitflag = 1;
			}
			else
			{
				unlimitflag = 0;
				break;
			}
		}
		USART3_RX_STA=0;
	}
	if(testopenflag)
	{
		*state = 2;
		printf("\n\r打开报警电路");
		GPIO_SetBits(GPIOA,GPIO_Pin_5);
		POINT_COLOR=GREEN;
		LCD_ShowString(114,90,200,16,16,"ON ");
		LED1 = 0;
		return 1; 
	}
	if(testcloseflag)
	{
		*state = 7;
		printf("\n\r关闭报警电路");
		GPIO_ResetBits(GPIOA,GPIO_Pin_5);
		POINT_COLOR=RED;
		LCD_ShowString(114,90,200,16,16,"OFF");
		LED1 = 1;
		return 1;
	}
	if(limitflag)
	{
		*state = 5;
		printf("\r\n温度报警已启动");
		upbit = chartou16(USART3_RX_BUF[46]);
		downbit = chartou16(USART3_RX_BUF[47]);
		for(t = 0;t < upbit;t++)
		{
			uplimit += chartou16(USART3_RX_BUF[48+t]);
			uplimit *= 10;
		}
		uplimit /= 10;
		*puplimit = uplimit;
		for(t = 0;t < downbit;t++)
		{
			downlimit += chartou16(USART3_RX_BUF[48+upbit+t]);
			downlimit *= 10;
		}
		downlimit /= 10;
		*pdownlimit = downlimit;
		*plimitflag = 1;
		POINT_COLOR=GREEN;
		LCD_ShowString(202,130,200,16,16,"ON ");
		return 1;
	}
	if(unlimitflag)
	{
		*state = 6;
		*plimitflag = 0;
		printf("\r\n温度报警已解除");
		POINT_COLOR=RED;
		LCD_ShowString(202,130,200,16,16,"OFF");
		return 1;
	}
	if(beginflag)
	{
		*state = 3;
		Adc_Init();
		POINT_COLOR=GREEN;
		LCD_ShowString(82,110,200,16,16,"ON ");
		printf("\r\nADC已启动");
		stopflag = 0;
		return 1;
	}
	if(stopflag)
	{
		*state = 4;
		ADC_Cmd(ADC1,DISABLE);
		POINT_COLOR=RED;
		LCD_ShowString(82,110,200,16,16,"OFF");
		if(!printflag)
		{
			printf("\r\nADC已停止");
			printflag = 1;
		}		
		beginflag = 0;
		return 0;
	}
	return 1;
}
//ATK-ESP8266发送命令后,检测接收到的应答
//str:期待的应答结果
//返回值:0,没有得到期待的应答结果
//    其他,期待应答结果的位置(str的位置)
u8* atk_8266_check_cmd(u8 *str)
{
	
	char *strx=0;
	if(USART3_RX_STA&0X8000)		//接收到一次数据了
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//添加结束符
		strx=strstr((const char*)USART3_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}
//向ATK-ESP8266发送命令
//cmd:发送的命令字符串
//ack:期待的应答结果,如果为空,则表示不需要等待应答
//waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果)
//       1,发送失败
u8 atk_8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART3_RX_STA=0;
	u3_printf("%s\r\n",cmd);	//发送命令
	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_ms(10);
			if(USART3_RX_STA&0X8000)//接收到期待的应答结果
			{
				if(atk_8266_check_cmd(ack))
				{
					//printf("ack:%s\r\n",(u8*)ack);
					break;//得到有效数据 
				}
					USART3_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
} 
//向ATK-ESP8266发送指定数据
//data:发送的数据(不需要添加回车了)
//ack:期待的应答结果,如果为空,则表示不需要等待应答
//waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果)luojian
u8 atk_8266_send_data(u8 *data,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART3_RX_STA=0;
	u3_printf("%s",data);	//发送命令
	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_ms(10);
			if(USART3_RX_STA&0X8000)//接收到期待的应答结果
			{
				if(atk_8266_check_cmd(ack))break;//得到有效数据 
				USART3_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
}

//ATK-ESP8266退出透传模式
//返回值:0,退出成功;
//       1,退出失败
u8 atk_8266_quit_trans(void)
{
	while((USART3->SR&0X40)==0);	//等待发送空
	USART3->DR='+';      
	delay_ms(15);					//大于串口组帧时间(10ms)
	while((USART3->SR&0X40)==0);	//等待发送空
	USART3->DR='+';      
	delay_ms(15);					//大于串口组帧时间(10ms)
	while((USART3->SR&0X40)==0);	//等待发送空
	USART3->DR='+';      
	delay_ms(500);					//等待500ms
	return atk_8266_send_cmd("AT","OK",20);//退出透传判断.
}
//获取ATK-ESP8266模块的AP+STA连接状态
//返回值:0，未连接;1,连接成功
u8 atk_8266_apsta_check(void)
{
	if(atk_8266_quit_trans())return 0;			//退出透传 
	atk_8266_send_cmd("AT+CIPSTATUS",":",50);	//发送AT+CIPSTATUS指令,查询连接状态
	if(atk_8266_check_cmd("+CIPSTATUS:0")&&
		 atk_8266_check_cmd("+CIPSTATUS:1")&&
		 atk_8266_check_cmd("+CIPSTATUS:2")&&
		 atk_8266_check_cmd("+CIPSTATUS:4"))
		return 0;
	else return 1;
}
//获取ATK-ESP8266模块的连接状态
//返回值:0,未连接;1,连接成功.
u8 atk_8266_consta_check(void)
{
	u8 *p;
	u8 res;
	if(atk_8266_quit_trans())return 0;			//退出透传 
	atk_8266_send_cmd("AT+CIPSTATUS",":",50);	//发送AT+CIPSTATUS指令,查询连接状态
	p=atk_8266_check_cmd("+CIPSTATUS:"); 
	res=*p;									//得到连接状态	
	return res;
}

//ATK-ESP8266模块测试主函数
void atk_8266_test(void)
{
	u16 adcx;
	u8 flag = 1;
	u16 integra;
	u16 little;
	//u8 temp2;
	u8 result[10];
	u8 time = 0;
	u16 uplimit = 0;
	u16 downlimit = 0;
	u16 *puplimit = &uplimit;
	u16 *pdownlimit = &downlimit;
	u8 limitflag = 0;
	u8 *plimitflag = &limitflag;
	u8 state = 1;
	u8 *pstate = &state;
	float temp;
	float showtemp;
	u16 showadcx;
	
	printf("\nwifi模块测试\n\r"); 
	while(atk_8266_send_cmd("AT","OK",20))//检查WIFI模块是否在线
	{
		atk_8266_quit_trans();//退出透传
		atk_8266_send_cmd("AT+CIPMODE=0","OK",200);  //
		//关闭透传模式	
		printf("\n未检测到模块！！！\n");
		delay_ms(800);
		printf("\n尝试链接模块...\n");
	} 
	while(atk_8266_send_cmd("ATE0","OK",200));//关闭回显
	while(atk_8266_send_cmd("AT+CWMODE=2","OK",200));
	while(atk_8266_send_cmd("AT+RST","OK",200));
	while(atk_8266_send_cmd("AT+CWSAP=\"ATK-ESP8266\",\"12345678\",1,4","OK",200));
	while(atk_8266_send_cmd("AT+CIPMUX=1","OK",200));   //0：单连接，1：多连接
	while(atk_8266_send_cmd("AT+CIPSERVER=1,8086","OK",200));
	
	while(1)
	{
		delay_ms(100); 
		if(flag == 1)
		{
			adcx=Get_Adc_Average(ADC_Channel_1,10);
			POINT_COLOR=BLUE;
			LCD_ShowxNum(106,150,adcx,4,16,0);//显示ADC的值
			temp=(float)adcx*(3.3/4096);
			showadcx = temp;
			showtemp = temp;
			LCD_ShowxNum(106,170,showadcx,1,16,0);//显示电压值
			showtemp -= showadcx;
			showtemp *= 1000;
			LCD_ShowxNum(122,170,temp,3,16,0X80);
			
			if(limitflag)
			{
				if(temp > (float)uplimit/1000 || temp < (float)downlimit/1000)
				{
					printf("\r\n%d",uplimit);
					printf("\r\n%d",downlimit);
					printf("\r\n警报");
					GPIO_SetBits(GPIOA,GPIO_Pin_5);
				}
				else
				{
					GPIO_ResetBits(GPIOA,GPIO_Pin_5);
				}
			}
			else
			{
				if(state==2)
				{
					GPIO_SetBits(GPIOA,GPIO_Pin_5);
				}
				else
				{
					GPIO_ResetBits(GPIOA,GPIO_Pin_5);
				}
			}
			integra=temp;
			temp-=integra;
			//little = temp * 1000;
			result[0] = u8tochar(integra);
			//result[1] = '.';
			
			temp *= 10;
			little = temp;
			result[1] = u8tochar(little);
			temp -= little;
			
			temp *= 10;
			little = temp;
			result[2] = u8tochar(little);
			temp -= little;
			
			temp *= 10;
			little = temp;
			result[3] = u8tochar(little);
			//result[4] = '\0';
			
			result[4] = u8tochar(state);
			result[5] = '\0';
			
			printf("\r\n采样值为：%s",result);
			
			
			while(!atk_8266_consta_check())
			{
				delay_ms(10);
				POINT_COLOR=RED;
				LCD_ShowString(98,70,200,16,16,"Disconnect");	
			}
			
			POINT_COLOR=GREEN;
			LCD_ShowString(98,70,200,16,16,"Open");	
			while(atk_8266_send_cmd("AT+CIPSEND=0,5","OK",20));
			delay_ms(20);
			while(atk_8266_send_data(result,"OK",200))
			{
				time++;
				delay_ms(10);
				if(time == 1)
				{
					time = 0;
					break;
				}
			}
			//USART3_RX_STA=0;
			printf("\r\n数据发送成功\n");
			delay_ms(200);
		}
		
		delay_ms(200);
		flag = atk_8266_at_response(puplimit,pdownlimit,plimitflag,pstate);//检查ATK-ESP8266模块发送过来的数据,及时上传给电脑	
		
		LED0=!LED0; 
	} 
}

char u8tochar(u16 num)
{
	char result;
	if(num == 0)
	{
		result = '0';
	}
	else if(num == 1)
	{
		result = '1';
	}
	else if(num == 2)
	{
		result = '2';
	}
	else if(num == 3)
	{
		result = '3';
	}
	else if(num == 4)
	{
		result = '4';
	}
	else if(num == 5)
	{
		result = '5';
	}
	else if(num == 6)
	{
		result = '6';
	}
	else if(num == 7)
	{
		result = '7';
	}
	else if(num == 8)
	{
		result = '8';
	}
	else if(num == 9)
	{
		result = '9';
	}
	else
	{
		printf("error\n");
		result = '\0';
	}
	return result;
}

u16 chartou16(char num)
{
	u16 result;
	if(num == '0')
	{
		result = 0;
	}
	else if(num == '1')
	{
		result = 1;
	}
	else if(num == '2')
	{
		result = 2;
	}
	else if(num == '3')
	{
		result = 3;
	}
	else if(num == '4')
	{
		result = 4;
	}
	else if(num == '5')
	{
		result = 5;
	}
	else if(num == '6')
	{
		result = 6;
	}
	else if(num == '7')
	{
		result = 7;
	}
	else if(num == '8')
	{
		result = 8;
	}
	else if(num == '9')
	{
		result = 9;
	}
	else
	{
		printf("error\n");
	}
	return result;
}


