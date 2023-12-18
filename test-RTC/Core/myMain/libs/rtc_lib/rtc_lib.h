/*
 * rtc_lib.h
 *
 *  Created on: Dec 15, 2023
 *      Author: armen
 */

#ifndef MYMAIN_LIBS_RTC_LIB_RTC_LIB_H_
#define MYMAIN_LIBS_RTC_LIB_RTC_LIB_H_

#include "main.h"

#define WAIT_LSE_ON_TIMEOUT	1000
#define WAIT_RTC_INIT_TIMEOUT 500

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
RTC_Status_t rtc_save_time(RTC_Date_t *m_time);
RTC_Status_t rtc_save_date(RTC_Date_t *m_date);

RTC_Status_t rtc_restore_time(RTC_Time_t *m_time);
RTC_Status_t rtc_restore_date(RTC_Date_t *m_date);

// ### ALARM ###
// Note -> Driver either ALARM A or ALARM B
RTC_Status_t rtc_bind_time_alarm(RTC_Time_t *m_time,
		RTC_Alarm_t alarm_type);

RTC_Status_t rtc_stop_alarm(void);
void rtc_alarm_interrupt_handler(RTC_Alarm_t curr_alarm);

// ### CALLBACKS ###
void rtc_alarm_callback(RTC_Time_t *m_time, RTC_Alarm_t curr_alarm);

#endif /* MYMAIN_LIBS_RTC_LIB_RTC_LIB_H_ */
