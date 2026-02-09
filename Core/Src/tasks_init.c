/**
 * @file tasks_init.c
 * @brief Initialize all tasks
 *
 * Defines function to intitialize all tasks
 *
 * @date Feb 7, 2026
 * @author: mdev
 */

#include "tasks_init.h"
#include "status_led_task.h"

/** @brief Create all tasks */
void tasks_create_all(void) {
	// Create status LED task
	status_led_task_create();
	// Create UART terminal task
	uart_terminal_task_create();
}
