#ifndef __ADCSPI_H
#define __ADCSPI_H
#include "sys.h"



#include "stm32f10x_i2c.h"
#include "delay.h"


/***************************************IO方向设置**********************************************/
#define ADC_SDA_IN()  {GPIOA->CRH&=0XFFFFFFF0;GPIOA->CRH|=8;}          //8<<12和下面的2<<12都是控制的下面的CNF1,CNF0,MODE1,MODE0;将PA3设置为输入模式，上拉/下拉输入模式，CNF1=1,CNF0=0,MODE1=0,MODE0=0;
#define ADC_SDA_OUT() {GPIOA->CRH&=0XFFFFFFF0;GPIOA->CRH|=2;}          //将PA3设置为通用推挽输出模式，CNF1=0,CNF0=0,MODE1=0,MODE0=0;最大速度2MHz   MODEy[1:0]为00是输入模式，01是输出模式，最大速度10mhz，10输出模式，最大速度2mhz，11输出模式，最大速度50mhz
/***************************************IO操作函数**********************************************/   
/***************************************IO操作函数**********************************************/   
#define ADC_SPI_CS        PAout(6)    //CS
#define ADC_SPI_SCL           PAout(7)    //SCLK

#define ADC_SPI_SDA_IN         PAin(8)    //SDA_in       

/*************************************ADC_SPI所有操作函数*******************************************/
float ADC_SPI_Read_Data(void);

#endif

























