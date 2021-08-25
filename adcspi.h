#ifndef __ADCSPI_H
#define __ADCSPI_H
#include "sys.h"



#include "stm32f10x_i2c.h"
#include "delay.h"


/***************************************IO��������**********************************************/
#define ADC_SDA_IN()  {GPIOA->CRH&=0XFFFFFFF0;GPIOA->CRH|=8;}          //8<<12�������2<<12���ǿ��Ƶ������CNF1,CNF0,MODE1,MODE0;��PA3����Ϊ����ģʽ������/��������ģʽ��CNF1=1,CNF0=0,MODE1=0,MODE0=0;
#define ADC_SDA_OUT() {GPIOA->CRH&=0XFFFFFFF0;GPIOA->CRH|=2;}          //��PA3����Ϊͨ���������ģʽ��CNF1=0,CNF0=0,MODE1=0,MODE0=0;����ٶ�2MHz   MODEy[1:0]Ϊ00������ģʽ��01�����ģʽ������ٶ�10mhz��10���ģʽ������ٶ�2mhz��11���ģʽ������ٶ�50mhz
/***************************************IO��������**********************************************/   
/***************************************IO��������**********************************************/   
#define ADC_SPI_CS        PAout(6)    //CS
#define ADC_SPI_SCL           PAout(7)    //SCLK

#define ADC_SPI_SDA_IN         PAin(8)    //SDA_in       

/*************************************ADC_SPI���в�������*******************************************/
float ADC_SPI_Read_Data(void);

#endif

























