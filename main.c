#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "led_control.h"
#include "joystick.h"
#include "button.h"
#include "oled_display.h"
#include "led_matrix.h"
// Remova a linha abaixo
// #include "ws2818b.pio.h"

#define I2C_PORT i2c1 // Usando i2c1 pois GPIO14 e 15 são do I2C1
#define I2C_SDA_PIN 14
#define I2C_SCL_PIN 15

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
#define SQUARE_SIZE 8

typedef struct
{
  int x;
  int y;
} Point;

typedef struct
{
  Point body[128];
  int length;
  Point direction;
} Snake;

static Snake snake;
static Point food;
static bool game_over = false;
static bool game_paused = false;

void setup()
{
  stdio_init_all();
  adc_init();
  i2c_init(I2C_PORT, 400 * 1000); // Initialize I2C at 400kHz
  gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
  gpio_pull_up(I2C_SDA_PIN);
  gpio_pull_up(I2C_SCL_PIN);

  led_init();
  joystick_init();
  button_a_init();
  button_b_init();     // Initialize button B
  oled_init(I2C_PORT); // Pass the I2C port to oled_init

  // Initialize snake
  snake.length = 1;
  snake.body[0].x = DISPLAY_WIDTH / 2;
  snake.body[0].y = DISPLAY_HEIGHT / 2;
  snake.direction.x = 1;
  snake.direction.y = 0;

  // Initialize food
  food.x = rand() % (DISPLAY_WIDTH / SQUARE_SIZE) * SQUARE_SIZE;
  food.y = rand() % (DISPLAY_HEIGHT / SQUARE_SIZE) * SQUARE_SIZE;
}

void reset_game()
{
  // Reset snake
  snake.length = 1;
  snake.body[0].x = DISPLAY_WIDTH / 2;
  snake.body[0].y = DISPLAY_HEIGHT / 2;
  snake.direction.x = 1;
  snake.direction.y = 0;

  // Reset food
  food.x = rand() % (DISPLAY_WIDTH / SQUARE_SIZE) * SQUARE_SIZE;
  food.y = rand() % (DISPLAY_HEIGHT / SQUARE_SIZE) * SQUARE_SIZE;

  // Reset game over flag
  game_over = false;
  game_paused = false;
}

void update_snake()
{
  // Move snake body
  for (int i = snake.length - 1; i > 0; i--)
  {
    snake.body[i] = snake.body[i - 1];
  }

  // Move snake head
  snake.body[0].x += snake.direction.x * SQUARE_SIZE;
  snake.body[0].y += snake.direction.y * SQUARE_SIZE;

  // Check for collisions with walls and wrap around
  if (snake.body[0].x < 0)
  {
    snake.body[0].x = DISPLAY_WIDTH - SQUARE_SIZE;
  }
  else if (snake.body[0].x >= DISPLAY_WIDTH)
  {
    snake.body[0].x = 0;
  }
  if (snake.body[0].y < 0)
  {
    snake.body[0].y = DISPLAY_HEIGHT - SQUARE_SIZE;
  }
  else if (snake.body[0].y >= DISPLAY_HEIGHT)
  {
    snake.body[0].y = 0;
  }

  // Check for collisions with itself
  for (int i = 1; i < snake.length; i++)
  {
    if (snake.body[0].x == snake.body[i].x && snake.body[0].y == snake.body[i].y)
    {
      game_over = true;
    }
  }

  // Check for collisions with food
  if (snake.body[0].x == food.x && snake.body[0].y == food.y)
  {
    snake.length++;
    food.x = rand() % (DISPLAY_WIDTH / SQUARE_SIZE) * SQUARE_SIZE;
    food.y = rand() % (DISPLAY_HEIGHT / SQUARE_SIZE) * SQUARE_SIZE;

    // Turn on LED on GPIO 12
    led_set_color(255, 0, 0); // Acende o LED RGB com a cor vermelha
    sleep_ms(200);
    led_set_color(0, 0, 0); // Apaga o LED RGB
  }
}

void update_display()
{
  oled_clear();

  // Draw snake
  for (int i = 0; i < snake.length; i++)
  {
    oled_draw_square(snake.body[i].x, snake.body[i].y, SQUARE_SIZE, SQUARE_SIZE, 1); // Cor da cobra
  }

  // Draw food
  oled_draw_gray(food.x, food.y, SQUARE_SIZE, SQUARE_SIZE, 2); // Cor da comida em 50% cinza

  oled_update();
}

void read_joystick()
{
  int joystick_x = joystick_read_x();
  int joystick_y = joystick_read_y();

  if (joystick_x > 3000 && snake.direction.x == 0)
  {
    snake.direction.x = 1;
    snake.direction.y = 0;
  }
  else if (joystick_x < 1000 && snake.direction.x == 0)
  {
    snake.direction.x = -1;
    snake.direction.y = 0;
  }
  else if (joystick_y > 3000 && snake.direction.y == 0)
  {
    snake.direction.x = 0;
    snake.direction.y = -1; // Corrigido para mover para cima
  }
  else if (joystick_y < 1000 && snake.direction.y == 0)
  {
    snake.direction.x = 0;
    snake.direction.y = 1; // Corrigido para mover para baixo
  }
}

int main()
{
  setup();

  PIO pio = pio0;
  uint offset = pio_add_program(pio, &ws2818b_program); // Adicione o programa PIO correto
  uint sm = pio_claim_unused_sm(pio, true);
  init_led_matrix(pio, sm, offset);

  while (true)
  {
    if (button_a_is_pressed())
    {
      reset_game();
    }

    if (game_over)
    {
      oled_display_message("Game Over!");
      for (int i = 0; i < 5; i++) // Loop to make the X blink 5 times
      {
        game_over_animation(pio, sm);
      }
      while (game_over)
      {
        if (button_a_is_pressed())
        {
          reset_game();
        }
        sleep_ms(100);
      }
    }
    else
    {
      if (button_b_is_pressed())
      {
        game_paused = !game_paused; // Toggle pause state
        if (game_paused)
        {
          oled_display_message("Paused");
        }
        else
        {
          oled_clear();
          oled_update();
        }
        sleep_ms(300); // Debounce delay
      }

      if (!game_paused)
      {
        read_joystick();
        update_snake();
        update_display();
      }
      sleep_ms(100);
    }
  }

  return 0;
}