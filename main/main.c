#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

// Определяем выводы
#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<GPIO_NUM_5) | (1ULL<<GPIO_NUM_18) | (1ULL<<GPIO_NUM_19) | (1ULL<<GPIO_NUM_21))

void app_main(void)
{
    // Настройка выводов
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);

    // Комбинации для перебора
    uint8_t combinations[16][4] = {
            {0, 0, 0, 0},
            {0, 0, 0, 1},
            {0, 0, 1, 0},
            {0, 0, 1, 1},
            {0, 1, 0, 0},
            {0, 1, 0, 1},
            {0, 1, 1, 0},
            {0, 1, 1, 1},
            {1, 0, 0, 0},
            {1, 0, 0, 1},
            {1, 0, 1, 0},
            {1, 0, 1, 1},
            {1, 1, 0, 0},
            {1, 1, 0, 1},
            {1, 1, 1, 0},
            {1, 1, 1, 1}
    };

    while(1) {
        for (int i = 0; i < 16; i++) {
            // Установка значений на выводы
            gpio_set_level(GPIO_NUM_5, combinations[i][0]);
            gpio_set_level(GPIO_NUM_18, combinations[i][1]);
            gpio_set_level(GPIO_NUM_19, combinations[i][2]);
            gpio_set_level(GPIO_NUM_21, combinations[i][3]);

            // Задержка 0.5 секунды
            vTaskDelay(pdMS_TO_TICKS(500));
        }
    }
}
