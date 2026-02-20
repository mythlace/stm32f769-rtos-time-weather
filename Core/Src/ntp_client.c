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

#include "ntp_client.h"


extern struct netif gnetif;

/** @brief Handle for NTP client task */
static TaskHandle_t ntp_client_task_handle = NULL;

/**
 * @def LOCAL_TIME_OFFSET_SECONDS
 * @brief Local time offset in seconds
 *
 */
#define LOCAL_TIME_OFFSET_SECONDS (-5 * 3600)

/** @brief Create NTP client task */
void ntp_client_task_create(void)
{
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

        printf("NTP sync ...\n");
        // Check if link is up
        if (!netif_is_up(&gnetif) || !netif_is_link_up(&gnetif)) {
            printf("Link is down\n");
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
            break;
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
            printf("Send failed\r\n");
            close(sock);
            break;
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
            break;
        }

        // Close socket
        close(sock);

        // Extract seconds from received packet
        uint32_t seconds;
        memcpy(&seconds, &packet[40], 4);
        seconds = ntohl(seconds);

        // Unix time stamp
        uint32_t unix_time = seconds - 2208988800UL;
        // Apply local offset
        uint32_t local_time = unix_time + LOCAL_TIME_OFFSET_SECONDS;

        // Format and print time
        time_t t = local_time;
        struct tm *tm_info = gmtime(&t);

        printf("EST Time: %02d:%02d:%02d  %02d-%02d-%04d\r\n",
		   tm_info->tm_hour,
		   tm_info->tm_min,
		   tm_info->tm_sec,
		   tm_info->tm_mday,
		   tm_info->tm_mon + 1,
		   tm_info->tm_year + 1900
	   );
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
