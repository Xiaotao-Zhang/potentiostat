#include "dacspi.h"
#include "adcspi.h"
/************************************************************************************************
** 函数名称：DAC_SPI_Init_Func
** 输入参数：无
** 输出参数：无
** 功能描述：初始化DAC_SPI的IO口
************************************************************************************************/
void DAC_SPI_Init_Func(void)
{                                             
                GPIO_InitTypeDef GPIO_InitStructure;
                //PA5->DAC_CS,PA2->DAC_SCLK,PA3->DAC_MOSI  MOSI主设备输出从设备输入
                GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_5;
                GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     //推挽输出
                GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
                GPIO_Init(GPIOA, &GPIO_InitStructure);
                DAC_SPI_SCL=0;
                DAC_SPI_CS=1;      
}
                                                                             
///************************************************************************************************
//** 函数名称：DAC_SPI_Send_Byte
//** 输入参数：DAC_data:要发送数据
//** 输出参数：无
//** 功能描述：SPI发送一个16bit，DAC_data数据。
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




