/**
 * @file rtc.c
 * @brief Implement RTC
 *
 * @date Feb 20, 2026
 * @author mdev 
 */

#include "main.h"
#include <time.h>
#include <stdio.h>
#include "rtc.h"

/** @brief RTC handler */
extern RTC_HandleTypeDef hrtc;

/** @brief Weekday names */
static const char *weekdays[7] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};

/**
 * @brief Set RTC from sec
 */
void rtc_set_from_sec(uint32_t sec) {

	time_t unix_time = sec;
	struct tm *utc_time = gmtime(&unix_time);

	if (!utc_time){
		printf("Error: RTC set failed. UTC time not available\n");
		return;
	}

	printf("UTC time: %02d:%02d:%02d  %s %02d-%02d-%04d\n",
		utc_time->tm_hour,
		utc_time->tm_min,
		utc_time->tm_sec,
		weekdays[utc_time->tm_wday],
		utc_time->tm_mon + 1,
		utc_time->tm_mday,
		utc_time->tm_year + 1900
   );

	printf("Setting RTC... ");
	RTC_TimeTypeDef system_time = {0};
	RTC_DateTypeDef system_date = {0};

	system_time.Hours = utc_time->tm_hour;
	system_time.Minutes = utc_time->tm_min;
	system_time.Seconds = utc_time->tm_sec;
	system_time.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	system_time.StoreOperation = RTC_STOREOPERATION_RESET;

	// Set RTC
	if(HAL_RTC_SetTime(&hrtc, &system_time, RTC_FORMAT_BIN) != HAL_OK){
		printf("Error: RTC time set failed (HAL)\n");
		return;
	}

	system_date.Date = utc_time->tm_mday;
	system_date.Month = utc_time->tm_mon + 1;
	system_date.Year = utc_time->tm_year - 100;
	system_date.WeekDay = utc_time->tm_wday == 0 ? 7 : utc_time->tm_wday;

	if(HAL_RTC_SetDate(&hrtc, &system_date, RTC_FORMAT_BIN) != HAL_OK){
		printf("Error: RTC date set failed (HAL)\n");
		return;
	}
	printf("Done\n");
}

/**
 * @brief Print current system time
 */
void rtc_print_current_time(void) {
	time_t now;
	struct tm local_time;

	time(&now);
	localtime_r(&now, &local_time);

	RTC_TimeTypeDef system_time;
	RTC_DateTypeDef system_date;

	HAL_RTC_GetTime(&hrtc, &system_time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &system_date, RTC_FORMAT_BIN);
	printf("System time:");
	printf("UTC:   %02d:%02d:%02d  %s %02d-%02d-%04d\n",
		system_time.Hours,
		system_time.Minutes,
		system_time.Seconds,
		weekdays[(system_date.WeekDay % 7)],
		system_date.Month,
		system_date.Date,
		system_date.Year + 2000
	);
	printf("Local: %02d:%02d:%02d  %s %02d-%02d-%04d\n",
		local_time.tm_hour,
		local_time.tm_min,
		local_time.tm_sec,
		weekdays[local_time.tm_wday],
		local_time.tm_mon + 1,
		local_time.tm_mday,
		local_time.tm_year + 1900
	);
}


