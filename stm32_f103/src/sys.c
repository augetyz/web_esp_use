#include"sys.h"

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