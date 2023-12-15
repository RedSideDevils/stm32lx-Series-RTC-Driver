/* MADE BY TRIPPINGCARPET */

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

	// ### SET Driving capability to medium high ###
    RCC->CSR &= ~RCC_CSR_LSEDRV_Msk;
    RCC->CSR |=  RCC_CSR_LSEDRV_1;

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

    // ### CHANGING RTC SOURCE TO LSE ###
    RCC->CSR &= ~RCC_CSR_RTCSEL_Msk;
    RCC->CSR |= RCC_CSR_RTCSEL_LSE;
    return RTC_OK;
}


RTC_Status_t rtc_start_clock(void)
{
    PWR->CR |= PWR_CR_DBP;
	RCC->CSR |= RCC_CSR_RTCEN;
    PWR->CR &= ~PWR_CR_DBP;
	return RTC_OK;
}


RTC_Status_t rtc_stop_clock(void)
{
	// ### ENABLE ACCESS TO THE RTC DOMAIN ###
    PWR->CR |= PWR_CR_DBP;

    // ### DISABLE RTC CLOCK ###
    RCC->CSR &= ~RCC_CSR_RTCEN;

    // ### DISABLE ACCESS TO THE RTC DOMAIN ###
    PWR->CR &= ~PWR_CR_DBP;
    return RTC_OK;
}


RTC_Status_t rtc_set_time(RTC_Time_t *m_time)
{
	uint32_t bcd_time = 	((dec_to_bcd(m_time->hours))   <<16U) |
			((dec_to_bcd(m_time->minutes)) <<8U)      |
			((dec_to_bcd(m_time->seconds)) <<0U);

	// ### ENABLE WRITE ACCESS ###
	rtc_disable_protection();

	// ### ENABLE RTC INIT MODE ###
	RTC->ISR |= RTC_ISR_INIT;

	// ### WAIT WHILE RTC INIT ###
	uint32_t now = HAL_GetTick();

	while((RTC->ISR & RTC_ISR_INITF) != RTC_ISR_INITF)
	{
		if(HAL_GetTick() - now >= WAIT_RTC_INIT_TIMEOUT)
		{
			return RTC_ERROR;
		}
	}

	// ### SET PRESCALER TO 1HZ ###
    RTC->PRER = (RTC_PRER_PREDIV_A << 16) | RTC_PRER_PREDIV_S;

	// ### SET BCD TIME TO REGISTER ###
	RTC->TR = bcd_time;

	// ### CONFIG RTC_MODE 24 / 12 hours mode ###
    RTC->CR &= ~RTC_CR_FMT;
    RTC->CR |= RTC_CR_FMT;

	// ### EXIT INIT MODE ###
    RTC->ISR &= ~RTC_ISR_INIT;

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
	uint32_t bcd_date = ((m_date->year % 100) << RTC_DR_YU_Pos) | ((m_date->month % 100) << RTC_DR_MU_Pos) | ((m_date->day % 100) << RTC_DR_DU_Pos);

	// ### ENABLE WRITE ACCESS ###
	rtc_disable_protection();

	// ### ENABLE RTC INIT MODE ###
	RTC->ISR |= RTC_ISR_INIT;

	// ### WAIT WHILE RTC INIT ###
	uint32_t now = HAL_GetTick();

	while((RTC->ISR & RTC_ISR_INITF) != RTC_ISR_INITF)
	{
		if(HAL_GetTick() - now >= WAIT_RTC_INIT_TIMEOUT)
		{
			return RTC_ERROR;
		}
	}

	// ### SET PRESCALER FOR 1HZ ###
    RTC->PRER = (RTC_PRER_PREDIV_A << 16) | RTC_PRER_PREDIV_S;

	// ### SET BCD DATE TO REGISTER ###
	RTC->DR = bcd_date;

	// ### EXIT INIT MODE ###
    RTC->ISR &= ~RTC_ISR_INIT;

    // ### DISABLE WRITE ACCESS ###
    rtc_enable_protection();

    return RTC_OK;
}


RTC_Status_t rtc_get_date(RTC_Date_t *m_date)
{
	m_date->year   = bcd_to_dec((RTC->DR & (RTC_DR_YU_Msk  | RTC_DR_YU_Msk )) >>RTC_DR_YU_Pos);
	m_date->month = bcd_to_dec((RTC->DR & (RTC_DR_MU_Msk | RTC_DR_MU_Msk)) >>RTC_DR_YU_Pos);
	m_date->day = bcd_to_dec((RTC->DR & (RTC_DR_DU_Msk  | RTC_DR_DU_Msk )) >>RTC_DR_DU_Pos);

	return RTC_OK;
}


RTC_Status_t rtc_set_time_alarm(RTC_Time_t *m_time)
{
	rtc_disable_protection();

	// ### USING ALARM A ###
	RTC->CR &= ~RTC_CR_ALRAE;

	// ### SET ALARM TIME ###
	RTC->ALRMAR = 0;

	RTC->ALRMAR = (m_time->seconds << RTC_ALRMAR_ST_Pos) |
				  (m_time->minutes << RTC_ALRMAR_MNT_Pos) |
				  (m_time->hours << RTC_ALRMAR_HT_Pos);


	 RTC->CR |= RTC_CR_ALRAE;
	// ### INTERRUPT CONFIG ###
	NVIC_SetPriority(RTC_IRQn, 1);

	NVIC_EnableIRQ(RTC_IRQn);

	RTC->CR |= RTC_CR_ALRAE | RTC_CR_ALRAIE;

	rtc_enable_protection();
	return RTC_OK;
}


RTC_Status_t rtc_stop_alarm(void)
{
	 RTC->CR &= ~RTC_CR_ALRAE;
	 return RTC_OK;
}


void rtc_alarm_handler(void)
{
	if ((RTC->ISR & RTC_ISR_ALRAF) == RTC_ISR_ALRAF)
	{
		RTC->ISR &= ~RTC_ISR_ALRAF;

		RTC_Time_t m_curr_time = {0};

		m_curr_time.hours   = bcd_to_dec((RTC->ALRMAR & (RTC_ALRMAR_HT_Msk  |
								 RTC_ALRMAR_HU_Msk )) >>RTC_ALRMAR_HU_Pos);

		m_curr_time.minutes = bcd_to_dec((RTC->ALRMAR & (RTC_ALRMAR_MNT_Msk |
								 RTC_ALRMAR_MNU_Msk)) >>RTC_ALRMAR_MNU_Pos);

		m_curr_time.seconds = bcd_to_dec((RTC->ALRMAR & (RTC_ALRMAR_ST_Msk  |
								 RTC_ALRMAR_SU_Msk )) >>RTC_ALRMAR_SU_Pos);

		rtc_alarm_callback(&m_curr_time);
	}
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
__weak void rtc_alarm_callback(RTC_Time_t *m_time){}



