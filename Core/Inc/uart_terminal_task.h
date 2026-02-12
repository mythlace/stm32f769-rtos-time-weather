/**
 * @file uart_terminal_task.h
 * @brief UART terminal interface
 *
 * Provides declarations for UART terminal
 *
 * @date Feb 8, 2026
 * @author mdev 
 */

#ifndef INC_UART_TERMINAL_TASK_H_
#define INC_UART_TERMINAL_TASK_H_

#include "task_config.h"
#include "queue.h"
/**
 * @def UART_QUEUE_LENGTH
 * @brief Length of the queue
 */
#define UART_TERMINAL_QUEUE_LENGTH			8

/**
 * @def UART_TX_BUFFER_SIZE
 * @brief Size of UART TX buffer (bytes)
 */
#define UART_TERMINAL_TX_BUFFER_SIZE		256

/**
 * @def UART_TERMINAL_TASK_PRIORITY
 * @brief Priority of UART terminal task
 */
#define UART_TERMINAL_TASK_PRIORITY			PRIORITY_SERVICE

/**
 * @def UART_TERMINAL_TASK_STACK_SIZE
 * @brief Stack size of UART terminal task
 */
#define UART_TERMINAL_TASK_STACK_SIZE		256


/** @brief Create UART terminal task*/
void uart_terminal_task_create(void);

/** @brief Send queue data to UART terminal */
void task_uart_terminal_send(void *arguments);

/** @brief Get queue handle */
QueueHandle_t uart_terminal_get_queue_handle(void);

/** @brief Get task handle */
TaskHandle_t uart_terminal_get_task_handle(void);


#endif /* INC_UART_TERMINAL_TASK_H_ */
