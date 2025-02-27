#include "led_matrix.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "ws2818b.pio.h" // Inclua o novo cabeçalho

// Ordem da matriz de leds
int ordem[] = {0, 1, 2, 3, 4, 9, 8, 7, 6, 5, 10, 11, 12, 13, 14, 19, 18, 17, 16, 15, 20, 21, 22, 23, 24};

// Função para definir a intensidade de cores do LED
uint32_t matrix_rgb(double b, double r, double g)
{
    unsigned char R, G, B;
    R = r * 255;
    G = g * 255;
    B = b * 255;
    return (G << 24) | (R << 16) | (B << 8);
}

// Função para acender todos os LEDs com uma cor específica e intensidade em ponto flutuante
void acende_matrizLEDS(bool r, bool g, bool b, double intensidade, PIO pio, uint sm)
{
    double R = r * intensidade;
    double G = g * intensidade;
    double B = b * intensidade;
    for (uint i = 0; i < LED_COUNT; ++i)
    {
        pio_sm_put_blocking(pio, sm, matrix_rgb(B, R, G));
    }
}

// Função para desenhar na matriz de LEDs
void desenho_pio(int desenho[][LED_COUNT], PIO pio, uint sm)
{
    uint32_t valor_led;
    for (int16_t k = 0; k < 15; k++)
    {
        for (int16_t i = 0; i < LED_COUNT; i++)
        {
            switch (desenho[k][ordem[24 - i]])
            {
            case 0:
                valor_led = matrix_rgb(0.0, 0.0, 0.0);
                break;
            case 1:
                valor_led = matrix_rgb(1, 0.0, 0.0); // azul
                break;
            case 2:
                valor_led = matrix_rgb(0.0, 1, 0.0); // vermelho
                break;
            case 3:
                valor_led = matrix_rgb(0.0, 0.0, 1); // verde
                break;
            case 4:
                valor_led = matrix_rgb(1, 1, 1); // branco
                break;
            case 5:
                valor_led = matrix_rgb(0, 1, 0.5); // laranja
                break;
            case 6:
                valor_led = matrix_rgb(0, 1, 1); // amarelo
                break;
            case 7:
                valor_led = matrix_rgb(1, 1, 0); // roxo
                break;
            }
            pio_sm_put_blocking(pio, sm, valor_led);
        }
    }
}

// Função para inicializar a PIO para controle da matriz de LEDs
void init_led_matrix(PIO pio, uint sm, uint offset)
{
    ws2818b_program_init(pio, sm, offset, LED_PIN, 800000); // Inicialize o novo programa PIO
}

// Função para animação de game over
void game_over_animation(PIO pio, uint sm)
{
    int x_pattern[5][5] = {
        {1, 0, 0, 0, 1},
        {0, 1, 0, 1, 0},
        {0, 0, 1, 0, 0},
        {0, 1, 0, 1, 0},
        {1, 0, 0, 0, 1}};

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (x_pattern[i][j] == 1)
            {
                pio_sm_put_blocking(pio, sm, matrix_rgb(1.0, 0.0, 0.0)); // Red
            }
            else
            {
                pio_sm_put_blocking(pio, sm, matrix_rgb(0.0, 0.0, 0.0)); // Off
            }
        }
    }
    sleep_ms(500);
    acende_matrizLEDS(0, 0, 0, 0, pio, sm); // Turn off all LEDs
    sleep_ms(500);
}