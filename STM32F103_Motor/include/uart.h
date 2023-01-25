#ifndef _UART_H
#define _UART_H

#include "stm32f1xx.h"
#include <stdio.h>

//串口波特�?
#define DEBUG_USART_BAUDRATE                    115200

//DeBug使用串口1�?
/*******************************************************/
#define DEBUG_USART                             USART1
#define DEBUG_USART_CLK_ENABLE()                __HAL_RCC_USART1_CLK_ENABLE();

#define DEBUG_USART_RX_GPIO_PORT                GPIOA
#define DEBUG_USART_RX_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()
#define DEBUG_USART_RX_PIN                      GPIO_PIN_10

#define DEBUG_USART_TX_GPIO_PORT                GPIOA
#define DEBUG_USART_TX_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define DEBUG_USART_TX_PIN                      GPIO_PIN_9

#define DEBUG_USART_IRQHandler                  USART1_IRQHandler
#define DEBUG_USART_IRQ                 		    USART1_IRQn
/************************************************************/

//模块通�??使用串口2�?
/*******************************************************/
#define AT_USART_BAUDRATE                    115200
#define AT_USART                             USART2
#define AT_USART_CLK_ENABLE()                __HAL_RCC_USART2_CLK_ENABLE();

#define AT_USART_RX_GPIO_PORT                GPIOA
#define AT_USART_RX_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()
#define AT_USART_RX_PIN                      GPIO_PIN_3

#define AT_USART_TX_GPIO_PORT                GPIOA
#define AT_USART_TX_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define AT_USART_TX_PIN                      GPIO_PIN_2

#define AT_USART_IRQHandler                  USART2_IRQHandler
#define AT_USART_IRQ                 		        USART2_IRQn
/************************************************************/



void Usart1_SendString(uint8_t *str);
void DEBUG_USART_Config(void);
void Usart2_SendString(uint8_t *str);
void AT_USART_Config(void);
int fputc(int ch, FILE *f);
int fgetc(FILE *f);
extern UART_HandleTypeDef UartHandle;

#endif