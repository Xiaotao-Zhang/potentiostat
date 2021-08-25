#include "adcspi.h"
/************************************************************************************************
** �������ƣ�ADC_SPI_Init_Func
** �����������
** �����������
** ������������ʼ��ADC_SPI��IO��
************************************************************************************************/
u16 ADC_data=0;	
void ADC_SPI_Init_Func(void)
{                                             
                GPIO_InitTypeDef GPIO_InitStructure;
                //PA6->ADC_CS,PA7->ADC_SCLK,PA8->ADC_MOSI  MOSI���豸������豸����
                GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;
                GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     //�������
                GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
                GPIO_Init(GPIOA, &GPIO_InitStructure);
                ADC_SPI_SCL=1;
                ADC_SPI_CS=1;      
}
                                                                             
///************************************************************************************************
//** �������ƣ�ADC_SPI_Read_Byte
//** ���������ADC_data:Ҫ���յ�����
//** ������������ز���Ϊadc��ѹֵ
//** ����������SPI����һ��16bit��ADC_data���ݡ�
//************************************************************************************************/                          
float ADC_SPI_Read_Data(void)
{         
       
	        float return_I=0;
          u8 t=0;
	        ADC_data=0;	
          ADC_SDA_IN(); 
          ADC_SPI_CS=0;
          delay_us(1);
          for(t=0;t<16;t++)
          {              
            	                  ADC_SPI_SCL=1;
                                delay_us(1);              
						                    if(ADC_SPI_SDA_IN)
																{
																	ADC_data++;
																}
                                ADC_data<<=1;           
                                delay_us(1); 
						                    ADC_SPI_SCL=0;
                                delay_us(1); 
          }   
					ADC_SPI_CS=1;
					delay_us(1);	
					ADC_SPI_SCL=1; 
	        return_I=(0.0000625f*ADC_data-2.048f)*5.0f;//��δ���Ե���

					return return_I;
}            




