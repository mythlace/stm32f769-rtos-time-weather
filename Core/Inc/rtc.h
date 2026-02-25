/**
 * @file rtc.h
 * @brief Implement RTC
 *
 * @date Feb 20, 2026
 * @author mdev 
 */

#ifndef INC_RTC_H_
#define INC_RTC_H_

/**
 * @brief Set RTC from tm
 *
 * @param t Pointer to tm
 */
void rtc_set_from_sec(uint32_t sec);

/**
 * @brief Print current system time
 */
void rtc_print_current_time(void);


#endif /* INC_RTC_H_ */
