#include "freertos/FreeRTOS.h"

#include <stdio.h>
#include <string.h>
#include "portmacro.h"

#include "esp_wifi.h"
#include "esp_eap_client.h"
#include "esp_netif.h"

#include "esp_log.h"
#include "esp_event.h"
#include "esp_err.h"

#include "consts.h"


#define WIFI_FAILURE 1 << 0
#define WIFI_SUCCESS 1 << 1
#define MAX_FAILURES 10 

#define WIFI_SSID CONFIG_WIFI_SSID
#define EAP_USERNAME CONFIG_EAP_USERNAME
#define EAP_PASSWORD CONFIG_EAP_PASSWORD 

/** GLOBALS **/
// event group to contain status information
static EventGroupHandle_t wifi_event_group;

// number of attempts at reconnecting
static int retry_count = 0;

static void wifi_event_handler(void * arg, esp_event_base_t event_base, int32_t event_id, void * event_data) {
    // first time connect to wifi 
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        ESP_LOGI(TAG, "Connecting to AP...");
        esp_wifi_connect();

    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        // retry connecting to wifi
        if (retry_count < MAX_FAILURES) {
            ESP_LOGI(TAG, "Reconnecting to AP...");
            esp_wifi_connect();
            retry_count++;
        } else {
            xEventGroupSetBits(wifi_event_group, WIFI_FAILURE);
        }
    }
}


static void ip_event_handler(void * arg, esp_event_base_t event_base, int32_t event_id, void * event_data) {
    if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t * event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "STA IP: " IPSTR, IP2STR(&event->ip_info.ip));
        retry_count = 0;
        xEventGroupSetBits(wifi_event_group, WIFI_SUCCESS);
    }
}

esp_err_t connect_wifi() {
    int status = WIFI_FAILURE;

    // *** INITIALIZATION **** //
    // Initialize netif
    ESP_ERROR_CHECK(esp_netif_init());

    // Initialize default event loop
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // create wifi station (esp32 connects to external wifi)
    esp_netif_create_default_wifi_sta();

    // get default wifi configuration
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // ** EVENT LOOP ** //
    // this is how we can check the status and result of the wifi event
    wifi_event_group = xEventGroupCreate();

    // Check for wifi events
    esp_event_handler_instance_t wifi_handler_event_instance;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, &wifi_handler_event_instance));

    // Check for ip events
    esp_event_handler_instance_t got_ip_event_instance;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &ip_event_handler, NULL, &got_ip_event_instance));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
        }
    };
    
    // Put esp32 into station mode with our configuration
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));

    // Authenticate with PEAP and MSCHAP-V2
    ESP_ERROR_CHECK(esp_eap_client_set_username((uint8_t *)EAP_USERNAME, strlen(EAP_USERNAME)) );
    ESP_ERROR_CHECK(esp_eap_client_set_password((uint8_t *)EAP_PASSWORD, strlen(EAP_PASSWORD)) );
    ESP_ERROR_CHECK(esp_eap_client_set_new_password((uint8_t *)EAP_PASSWORD, strlen(EAP_PASSWORD)));
    
    // Enable enterprise wifi mode and start the wifi event loop
    ESP_ERROR_CHECK(esp_wifi_sta_enterprise_enable());
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "Station initialization complete complete");

    // **** BLOCK FOR WIFI EVENT LOOP TO FINISH *** // 
    EventBits_t bits = xEventGroupWaitBits(
        wifi_event_group,
        WIFI_SUCCESS | WIFI_FAILURE,
        pdFALSE, // return bits are not altered/cleared on exit
        pdFALSE, // only one of the bits needs to return
        portMAX_DELAY // wait indefinitely (about 7 weeks)
    );

    // The event bits are returned at this point
    if (bits & WIFI_SUCCESS) {
        status = WIFI_SUCCESS;
        ESP_LOGI(TAG, "Connected to access point!");
    } else if (bits & WIFI_FAILURE) {
        status = WIFI_FAILURE;
        ESP_LOGI(TAG, "Failed to connect to access point!");
    }

    // Remove the event group since the task is completed
    ESP_ERROR_CHECK(esp_event_handler_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_handler_event_instance));
    ESP_ERROR_CHECK(esp_event_handler_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, got_ip_event_instance));
    vEventGroupDelete(wifi_event_group);
    
    return status;
}
