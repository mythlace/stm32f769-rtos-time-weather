/**
 * @file system_stats_task.h
 * @brief Print system statistics
 *
 * Gets, formats and prints system statistics
 *
 * @date Feb 9, 2026
 * @author mdev 
 */


#ifndef INC_SYSTEM_STATS_TASK_H_
#define INC_SYSTEM_STATS_TASK_H_

#include "task_config.h"

/**
 * @def SYSTEM_STATS_TASK_PRIORITY
 * @brief Priority of system stats task
 */
#define SYSTEM_STATS_TASK_PRIORITY		PRIORITY_BACKGROUND

/**
 * @def SYSTEM_STATS_TASK_STACK_SIZE
 * @brief Stack size of system stats task
 */
#define SYSTEM_STATS_TASK_STACK_SIZE	256

/** @brief Create system stats task */
void system_stats_task_create(void);

/**
 * @brief Implement system stats task
 * @param arguments (unused)
 */
void task_system_stats(void *arguments);

/**
 * @brief Get system stats task handle
 * @return TaskHandle_t Task handle of system stats task
 */
TaskHandle_t system_stats_task_get_handle(void);

/**
 * @brief Print system stats
 * Should be called from ISR
 */
void print_system_stats_FromISR(void);

#endif /* INC_SYSTEM_STATS_TASK_H_ */
