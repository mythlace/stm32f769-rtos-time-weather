/**
 * @file task_config.h
 *
 * @date Feb 7, 2026
 * @author: mdev
 */

#ifndef INC_TASK_CONFIG_H_
#define INC_TASK_CONFIG_H_

#include "FreeRTOS.h"
#include "task.h"

/**
 * @defgroup Task priorities macro
 * @brief Priority values for the tasks
 * @{
 */
#define TASK_PRIORITY_HIGHEST	tskIDLE_PRIORITY + 5	/**< Highest task priority */
#define TASK_PRIORITY_HIGH		tskIDLE_PRIORITY + 4	/**< High task priority */
#define TASK_PRIORITY_NORMAL	tskIDLE_PRIORITY + 3	/**< Normal task priority */
#define TASK_PRIORITY_LOW		tskIDLE_PRIORITY + 2	/**< Low task priority */
#define TASK_PRIORITY_LOWEST	tskIDLE_PRIORITY + 1	/**< Lowest task priority */
/** @} */


/**
 * @def PRIORITY_CRITICAL
 * @brief Priority of tasks that have hard real-time constraints
 */
#define PRIORITY_CRITICAL	TASK_PRIORITY_HIGHEST

/**
 * @def PRIORITY_REALTIME
 * @brief Priority of tasks that are time critical
 */
#define PRIORITY_REALTIME	TASK_PRIORITY_HIGH

/**
 * @def PRIORITY_APPLICATION
 * @brief Priority of main application tasks
 */
#define PRIORITY_APPLICATION	TASK_PRIORITY_NORMAL

/**
 * @def PRIORITY_SERVICE
 * @brief Priority of tasks that can defer work
 */
#define PRIORITY_SERVICE	TASK_PRIORITY_LOW

/**
 * @def PRIORITY_BACKGROUND
 * @brief Priority of tasks that run in background
 */
#define PRIORITY_BACKGROUND	TASK_PRIORITY_LOWEST


#endif /* INC_TASK_CONFIG_H_ */
