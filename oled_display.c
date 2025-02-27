#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// Ajuste se seu display tiver outro endereço
#define OLED_ADDR 0x3C

#define OLED_CMD 0x00
#define OLED_DATA 0x40

// Buffer para 128x64 (1 bit/pixel = 1024 bytes)
static uint8_t buffer[1024];
static uint8_t work_buffer[1024]; // Buffer de trabalho

// Variável global para armazenar a instância de I2C usada
static i2c_inst_t *oled_i2c = NULL;

const uint8_t font[52][5] = {
    {0x7E, 0x11, 0x11, 0x11, 0x7E}, // A
    {0x7F, 0x49, 0x49, 0x49, 0x36}, // B
    {0x3E, 0x41, 0x41, 0x41, 0x22}, // C
    {0x7F, 0x41, 0x41, 0x22, 0x1C}, // D
    {0x7F, 0x49, 0x49, 0x49, 0x41}, // E
    {0x7F, 0x09, 0x09, 0x09, 0x01}, // F
    {0x3E, 0x41, 0x49, 0x49, 0x7A}, // G
    {0x7F, 0x08, 0x08, 0x08, 0x7F}, // H
    {0x00, 0x41, 0x7F, 0x41, 0x00}, // I
    {0x20, 0x40, 0x41, 0x3F, 0x01}, // J
    {0x7F, 0x08, 0x14, 0x22, 0x41}, // K
    {0x7F, 0x40, 0x40, 0x40, 0x40}, // L
    {0x7F, 0x02, 0x0C, 0x02, 0x7F}, // M
    {0x7F, 0x04, 0x08, 0x10, 0x7F}, // N
    {0x3E, 0x41, 0x41, 0x41, 0x3E}, // O
    {0x7F, 0x09, 0x09, 0x09, 0x06}, // P
    {0x3E, 0x41, 0x51, 0x21, 0x5E}, // Q
    {0x7F, 0x09, 0x19, 0x29, 0x46}, // R
    {0x46, 0x49, 0x49, 0x49, 0x31}, // S
    {0x01, 0x01, 0x7F, 0x01, 0x01}, // T
    {0x3F, 0x40, 0x40, 0x40, 0x3F}, // U
    {0x1F, 0x20, 0x40, 0x20, 0x1F}, // V
    {0x3F, 0x40, 0x38, 0x40, 0x3F}, // W
    {0x63, 0x14, 0x08, 0x14, 0x63}, // X
    {0x07, 0x08, 0x70, 0x08, 0x07}, // Y
    {0x61, 0x51, 0x49, 0x45, 0x43}, // Z
    {0x20, 0x54, 0x54, 0x54, 0x78}, // a
    {0x7F, 0x48, 0x44, 0x44, 0x38}, // b
    {0x38, 0x44, 0x44, 0x44, 0x28}, // c
    {0x38, 0x44, 0x44, 0x48, 0x7F}, // d
    {0x38, 0x54, 0x54, 0x54, 0x18}, // e
    {0x08, 0x7E, 0x09, 0x01, 0x02}, // f
    {0x08, 0x14, 0x54, 0x54, 0x3C}, // g
    {0x7F, 0x08, 0x04, 0x04, 0x78}, // h
    {0x00, 0x44, 0x7D, 0x40, 0x00}, // i
    {0x20, 0x40, 0x44, 0x3D, 0x00}, // j
    {0x7F, 0x10, 0x28, 0x44, 0x00}, // k
    {0x00, 0x41, 0x7F, 0x40, 0x00}, // l
    {0x7C, 0x04, 0x18, 0x04, 0x78}, // m
    {0x7C, 0x08, 0x04, 0x04, 0x78}, // n
    {0x38, 0x44, 0x44, 0x44, 0x38}, // o
    {0x7C, 0x14, 0x14, 0x14, 0x08}, // p
    {0x08, 0x14, 0x14, 0x18, 0x7C}, // q
    {0x7C, 0x08, 0x04, 0x04, 0x08}, // r
    {0x48, 0x54, 0x54, 0x54, 0x20}, // s
    {0x04, 0x3F, 0x44, 0x40, 0x20}, // t
    {0x3C, 0x40, 0x40, 0x20, 0x7C}, // u
    {0x1C, 0x20, 0x40, 0x20, 0x1C}, // v
    {0x3C, 0x40, 0x30, 0x40, 0x3C}, // w
    {0x44, 0x28, 0x10, 0x28, 0x44}, // x
    {0x0C, 0x50, 0x50, 0x50, 0x3C}, // y
    {0x44, 0x64, 0x54, 0x4C, 0x44}  // z
};

/**
 * @brief Atualiza o display com o conteúdo do buffer
 */
void oled_update(void)
{
  // Copia o buffer de trabalho para o buffer de exibição
  memcpy(buffer, work_buffer, sizeof(buffer));

  for (uint8_t page = 0; page < 8; page++)
  {
    // Seleciona a página
    i2c_write_blocking(oled_i2c, OLED_ADDR,
                       (uint8_t[]){OLED_CMD, (uint8_t)(0xB0 + page)}, 2, false);

    // Endereço de coluna (parte baixa e alta)
    i2c_write_blocking(oled_i2c, OLED_ADDR,
                       (uint8_t[]){OLED_CMD, 0x00}, 2, false);
    i2c_write_blocking(oled_i2c, OLED_ADDR,
                       (uint8_t[]){OLED_CMD, 0x10}, 2, false);

    // Envia os 128 bytes daquela página
    uint8_t temp[129];
    temp[0] = OLED_DATA; // Indica que os próximos são dados
    memcpy(&temp[1], &buffer[page * 128], 128);
    i2c_write_blocking(oled_i2c, OLED_ADDR, temp, 129, false);
  }
}

/**
 * @brief Inicializa o display e limpa o buffer
 */
void oled_init(i2c_inst_t *i2c)
{
  // Armazena a instância para uso em todo o módulo
  oled_i2c = i2c;

  // Sequência de inicialização para 128x64
  i2c_write_blocking(oled_i2c, OLED_ADDR,
                     (uint8_t[]){OLED_CMD, 0xAE}, 2, false); // Display off
  i2c_write_blocking(oled_i2c, OLED_ADDR,
                     (uint8_t[]){OLED_CMD, 0xD5, 0x80}, 3, false); // Display clock
  i2c_write_blocking(oled_i2c, OLED_ADDR,
                     (uint8_t[]){OLED_CMD, 0xA8, 0x3F}, 3, false); // Multiplex = 0x3F (64 linhas)
  i2c_write_blocking(oled_i2c, OLED_ADDR,
                     (uint8_t[]){OLED_CMD, 0xD3, 0x00}, 3, false); // Display offset = 0
  i2c_write_blocking(oled_i2c, OLED_ADDR,
                     (uint8_t[]){OLED_CMD, 0x40}, 2, false); // Start line = 0
  i2c_write_blocking(oled_i2c, OLED_ADDR,
                     (uint8_t[]){OLED_CMD, 0x8D, 0x14}, 3, false); // Charge pump ON
  i2c_write_blocking(oled_i2c, OLED_ADDR,
                     (uint8_t[]){OLED_CMD, 0x20, 0x00}, 3, false); // Memory addressing mode (Horizontal)
  i2c_write_blocking(oled_i2c, OLED_ADDR,
                     (uint8_t[]){OLED_CMD, 0xA1}, 2, false); // Segment remap
  i2c_write_blocking(oled_i2c, OLED_ADDR,
                     (uint8_t[]){OLED_CMD, 0xC8}, 2, false); // COM scan direction
  i2c_write_blocking(oled_i2c, OLED_ADDR,
                     (uint8_t[]){OLED_CMD, 0xDA, 0x12}, 3, false); // COM pins
  i2c_write_blocking(oled_i2c, OLED_ADDR,
                     (uint8_t[]){OLED_CMD, 0x81, 0x7F}, 3, false); // Contrast
  i2c_write_blocking(oled_i2c, OLED_ADDR,
                     (uint8_t[]){OLED_CMD, 0xD9, 0xF1}, 3, false); // Pre-charge
  i2c_write_blocking(oled_i2c, OLED_ADDR,
                     (uint8_t[]){OLED_CMD, 0xDB, 0x40}, 3, false); // VCOMH deselect
  i2c_write_blocking(oled_i2c, OLED_ADDR,
                     (uint8_t[]){OLED_CMD, 0xA4}, 2, false); // Output follows RAM
  i2c_write_blocking(oled_i2c, OLED_ADDR,
                     (uint8_t[]){OLED_CMD, 0xA6}, 2, false); // Normal display (A7 = invert)
  i2c_write_blocking(oled_i2c, OLED_ADDR,
                     (uint8_t[]){OLED_CMD, 0xAF}, 2, false); // Display ON

  // Limpa o buffer e atualiza
  memset(buffer, 0, sizeof(buffer));
  oled_update();
}

/**
 * @brief Limpa todo o buffer de trabalho
 */
void oled_clear(void)
{
  memset(work_buffer, 0, sizeof(work_buffer));
}

/**
 * @brief Desenha (ou limpa) um pixel no buffer de trabalho
 *
 * @param x Coordenada X do pixel
 * @param y Coordenada Y do pixel
 * @param color Cor do pixel (1 para acender, 0 para apagar)
 */
void oled_draw_pixel(uint8_t x, uint8_t y, uint8_t color)
{
  if (x >= 128 || y >= 64)
    return; // Fora dos limites
  uint16_t index = (y / 8) * 128 + x;
  uint8_t bit = 1 << (y % 8);

  if (color)
    work_buffer[index] |= bit; // Acende pixel
  else
    work_buffer[index] &= ~bit; // Apaga pixel
}

/**
 * @brief Desenha um caractere no buffer
 *
 * @param x Coordenada X inicial
 * @param y Coordenada Y inicial
 * @param c Caractere a ser desenhado
 */
void oled_draw_char(uint8_t x, uint8_t y, char c)
{
  if (c < 'A' || c > 'z')
    return; // Caractere fora do intervalo imprimível

  uint8_t index = (c >= 'a') ? (c - 'a' + 26) : (c - 'A');
  for (uint8_t i = 0; i < 5; i++)
  {
    uint8_t line = font[index][i];
    for (uint8_t j = 0; j < 8; j++)
    {
      oled_draw_pixel(x + i, y + j, (line >> j) & 1);
    }
  }
}

/**
 * @brief Desenha uma string
 *
 * @param x Coordenada X inicial
 * @param y Coordenada Y inicial
 * @param str String a ser desenhada
 */
void oled_draw_string(uint8_t x, uint8_t y, const char *str)
{
  while (*str)
  {
    oled_draw_char(x, y, *str);
    x += 6; // Largura de caractere 5 + 1 de espaço
    str++;
  }
}

/**
 * @brief Escreve uma mensagem simples na posição (0,0)
 *
 * @param message Mensagem a ser exibida
 */
void oled_display_message(const char *message)
{
  oled_clear();
  oled_draw_string(0, 0, message);
  oled_update();
}

/**
 * @brief Desenha tons de cinza usando padrões de dithering
 *
 * @param x Coordenada X inicial
 * @param y Coordenada Y inicial
 * @param width Largura do retângulo
 * @param height Altura do retângulo
 * @param gray_level Nível de cinza (0 = preto, 1 = 25%, 2 = 50%, 3 = 75%, 4 = branco)
 */
void oled_draw_gray(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t gray_level)
{
  for (uint8_t i = 0; i < width; i++)
  {
    for (uint8_t j = 0; j < height; j++)
    {
      uint8_t pattern = 0;
      switch (gray_level)
      {
      case 0: // Preto
        pattern = 0;
        break;
      case 1: // 25% Cinza
        pattern = ((i % 2 == 0 && j % 2 == 0) || (i % 2 != 0 && j % 2 != 0)) ? 1 : 0;
        break;
      case 2: // 50% Cinza
        pattern = ((i + j) % 2 == 0) ? 1 : 0;
        break;
      case 3: // 75% Cinza
        pattern = ((i % 2 == 0 && j % 2 != 0) || (i % 2 != 0 && j % 2 == 0)) ? 0 : 1;
        break;
      case 4: // Branco
        pattern = 1;
        break;
      default:
        pattern = 0;
        break;
      }
      oled_draw_pixel(x + i, y + j, pattern);
    }
  }
}

/**
 * @brief Desenha um quadrado cheio
 *
 * @param x Coordenada X inicial
 * @param y Coordenada Y inicial
 * @param width Largura do quadrado
 * @param height Altura do quadrado
 * @param color Cor do quadrado (1 para acender, 0 para apagar)
 */
void oled_draw_square(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color)
{
  for (uint8_t i = 0; i < width; i++)
  {
    for (uint8_t j = 0; j < height; j++)
    {
      oled_draw_pixel(x + i, y + j, color);
    }
  }
}

/**
 * @brief Desenha uma borda (contorno) na tela inteira sem limpar o buffer
 *
 * @param style Estilo da borda (true para sólida, false para pontilhada)
 */
void oled_draw_border(bool style)
{
  if (style)
  {
    // Borda sólida
    for (uint8_t i = 0; i < 128; i++)
    {
      oled_draw_pixel(i, 0, 1);  // Topo
      oled_draw_pixel(i, 63, 1); // Base
    }
    for (uint8_t i = 0; i < 64; i++)
    {
      oled_draw_pixel(0, i, 1);   // Esquerda
      oled_draw_pixel(127, i, 1); // Direita
    }
  }
  else
  {
    // Borda pontilhada
    for (uint8_t i = 0; i < 128; i += 2)
    {
      oled_draw_pixel(i, 0, 1);
      oled_draw_pixel(i, 63, 1);
    }
    for (uint8_t i = 0; i < 64; i += 2)
    {
      oled_draw_pixel(0, i, 1);
      oled_draw_pixel(127, i, 1);
    }
  }
}

/**
 * @brief Desenha uma borda (contorno) na tela inteira
 *
 * @param style Estilo da borda (true para sólida, false para pontilhada)
 */
void oled_set_border(bool style)
{
  oled_clear();
  oled_draw_border(style);
  oled_update();
}