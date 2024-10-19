/************************************************************************************************************
* FILE:         main.c                                                                                      *
* AUTHOR:       Thiago Silva                                                                                *
* CREATION:     18 OUT 2024                                                                                 *
* MODIFICATION: 19 OUT 2024                                                                                 *
* FRAMEWORK:    ESPRESSIF IDF                                                                               *
* DESCRIPTION:  Envia mensagens MIDI seriais continuamente utilizando o Esp32                               *
*               variando de 21 a 108 de forma sequencial                                                    *
*************************************************************************************************************/

#include "driver/uart.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#define UART_NUM UART_NUM_0
#define TX_PIN UART_PIN_NO_CHANGE
#define RX_PIN UART_PIN_NO_CHANGE

// CONFIG
#define MIDI_BAUD_RATE  115200
#define MIDI_NOTE_ON    0b10010000
#define MIDI_NOTE_OFF   0b10000000
#define INTENSITY       0b01111111

// NOTAS
#define G1      31
#define C2      36
#define G2      43
#define C4      60
#define C_SHARP 61
#define D4      62
#define D_SHARP 63
#define E4      64
#define F4      65
#define F_SHARP 66
#define G4      67
#define G_SHARP 68
#define A4      69
#define A_SHARP 70
#define B4      71
#define C5      72


// UART UART
void uart_init()
{
    const uart_config_t uart_config = {
        .baud_rate = MIDI_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };

    uart_param_config(UART_NUM, &uart_config);
    uart_set_pin(UART_NUM, TX_PIN, RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_NUM, 1024 * 2, 0, 0, NULL, 0);
}

// FUNCAO DE ENVIO DE DADOS
void send_midi(uint8_t status, uint8_t data_1, uint8_t data_2)
{
    uint8_t midi_msg[3];
    midi_msg[0] = status;
    midi_msg[1] = data_1;
    midi_msg[2] = data_2;

    uart_write_bytes(UART_NUM, (const char*)midi_msg, 3);
}

// LACO DE ENVIO CONTINUO DE MENSAGENS
void midi_task(void* param)
{
    int i = 21;
    while (true)
    {
        send_midi(MIDI_NOTE_ON, i, INTENSITY);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        send_midi(MIDI_NOTE_OFF, i, 0);

        if (i == 108)
        {
            i = 21;
        }
        else
        {
            i++;
        }
    }
}

// FUNCAO PRINCIPAL
void app_main()
{
    uart_init();
    xTaskCreate(midi_task, "midi_task", 2048, NULL, 5, NULL);
}
