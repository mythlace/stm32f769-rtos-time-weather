/**
 * @file network_monitor.h
 *
 * @date Feb 23, 2026
 * @author mdev 
 */

#ifndef INC_NET_MONITOR_H_
#define INC_NET_MONITOR_H_

#include "lwip/netif.h"

/**
 * @brief Register netif callbacks
 * Must be called after LWIP initialization
 */
void net_monitor_reg_netif_callbacks(void);

/**
 * @brief Update network state
 *
 * @param netif Network interface structure
 */
void net_monitor_update_state(struct netif *netif);

/**
 * @brief Check if network is ready
 *
 * @return int Status of network
 */
int is_network_ready(void);


#endif /* INC_NET_MONITOR_H_ */
