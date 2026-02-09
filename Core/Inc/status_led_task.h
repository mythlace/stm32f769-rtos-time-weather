/**
 * @file status_led_task.h
 * @brief Status LED task
 *
 * Provides functions to create and implement the status LED task.
 *
 * @date Feb 7, 2026
 * @author: mdev
 */

#ifndef INC_STATUS_LED_TASK_H_
#define INC_STATUS_LED_TASK_H_

#include "task_config.h"

/**
 * @def STATUS_LED_TASK_PRIRITY
 * @brief Priority assigned the task
 */
#define STATUS_LED_TASK_PRIORITY	PRIORITY_BACKGROUND

/**
 * @def STATUS_LED_TASK_STACK
 * @brief Stack size allocated for task (in words)
 */
#define STATUS_LED_TASK_STACK_SIZE	256

/** @brief Create the status LED task */
void status_led_task_create(void);


/** @brief Implement status LED task function */
void task_status_led(void *argument);

/**
 * @brief Get the status LED task handle
 * @return TaskHandle_t Handle of the status led task
 */
TaskHandle_t status_led_task_get_handle(void);

#endif /* INC_STATUS_LED_TASK_H_ */
