#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "wifi.c"
#include "consts.h"

#include "mercury_api.h"

#define BLINK_LED 2

void blink_forever() {
    gpio_reset_pin(BLINK_LED);
    gpio_set_direction(BLINK_LED, GPIO_MODE_OUTPUT);

    while (1) {
        gpio_set_level(BLINK_LED, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        gpio_set_level(BLINK_LED, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    // Test the mercury api component
    func(); 

    // char * taskName = pcTaskGetName(NULL);
    // ESP_LOGI(taskName, "Hello!");

    // esp_err_t wifi_status = WIFI_FAILURE;

    // // Initialize flash storage for credentials
    // esp_err_t ret = nvs_flash_init();
    // if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    //     ESP_ERROR_CHECK(nvs_flash_erase());
    //     ret = nvs_flash_init();
    // }
    // ESP_ERROR_CHECK(ret);

    // ret = connect_wifi();
    // if (ret == WIFI_SUCCESS) {

    //     blink_forever();
    // }
}
