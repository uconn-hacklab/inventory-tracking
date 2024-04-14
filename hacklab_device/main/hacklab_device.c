#include "freertos/FreeRTOS.h"

#include <stdio.h>
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "portmacro.h"

#define BLINK_LED 2

void app_main(void)
{
    char * taskName = pcTaskGetName(NULL);
    ESP_LOGI(taskName, "Hello!");
    gpio_reset_pin(BLINK_LED);
    gpio_set_direction(BLINK_LED, GPIO_MODE_OUTPUT);

    
    while (1) {
        gpio_set_level(BLINK_LED, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        gpio_set_level(BLINK_LED, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
