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
//ALIENTEK Mini STM32�����巶������7
//��ʱ���ж�ʵ��   
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾



void HC05_Role_Show(void)
{
	if(HC05_Get_Role()==1)LCD_ShowString(30,140,200,16,16,"ROLE:Master");	//����
	else LCD_ShowString(30,140,200,16,16,"ROLE:Slave ");			 		//�ӻ�
}
//��ʾATK-HC05ģ�������״̬
void HC05_Sta_Show(void)
{												 
	if(HC05_LED)LCD_ShowString(120,140,120,16,16,"STA:Connected ");			//���ӳɹ�
	else LCD_ShowString(120,140,120,16,16,"STA:Disconnect");	 			//δ����				 
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
	


	 

	delay_init();	    	 //��ʱ������ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ9600
	
	TIM3_Int_Init(199,7199);//10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms  
	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();  
  LCD_Init();				//��ʼ��LCD	 //��ʼ���밴�����ӵ�Ӳ���ӿ�
	 	ADC_SPI_Init_Func();
	 DAC_SPI_Init_Func();
	delay_ms(1000);
	while(HC05_Init()) 		//��ʼ��ATK-HC05ģ��  
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
 	if(USART3_RX_STA&0X8000)			//���յ�һ��������
		{
			
			
			for(D=0;D<9;D++)
      {
				data[D]=0;
			}		
			D=0;
			
 			reclen=USART3_RX_STA&0X7FFF;	//�õ����ݳ���
		  USART3_RX_BUF[reclen]=0;	 	//���������
			
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
			TIM3_Int_Init(data[4],7199);//10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms 
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

  		
			TIM3_Int_Init(9,7199);//10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms 
			
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

  		
			TIM3_Int_Init(data[3],7199);//10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms 
			
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
