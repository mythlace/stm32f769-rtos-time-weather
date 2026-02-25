/**
 * @file ntp_client.c
 * @brief Implement NTP client task
 *
 * @date Feb 14, 2026
 * @author mdev 
 */

#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include "lwip/inet.h"
#include "lwip/ip_addr.h"
#include "lwip/netif.h"

#include "FreeRTOS.h"
#include "task.h"

#include <string.h>
#include <stdio.h>
#include <time.h>

#include "ntp_client_task.h"
#include "rtc.h"
#include "net_monitor.h"

/**
 * @def TZ_VALUE
 * @brief POSIX time zone format string
 */
#define TZ_VALUE "EST5EDT,M3.2.0/2,M11.1.0/2"

/** @brief Network interface data structure */
extern struct netif gnetif;

/** @brief Handle for NTP client task */
static TaskHandle_t ntp_client_task_handle = NULL;

/** @brief Create NTP client task */
void ntp_client_task_create(void)
{
	// Set time zone env variable
	setenv("TZ", TZ_VALUE, 1);
	tzset();

    xTaskCreate(
        task_ntp_client,
        "NTP Client",
        NTP_CLIENT_STACK_SIZE,
        NULL,
        NTP_CLIENT_PRIORITY,
        &ntp_client_task_handle
    );
}

/**
 * @brief Notify NTP client task
 * Intended to be called from an ISR
 */
TaskHandle_t ntp_client_get_handle(void)
{
    return ntp_client_task_handle;
}

/**
 * @brief Implement NTP client task
 *
 * @param arguments (unused)
 */
void task_ntp_client(void *argument)
{
    while(1) {
    	// Wait for notification
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        printf("NTP sync... ");
        // Check if link is up
        if (!is_network_ready()) {
            printf("failed!\nNetwork error.\n");
            return;
        }

        // Resolve DNS
        struct hostent *host = gethostbyname("pool.ntp.org");
        if (!host) {
            printf("DNS failed\n");
            return;
        }

        // Create socket
        int sock = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock < 0) {
            printf("Socket failed\n");
            return;
        }

        // Configure server address and port
        struct sockaddr_in server;
        memset(&server, 0, sizeof(server));
        server.sin_family = AF_INET;
        server.sin_port = htons(123);
        memcpy(&server.sin_addr, host->h_addr, host->h_length);

        uint8_t packet[48];
        memset(packet, 0, sizeof(packet));
        packet[0] = 0x23;  // LI=0, VN=4, Mode=3

        // Send NTP request
        if (sendto(sock, packet, sizeof(packet), 0, (struct sockaddr *)&server, sizeof(server)) < 0) {
            printf("Send failed\n");
            close(sock);
            return;
        }

        // Set receive timeout
        struct timeval timeout = { .tv_sec = 5, .tv_usec = 0 };
        setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

        // Wait for NTP response
        struct sockaddr_in from;
		socklen_t from_len = sizeof(from);
        if (recvfrom(sock, packet, sizeof(packet), 0, (struct sockaddr *)&from, &from_len) < 0) {
            printf("NTP receive timeout\n");
            close(sock);
            return;
        }
        // Close socket
        close(sock);

        printf("Done\n");
        // Extract seconds from received packet
        uint32_t seconds;
        memcpy(&seconds, &packet[40], 4);
        seconds = ntohl(seconds);

        // Unix time stamp
        uint32_t unix_time = seconds - 2208988800UL;
        //Set RTC
        rtc_set_from_sec(unix_time);
		rtc_print_current_time();
    }
}

/**
 * @brief Notify NTP client task
 * Intended to be called from an ISR
 */
void sync_time_FromISR(void){
	static uint32_t last_tick = 0;
	uint32_t now = xTaskGetTickCountFromISR();
	if((now - last_tick) > pdMS_TO_TICKS(50)) {
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		vTaskNotifyGiveFromISR(ntp_client_task_handle, &xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
}
