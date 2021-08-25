#include "adcspi.h"
/************************************************************************************************
** 函数名称：ADC_SPI_Init_Func
** 输入参数：无
** 输出参数：无
** 功能描述：初始化ADC_SPI的IO口
************************************************************************************************/
u16 ADC_data=0;	
void ADC_SPI_Init_Func(void)
{                                             
                GPIO_InitTypeDef GPIO_InitStructure;
                //PA6->ADC_CS,PA7->ADC_SCLK,PA8->ADC_MOSI  MOSI主设备输出从设备输入
                GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;
                GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     //推挽输出
                GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
                GPIO_Init(GPIOA, &GPIO_InitStructure);
                ADC_SPI_SCL=1;
                ADC_SPI_CS=1;      
}
                                                                             
///************************************************************************************************
//** 函数名称：ADC_SPI_Read_Byte
//** 输入参数：ADC_data:要接收的数据
//** 输出参数：返回参数为adc电压值
//** 功能描述：SPI接收一个16bit，ADC_data数据。
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
	        return_I=(0.0000625f*ADC_data-2.048f)*5.0f;//还未除以电阻

					return return_I;
}            




