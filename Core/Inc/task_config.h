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
 * @def TASK_PRIORITY_BACKGROUND
 * @brief Priority of task that run in background
 */
#define TASK_PRIORITY_BACKGROUND	TASK_PRIORITY_LOWEST


#endif /* INC_TASK_CONFIG_H_ */
