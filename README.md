# Low level RTC Driver for STM32L0 Series 
Made by **Trippingcarpet** 

[![Build Status](https://app.travis-ci.com/microsoft/DynamicProto-JS.svg?branch=master)](https://github.com/RedSideDevils/dma_driver_for_stm32/releases/tag/V1.0.0) 
> Note: Tested with `STM32L071KBT6`

## Features
- Compatible with all **STM32L0** series.
- Fast solution for your STM projects.
- Easy integratation.
 
> Note: Library use LSE as clock source. (Freq: 32.768 KHz)

## Installation

``` sh
1. git clone https://github.com/RedSideDevils/stm32lx-Series-RTC-Driver.git
2. move rtc_lib directory of your STM project directory.
3. include "rtc_lib.h".
```

> Note: Library use definitions from main.h file. 



## IOC file config

```md
1. Turn on RTC. (Timers -> RTC -> Activate Clock Source)
2. Change Low Speed Clock (LSE) to Crystal/Ceramic Resonator. (RCC Section)
3. Go to "Project Manager" -> "Advanced Settings" and change RTC code generator to LL.
```

## Functions

1. Init library function.
`RTC_Status_t rtc_init_clock(void)` Function change clock source as LSE, sets prescaler to **1Hz**, and sync RTC clock. 
**Call at the start of program**.
2. Start / Stop functions.
`RTC_Status_t rtc_start_clock(void)` Starts RTC Clock.
`RTC_Status_t rtc_stop_clock(void)` Stops RTC Clock.
3. Time / Date config functions.
**Setters**
`RTC_Status_t rtc_set_time(RTC_Time_t *m_time)` Sets time, but not save to backup registers. Call `rtc_save_time()` to save current time to memory.
`RTC_Status_t rtc_set_date(RTC_Date_t *m_date)` Sets date, but not save to backup registers. Call `rtc_save_date()` to save current date to memory.
**Getters**
`RTC_Status_t rtc_set_date(RTC_Date_t *m_date)` Returns current time.
`RTC_Status_t rtc_get_date(RTC_Date_t *m_date)` Returns current date.
**Memory**
`RTC_Status_t rtc_save_time(void)` Saves current time to BKP0R register.
`RTC_Status_t rtc_save_date(void)` Saves current date to BKP1R register.
`RTC_Status_t rtc_restore_time(void)` Read BKP3R register to validate magic value. If magic value is correct, loads time from BKP0R register. 
`RTC_Status_t rtc_restore_date(void)` Read BKP4R register to validate magic value. If magic value is correct, loads date from BKP1R register. 

4. Alarms config functions
The library use `ALARM A` and `ALARM B`.
`RTC_Status_t rtc_bind_time_alarm(RTC_Time_t *m_time, RTC_Alarm_t alarm_type)` Binds alarm for time.
`RTC_Status_t rtc_stop_alarm(RTC_Alarm_t alarm_type)` Stops alarm and interrupt attached to that alarm.
`void rtc_alarm_interrupt_handler(RTC_Alarm_t curr_alarm)` Alarm handler for A and B. Call in `RTC_IRQHandler` or in while loop.
`void rtc_alarm_callback(RTC_Time_t *m_time, RTC_Alarm_t curr_alarm)` This callback calls when alarm works.

## Data Types
1. Functions return status.
```C
typedef enum
{
	RTC_ERROR = 0,
	RTC_OK = 1
} RTC_Status_t;
```
2. Time init struct.
```C
typedef struct
{
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
} RTC_Time_t;
```
3. Date init struct.
```C
typedef struct
{
	uint8_t year;
	uint8_t month;
	uint8_t day;
} RTC_Date_t;
```
4. Alarm type.
```C
typedef enum
{
	ALARM_A,
	ALARM_B
} RTC_Alarm_t;
```
## Defines
Magic values.
```C
#define RTC_MEMORY_TIME_MAGICVALUE 0x10F2C
#define RTC_MEMORY_DATE_MAGICVALUE 0x10F3C
```
Backup registers memory blocks.
```C
#define RTC_MEMORY_TIME_ADDR 	   0x0
#define RTC_MEMORY_DATE_ADDR 	   0x4
#define RTC_MEMORY_MAGIC_TIME_ADDR 0x8
#define RTC_MEMORY_MAGIC_DATE_ADDR 0xC
```
Timeouts
```C
#define WAIT_LSE_ON_TIMEOUT	  1000
#define WAIT_RTC_SYNC_TIMEOUT 1000
#define WAIT_RTC_INIT_TIMEOUT 1000
```

