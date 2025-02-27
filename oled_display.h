#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

#include "hardware/i2c.h"

/**
 * @brief Inicializa o display e limpa o buffer
 *
 * @param i2c Instância de I2C a ser usada
 */
void oled_init(i2c_inst_t *i2c);

/**
 * @brief Limpa todo o buffer e atualiza
 */
void oled_clear(void);

/**
 * @brief Atualiza o display com o conteúdo do buffer
 */
void oled_update(void);

/**
 * @brief Desenha (ou limpa) um pixel no buffer
 *
 * @param x Coordenada X do pixel
 * @param y Coordenada Y do pixel
 * @param color Cor do pixel (1 para acender, 0 para apagar)
 */
void oled_draw_pixel(uint8_t x, uint8_t y, uint8_t color);

/**
 * @brief Desenha um caractere no buffer
 *
 * @param x Coordenada X inicial
 * @param y Coordenada Y inicial
 * @param c Caractere a ser desenhado
 */
void oled_draw_char(uint8_t x, uint8_t y, char c);

/**
 * @brief Desenha uma string
 *
 * @param x Coordenada X inicial
 * @param y Coordenada Y inicial
 * @param str String a ser desenhada
 */
void oled_draw_string(uint8_t x, uint8_t y, const char *str);

/**
 * @brief Escreve uma mensagem simples na posição (0,0)
 *
 * @param message Mensagem a ser exibida
 */
void oled_display_message(const char *message);

/**
 * @brief Desenha um quadrado cheio
 *
 * @param x Coordenada X inicial
 * @param y Coordenada Y inicial
 * @param width Largura do quadrado
 * @param height Altura do quadrado
 * @param color Cor do quadrado (1 para acender, 0 para apagar)
 */
void oled_draw_square(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color);

/**
 * @brief Desenha tons de cinza usando padrões de dithering
 *
 * @param x Coordenada X inicial
 * @param y Coordenada Y inicial
 * @param width Largura do retângulo
 * @param height Altura do retângulo
 * @param gray_level Nível de cinza (0 = preto, 1 = 25%, 2 = 50%, 3 = 75%, 4 = branco)
 */
void oled_draw_gray(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t gray_level);

/**
 * @brief Desenha uma borda (contorno) na tela inteira sem limpar o buffer
 *
 * @param style Estilo da borda (true para sólida, false para pontilhada)
 */
void oled_draw_border(bool style);

/**
 * @brief Desenha uma borda (contorno) na tela inteira
 *
 * @param style Estilo da borda (true para sólida, false para pontilhada)
 */
void oled_set_border(bool style);

#endif // OLED_DISPLAY_H