/**
 * @file uart_terminal_task.c
 * @brief UART terminal implementation
 *
 * Implementation of queue based UART terminal.
 * Redirects printf to UART terminal
 *
 * @date Feb 8, 2026
 * @author mdev 
 */


#include "main.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "uart_terminal_task.h"
#include "string.h"
#include <stdio.h>

/** @brief Handle of queue */
static QueueHandle_t uart_tx_queue_handle = NULL;

/** @brief Handle of uart_terminal_task_send */
static TaskHandle_t uart_terminal_task_handle = NULL;

/** @brief UART in use */
extern UART_HandleTypeDef huart6;

/** @brief Create UART terminal task */
void uart_terminal_task_create(void){
	// Create queue
	uart_tx_queue_handle = xQueueCreate(UART_TERMINAL_QUEUE_LENGTH, UART_TERMINAL_TX_BUFFER_SIZE);
	// Create task
	xTaskCreate(
		task_uart_terminal_send,
		"UART terminal send",
		UART_TERMINAL_TASK_STACK_SIZE,
		NULL,
		UART_TERMINAL_TASK_PRIORITY,
		&uart_terminal_task_handle
	);

	// Force unbuffered stdout. printf() -> _write()
	setvbuf(stdout, NULL, _IONBF, 0);
}

/**
 * @brief Implement UART terminal task
 *
 * UART setup
 * - Baud: 115200
 * - Word length: 8 bits
 * - Parity: None
 * - Stop bits: 1
 */
void task_uart_terminal_send(void *arguments){
	char tx_buffer[UART_TERMINAL_TX_BUFFER_SIZE];
	while(1) {
		// Wait until queue receives data
		xQueueReceive(uart_tx_queue_handle, tx_buffer, portMAX_DELAY);
		// Send data
		HAL_UART_Transmit(&huart6, (uint8_t *)tx_buffer, strlen(tx_buffer), HAL_MAX_DELAY);
	}
}

/**
 * @brief Get queue handle
 * @return QueueHandle_t Handle of queue
 */
QueueHandle_t uart_terminal_get_queue_handle(void){
	return uart_tx_queue_handle;
}

/**
 * @brief Get task handle
 * @return TaskHandle_t Handle of the UART terminal task
 */
TaskHandle_t uart_terminal_get_task_handle(void){
	return uart_terminal_task_handle;
}

/**
 * @brief Redirect printf to UART
 *
 * @param file File descriptor (unused)
 * @param buffer Pointer to write buffer
 * @param len Length of data to be written (bytes)
 *
 * @return int Number of bytes actually written
 */
int _write(int file, char *buffer, int len){
	// Do not write if queue is not initialized or if calling from ISR
	if (uart_tx_queue_handle == NULL || xPortIsInsideInterrupt()){
		return 0;
	}
	char temp_buffer[UART_TERMINAL_TX_BUFFER_SIZE];
	int chunk_length = 0, write_length = 0;
	while (write_length < len) {
		// Compute length of remaining data
		chunk_length = len - write_length;
		// Compute chunk length
		if (chunk_length > (UART_TERMINAL_TX_BUFFER_SIZE - 1)){
			chunk_length = UART_TERMINAL_TX_BUFFER_SIZE - 1;
		}
		// Copy data to local buffer
		memcpy(temp_buffer, &buffer[write_length], chunk_length);
		// Terminate string
		temp_buffer[chunk_length] = '\0';
		// Queue the data.
		if (xQueueSend(uart_tx_queue_handle, temp_buffer, pdMS_TO_TICKS(10)) != pdPASS){
			break;
		}

		write_length += chunk_length;
	}
	return write_length;
}
