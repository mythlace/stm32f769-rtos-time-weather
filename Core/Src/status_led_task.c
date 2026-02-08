/**
 * @file status_led_task.c
 * @brief status LED task implementation
 *
 * Provides function definitions to create and implement the status LED task.
 *
 * @date Feb 7, 2026
 * @author: mdev
 */


#include "status_led_task.h"
#include "main.h"
#include "cmsis_os.h"

/** @brief Handle for the task */
static TaskHandle_t status_led_task_handle = NULL;

/** @brief Create the status LED task */
void status_led_task_create(void) {
	xTaskCreate(
		task_status_led,
		"Status LED",
		STATUS_LED_TASK_STACK_SIZE,
		NULL,
		STATUS_LED_TASK_PRIORITY,
		&status_led_task_handle
	);
}

/**
 * @brief Status LED task
 *
 * Repeating blink pattern
 * - Heart beat (System running): Quick double flash every 5 seconds
 *
 * @param argument (unused)
 */
void task_status_led(void *argument) {
	TickType_t wake_tick = xTaskGetTickCount();;
	while(1) {
		// Double flash
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
		vTaskDelay(pdMS_TO_TICKS(50));
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
		vTaskDelay(pdMS_TO_TICKS(100));
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
		vTaskDelay(pdMS_TO_TICKS(50));
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
		// Wait until next 5s
		vTaskDelayUntil(&wake_tick, pdMS_TO_TICKS(5000));
	}
}

/**
 * @brief Get the status LED task handle
 * @return TaskHandle_t Handle of the status led task
 */
TaskHandle_t status_led_task_get_handle(void) {
	return status_led_task_handle;
}



