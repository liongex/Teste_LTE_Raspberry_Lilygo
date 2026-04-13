#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_log.h"

#define TAG "PONTE_SERIAL"

// Pinos do Modem A7670X na placa LilyGo
#define MODEM_TX_PIN  26
#define MODEM_RX_PIN  25
#define MODEM_PWR_PIN 4

// Uart configurada para o Modem e para o USB
#define MODEM_UART_NUM UART_NUM_1
#define USB_UART_NUM   UART_NUM_0 // A porta USB nativa do ESP32
#define BUF_SIZE       1024

// Função para ligar o hardware da LilyGo
static void power_on_modem(void) {
    // 1. Liga o chip gerenciador de energia (IP5306) via I2C
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = 21,
        .scl_io_num = 22,
        .sda_pullup_en = 1,
        .scl_pullup_en = 1,
        .master.clk_speed = 100000
    };
    i2c_param_config(I2C_NUM_0, &conf);
    i2c_driver_install(I2C_NUM_0, conf.mode, 0, 0, 0);
    uint8_t data[] = {0x00, 0xFF};
    i2c_master_write_to_device(I2C_NUM_0, 0x75, data, 2, 1000 / portTICK_PERIOD_MS);

    // 2. Pulso de ignição no pino PWRKEY
    gpio_reset_pin(MODEM_PWR_PIN);
    gpio_set_direction(MODEM_PWR_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(MODEM_PWR_PIN, 1);
    vTaskDelay(pdMS_TO_TICKS(100));
    gpio_set_level(MODEM_PWR_PIN, 0);
    vTaskDelay(pdMS_TO_TICKS(1500));
    gpio_set_level(MODEM_PWR_PIN, 1);
    
    // Aguarda o modem iniciar
    vTaskDelay(pdMS_TO_TICKS(5000));
}

// Tarefa: Lê da USB (Raspberry) e escreve no Modem
static void usb_to_modem_task(void *arg) {
    uint8_t *data = (uint8_t *) malloc(BUF_SIZE);
    while (1) {
        int len = uart_read_bytes(USB_UART_NUM, data, BUF_SIZE, pdMS_TO_TICKS(20));
        if (len > 0) {
            uart_write_bytes(MODEM_UART_NUM, (const char *) data, len);
        }
    }
}

// Tarefa: Lê do Modem e escreve na USB (Raspberry)
static void modem_to_usb_task(void *arg) {
    uint8_t *data = (uint8_t *) malloc(BUF_SIZE);
    while (1) {
        int len = uart_read_bytes(MODEM_UART_NUM, data, BUF_SIZE, pdMS_TO_TICKS(20));
        if (len > 0) {
            uart_write_bytes(USB_UART_NUM, (const char *) data, len);
        }
    }
}

void app_main(void) {
    ESP_LOGI(TAG, "Iniciando hardware do modem...");
    power_on_modem();

    // Configura a UART do Modem (A7670X)
    uart_config_t modem_uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(MODEM_UART_NUM, &modem_uart_config);
    uart_set_pin(MODEM_UART_NUM, MODEM_TX_PIN, MODEM_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(MODEM_UART_NUM, BUF_SIZE * 2, 0, 0, NULL, 0);

    // Reinstala o driver da USB (UART0) para permitir leitura direta
    uart_driver_install(USB_UART_NUM, BUF_SIZE * 2, 0, 0, NULL, 0);

    ESP_LOGI(TAG, "Ponte Serial ativada. Tudo que entrar na USB vai para o Modem e vice-versa.");

    // Cria as tarefas de repasse (Rodando paralelamente)
    xTaskCreate(usb_to_modem_task, "usb_to_modem", 2048, NULL, 10, NULL);
    xTaskCreate(modem_to_usb_task, "modem_to_usb", 2048, NULL, 10, NULL);
}