#include "pico/stdlib.h"
#include <stdio.h>

#define BUTTON_A_PIN 5 // Pino do botão A
#define BUTTON_B_PIN 6 // Pino do botão B
#define DEBOUNCE_DELAY_MS 50

static volatile bool button_a_pressed = false;
static volatile bool button_b_pressed = false;
static uint32_t last_button_a_press_time = 0;
static uint32_t last_button_b_press_time = 0;

/**
 * @brief Callback para tratar o pressionamento dos botões com lógica de debounce
 *
 * @param gpio Pino GPIO do botão
 * @param events Eventos de interrupção
 */
void button_callback(uint gpio, uint32_t events)
{
    uint32_t current_time = to_ms_since_boot(get_absolute_time());
    if (gpio == BUTTON_A_PIN && current_time - last_button_a_press_time > DEBOUNCE_DELAY_MS)
    {
        last_button_a_press_time = current_time;
        button_a_pressed = true;
    }
    else if (gpio == BUTTON_B_PIN && current_time - last_button_b_press_time > DEBOUNCE_DELAY_MS)
    {
        last_button_b_press_time = current_time;
        button_b_pressed = true;
    }
}

/**
 * @brief Inicializa o botão A configurando o pino e a interrupção
 */
void button_a_init()
{
    // Configuração do botão A no pino 5
    gpio_init(BUTTON_A_PIN);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_A_PIN);
    gpio_set_irq_enabled_with_callback(BUTTON_A_PIN, GPIO_IRQ_EDGE_FALL, true, &button_callback);
    printf("Botão A inicializado no pino %d\n", BUTTON_A_PIN);
}

/**
 * @brief Inicializa o botão B configurando o pino e a interrupção
 */
void button_b_init()
{
    // Configuração do botão B no pino 6
    gpio_init(BUTTON_B_PIN);
    gpio_set_dir(BUTTON_B_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_B_PIN);
    gpio_set_irq_enabled_with_callback(BUTTON_B_PIN, GPIO_IRQ_EDGE_FALL, true, &button_callback);
    printf("Botão B inicializado no pino %d\n", BUTTON_B_PIN);
}

/**
 * @brief Verifica se o botão A foi pressionado
 *
 * @return true se o botão foi pressionado, false caso contrário
 */
bool button_a_is_pressed()
{
    if (button_a_pressed)
    {
        button_a_pressed = false;
        return true;
    }
    return false;
}

/**
 * @brief Verifica se o botão B foi pressionado
 *
 * @return true se o botão foi pressionado, false caso contrário
 */
bool button_b_is_pressed()
{
    if (button_b_pressed)
    {
        button_b_pressed = false;
        return true;
    }
    return false;
}