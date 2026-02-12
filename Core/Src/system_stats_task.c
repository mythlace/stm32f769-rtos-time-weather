/**
 * @file system_stats_task.c
 * @brief Print system statistics
 *
 * Gets, formats and prints system statistics.
 *
 * @date Feb 9, 2026
 * @author mdev 
 */

#include "main.h"
#include "FreeRTOS.h"
#include "system_stats_task.h"
#include <stdio.h>

/** @brief Handle of system stats task*/
static TaskHandle_t system_stats_task_handle = NULL;

/** @brief Timer for run time stats measurement */
extern TIM_HandleTypeDef htim5;

/** @brief Start timer for run time stats */
void configureTimerForRunTimeStats(void) {
	HAL_TIM_Base_Start(&htim5);
}

/**
 * @brief Get timer value
 * @return unsigned long Current counter value of timer
 */
unsigned long getRunTimeCounterValue(void) {
	return __HAL_TIM_GET_COUNTER(&htim5);
}

/** @brief Create system stats task */
void system_stats_task_create(void) {
	xTaskCreate(
		task_system_stats,
		"System Statistics",
		SYSTEM_STATS_TASK_STACK_SIZE,
		NULL,
		SYSTEM_STATS_TASK_PRIORITY,
		&system_stats_task_handle
	);
}

/**
 * @brief Implement system stats task
 * @param arguments (unused)
 */
void task_system_stats(void *arguments) {
	//char *buffer;
	BaseType_t task_count = uxTaskGetNumberOfTasks();
	TaskStatus_t *task_status_array;
	unsigned long total_run_time;
	unsigned long cpu_percent;
	// Make sure configSUPPORT_DYNAMIC_ALLOCATION  is set to 1
	assert(configSUPPORT_DYNAMIC_ALLOCATION == 1);


	while(1) {
		// Wait for notification
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		// Allocate required space
		task_status_array = pvPortMalloc(task_count * sizeof( TaskStatus_t));
		configASSERT(task_status_array != NULL);
		// get system state
		long actual_task_count = uxTaskGetSystemState(task_status_array, task_count, &total_run_time);

		if (total_run_time > 0) {
			printf("%-20s%15s%15s\n","Task", "Free Stack", "CPU");
			printf("%-20s%15s%15s\n","----------", "----------", "---");
			for (long i = 0; i < actual_task_count; i++){
				cpu_percent = ((task_status_array[i].ulRunTimeCounter * 100ul) / total_run_time);
				if (cpu_percent > 0){
					printf("%-20s%15u%15lu%%\n",
						task_status_array[i].pcTaskName,
						task_status_array[i].usStackHighWaterMark,
						cpu_percent
					);
				} else {
					printf("%-20s%15u%15s%%\n",
						task_status_array[i].pcTaskName,
						task_status_array[i].usStackHighWaterMark,
						"<1"
					);
				}

			}
			printf("\n");
		}

		// Memory usage
		unsigned long totalHeap = configTOTAL_HEAP_SIZE;
		unsigned long freeHeap = xPortGetFreeHeapSize();
		unsigned long usedHeap = totalHeap - freeHeap;
		unsigned long minFreeHeap = xPortGetMinimumEverFreeHeapSize();
		unsigned long maxUsedHeap = totalHeap - minFreeHeap;

		unsigned long usedPercentage = (usedHeap*100) / totalHeap;
		unsigned long maxUsedPercentage = (maxUsedHeap*100) / totalHeap;

		printf("Total Memory: %lu KB\nMemory used = %lu KB (%lu%%)\nMax used = %lu KB (%lu%%)\n\n",
			totalHeap/1024, usedHeap/1024, usedPercentage, maxUsedHeap/1024, maxUsedPercentage
		);
	}
}

/**
 * @brief Get system stats task handle
 * @return TaskHandle_t Task handle of system stats task
 */
TaskHandle_t system_stats_task_get_handle(void) {
	return system_stats_task_handle;
}

/**
 * @brief Print system stats
 * Should be called from ISR.
 *
 */
void print_system_stats_FromISR(){
	static uint32_t last_tick = 0;

	// Get current tick count
	uint32_t now = xTaskGetTickCountFromISR();
	if((now - last_tick) > pdMS_TO_TICKS(50)) {
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		// Notify task
		vTaskNotifyGiveFromISR(system_stats_task_handle, &xHigherPriorityTaskWoken);
		// Yield immediately if task priority is higher
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
		last_tick = now;
	}
}
