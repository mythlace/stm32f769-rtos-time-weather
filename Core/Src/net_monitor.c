/**
 * @file network_monitor.c
 * @brief Event based network monitor.
 *
 * @date Feb 23, 2026
 * @author mdev 
 */


#include "FreeRTOS.h"
#include "task.h"

#include "net_monitor.h"

#include "lwip/netif.h"
#include "ethernetif.h"

#include <stdbool.h>
#include <stdio.h>
#include "ntp_client_task.h"


/** @brief Flag to mark network ready status */
static volatile int network_ready = 0;

/** @brief Network interface data structure */
extern struct netif gnetif;

void netif_link_update_callback(struct netif *netif){
	net_monitor_update_state(netif);
}

void netif_status_update_callback(struct netif *netif) {
	net_monitor_update_state(netif);
}

/**
 * @brief Register netif callbacks
 * Must be called after LWIP initialization
 */
void net_monitor_reg_netif_callbacks(void) {
	// Register link update callback
	netif_set_link_callback(&gnetif, netif_link_update_callback);
	// Register status update callback
	netif_set_status_callback(&gnetif, netif_status_update_callback);
}

/**
 * @brief Update network state
 *
 * @param netif Network interface structure
 */
void net_monitor_update_state(struct netif *netif) {
	// Check if the interface is correct
	if (netif != &gnetif){
		return;
	}

	int network_state = netif_is_link_up(netif) && netif_is_up(netif) && !ip_addr_isany(&netif->ip_addr);
	// Avoid multiple calls for same event
	// React only to network state change
	if(network_state != network_ready) {
		if(network_state == 1) {
			printf("Connected to network\n");
			network_ready = 1;
			// Initiate NTP sync
			xTaskNotifyGive(ntp_client_get_handle());
		} else {
			network_ready = 0;
			printf("Network disconnected\n");
		}
	}
}

/**
 * @brief Check if network is ready
 *
 * @return int Status of network
 */
int is_network_ready(void) {
	return network_ready;
}
