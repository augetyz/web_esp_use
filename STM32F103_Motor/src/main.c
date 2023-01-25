#include "stm32f1xx_hal.h"
#include "key.h"
#include "motor.h"
#include "led.h"
#include "key.h"
#include "uart.h"
#include "delay.h"
#include "sys.h"

int main(void)
{
	HAL_Init();
	SystemClock_Config();   	//设置时钟,72M
	delay_init(72);               		//初始化延时函数
	DEBUG_USART_Config();					//初始化串口
	delay_ms(100);
	LED_Init();							//初始化LED	
	KEY_Init();							//初始化按键
	
    while(1)
    {
		Usart1_SendString("OK\r\n");          	 	//打印OK提示程序运行
        delay_ms(1000);             	//每隔1s打印一次 
	}
}