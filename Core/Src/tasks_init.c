/**
 * @file tasks_init.c
 * @brief Initialize all tasks
 *
 * Defines function to intitialize all tasks
 *
 * @date Feb 7, 2026
 * @author: mdev
 */

#include "main.h"
#include "tasks_init.h"
#include "status_led_task.h"
#include "uart_terminal_task.h"
#include "system_stats_task.h"
#include "ntp_client.h"

#include "FreeRTOS.h"
#include "task.h"

#include "lwip/netif.h"
#include <stdio.h>
#include "task_config.h"

/** @brief Network interface data structure */
extern struct netif gnetif;

/**
 * @brief Task to sync time at boot
 */
static void task_sync_time(void *arguments){
	int attempt_count = 0;
	printf("\nChecking link...\n");
	// Check if link is up
	while ((!netif_is_up(&gnetif) || !netif_is_link_up(&gnetif) || gnetif.ip_addr.addr == 0) &&
			attempt_count < 5) {
		printf("Network is not ready. Retrying...\n");
		attempt_count++;
		vTaskDelay(pdMS_TO_TICKS(2000));
	}

	if(attempt_count < 5) {
		xTaskNotifyGive(ntp_client_get_handle());
	} else {
		printf("NTP sync failed. Network connection error\n\n");
	}

	vTaskDelete(NULL);
}

/** @brief Create all tasks */
void tasks_create_all(void) {
	// Create status LED task
	status_led_task_create();

	// Create UART terminal task
	uart_terminal_task_create();

	// Create System stats task
	system_stats_task_create();

	// Create NTP client task
	ntp_client_task_create();

	// Task to sync with NTP
	xTaskCreate(
		task_sync_time,
		"Sync time",
		256,
		NULL,
		PRIORITY_SERVICE,
		NULL
	);
}










