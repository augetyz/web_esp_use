#include "stm32f1xx_hal.h"
#include "LED.h"
#include "sys.h"
#include "Usart.h"

int main(void)
{
  //外设初始化
  /*********************************/
  SystemClock_Config();
  LED_GPIO_Config();
  DEBUG_USART_Config();
  AT_USART_Config();
  Delay_ms(1000);
  printf("初始化OK\n");

  /*********************************/
  //执行层

  uint8_t usart1_date[100] = "USART1  ok\n";
  uint8_t usart2_date[100] = "USART2  ok\n";
  while (1)
  {
    Usart2_SendString(usart2_date);
    HAL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_PIN);
    Usart1_SendString(usart1_date);
    HAL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);
    Delay_ms(1000);
  }
}
