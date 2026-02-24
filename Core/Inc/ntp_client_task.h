/**
 * @file ntp_client_task.h
 * @brief Implement NTP client task
 *
 * @date Feb 14, 2026
 * @author mdev 
 */

#ifndef INC_NTP_CLIENT_TASK_H_
#define INC_NTP_CLIENT_TASK_H_

#include "task_config.h"

/**
 * @def NTP_CLIENT_PRIORITY
 * @brief Priority level of the NTP client task.
 */
#define NTP_CLIENT_PRIORITY		TASK_PRIORITY_LOW

/**
 * @def NTP_CLIENT_STACK_SIZE
 * @brief Stack size allocated for the NTP client task.
 */
#define NTP_CLIENT_STACK_SIZE	1024


/** @brief Create NTP client task */
void ntp_client_task_create(void);


/**
 * @brief Implement NTP client task
 *
 * @param arguments (unused)
 */
void task_ntp_client(void *arguments);

/**
 * @brief Notify NTP client task
 * Intended to be called from an ISR
 */
void sync_time_FromISR(void);

/**
 * @brief Get NTP client task handle
 *
 * @return TaskHandle_t Task handle of NTP client task
 */
TaskHandle_t ntp_client_get_handle(void);


#endif /* INC_NTP_CLIENT_TASK_H_ */
