#include"stm32f1xx_hal.h"
#include"LED.h"

void Delay_us(int16_t nus) 
{
  int32_t temp; 
  SysTick->LOAD = nus*9; //72MHz
  SysTick->VAL=0X00;
  SysTick->CTRL=0X01;
  do 
  { 
    temp=SysTick->CTRL;
  }
  while((temp&0x01)&&(!(temp&(1<<16))));
     
  SysTick->CTRL=0x00; 
  SysTick->VAL =0X00; 
}

void Delay_ms(int16_t nms)
{
    int a = 0;
    for (a = 0; a < nms;a++)
    {
        Delay_us(1000);
    }
}

int main(void)
{
    SystemClock_Config();
    LED_GPIO_Config();
    LED1(ON);
    LED2(ON);
    LED3(OFF);
    while (1)
    {
        LED1(ON);
        LED2(ON);
        LED3(OFF);
        Delay_ms(1000);
        LED1(OFF);
        LED2(OFF);
        LED3(ON);
        Delay_ms(1000);
    }
    
}


