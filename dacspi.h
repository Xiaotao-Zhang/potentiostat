#ifndef __DACSPI_H
#define __DACSPI_H
#include "sys.h"



#include "stm32f10x_i2c.h"
#include "delay.h"


/***************************************IO��������**********************************************/
#define DAC_SDA_IN()  {GPIOA->CRL&=0XFFFF0FFF;GPIOA->CRL|=8<<12;}          //8<<12�������2<<12���ǿ��Ƶ������CNF1,CNF0,MODE1,MODE0;��PA3����Ϊ����ģʽ������/��������ģʽ��CNF1=1,CNF0=0,MODE1=0,MODE0=0;
#define DAC_SDA_OUT() {GPIOA->CRL&=0XFFFF0FFF;GPIOA->CRL|=2<<12;}          //��PA3����Ϊͨ���������ģʽ��CNF1=0,CNF0=0,MODE1=0,MODE0=0;����ٶ�2MHz   MODEy[1:0]Ϊ00������ģʽ��01�����ģʽ������ٶ�10mhz��10���ģʽ������ٶ�2mhz��11���ģʽ������ٶ�50mhz
/***************************************IO��������**********************************************/   
/***************************************IO��������**********************************************/   
#define DAC_SPI_CS        PAout(5)    //CS
#define DAC_SPI_SCL           PAout(2)    //SCLK

#define DAC_SPI_SDA_OUT          PAout(3)    //SDA        

/*************************************DAC_SPI���в�������*******************************************/
void DAC_SPI_Send_Byte(u16 DAC_data);

#endif

























