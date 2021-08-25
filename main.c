#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "key.h"
#include "dacspi.h"
#include "lcd.h"
#include "adcspi.h"
#include "hc05.h"
#include "usart3.h"			 	 
#include "string.h"	   
//ALIENTEK Mini STM32开发板范例代码7
//定时器中断实验   
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司



void HC05_Role_Show(void)
{
	if(HC05_Get_Role()==1)LCD_ShowString(30,140,200,16,16,"ROLE:Master");	//主机
	else LCD_ShowString(30,140,200,16,16,"ROLE:Slave ");			 		//从机
}
//显示ATK-HC05模块的连接状态
void HC05_Sta_Show(void)
{												 
	if(HC05_LED)LCD_ShowString(120,140,120,16,16,"STA:Connected ");			//连接成功
	else LCD_ShowString(120,140,120,16,16,"STA:Disconnect");	 			//未连接				 
}	
 int main(void)
 {
	 float data[9];	 
	 u16 D=0;
	 u16 J=0;
	 u16 T=0;
   u8 fuhao_flag=0;
	 u8 decimal_flag=0;
	 u16 K=1;
	  u8 reclen=0;  	
	


	 

	delay_init();	    	 //延时函数初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	
	uart_init(115200);	 	//串口初始化为9600
	
	TIM3_Int_Init(199,7199);//10Khz的计数频率，计数到5000为500ms  
	LED_Init();		  	//初始化与LED连接的硬件接口
	KEY_Init();  
  LCD_Init();				//初始化LCD	 //初始化与按键连接的硬件接口
	 	ADC_SPI_Init_Func();
	 DAC_SPI_Init_Func();
	delay_ms(1000);
	while(HC05_Init()) 		//初始化ATK-HC05模块  
	{
		
		LCD_ShowString(30,90,200,16,16,"ATK-HC05 Error!"); 
		delay_ms(500);
		LCD_ShowString(30,90,200,16,16,"Please Check!!!"); 
		delay_ms(100);
	}	 										   	   
	LCD_ShowString(30,90,210,16,16,"KEY1:ROLE KEY0:SEND/STOP");  
	LCD_ShowString(30,110,200,16,16,"ATK-HC05 Standby!");  
  	LCD_ShowString(30,160,200,16,16,"Send:");	
	LCD_ShowString(30,180,200,16,16,"Receive:"); 
	POINT_COLOR=BLUE;
	HC05_Role_Show();
	delay_ms(100);
	USART3_RX_STA=0;

   	while(1)
	{
 	if(USART3_RX_STA&0X8000)			//接收到一次数据了
		{
			
			
			for(D=0;D<9;D++)
      {
				data[D]=0;
			}		
			D=0;
			
 			reclen=USART3_RX_STA&0X7FFF;	//得到数据长度
		  USART3_RX_BUF[reclen]=0;	 	//加入结束符
			
			if(USART3_RX_BUF[0]==115)//stop
			{
				get_cv_stage(0);
				get_it_stage(0);
				get_dpv_stage(0);
					LED1=!LED1;
			}
			else if(USART3_RX_BUF[0]==99)//cv
			{
			for(J=0;J<reclen;J++)
      {
				switch (fuhao_flag)
				{
					case 0:
					{
						if(USART3_RX_BUF[J]==45)//45-
						{
							fuhao_flag=1;
						}
							else if(USART3_RX_BUF[J]==46)//46.
						{
							decimal_flag=1;
						}
						else if(USART3_RX_BUF[J]==44)//44,
						{
							if(decimal_flag==1)
							{
							data[D]=data[D]/K;
								K=1;
							}
							decimal_flag=0;
							D++;
						}
						else
						{
							if(decimal_flag==0)
							{
							data[D]=data[D]*10.0f+(USART3_RX_BUF[J]-'0');
							}
							else
							{
							K=K*10;
							data[D]=data[D]*10.0f+(USART3_RX_BUF[J]-'0');
							}
						}
						break;
					}
					case 1:
					{
						if(USART3_RX_BUF[J]==46)
						{
							decimal_flag=1;
						}
						else if(USART3_RX_BUF[J]==44)
						{
							if(decimal_flag==1)
							{
							data[D]=data[D]/K;
								K=1;
							}
							decimal_flag=0;
							data[D]=-1.0*data[D];
							D++;
							fuhao_flag=0;
						}
						else
						{
							if(decimal_flag==0)
							{
							data[D]=data[D]*10.0f+(USART3_RX_BUF[J]-'0');
							}
							else
							{
							K=K*10;
							data[D]=data[D]*10.0f+(USART3_RX_BUF[J]-'0');
							}
						}
						break;
					}
					default:
						break;
					}
			
			}
  		get_cv_scope(data[1]*1000,data[2]*1000);
//			printf("%f\r\n",data[0]*1000);
//			printf("%f\r\n",data[1]*1000);
//			printf(",");
			TIM3_Int_Init(data[4],7199);//10Khz的计数频率，计数到5000为500ms 
			get_cv_times(data[5]);
			for(T=0;T<data[6];T++)			
			{
			delay_ms(1000);	
			}
			get_cv_stage(1);
		}
			/////////////////////////////////		
					else if(USART3_RX_BUF[0]==100)//dpv
			{
			for(J=0;J<reclen;J++)
      {
				switch (fuhao_flag)
				{
					case 0:
					{
						if(USART3_RX_BUF[J]==45)//45-
						{
							fuhao_flag=1;
						}
							else if(USART3_RX_BUF[J]==46)//46.
						{
							decimal_flag=1;
						}
						else if(USART3_RX_BUF[J]==44)//44,
						{
							if(decimal_flag==1)
							{
							data[D]=data[D]/K;
								K=1;
							}
							decimal_flag=0;
							D++;
						}
						else
						{
							if(decimal_flag==0)
							{
							data[D]=data[D]*10.0f+(USART3_RX_BUF[J]-'0');
							}
							else
							{
							K=K*10;
							data[D]=data[D]*10.0f+(USART3_RX_BUF[J]-'0');
							}
						}
						break;
					}
					case 1:
					{
						if(USART3_RX_BUF[J]==46)
						{
							decimal_flag=1;
						}
						else if(USART3_RX_BUF[J]==44)
						{
							if(decimal_flag==1)
							{
							data[D]=data[D]/K;
								K=1;
							}
							decimal_flag=0;
							data[D]=-1.0*data[D];
							D++;
							fuhao_flag=0;
						}
						else
						{
							if(decimal_flag==0)
							{
							data[D]=data[D]*10.0f+(USART3_RX_BUF[J]-'0');
							}
							else
							{
							K=K*10;
							data[D]=data[D]*10.0f+(USART3_RX_BUF[J]-'0');
							}
						}
						break;
					}
					default:
						break;
					}
			
			}
			get_dpv_scope(data[5]*1000,data[6]*1000,data[7]*1000,data[1]*1000,data[2]*1000,data[4]*1000,data[3]*1000);

  		
			TIM3_Int_Init(9,7199);//10Khz的计数频率，计数到5000为500ms 
			
			for(T=0;T<data[8];T++)			
			{
			delay_ms(1000);	
			}
			get_dpv_stage(1);
		}
			//////////////////////////////////////////
		else if(USART3_RX_BUF[0]==105)//it
			{
			for(J=0;J<reclen;J++)
      {
				switch (fuhao_flag)
				{
					case 0:
					{
						if(USART3_RX_BUF[J]==45)//45-
						{
							fuhao_flag=1;
						}
							else if(USART3_RX_BUF[J]==46)//46.
						{
							decimal_flag=1;
						}
						else if(USART3_RX_BUF[J]==44)//44,
						{
							if(decimal_flag==1)
							{
							data[D]=data[D]/K;
								K=1;
							}
							decimal_flag=0;
							D++;
						}
						else
						{
							if(decimal_flag==0)
							{
							data[D]=data[D]*10.0f+(USART3_RX_BUF[J]-'0');
							}
							else
							{
							K=K*10;
							data[D]=data[D]*10.0f+(USART3_RX_BUF[J]-'0');
							}
						}
						break;
					}
					case 1:
					{
						if(USART3_RX_BUF[J]==46)
						{
							decimal_flag=1;
						}
						else if(USART3_RX_BUF[J]==44)
						{
							if(decimal_flag==1)
							{
							data[D]=data[D]/K;
								K=1;
							}
							decimal_flag=0;
							data[D]=-1.0*data[D];
							D++;
							fuhao_flag=0;
						}
						else
						{
							if(decimal_flag==0)
							{
							data[D]=data[D]*10.0f+(USART3_RX_BUF[J]-'0');
							}
							else
							{
							K=K*10;
							data[D]=data[D]*10.0f+(USART3_RX_BUF[J]-'0');
							}
						}
						break;
					}
					default:
						break;
					}
			
			}
			get_it_scope(data[1]*1000,data[2],data[4]);

  		
			TIM3_Int_Init(data[3],7199);//10Khz的计数频率，计数到5000为500ms 
			
			for(T=0;T<data[5];T++)			
			{
			delay_ms(1000);	
			}
			get_it_stage(1);
		}
		/////////////////////////////////////////////
 			USART3_RX_STA=0;

		}
		
	}
}
