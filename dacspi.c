#include "dacspi.h"
#include "adcspi.h"
/************************************************************************************************
** �������ƣ�DAC_SPI_Init_Func
** �����������
** �����������
** ������������ʼ��DAC_SPI��IO��
************************************************************************************************/
void DAC_SPI_Init_Func(void)
{                                             
                GPIO_InitTypeDef GPIO_InitStructure;
                //PA5->DAC_CS,PA2->DAC_SCLK,PA3->DAC_MOSI  MOSI���豸������豸����
                GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_5;
                GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     //�������
                GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
                GPIO_Init(GPIOA, &GPIO_InitStructure);
                DAC_SPI_SCL=0;
                DAC_SPI_CS=1;      
}
                                                                             
///************************************************************************************************
//** �������ƣ�DAC_SPI_Send_Byte
//** ���������DAC_data:Ҫ��������
//** �����������
//** ����������SPI����һ��16bit��DAC_data���ݡ�
//************************************************************************************************/                          
void DAC_SPI_Send_Byte(u16 DAC_data)
{                        
          u8 t;   
          DAC_SDA_OUT(); 
          DAC_SPI_CS=0;
	        delay_us(1);
        
          for(t=0;t<16;t++)
          {              
            	                  DAC_SPI_SCL=0;
                                delay_us(1);                     
						                    DAC_SPI_SDA_OUT=(DAC_data&0x8000)>>15;
                                DAC_data<<=1;           
                                delay_us(1); 
						                    DAC_SPI_SCL=1;
                                delay_us(1); 
          }   
					DAC_SPI_CS=1;
					delay_us(1);	
					DAC_SPI_SCL=0;       					
}            




