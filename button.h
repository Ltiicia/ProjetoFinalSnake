#ifndef BUTTON_H
#define BUTTON_H

#include <stdbool.h>

/**
 * @brief Inicializa o botão A configurando o pino e a interrupção
 */
void button_a_init(void);

/**
 * @brief Inicializa o botão B configurando o pino e a interrupção
 */
void button_b_init(void);

/**
 * @brief Verifica se o botão A foi pressionado
 *
 * @return true se o botão foi pressionado, false caso contrário
 */
bool button_a_is_pressed(void);

/**
 * @brief Verifica se o botão B foi pressionado
 *
 * @return true se o botão foi pressionado, false caso contrário
 */
bool button_b_is_pressed(void);

#endif // BUTTON_H