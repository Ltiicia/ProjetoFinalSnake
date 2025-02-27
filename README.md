# Jogo Snake com Raspberry Pi Pico e Feedback Visual

**Desenvolvido por:** Letícia Gonçalves Souza

Este projeto foi desenvolvido como parte do programa **EmbarcaTech - Fase 1**, com o objetivo de consolidar conhecimentos em **ADC**, **I2C**, **PWM** e **interrupções com debouncing**. O jogo Snake foi implementado utilizando a placa **Raspberry Pi Pico**, um **joystick**, **botões físicos**, um **display OLED** e **LEDs RGB** para feedback visual.

---

## Sobre o Projeto

O projeto consiste no desenvolvimento do clássico jogo **Snake**, onde o jogador controla uma cobra que se move na tela, coletando comida e crescendo conforme avança. A implementação utiliza:

- **Joystick analógico** para controle da movimentação da cobra.
- **Display OLED** para exibição do jogo.
- **LEDs RGB** para indicar eventos no jogo.
- **Botões físicos** para pausa e reinicialização do jogo.

### Funcionalidades

1. **Movimentação da Cobra:**
   - Utiliza o **joystick** para controlar a direção.
   - Conversão analógica-digital (ADC) para interpretar os movimentos.

2. **Exibição do Jogo:**
   - Utiliza um **display OLED SSD1306** conectado via I2C.
   - Renderiza a cobra e os elementos do jogo dinamicamente.

3. **Indicação Visual com LEDs RGB:**
   - **LED Azul:** Indica que o jogo está em execução.
   - **LED Vermelho:** Pisca ao perder o jogo.
   - **LED Verde:** Acende ao coletar comida.

4. **Interrupções com Debouncing:**
   - **Botão A:** Pausa e retoma o jogo.
   - **Botão B:** Reinicia o jogo.

---

## Tecnologias e Ferramentas Utilizadas

### **Hardware:**
- **Raspberry Pi Pico (RP2040)**
- **Joystick analógico KY-023**
- **Botões físicos (A e B)**
- **Display OLED SSD1306 (I2C)**
- **LEDs RGB (PWM)**

### **Software:**
- Linguagem de programação: **C**
- Ambiente de Desenvolvimento: **Visual Studio Code**
- **Pico SDK** para controle de hardware

---

## Como Executar o Projeto

1. **Configuração do Ambiente:**
   - Instale e configure o **Pico SDK**.
   - Certifique-se de que o **Visual Studio Code** está configurado para desenvolvimento em C.
   - Conecte a **Raspberry Pi Pico** ao computador.

2. **Clonar o Repositório:**
   ```bash
   git clone https://github.com/Ltiicia/ProjetoFinalSnake.git
   cd ProjetoFinalSnake
   ```

3. **Compilar e Carregar o Código:**
   - Utilize o **CMake** para compilar o projeto.
   - Gere o arquivo `.uf2` e transfira para a **Pico**.

---

## Possíveis Melhorias Futuras

- Implementação de obstáculos para aumentar a dificuldade.
- Suporte a múltiplos jogadores.
- Expansão do sistema de pontuação e efeitos gráficos.

## Referências

- Datasheet do RP2040
- Documentação do Pico SDK
- Banco de Informações de Hardware (BIH) da BitDogLab

