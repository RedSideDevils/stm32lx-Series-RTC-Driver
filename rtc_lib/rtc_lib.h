/*
 * rtc_lib.h
 *
 *  Created on: Dec 15, 2023
 *      Author: armen
 */

#ifndef MYMAIN_LIBS_RTC_LIB_RTC_LIB_H_
#define MYMAIN_LIBS_RTC_LIB_RTC_LIB_H_

#include "main.h"

// ### DEFINE MEMORY ###
#define RTC_MEMORY_TIME_MAGICVALUE 0x10F2C
#define RTC_MEMORY_DATE_MAGICVALUE 0x10F3C

#define RTC_MEMORY_TIME_ADDR 	   0x0
#define RTC_MEMORY_DATE_ADDR 	   0x4
#define RTC_MEMORY_MAGIC_TIME_ADDR 0x8
#define RTC_MEMORY_MAGIC_DATE_ADDR 0xC

#define WAIT_LSE_ON_TIMEOUT	  1000
#define WAIT_RTC_SYNC_TIMEOUT 10000
#define WAIT_RTC_INIT_TIMEOUT 10000

// ### WORKING MODES TYPEDEFS ###
typedef enum
{
	RTC_ERROR = 0,
	RTC_OK = 1
} RTC_Status_t;


typedef struct
{
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
} RTC_Time_t;


typedef struct
{
	uint8_t year;
	uint8_t month;
	uint8_t day;
} RTC_Date_t;


typedef enum
{
	ALARM_A,
	ALARM_B
} RTC_Alarm_t;

// ### INITILIZATION FUNCTION ###
RTC_Status_t rtc_init_clock(void);
RTC_Status_t rtc_start_clock(void);
RTC_Status_t rtc_stop_clock(void);

// ### GETERS SETTERS ###

// ## TIME
RTC_Status_t rtc_set_time(RTC_Time_t *m_time);
RTC_Status_t rtc_get_time(RTC_Time_t *m_time);

// ## DATE
RTC_Status_t rtc_set_date(RTC_Date_t *m_date);
RTC_Status_t rtc_get_date(RTC_Date_t *m_date);

// ## MEMORY
RTC_Status_t rtc_save_time(void);
RTC_Status_t rtc_save_date(void);

RTC_Status_t rtc_restore_time(void);
RTC_Status_t rtc_restore_date(void);

// ### ALARM ###
// Note -> Driver use either ALARM A or ALARM B
RTC_Status_t rtc_bind_time_alarm(RTC_Time_t *m_time,
		RTC_Alarm_t alarm_type);

RTC_Status_t rtc_stop_alarm(RTC_Alarm_t alarm_type);
void rtc_alarm_interrupt_handler(RTC_Alarm_t curr_alarm);

// ### CALLBACKS ###
void rtc_alarm_callback(RTC_Time_t *m_time, RTC_Alarm_t curr_alarm);

#endif /* MYMAIN_LIBS_RTC_LIB_RTC_LIB_H_ */
