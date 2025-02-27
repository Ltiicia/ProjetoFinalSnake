#ifndef LED_MATRIX_H
#define LED_MATRIX_H

#include "pico/stdlib.h"
#include <hardware/pio.h>
#include "ws2818b.pio.h" // Inclua o novo cabeçalho

// Definições de constantes
#define LED_COUNT 25 // Número de LEDs na matriz
#define LED_PIN 7    // Pino GPIO conectado aos LEDs

// Ordem da matriz de leds
extern int ordem[LED_COUNT];

// Funções da biblioteca
void init_led_matrix(PIO pio, uint sm, uint offset);
void acende_matrizLEDS(bool r, bool g, bool b, double intensidade, PIO pio, uint sm);
void desenho_pio(int desenho[][LED_COUNT], PIO pio, uint sm);
void game_over_animation(PIO pio, uint sm);
uint32_t matrix_rgb(double b, double r, double g);

#endif // LED_MATRIX_H