#include "driver/uart.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#define UART_NUM UART_NUM_0
#define TX_PIN UART_PIN_NO_CHANGE
#define RX_PIN UART_PIN_NO_CHANGE

// CONFIGURACOES DO MIDI
#define MIDI_BAUD_RATE 115200
#define MIDI_NOTE_ON 0x90
#define MIDI_NOTE_OFF 0x80
#define VELOCITY_ON 127
#define VELOCITY_OFF 0

// NOTAS
#define G1 31
#define C2 36
#define G2 43
#define C4 60
#define C_SHARP 61
#define D4 62
#define D_SHARP 63
#define E4 64
#define F4 65
#define F_SHARP 66
#define G4 67
#define G_SHARP 68
#define A4 69
#define A_SHARP 70
#define B4 71
#define C5 72



void uart_init() {
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

void send_midi(uint8_t status, uint8_t data1, uint8_t data2) {
    uint8_t midi_message[3];
    midi_message[0] = status;
    midi_message[1] = data1;
    midi_message[2] = data2;

    uart_write_bytes(UART_NUM, (const char*)midi_message, 3);
}

void midi_task(void* param) {
    while (1) {
        send_midi(MIDI_NOTE_ON, C4, VELOCITY_ON);
        send_midi(MIDI_NOTE_ON, E4, VELOCITY_ON);
        send_midi(MIDI_NOTE_ON, G4, VELOCITY_ON);
        send_midi(MIDI_NOTE_ON, C5, VELOCITY_ON);
        send_midi(MIDI_NOTE_ON, G1, VELOCITY_ON);
        send_midi(MIDI_NOTE_ON, C2, VELOCITY_ON);
        send_midi(MIDI_NOTE_ON, G2, VELOCITY_ON);
        vTaskDelay(2000 / portTICK_PERIOD_MS);

        send_midi(MIDI_NOTE_OFF, C4, VELOCITY_OFF);
        send_midi(MIDI_NOTE_OFF, E4, VELOCITY_OFF);
        send_midi(MIDI_NOTE_OFF, G4, VELOCITY_OFF);
        send_midi(MIDI_NOTE_OFF, C5, VELOCITY_OFF);
        send_midi(MIDI_NOTE_OFF, G1, VELOCITY_OFF);
        send_midi(MIDI_NOTE_OFF, C2, VELOCITY_OFF);
        send_midi(MIDI_NOTE_OFF, G2, VELOCITY_OFF);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void app_main() {
    uart_init();
    xTaskCreate(midi_task, "midi_task", 2048, NULL, 5, NULL);
}
