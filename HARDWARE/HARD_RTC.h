#ifndef	__HARD_RTC_H
#define	__HARD_RTC_H

#include "stm32f10x.h"


#define RTC_BKP_DRX          BKP_DR1
// 写入到备份寄存器的数据宏定义
#define RTC_BKP_DATA         0xA5A5

//北京时间的时区秒数差
#define TIME_ZOOM						(8*60*60)


struct rtc_time {
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
};


void to_tm(u32 tim, struct rtc_time * tm);


void RTC_CheckAndConfig(struct rtc_time *tm);
void RTC_Configuration(void);
void Time_Adjust(struct rtc_time *tm);



#endif




