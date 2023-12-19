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

	RTC_Time_t alarm_time_a = {
		.hours = 15,
		.minutes = 8,
		.seconds = 0
	};

	RTC_Time_t alarm_time_b = {
		.hours = 15,
		.minutes = 8,
		.seconds = 25
	};


	if(!rtc_init_clock())
	{
		errno_handler();
	}

	if(!rtc_start_clock())
	{
		errno_handler();
	}

	if(!rtc_restore_time())
	{
		if(!rtc_set_time(&time_set))
		{
			errno_handler();
		}
		if(!rtc_save_time())
		{
			errno_handler();
		}
	}

	if(!rtc_restore_date())
	{
		if(!rtc_set_date(&date_set))
		{
			errno_handler();
		}
		if(!rtc_save_date())
		{
			errno_handler();
		}
	}

	if(!rtc_get_time(&time_get))
	{
		errno_handler();
	}

	if(!rtc_get_date(&date_get))
	{
		errno_handler();
	}

	if(!(rtc_bind_time_alarm(&alarm_time_a, ALARM_A)))
	{
		errno_handler();
	}

	if(!(rtc_bind_time_alarm(&alarm_time_b, ALARM_B)))
	{
		errno_handler();
	}

	uint32_t last_auto_time = HAL_GetTick();

	for(;;)
	{
		if(HAL_GetTick() - last_auto_time >= 10000)
		{
			if(!rtc_get_time(&time_get))
			{
				errno_handler();
			}
			last_auto_time = HAL_GetTick();
		}

		rtc_alarm_interrupt_handler(ALARM_A);
		rtc_alarm_interrupt_handler(ALARM_B);
	}
}

