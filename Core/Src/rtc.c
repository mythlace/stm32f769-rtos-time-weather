/**
 * @file rtc.c
 * @brief Implement RTC
 *
 * @date Feb 20, 2026
 * @author mdev 
 */

#ifndef SRC_RTC_C_
#define SRC_RTC_C_

#include "main.h"
#include <time.h>
#include <stdio.h>
#include "rtc.h"

/** @brief RTC handler */
extern RTC_HandleTypeDef hrtc;

/** @brief Weekday names (RTC compatible) */
static const char *weekdays[7] = {"MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN"};

/**
 * @brief Set RTC from tm
 */
void rtc_set_from_tm(struct tm *t) {
	printf("Setting RTC...");
	RTC_TimeTypeDef system_time = {0};
	RTC_DateTypeDef system_date = {0};

	system_time.Hours = t->tm_hour;
	system_time.Minutes = t->tm_min;
	system_time.Seconds = t->tm_sec;
	// Daylight time is pre-calculated
	system_time.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	// No store operation
	system_time.StoreOperation = RTC_STOREOPERATION_RESET;

	// Set RTC
	HAL_RTC_SetTime(&hrtc, &system_time, RTC_FORMAT_BIN);


	system_date.Date = t->tm_mday;
	system_date.Month = t->tm_mon;
	system_date.Year = t->tm_year;
	system_date.WeekDay = t->tm_wday == 0 ? 7 : t->tm_wday;

	HAL_RTC_SetDate(&hrtc, &system_date, RTC_FORMAT_BIN);
	printf(" Done\n");
}

/**
 * @brief Print current system time
 */
void rtc_print_current_time(void) {
	RTC_TimeTypeDef system_time;
	RTC_DateTypeDef system_date;

	HAL_RTC_GetTime(&hrtc, &system_time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &system_date, RTC_FORMAT_BIN);

	printf("System time: %02d:%02d:%02d  %s %02d-%02d-%04d\n\n",
		system_time.Hours,
		system_time.Minutes,
		system_time.Seconds,
		weekdays[system_date.WeekDay - 1],
		system_date.Month + 1,
		system_date.Date,
		system_date.Year + 1900
	);
}


#endif /* SRC_RTC_C_ */
