#include "myMain.h"

void errno_handler(void)
{
	while(1);
}

void myMain(void)
{
	RTC_Time_t time_set = {
			.hours = 15,
			.minutes = 7,
			.seconds = 0
	};

	RTC_Time_t time_get = {0};
	RTC_Date_t date_set = {
			.day = 22,
			.month = 11,
			.year = 24
	};
	RTC_Date_t date_get = {0};

	RTC_Time_t alarm_time = {
		.hours = 15,
		.minutes = 8,
		.seconds = 0
	};

	if(!rtc_init_clock())
	{
		errno_handler();
	}

	if(!rtc_start_clock())
	{
		errno_handler();
	}

	if(!rtc_set_time(&time_set))
	{
		errno_handler();
	}

	if(!rtc_set_date(&date_set))
	{
		errno_handler();
	}

	HAL_Delay(5000);

	if(!rtc_get_time(&time_get))
	{
		errno_handler();
	}

	if(!rtc_get_date(&date_get))
	{
		errno_handler();
	}

	if(!rtc_bind_time_alarm(&alarm_time, ALARM_B))
	{
		errno_handler();
	}

	for(;;)
	{
		rtc_alarm_interrupt_handler(ALARM_B);
	}
}
