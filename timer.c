#include "timer.h"
#include "led.h"
#include "usart.h"
#include "key.h"
#include "delay.h"
#include "dacspi.h"
#include "adcspi.h"
#include "usart3.h"		

extern vu16 USART3_RX_STA;

float I_qian=0.0f;
	float I_hou=0.0f;;//前电流
	u32 firstdrag_dpv=0;
float DPV_current=0.0f;
u16 DPV_step_already=0;
u16 DPV_step_times=0;
u32 DAC_data_baoliu=voltage_2048v;
u16 flagg=1;//脉冲波不同段位的标志
u8 dpv_status=0;//上电默认关闭dpv
u16 pulse1=0;
u16 pulse2=0;
u16 samplewidth=0;
u16 zhengfu=50;//振幅一般可以为50mv
u16 jieyue=4;//阶跃一般可以为4mv


u32 firstdrag_cv=0;
float DAC_data_float=0.0f;
float Data_V=0.0f;
u32  flag=1;//阶梯波不同段位的标志
float Data_I=0;
u32 DAC_data=voltage_2048v;//dac输出为2.048v时候对应的dac编码值
u8  cv_times=1;//cv扫描的段数，初始为一段
u8 cv_already_run_times=0;//cv已经在扫描中的段数，初始为0
u16 CV_step_times=0;
u8 cv_status=0;//cv工作状态标识，0为不cv，1则开始cv
u8 fangxian=0;


u32 firstdrag_it=0;
float Data_Time=0.0f;
float Data_itsample=0;
float it_Data_I=0;
u16 it_step_already=0;
u16 it_step_times=0;
u8 it_status=0;//cv工作状态标识，0为不cv，1则开始cv

void get_it_scope(float itinit,float itsample,float itruntime)
{
	Data_itsample=0;
															Data_Time=0;
	 it_step_already=0;
	Data_itsample=itsample;
			it_step_times=itruntime/itsample;
	DAC_data=(u16)((2048.0f-itinit)*65535.0f/4096.0f);
	DAC_SPI_Send_Byte(DAC_data);
			it_status=0;//cv开启状态设置为0，不能立即开启cv工作
      
}
void get_it_stage(u8 begin)//得到cv状态为，为1开始cv波形的施加
{
	it_status=begin;
	
}
void get_dpv_stage(u8 begin)//得到cv状态为，为1开始dpv波形的施加
{
	dpv_status=begin;
	flagg=1;//脉冲波不同段位的标志
//	I_qian=0.0f;
//	I_hou=0.0f;;//前电流
}

void get_dpv_scope(u16 pulse_w,u16 sample_w,u16 pulse_p,float scope_n,float scope_p,u16 zheng_fu,float jie_yue)//得到dpv边界范围
{
	     samplewidth=sample_w;
	    pulse1=pulse_p-pulse_w;
      pulse2= pulse_p;
			DAC_data=(u16)((2048.0f-scope_n)*65535.0f/4096.0f);
      DAC_data_baoliu=DAC_data;
	    DPV_step_already=0;
			DPV_step_times=(scope_p-scope_n)/jie_yue;
			DAC_SPI_Send_Byte(DAC_data);
			dpv_status=0;//cv开启状态设置为0，不能立即开启cv工作
//		
}


void get_cv_stage(u8 begin)//得到cv状态为，为1开始cv波形的施加
{
	cv_status=begin;
	
}
void get_cv_times(u8 times)//可得到cv扫描段数
{
	cv_times=times;
}
void get_cv_scope(float scope_n,float scope_p)//得到dpv边界范围
{
	    firstdrag_cv=0;
	    fangxian=0;
			flag=1;
	    cv_already_run_times=0;
			DAC_data=(u16)((2048.0f-scope_n)*65535.0f/4096.0f);

			CV_step_times=(scope_p-scope_n);
	  
			cv_status=0;//cv开启状态设置为0，不能立即开启cv工作
	    
	    DAC_SPI_Send_Byte(DAC_data);
	    ADC_SPI_Read_Data();
    
}


void TIM7_IRQHandler(void)
{ 	
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)//是更新中断
	{	 			   
		USART3_RX_STA|=1<<15;	//标记接收完成
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update  );  //清除TIM7更新中断标志    
		TIM_Cmd(TIM7, DISABLE);  //关闭TIM7 
	}	    
}
 
//通用定时器7中断初始化
//这里时钟选择为APB1的2倍，而APB1为42M
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz 
//通用定时器中断初始化
//这里始终选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数		 
void TIM7_Int_Init(u16 arr,u16 psc)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);//TIM7时钟使能    
	
	//定时器TIM7初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE ); //使能指定的TIM7中断,允许更新中断
	
	TIM_Cmd(TIM7,ENABLE);//开启定时器7
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
}
	 




void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM3, //TIM2
		TIM_IT_Update ,
		ENABLE  //使能
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设
							 
}

void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
			if(cv_status==1)
						{
							
							DAC_SPI_Send_Byte(DAC_data);
							DAC_data_float=DAC_data;
							Data_I=ADC_SPI_Read_Data();
		          Data_V=(2048.0f-DAC_data_float*4096.0f/65535.0f)*0.001f;
							if(firstdrag_cv==1)
							{		
    							  u3_printf("%.3f,%.4f,",Data_V,Data_I); 
////							  u3_printf(","); 
////								u3_printf("%.4f",Data_I);
////							  u3_printf(",");
//								
								 printf("%.3f,%.4f\r\n",Data_V,Data_I); 
							 
							}	
									firstdrag_cv=1;
						

								if(cv_already_run_times<cv_times)
								{
												switch (flag)
												{
//													case 1:   //Vwe从负压向着Vwe=0步进
//													{
//														  if(fangxian==0)
//															{
//															DAC_data=DAC_data-bias_1mv;
//															}
//															else
//															{
//															DAC_data=DAC_data+bias_1mv;
//														  }
//																flag++;
//															break;	          				 
//													}
													
													
													default:
													{  
														flag++;
														if(flag==(CV_step_times+1))
														{
															DAC_data=DAC_data-bias_1mv;
															fangxian=1;
														}

															else if(flag==(2*(CV_step_times+1)-1))
															{
																DAC_data=DAC_data+bias_1mv;
																fangxian=0;
																flag=1;
														    cv_already_run_times++;
															}
															
														 else if(fangxian==0)
																{	
																DAC_data=DAC_data-bias_1mv;
																}
															else
															{
															
																DAC_data=DAC_data+bias_1mv;
															}
                           break;
													}
												} 
								}
								else
								{
									
								  u3_printf("END"); 
									printf("END"); 
									firstdrag_cv=0;
								 
//									DAC_data=voltage_2048v;
									flag=1;
									cv_status=0;
									cv_already_run_times=0;
//									DAC_SPI_Send_Byte(voltage_2048v);
								}

			    	}
							////////////////////////////////////////////
						if(dpv_status==1)//dcv开始状态就位则开始工作

			{
				             if(flagg==1)
						   
												{
												  DAC_SPI_Send_Byte(DAC_data);
												
												  flagg++;
											
											  }
												else if(flagg==(pulse1-samplewidth))
											{
												
												
																 I_qian=ADC_SPI_Read_Data();	//     _______                   _______Iqian
																 DPV_current=I_qian-I_hou;			//		_1	    1___________Ihou__1      1_______________
																 Data_I=DPV_current;
																DAC_data_float=DAC_data;
																		Data_V=(2048.0f-DAC_data_float*4096.0f/65535.0f)*0.001f;
																		if(firstdrag_dpv==1)
																		{
																			 u3_printf("%.3f,%.4f,",Data_V,Data_I); 

													
														 printf("%.3f,%.4f\r\n",Data_V,Data_I); 
														
																		}
																		firstdrag_dpv=1;
						//                       											
																		flagg++;
												
											}
											else if(flagg==pulse1)
											{
												DAC_data=DAC_data-zhengfu*bias_1mv;
												DAC_SPI_Send_Byte(DAC_data);
												flagg++;
				
											}
											
								
											else if(flagg==(pulse2-samplewidth))
											{
												
												I_hou=ADC_SPI_Read_Data();//此处有adc采样前电流I前//此处有adc采样后电流I后
												flagg++;
			
											}
												else if(flagg==pulse2)
											{
												   DPV_step_already++;  
												  																				     
												if(DPV_step_already==DPV_step_times)//比如dpv_scope是在-200到600mv，振幅为50mv,阶跃为4mv，则在DAC_data代表的是654mv
											      {
															 u3_printf("END"); 
									 printf("END"); 
															
															DAC_SPI_Send_Byte(voltage_2048v);
											        dpv_status=0;
															DPV_step_already=0;
													
															flagg=1;
															DAC_data=DAC_data_baoliu;
															firstdrag_dpv=0;
											      }
											  else
														{
															
														    DAC_data=DAC_data+(zhengfu-jieyue)*bias_1mv;  
													      
															
														}
										
												
												flagg=1;
								
											}
											else
											{
												flagg++;
											}
				}
									
//////////////////////////////////////////////////////	
						if(it_status==1)//dcv开始状态就位则开始工作

			{                           Data_Time=Data_Time+Data_itsample;
				           
																 Data_I=ADC_SPI_Read_Data();	//     _______                   _______Iqian
																 
																		if(firstdrag_it==1)
																		{
																			 u3_printf("%.3f,%.4f,",Data_Time,Data_I); 

													
														      printf("%.3f,%.4f\r\n",Data_Time,Data_I); 
														
																		}
																		firstdrag_it=1;
																		it_step_already++;
											
												
									
												  																				     
												if(it_step_already==it_step_times)//比如dpv_scope是在-200到600mv，振幅为50mv,阶跃为4mv，则在DAC_data代表的是654mv
											      {
															 u3_printf("END"); 
									             printf("END"); 
															
															DAC_SPI_Send_Byte(voltage_2048v);
											        it_status=0;
															it_step_already=0;
													Data_itsample=0;
															Data_Time=0;
															firstdrag_it=0;
											      }
											  
										
											
				}				
////////////////////////////////////////////////////////				
				 }

			 }












