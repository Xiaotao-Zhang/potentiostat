#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK Mini STM32������
//ͨ�ö�ʱ�� ��������			   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/12/03
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  
#define voltage_2048v 32768
#define bias_1mv  16
void get_dpv_stage(u8 begin);
void get_dpv_scope(u16 pulse_w,u16 sample_w,u16 pulse_p,float scope_n,float scope_p,u16 zheng_fu,float jie_yue);
void TIM3_Int_Init(u16 arr,u16 psc); 
void get_cv_stage(u8 begin);
void get_cv_times(u8 times);
void get_cv_scope(float scope_n,float scope_p);
void get_it_scope(float itinit,float itsample,float itruntime);
void get_it_stage(u8 begin);
#endif
