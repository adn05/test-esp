/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"

static const char *TAG = "Blink";

/* Use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define BLINK_GPIO 6

static uint8_t s_led_state = 0;
static uint8_t led_actu = 0;
static uint8_t leds[] = {3, 4, 6};

static void blink_led(void)
{
    /* Set the GPIO level according to the state (LOW or HIGH)*/
    gpio_set_level(6, s_led_state);
    s_led_state = !s_led_state;
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    led_actu ++;
    led_actu %= 3;
}

static void configure_led(void)
{
    ESP_LOGI(TAG, "Example configured to blink GPIO LED!");
    for (led_actu=0; led_actu < 3; led_actu++){
        gpio_reset_pin(leds[led_actu]);
        /* Set the GPIO as a push/pull output */
        gpio_set_direction(leds[led_actu], GPIO_MODE_OUTPUT);
    }
    led_actu = 0;
}  
    

void app_main(void)
{

    /* Configure the peripheral according to the LED type */
    configure_led();

    while (1) {
        // ESP_LOGI(TAG, "Turning the LED %s!", s_led_state == true ? "ON" : "OFF");
        blink_led();
        /* Toggle the LED state */
    }
}