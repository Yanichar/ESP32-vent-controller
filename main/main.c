#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<GPIO_NUM_5) | (1ULL<<GPIO_NUM_18) | (1ULL<<GPIO_NUM_19) | (1ULL<<GPIO_NUM_21))

void set_inverted_gpio_values(uint8_t value)
{
    gpio_set_level(GPIO_NUM_5, !(value & 0x01));
    gpio_set_level(GPIO_NUM_18, !(value & 0x02));
    gpio_set_level(GPIO_NUM_19, !(value & 0x04));
    gpio_set_level(GPIO_NUM_21, !(value & 0x08));
}

void app_main(void)
{
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);

    while(1)
    {
        for (uint8_t i = 0; i < 16; i++)
        {
            set_inverted_gpio_values(i);
            vTaskDelay(pdMS_TO_TICKS(5000));
        }
    }
}
