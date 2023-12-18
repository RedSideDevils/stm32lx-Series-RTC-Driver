/* MADE BY TRIPPINGCARPET */
/*
 * GITHUB
 * https://github.com/RedSideDevils
 *
 */
#include "rtc_lib.h"

// ### PRIVATE FUNCTIONS ####
void rtc_enable_protection(void);
void rtc_disable_protection(void);
uint8_t dec_to_bcd(uint8_t val);
uint8_t bcd_to_dec(uint8_t bcd);
// ##########################


RTC_Status_t rtc_init_clock(void)
{
	// ### ENABLE PWR CLOCK ###
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;

	// ### ENABLE ACCESS TO RTC AND BACKUP REGISTERS ###
	PWR->CR |= PWR_CR_DBP;

	// ### RESET BACKUP DOMAIN ###
	RCC->CSR |= RCC_CSR_RTCRST;
	RCC->CSR &= ~RCC_CSR_RTCRST;

	// ### SET Driving capability to medium high ###
    RCC->CSR &= ~RCC_CSR_LSEDRV_Msk;
    RCC->CSR |= (0x02 << RCC_CSR_LSEDRV_Pos);

    // ### LSE CLOCK START ###
    RCC->CSR |= RCC_CSR_LSEON;

    // ### CHECK IF LSE CLOCK IS READY ###
    uint32_t now = HAL_GetTick();

    while((RCC->CSR & RCC_CSR_LSERDY) != RCC_CSR_LSERDY)
    {
    	if(HAL_GetTick() - now >= WAIT_LSE_ON_TIMEOUT)
    	{
    		return RTC_ERROR;
    	}
    }

    // ### SET PRESCALER TO 1HZ ###
    RTC->PRER = (127 << RTC_PRER_PREDIV_S_Pos) | (255 << RTC_PRER_PREDIV_A_Pos);

    while (!(RTC->ISR & RTC_ISR_RSF))
    {
    	if(HAL_GetTick() - now >= 1000)
    	{
    		return RTC_ERROR;
    	}
    }

    // ### CHANGING RTC SOURCE TO LSE ###
    RCC->CSR &= ~RCC_CSR_RTCSEL_Msk;
    RCC->CSR |= RCC_CSR_RTCSEL_LSE;

    return RTC_OK;
}


RTC_Status_t rtc_start_clock(void)
{
    // ### ENABLE RTC CLOCK ###
    RCC->CSR |= RCC_CSR_RTCEN;

    // ### WAIT UNTIL RTC IS SYNC ###
    uint32_t now = HAL_GetTick();

    while (!(RTC->ISR & RTC_ISR_RSF))
    {
    	if(HAL_GetTick() - now >= 10000)
    	{
    		return RTC_ERROR;
    	}
    }

    return RTC_OK;
}


RTC_Status_t rtc_stop_clock(void)
{
    // ### DISABLE RTC CLOCK ###
    RCC->CSR &= ~RCC_CSR_RTCEN;

    // ### DISABLE ACCESS TO RTC AND BACKUP REGISTERS ###
	PWR->CR &= ~PWR_CR_DBP;

	// ### DISABLE PWR CLOCK ###
	RCC->APB1ENR &= ~RCC_APB1ENR_PWREN;
    return RTC_OK;
}


RTC_Status_t rtc_set_time(RTC_Time_t *m_time)
{
	// ### ENABLE WRITE ACCESS ###
	rtc_disable_protection();

	// ### ENABLE RTC INIT MODE ###
	RTC->ISR |= RTC_ISR_INIT;

	// ### WAIT WHILE RTC INIT ###
	uint32_t now = HAL_GetTick();

	while (!(RTC->ISR & RTC_ISR_INITF))
	{
    	if(HAL_GetTick() - now >= 1000)
    	{
    		return RTC_ERROR;
    	}
	}

	// ### SET BCD TIME TO REGISTER ###
	RTC->TR = ((m_time->hours / 10) << RTC_TR_HT_Pos) | ((m_time->hours % 10) << RTC_TR_HU_Pos) |
			  ((m_time->minutes / 10) << RTC_TR_MNT_Pos) | ((m_time->minutes % 10) << RTC_TR_MNU_Pos) |
			  ((m_time->seconds / 10) << RTC_TR_ST_Pos) | (m_time->seconds % 10);

	// ### CONFIG RTC_MODE 24 / 12 hours mode ###
	RTC->CR &= ~RTC_CR_FMT;

	// ### EXIT INIT MODE ###
	RTC->ISR &= ~RTC_ISR_INIT;

	now = HAL_GetTick();

	while (!(RTC->ISR & RTC_ISR_RSF))
	{
    	if(HAL_GetTick() - now >= 10000)
    	{
    		return RTC_ERROR;
    	}
	}

	// ### DISABLE WRITE ACCESS ###
	rtc_enable_protection();

	return RTC_OK;
}


RTC_Status_t rtc_get_time(RTC_Time_t *m_time)
{
	// ### CONVERT REGISTERS BCD TO DEC ###
	m_time->hours   = bcd_to_dec((RTC->TR & (RTC_TR_HT_Msk  | RTC_TR_HU_Msk )) >>RTC_TR_HU_Pos);
	m_time->minutes = bcd_to_dec((RTC->TR & (RTC_TR_MNT_Msk | RTC_TR_MNU_Msk)) >>RTC_TR_MNU_Pos);
	m_time->seconds = bcd_to_dec((RTC->TR & (RTC_TR_ST_Msk  | RTC_TR_SU_Msk )) >>RTC_TR_SU_Pos);

	return RTC_OK;
}


RTC_Status_t rtc_set_date(RTC_Date_t *m_date)
{
	// ### ENABLE WRITE ACCESS ###
	rtc_disable_protection();

	// ### ENABLE RTC INIT MODE ###
	RTC->ISR |= RTC_ISR_INIT;

	// ### WAIT WHILE RTC INIT ###
	uint32_t now = HAL_GetTick();

	while (!(RTC->ISR & RTC_ISR_INITF))
	{
    	if(HAL_GetTick() - now >= 1000)
    	{
    		return RTC_ERROR;
    	}
	}

	// ### SET BCD DATE TO REGISTER ###
    RTC->DR = ((m_date->year % 100) / 10 << RTC_DR_YT_Pos) | ((m_date->year % 100) % 10 << RTC_DR_YU_Pos) |
              (m_date->month / 10 << RTC_DR_MT_Pos) | (m_date->month % 10 << RTC_DR_MU_Pos) |
              (m_date->day / 10 << RTC_DR_DT_Pos) | (m_date->day % 10 << RTC_DR_DU_Pos);

	// ### CONFIG RTC_MODE 24 / 12 hours mode ###
	RTC->CR &= ~RTC_CR_FMT;

	// ### EXIT INIT MODE ###
	RTC->ISR &= ~RTC_ISR_INIT;

	now = HAL_GetTick();

	while (!(RTC->ISR & RTC_ISR_RSF))
	{
    	if(HAL_GetTick() - now >= 10000)
    	{
    		return RTC_ERROR;
    	}
	}

	// ### DISABLE WRITE ACCESS ###
	rtc_enable_protection();

	return RTC_OK;
}


RTC_Status_t rtc_get_date(RTC_Date_t *m_date)
{
	uint32_t dr = RTC->DR;
	m_date->year = ((dr & RTC_DR_YT_Msk) >> RTC_DR_YT_Pos) * 10 + ((dr & RTC_DR_YU_Msk) >> RTC_DR_YU_Pos);
	m_date->month = ((dr & RTC_DR_MT_Msk) >> RTC_DR_MT_Pos) * 10 + ((dr & RTC_DR_MU_Msk) >> RTC_DR_MU_Pos);
	m_date->day = ((dr & RTC_DR_DT_Msk) >> RTC_DR_DT_Pos) * 10 + ((dr & RTC_DR_DU_Msk) >> RTC_DR_DU_Pos);
	return RTC_OK;
}


RTC_Status_t rtc_bind_time_alarm(RTC_Time_t *m_time,
		RTC_Alarm_t alarm_type)
{
	uint32_t now = 0;
	uint8_t crc = m_time->hours + m_time->minutes + m_time->seconds;
	uint32_t _bcd_alarm_time = (RTC_ALRMAR_MSK4 | (m_time->hours / 10 << RTC_ALRMAR_HT_Pos) | (m_time->hours % 10 << RTC_ALRMAR_HU_Pos) |
			   (m_time->minutes / 10 << RTC_ALRMAR_MNT_Pos) | (m_time->minutes % 10 << RTC_ALRMAR_MNU_Pos) |
			   (m_time->seconds / 10 << RTC_ALRMAR_ST_Pos) | (m_time->seconds % 10 << RTC_ALRMAR_SU_Pos));

	uint8_t _hours = 0, _minutes = 0, _seconds = 0;

	rtc_disable_protection();

	switch(alarm_type)
	{
	case ALARM_A: // ALARM A CONFIGURATION
		// ### DISABLE ALARM A ###
		RTC->CR &= ~RTC_CR_ALRAE;

		// ### CHECK IF ALRMAR REGISTER CAN BE ACCESSED
		now = HAL_GetTick();

		while(!(RTC->ISR & RTC_ISR_ALRAWF))
		{
			if(HAL_GetTick() - now >= 1000)
			{
				return RTC_ERROR;
			}
		}

		// ### SET ALARM TIME ###
		RTC->ALRMAR = _bcd_alarm_time;

		// ### ENABLE RTC ALARM A ###
		RTC->CR |= RTC_CR_ALRAE;

		// ### ENABLE RTC ALARM A INTERRUPT ###
		RTC->CR |= RTC_CR_ALRAIE;

		_hours = ((RTC->ALRMAR & RTC_ALRMAR_HT_Msk) >> RTC_ALRMAR_HT_Pos) * 10 +
			 ((RTC->ALRMAR & RTC_ALRMAR_HU_Msk) >> RTC_ALRMAR_HU_Pos);
		_minutes = ((RTC->ALRMAR & RTC_ALRMAR_MNT_Msk) >> RTC_ALRMAR_MNT_Pos) * 10 +
			   ((RTC->ALRMAR & RTC_ALRMAR_MNU_Msk) >> RTC_ALRMAR_MNU_Pos);
		_seconds = ((RTC->ALRMAR & RTC_ALRMAR_ST_Msk) >> RTC_ALRMAR_ST_Pos) * 10 +
			   ((RTC->ALRMAR & RTC_ALRMAR_SU_Msk) >> RTC_ALRMAR_SU_Pos);

		break;

	case ALARM_B: // ALARM B CONFIGURATION
		// ### DISABLE ALARM B ###
		RTC->CR &= ~RTC_CR_ALRBE;

		// ### CHECK IF ALRMBR REGISTER CAN BE ACCESSED
		now = HAL_GetTick();

		while(!(RTC->ISR & RTC_ISR_ALRBWF))
		{
			if(HAL_GetTick() - now >= 1000)
			{
				return RTC_ERROR;
			}
		}

		// ### SET ALARM TIME ###
		RTC->ALRMBR = _bcd_alarm_time;

		// ### ENABLE RTC ALARM B ###
		RTC->CR |= RTC_CR_ALRBE;

		// ### ENABLE RTC ALARM B INTERRUPT ###
		RTC->CR |= RTC_CR_ALRBIE;

		_hours = ((RTC->ALRMBR & RTC_ALRMBR_HT_Msk) >> RTC_ALRMBR_HT_Pos) * 10 +
			 ((RTC->ALRMBR & RTC_ALRMBR_HU_Msk) >> RTC_ALRMBR_HU_Pos);
		_minutes = ((RTC->ALRMBR & RTC_ALRMBR_MNT_Msk) >> RTC_ALRMBR_MNT_Pos) * 10 +
			   ((RTC->ALRMBR & RTC_ALRMBR_MNU_Msk) >> RTC_ALRMBR_MNU_Pos);
		_seconds = ((RTC->ALRMBR & RTC_ALRMBR_ST_Msk) >> RTC_ALRMBR_ST_Pos) * 10 +
			   ((RTC->ALRMBR & RTC_ALRMBR_SU_Msk) >> RTC_ALRMBR_SU_Pos);
		break;
	}


	rtc_enable_protection();

	if(_hours + _minutes + _seconds == crc)
	{
		return RTC_OK;
	}
	else
	{
		return RTC_ERROR;
	}
}


RTC_Status_t rtc_stop_alarm(void)
{
	RTC->CR &= ~RTC_CR_ALRAE;
	RTC->CR &= ~RTC_CR_ALRAIE;
	return RTC_OK;
}


void rtc_alarm_interrupt_handler(RTC_Alarm_t curr_alarm)
{
	switch(curr_alarm)
	{
	case ALARM_A:
		if ((RTC->ISR & RTC_ISR_ALRAF))
		{
			RTC->ISR &= ~RTC_ISR_ALRAF;

			RTC_Time_t m_curr_time = {0};

			m_curr_time.hours = ((RTC->ALRMAR & RTC_ALRMAR_HT_Msk) >> RTC_ALRMAR_HT_Pos) * 10 +
				 ((RTC->ALRMAR & RTC_ALRMAR_HU_Msk) >> RTC_ALRMAR_HU_Pos);
			m_curr_time.minutes = ((RTC->ALRMAR & RTC_ALRMAR_MNT_Msk) >> RTC_ALRMAR_MNT_Pos) * 10 +
				   ((RTC->ALRMAR & RTC_ALRMAR_MNU_Msk) >> RTC_ALRMAR_MNU_Pos);
			m_curr_time.seconds = ((RTC->ALRMAR & RTC_ALRMAR_ST_Msk) >> RTC_ALRMAR_ST_Pos) * 10 +
				   ((RTC->ALRMAR & RTC_ALRMAR_SU_Msk) >> RTC_ALRMAR_SU_Pos);

			rtc_alarm_callback(&m_curr_time, ALARM_A);
		}
		break;

	case ALARM_B:
		if ((RTC->ISR & RTC_ISR_ALRBF))
		{
			RTC->ISR &= ~RTC_ISR_ALRBF;

			RTC_Time_t m_curr_time = {0};

			m_curr_time.hours = ((RTC->ALRMBR & RTC_ALRMBR_HT_Msk) >> RTC_ALRMBR_HT_Pos) * 10 +
				 ((RTC->ALRMBR & RTC_ALRMBR_HU_Msk) >> RTC_ALRMBR_HU_Pos);
			m_curr_time.minutes = ((RTC->ALRMBR & RTC_ALRMBR_MNT_Msk) >> RTC_ALRMBR_MNT_Pos) * 10 +
				   ((RTC->ALRMBR & RTC_ALRMBR_MNU_Msk) >> RTC_ALRMBR_MNU_Pos);
			m_curr_time.seconds = ((RTC->ALRMBR & RTC_ALRMBR_ST_Msk) >> RTC_ALRMBR_ST_Pos) * 10 +
				   ((RTC->ALRMBR & RTC_ALRMBR_SU_Msk) >> RTC_ALRMBR_SU_Pos);

			rtc_alarm_callback(&m_curr_time, ALARM_B);
		}
		break;
	}
}


// ### MEMORY BLOCK ###
RTC_Status_t rtc_save_time(RTC_Date_t *m_time)
{

}


RTC_Status_t rtc_save_date(RTC_Date_t *m_date)
{

}


RTC_Status_t rtc_restore_time(RTC_Time_t *m_time)
{

}


RTC_Status_t rtc_restore_date(RTC_Date_t *m_date)
{

}

// ### PRIVATE FUNCTIONS DECLORATION ###
void rtc_disable_protection(void)
{
	RTC->WPR = 0xCA;
	RTC->WPR = 0x53;
}


void rtc_enable_protection(void)
{
    RTC->WPR = 0xFF;
}


uint8_t dec_to_bcd(uint8_t byte)
{
  uint8_t bcdhigh = 0;

  while (byte >= 10)
  {
    bcdhigh++;
    byte -= 10;
  }

  return  ((uint8_t)(bcdhigh << 4) | byte);
}


uint8_t bcd_to_dec(uint8_t bcd)
{
  uint8_t tmp = 0;

  tmp = ((uint8_t)(bcd & (uint8_t)0xF0) >> (uint8_t)0x4) * 10;

  return (tmp + (bcd & (uint8_t)0x0F));
}


// ### CALLBACKS ####
__weak void rtc_alarm_callback(RTC_Time_t *m_time, RTC_Alarm_t curr_alarm);



