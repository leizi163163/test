#include "stm32f10x.h"



#define FEBRUARY		2
#define	STARTOFTIME		1970
#define SECDAY			86400L           /*  һ���ж���s */
#define SECYR			(SECDAY * 365)
#define	leapyear(year)		((year) % 4 == 0)
#define	days_in_year(a) 	(leapyear(a) ? 366 : 365)
#define	days_in_month(a) 	(month_days[(a) - 1])

static int month_days[12] = {	31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

/*
 * This only works for the Gregorian calendar - i.e. after 1752 (in the UK)
 */
 /*���㹫��*/
void GregorianDay(struct rtc_time * tm)
{
	int leapsToDate;
	int lastYear;
	int day;
	int MonthOffset[] = { 0,31,59,90,120,151,181,212,243,273,304,334 };

	lastYear=tm->tm_year-1;

	/*����ӹ�ԪԪ�굽������ǰһ��֮��һ�������˶��ٸ�����*/
	leapsToDate = lastYear/4 - lastYear/100 + lastYear/400;      

     /*������������һ��Ϊ���꣬�Ҽ������·���2��֮����������1�����򲻼�1*/
	if((tm->tm_year%4==0) &&
	   ((tm->tm_year%100!=0) || (tm->tm_year%400==0)) &&
	   (tm->tm_mon>2)) {
		/*
		 * We are past Feb. 29 in a leap year
		 */
		day=1;
	} else {
		day=0;
	}

	day += lastYear*365 + leapsToDate + MonthOffset[tm->tm_mon-1] + tm->tm_mday; /*����ӹ�ԪԪ��Ԫ������������һ���ж�����*/

	tm->tm_wday=day%7;
}

/* Converts Gregorian date to seconds since 1970-01-01 00:00:00.
 * Assumes input in normal date format, i.e. 1980-12-31 23:59:59
 * => year=1980, mon=12, day=31, hour=23, min=59, sec=59.
 *
 * [For the Julian calendar (which was used in Russia before 1917,
 * Britain & colonies before 1752, anywhere else before 1582,
 * and is still in use by some communities) leave out the
 * -year/100+year/400 terms, and add 10.]
 *
 * This algorithm was first published by Gauss (I think).
 *
 * WARNING: this function will overflow on 2106-02-07 06:28:16 on
 * machines were long is 32-bit! (However, as time_t is signed, we
 * will already get problems at other places on 2038-01-19 03:14:08)
 *
 */
u32 mktimev(struct rtc_time *tm)
{
	if (0 >= (int) (tm->tm_mon -= 2)) {	/* 1..12 -> 11,12,1..10 */
		tm->tm_mon += 12;		/* Puts Feb last since it has leap day */
		tm->tm_year -= 1;
	}

	return (((
		(u32) (tm->tm_year/4 - tm->tm_year/100 + tm->tm_year/400 + 367*tm->tm_mon/12 + tm->tm_mday) +
			tm->tm_year*365 - 719499
	    )*24 + tm->tm_hour /* now have hours */
	  )*60 + tm->tm_min /* now have minutes */
	)*60 + tm->tm_sec; /* finally seconds */	 
}



void to_tm(u32 tim, struct rtc_time * tm)
{
	register u32    i;
	register long   hms, day;

	day = tim / SECDAY;			/* �ж����� */
	hms = tim % SECDAY;			/* �����ʱ�䣬��λs */

	/* Hours, minutes, seconds are easy */
	tm->tm_hour = hms / 3600;
	tm->tm_min = (hms % 3600) / 60;
	tm->tm_sec = (hms % 3600) % 60;

	/* Number of years in days */ /*�����ǰ��ݣ���ʼ�ļ������Ϊ1970��*/
	for (i = STARTOFTIME; day >= days_in_year(i); i++) {
		day -= days_in_year(i);
	}
	tm->tm_year = i;

	/* Number of months in days left */ /*���㵱ǰ���·�*/
	if (leapyear(tm->tm_year)) {
		days_in_month(FEBRUARY) = 29;
	}
	for (i = 1; day >= days_in_month(i); i++) {
		day -= days_in_month(i);
	}
	days_in_month(FEBRUARY) = 28;
	tm->tm_mon = i;

	/* Days are what is left over (+1) from all that. *//*���㵱ǰ����*/
	tm->tm_mday = day + 1;

	/*
	 * Determine the day of week
	 */
	GregorianDay(tm);
	
}




/*
 * ��������RTC_CheckAndConfig
 * ����  ����鲢����RTC
 * ����  �����ڶ�ȡRTCʱ��Ľṹ��ָ��
 * ���  ����
 * ����  ���ⲿ����
 */
void RTC_CheckAndConfig(struct rtc_time *tm)
{
   	/*������ʱ��鱸�ݼĴ���BKP_DR1��������ݲ���0xA5A5,
	  ������������ʱ�䲢ѯ���û�����ʱ��*/
	if (BKP_ReadBackupRegister(RTC_BKP_DRX) != RTC_BKP_DATA)
	{
		log_title("\r\n\r\n RTC not yet configured....");
		log_title("\r\n\r\n RTC configured....");

		/* ʹ��tm��ʱ������RTC�Ĵ��� */
		Time_Adjust(tm);
		
		/*��BKP_DR1�Ĵ���д���־��˵��RTC��������*/
		BKP_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
	}
	else
	{
		
		/* ʹ�� PWR �� Backup ʱ�� */
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	
		/* ������� Backup ���� */
	  PWR_BackupAccessCmd(ENABLE);
		
	  /*LSE��������������ʱ��*/
		
#ifdef RTC_CLOCK_SOURCE_LSI		
			/* ʹ�� LSI */
			RCC_LSICmd(ENABLE);

			/* �ȴ� LSI ׼���� */
			while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
			{}
#endif

		/*����Ƿ��������*/
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
		{
		    log_title("\r\n\r\n Power On Reset occurred....");
		}
		/*����Ƿ�Reset��λ*/
		else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
		{
			log_title("\r\n\r\n External Reset occurred....");
		}
	
		log_title("\r\n No need to configure RTC....");
		
		/*�ȴ��Ĵ���ͬ��*/
		RTC_WaitForSynchro();
		
		/*����RTC���ж�*/
		RTC_ITConfig(RTC_IT_SEC, ENABLE);
		
		/*�ȴ��ϴ�RTC�Ĵ���д�������*/
		RTC_WaitForLastTask();
	}
	   /*������ʱ������꣬������У��ʱ�������PC13*/
	#ifdef RTCClockOutput_Enable
	/* ʹ�� PWR �� Backup ʱ�� */
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	
	/* ������� Backup ���� */
	  PWR_BackupAccessCmd(ENABLE);
	
	  /* ��ֹ Tamper ���� */
	  /* Ҫ��� RTCCLK/64 �� Tamper ����,  tamper ���ܱ����ֹ */	
	  BKP_TamperPinCmd(DISABLE); 
	
	  /* ʹ�� RTC ʱ������� Tamper ���� */
	  BKP_RTCOutputConfig(BKP_RTCOutputSource_CalibClock);
	#endif
	
	  /* �����λ��־ flags */
	  RCC_ClearFlag();

}



//#define RTC_CLOCK_SOURCE_LSE

/*
 * ��������RTC_Configuration
 * ����  ������RTC
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void RTC_Configuration(void)
{
	/* ʹ�� PWR �� Backup ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	
	/* ������� Backup ���� */
	PWR_BackupAccessCmd(ENABLE);
	
	/* ��λ Backup ���� */
	BKP_DeInit();
	
//ʹ���ⲿʱ�ӻ����ڲ�ʱ�ӣ���bsp_rtc.h�ļ����壩	
//ʹ���ⲿʱ��ʱ������Щ����¾�������
//������Ʒ��ʱ�򣬺����׳����ⲿ����������������̫�ɿ�	
#ifdef 	RTC_CLOCK_SOURCE_LSE
	/* ʹ�� LSE */
	RCC_LSEConfig(RCC_LSE_ON);
	
	/* �ȴ� LSE ׼���� */
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
	{}
	
	/* ѡ�� LSE ��Ϊ RTC ʱ��Դ */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	
	/* ʹ�� RTC ʱ�� */
	RCC_RTCCLKCmd(ENABLE);
	
	/* �ȴ� RTC �Ĵ��� ͬ��
	 * ��ΪRTCʱ���ǵ��ٵģ��ڻ�ʱ���Ǹ��ٵģ�����Ҫͬ��
	 */
	RTC_WaitForSynchro();
	
	/* ȷ����һ�� RTC �Ĳ������ */
	RTC_WaitForLastTask();
	
	/* ʹ�� RTC ���ж� */
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
	
	/* ȷ����һ�� RTC �Ĳ������ */
	RTC_WaitForLastTask();
	
	/* ���� RTC ��Ƶ: ʹ RTC ����Ϊ1s  */
	/* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) = 1HZ */
	RTC_SetPrescaler(32767); 
	
	/* ȷ����һ�� RTC �Ĳ������ */
	RTC_WaitForLastTask();
	
#else

	/* ʹ�� LSI */
	RCC_LSICmd(ENABLE);

	/* �ȴ� LSI ׼���� */
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
	{}
	
	/* ѡ�� LSI ��Ϊ RTC ʱ��Դ */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	
	/* ʹ�� RTC ʱ�� */
	RCC_RTCCLKCmd(ENABLE);
	
	/* �ȴ� RTC �Ĵ��� ͬ��
	 * ��ΪRTCʱ���ǵ��ٵģ��ڻ�ʱ���Ǹ��ٵģ�����Ҫͬ��
	 */
	RTC_WaitForSynchro();
	
	/* ȷ����һ�� RTC �Ĳ������ */
	RTC_WaitForLastTask();
	
	/* ʹ�� RTC ���ж� */
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
	
	/* ȷ����һ�� RTC �Ĳ������ */
	RTC_WaitForLastTask();
	
	/* ���� RTC ��Ƶ: ʹ RTC ����Ϊ1s ,LSIԼΪ40KHz */
	/* RTC period = RTCCLK/RTC_PR = (40 KHz)/(40000-1+1) = 1HZ */	
	RTC_SetPrescaler(40000-1); 
	
	/* ȷ����һ�� RTC �Ĳ������ */
	RTC_WaitForLastTask();
#endif
	
}


/*
 * ��������Time_Adjust
 * ����  ��ʱ�����
 * ����  �����ڶ�ȡRTCʱ��Ľṹ��ָ�루����ʱ�䣩
 * ���  ����
 * ����  ���ⲿ����
 */
void Time_Adjust(struct rtc_time *tm)
{
	
			/* RTC ���� */
	  RTC_Configuration();

	  /* �ȴ�ȷ����һ�β������ */
	  RTC_WaitForLastTask();
		  
	  /* �������� */
	  GregorianDay(tm);

	  /* �����ڼ���ʱ�����д�뵽RTC�����Ĵ��� */
	  RTC_SetCounter(mktimev(tm)-TIME_ZOOM);

	  /* �ȴ�ȷ����һ�β������ */
	  RTC_WaitForLastTask();
}


