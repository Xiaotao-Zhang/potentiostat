#ifndef __DACSPI_H
#define __DACSPI_H
#include "sys.h"



#include "stm32f10x_i2c.h"
#include "delay.h"


/***************************************IO方向设置**********************************************/
#define DAC_SDA_IN()  {GPIOA->CRL&=0XFFFF0FFF;GPIOA->CRL|=8<<12;}          //8<<12和下面的2<<12都是控制的下面的CNF1,CNF0,MODE1,MODE0;将PA3设置为输入模式，上拉/下拉输入模式，CNF1=1,CNF0=0,MODE1=0,MODE0=0;
#define DAC_SDA_OUT() {GPIOA->CRL&=0XFFFF0FFF;GPIOA->CRL|=2<<12;}          //将PA3设置为通用推挽输出模式，CNF1=0,CNF0=0,MODE1=0,MODE0=0;最大速度2MHz   MODEy[1:0]为00是输入模式，01是输出模式，最大速度10mhz，10输出模式，最大速度2mhz，11输出模式，最大速度50mhz
/***************************************IO操作函数**********************************************/   
/***************************************IO操作函数**********************************************/   
#define DAC_SPI_CS        PAout(5)    //CS
#define DAC_SPI_SCL           PAout(2)    //SCLK

#define DAC_SPI_SDA_OUT          PAout(3)    //SDA        

/*************************************DAC_SPI所有操作函数*******************************************/
void DAC_SPI_Send_Byte(u16 DAC_data);

#endif

























