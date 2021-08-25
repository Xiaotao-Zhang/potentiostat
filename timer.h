#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK Mini STM32开发板
//通用定时器 驱动代码			   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/12/03
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
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
