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
2. move the rtc_lib directory of your STM project directory.
3. include "rtc_lib.h".
```

> Note: Library uses definitions from main.h file. 



## IOC file config

```md
1. Turn on RTC. (Timers -> RTC -> Activate Clock Source)
2. Change the Low-Speed Clock (LSE) to a Crystal/Ceramic Resonator. (RCC Section)
3. Go to "Project Manager" -> "Advanced Settings" and change the RTC code generator to LL.
```

## Functions

1. Init library function.  <br/>
`RTC_Status_t rtc_init_clock(void)` Function change clock source as LSE, sets prescaler to **1Hz**, and sync RTC clock.  <br/>
**Call at the start of the program**.  <br/>
2. Start / Stop functions.  <br/>
`RTC_Status_t rtc_start_clock(void)` Starts RTC Clock.  <br/>
`RTC_Status_t rtc_stop_clock(void)` Stops RTC Clock.  <br/>
3. Time / Date config functions.  <br/>
**Setters**  <br/>
`RTC_Status_t rtc_set_time(RTC_Time_t *m_time)` Sets time, but not save to backup registers. Call `rtc_save_time()` to save current time to memory.  <br/>
`RTC_Status_t rtc_set_date(RTC_Date_t *m_date)` Sets the date, but not save it to backup registers. Call `rtc_save_date()` to save the current date to memory.  <br/>
**Getters**  <br/>
`RTC_Status_t rtc_set_date(RTC_Date_t *m_date)` Returns the current time.  <br/>
`RTC_Status_t rtc_get_date(RTC_Date_t *m_date)` Returns the current date.  <br/>
**Memory**  <br/>
`RTC_Status_t rtc_save_time(void)` Saves current time to BKP0R register.  <br/>
`RTC_Status_t rtc_save_date(void)` Saves current date to BKP1R register.  <br/>
`RTC_Status_t rtc_restore_time(void)` Read the BKP3R register to validate the magic value. If the magic value is correct, load time from the BKP0R register.   <br/>
`RTC_Status_t rtc_restore_date(void)` Read the BKP4R register to validate the magic value. If the magic value is correct, load the date from the BKP1R register.   <br/>

4. Alarms config functions  <br/>
The library uses `ALARM A` and `ALARM B`.  <br/>
`RTC_Status_t rtc_bind_time_alarm(RTC_Time_t *m_time, RTC_Alarm_t alarm_type)` Binds alarm for time.  <br/>
`RTC_Status_t rtc_stop_alarm(RTC_Alarm_t alarm_type)` Stops the alarm and interrupt attached to that alarm.  <br/>
`void rtc_alarm_interrupt_handler(RTC_Alarm_t curr_alarm)` Alarm handler for A and B. Call in `RTC_IRQHandler` or in the while loop.  <br/>
`void rtc_alarm_callback(RTC_Time_t *m_time, RTC_Alarm_t curr_alarm)` This callback calls when alarm works.  <br/>

## Data Types <br/>
1. Functions return status. <br/>
```C
typedef enum
{
	RTC_ERROR = 0,
	RTC_OK = 1
} RTC_Status_t;
```
2. Time init struct.  <br/>
```C 
typedef struct
{
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
} RTC_Time_t;
```
3. Date init struct.  <br/>
```C  
typedef struct
{
	uint8_t year;
	uint8_t month;
	uint8_t day;
} RTC_Date_t;
```
4. Alarm type.  <br/>
```C 
typedef enum
{
	ALARM_A,
	ALARM_B
} RTC_Alarm_t;
```
## Defines  <br/>
Magic values.  <br/>
```C
#define RTC_MEMORY_TIME_MAGICVALUE 0x10F2C
#define RTC_MEMORY_DATE_MAGICVALUE 0x10F3C
```
Backup registers memory blocks.  <br/>
```C
#define RTC_MEMORY_TIME_ADDR 	   0x0
#define RTC_MEMORY_DATE_ADDR 	   0x4
#define RTC_MEMORY_MAGIC_TIME_ADDR 0x8
#define RTC_MEMORY_MAGIC_DATE_ADDR 0xC
```
Timeouts  <br/>
```C
#define WAIT_LSE_ON_TIMEOUT	  1000
#define WAIT_RTC_SYNC_TIMEOUT 1000
#define WAIT_RTC_INIT_TIMEOUT 1000
```

