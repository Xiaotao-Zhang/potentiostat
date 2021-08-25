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
	float I_hou=0.0f;;//ǰ����
	u32 firstdrag_dpv=0;
float DPV_current=0.0f;
u16 DPV_step_already=0;
u16 DPV_step_times=0;
u32 DAC_data_baoliu=voltage_2048v;
u16 flagg=1;//���岨��ͬ��λ�ı�־
u8 dpv_status=0;//�ϵ�Ĭ�Ϲر�dpv
u16 pulse1=0;
u16 pulse2=0;
u16 samplewidth=0;
u16 zhengfu=50;//���һ�����Ϊ50mv
u16 jieyue=4;//��Ծһ�����Ϊ4mv


u32 firstdrag_cv=0;
float DAC_data_float=0.0f;
float Data_V=0.0f;
u32  flag=1;//���ݲ���ͬ��λ�ı�־
float Data_I=0;
u32 DAC_data=voltage_2048v;//dac���Ϊ2.048vʱ���Ӧ��dac����ֵ
u8  cv_times=1;//cvɨ��Ķ�������ʼΪһ��
u8 cv_already_run_times=0;//cv�Ѿ���ɨ���еĶ�������ʼΪ0
u16 CV_step_times=0;
u8 cv_status=0;//cv����״̬��ʶ��0Ϊ��cv��1��ʼcv
u8 fangxian=0;


u32 firstdrag_it=0;
float Data_Time=0.0f;
float Data_itsample=0;
float it_Data_I=0;
u16 it_step_already=0;
u16 it_step_times=0;
u8 it_status=0;//cv����״̬��ʶ��0Ϊ��cv��1��ʼcv

void get_it_scope(float itinit,float itsample,float itruntime)
{
	Data_itsample=0;
															Data_Time=0;
	 it_step_already=0;
	Data_itsample=itsample;
			it_step_times=itruntime/itsample;
	DAC_data=(u16)((2048.0f-itinit)*65535.0f/4096.0f);
	DAC_SPI_Send_Byte(DAC_data);
			it_status=0;//cv����״̬����Ϊ0��������������cv����
      
}
void get_it_stage(u8 begin)//�õ�cv״̬Ϊ��Ϊ1��ʼcv���ε�ʩ��
{
	it_status=begin;
	
}
void get_dpv_stage(u8 begin)//�õ�cv״̬Ϊ��Ϊ1��ʼdpv���ε�ʩ��
{
	dpv_status=begin;
	flagg=1;//���岨��ͬ��λ�ı�־
//	I_qian=0.0f;
//	I_hou=0.0f;;//ǰ����
}

void get_dpv_scope(u16 pulse_w,u16 sample_w,u16 pulse_p,float scope_n,float scope_p,u16 zheng_fu,float jie_yue)//�õ�dpv�߽緶Χ
{
	     samplewidth=sample_w;
	    pulse1=pulse_p-pulse_w;
      pulse2= pulse_p;
			DAC_data=(u16)((2048.0f-scope_n)*65535.0f/4096.0f);
      DAC_data_baoliu=DAC_data;
	    DPV_step_already=0;
			DPV_step_times=(scope_p-scope_n)/jie_yue;
			DAC_SPI_Send_Byte(DAC_data);
			dpv_status=0;//cv����״̬����Ϊ0��������������cv����
//		
}


void get_cv_stage(u8 begin)//�õ�cv״̬Ϊ��Ϊ1��ʼcv���ε�ʩ��
{
	cv_status=begin;
	
}
void get_cv_times(u8 times)//�ɵõ�cvɨ�����
{
	cv_times=times;
}
void get_cv_scope(float scope_n,float scope_p)//�õ�dpv�߽緶Χ
{
	    firstdrag_cv=0;
	    fangxian=0;
			flag=1;
	    cv_already_run_times=0;
			DAC_data=(u16)((2048.0f-scope_n)*65535.0f/4096.0f);

			CV_step_times=(scope_p-scope_n);
	  
			cv_status=0;//cv����״̬����Ϊ0��������������cv����
	    
	    DAC_SPI_Send_Byte(DAC_data);
	    ADC_SPI_Read_Data();
    
}


void TIM7_IRQHandler(void)
{ 	
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)//�Ǹ����ж�
	{	 			   
		USART3_RX_STA|=1<<15;	//��ǽ������
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update  );  //���TIM7�����жϱ�־    
		TIM_Cmd(TIM7, DISABLE);  //�ر�TIM7 
	}	    
}
 
//ͨ�ö�ʱ��7�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ42M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz 
//ͨ�ö�ʱ���жϳ�ʼ��
//����ʼ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��		 
void TIM7_Int_Init(u16 arr,u16 psc)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);//TIM7ʱ��ʹ��    
	
	//��ʱ��TIM7��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM7�ж�,��������ж�
	
	TIM_Cmd(TIM7,ENABLE);//������ʱ��7
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
}
	 




void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM3, //TIM2
		TIM_IT_Update ,
		ENABLE  //ʹ��
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����
							 
}

void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
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
//													case 1:   //Vwe�Ӹ�ѹ����Vwe=0����
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
						if(dpv_status==1)//dcv��ʼ״̬��λ��ʼ����

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
												
												I_hou=ADC_SPI_Read_Data();//�˴���adc����ǰ����Iǰ//�˴���adc���������I��
												flagg++;
			
											}
												else if(flagg==pulse2)
											{
												   DPV_step_already++;  
												  																				     
												if(DPV_step_already==DPV_step_times)//����dpv_scope����-200��600mv�����Ϊ50mv,��ԾΪ4mv������DAC_data�������654mv
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
						if(it_status==1)//dcv��ʼ״̬��λ��ʼ����

			{                           Data_Time=Data_Time+Data_itsample;
				           
																 Data_I=ADC_SPI_Read_Data();	//     _______                   _______Iqian
																 
																		if(firstdrag_it==1)
																		{
																			 u3_printf("%.3f,%.4f,",Data_Time,Data_I); 

													
														      printf("%.3f,%.4f\r\n",Data_Time,Data_I); 
														
																		}
																		firstdrag_it=1;
																		it_step_already++;
											
												
									
												  																				     
												if(it_step_already==it_step_times)//����dpv_scope����-200��600mv�����Ϊ50mv,��ԾΪ4mv������DAC_data�������654mv
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












