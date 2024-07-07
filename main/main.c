#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "ds18b20.c"
#include "freertos/queue.h"

#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<GPIO_NUM_5) | (1ULL<<GPIO_NUM_18) | (1ULL<<GPIO_NUM_19) | (1ULL<<GPIO_NUM_21))
#define GPIO_INPUT_IO_0      0
#define GPIO_INPUT_PIN_SEL   (1ULL<<GPIO_INPUT_IO_0)
#define ESP_INTR_FLAG_DEFAULT 0

#define LONG_PRESS_DURATION_MS 500

double target_temp = 23.0;

QueueHandle_t gpio_evt_queue = NULL;

void set_inverted_gpio_values(uint8_t value)
{
    gpio_set_level(GPIO_NUM_5, !(value & 0x01));
    gpio_set_level(GPIO_NUM_18, !(value & 0x02));
    gpio_set_level(GPIO_NUM_19, !(value & 0x04));
    gpio_set_level(GPIO_NUM_21, !(value & 0x08));
}

static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

void app_main(void)
{
    sensor_data_queue = xQueueCreate(1, sizeof(Temperatures));
    xTaskCreate(&ds18b20_main, "ds18b20_task", 4096, NULL, 5, NULL);

    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);

    io_conf.intr_type = GPIO_INTR_ANYEDGE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 1;
    gpio_config(&io_conf);

    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));

    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    gpio_isr_handler_add(GPIO_INPUT_IO_0, gpio_isr_handler, (void*) GPIO_INPUT_IO_0);

    uint8_t counter = 0;
    set_inverted_gpio_values(counter);

    Temperatures temps;
    while(1)
    {
        //pdMS_TO_TICKS(5000)
        if (xQueueReceive(sensor_data_queue, &temps, portMAX_DELAY))
        {
            // Process the received temperature value
            printf("Received temperature from main, internal: %.3f\n", temps.indoor_temp);
            printf("Received temperature from main, external: %.3f\n", temps.outdoor_temp);
        }

        if (temps.outdoor_temp > temps.indoor_temp)
        {
            printf("Outdoor temp is higher than indoor, cant do anything, disable\n");
            set_inverted_gpio_values(0 & 0x0F);
        } else
        {
            printf("Outdoor temp is higher than indoor, cant do anything, disable\n");
            double differnce =
        }



//        uint32_t io_num;
//        if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY))
//        {
//            if(gpio_get_level(io_num) == 0)
//            {
//                int64_t press_time = esp_timer_get_time();
//                while(gpio_get_level(io_num) == 0)
//                {
//                    vTaskDelay(10 / portTICK_PERIOD_MS);
//                }
//                int64_t release_time = esp_timer_get_time();
//                int64_t press_duration = release_time - press_time;
//
//                if (press_duration < LONG_PRESS_DURATION_MS * 1000)
//                {
//                    counter++;
//                } else {
//                    counter--;
//                }
//
//                set_inverted_gpio_values(counter & 0x0F);
//            }
//        }
    }
}
