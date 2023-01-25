#include "uart.h"
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
// extern uint8_t ucTemp;

/**
 * @brief  DEBUG_USART GPIO 閰嶇�?,宸ヤ綔妯″紡閰嶇疆銆�?115200 8-N-1
 * @param  鏃�
 * @retval 鏃�
 */
void DEBUG_USART_Config(void)
{

	huart1.Instance = DEBUG_USART;

	huart1.Init.BaudRate = DEBUG_USART_BAUDRATE;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;

	HAL_UART_Init(&huart1);

	/*浣胯兘涓插彛鎺ユ敹鏂�? */
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
}
void AT_USART_Config(void)
{

	huart2.Instance = AT_USART;

	huart2.Init.BaudRate = AT_USART_BAUDRATE;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;

	HAL_UART_Init(&huart2);

	/*浣胯兘涓插彛鎺ユ敹鏂�? */
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);
}

/**
 * @brief UART MSP 鍒濆��??�?
 * @param huart: UART handle
 * @retval 鏃�
 */
void HAL_UART_MspInit(UART_HandleTypeDef *usart_Handle)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	if (usart_Handle->Instance == DEBUG_USART)
	{
		DEBUG_USART_CLK_ENABLE();

		DEBUG_USART_RX_GPIO_CLK_ENABLE();
		DEBUG_USART_TX_GPIO_CLK_ENABLE();

		/**USART1 GPIO Configuration
		 PA9     ------> USART1_TX
		PA10    ------> USART1_RX
		*/
		/* 閰嶇疆Tx�?曡剼涓哄�嶇敤鍔熻兘  */
		GPIO_InitStruct.Pin = DEBUG_USART_TX_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStruct);

		/* 閰嶇疆Rx�?曡剼涓哄�嶇敤鍔熻兘 */
		GPIO_InitStruct.Pin = DEBUG_USART_RX_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_INPUT; //�?″紡瑕佽�剧疆涓哄�嶇敤杈撳叆妯″紡锛�
		HAL_GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStruct);
		//鎶㈠崰浼樺厛绾�0锛屽瓙浼樺厛绾�1
		HAL_NVIC_SetPriority(DEBUG_USART_IRQ, 0, 1);
		//浣胯兘USART1涓�鏂��?氶亾
		HAL_NVIC_EnableIRQ(DEBUG_USART_IRQ);
	}
	else
	{
		if (usart_Handle->Instance == AT_USART)
		{
			AT_USART_CLK_ENABLE();

			AT_USART_RX_GPIO_CLK_ENABLE();
			AT_USART_TX_GPIO_CLK_ENABLE();

			// 閰嶇疆Tx�?曡剼涓哄�嶇敤鍔熻兘
			GPIO_InitStruct.Pin = AT_USART_TX_PIN;
			GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
			GPIO_InitStruct.Pull = GPIO_PULLUP;
			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
			HAL_GPIO_Init(AT_USART_TX_GPIO_PORT, &GPIO_InitStruct);

			// 閰嶇疆Rx�?曡剼涓哄�嶇敤鍔熻兘
			GPIO_InitStruct.Pin = AT_USART_RX_PIN;
			GPIO_InitStruct.Mode = GPIO_MODE_AF_INPUT; //�?″紡瑕佽�剧疆涓哄�嶇敤杈撳叆妯″紡锛�
			HAL_GPIO_Init(AT_USART_RX_GPIO_PORT, &GPIO_InitStruct);
			//鎶㈠崰浼樺厛绾�0锛屽瓙浼樺厛绾�1
			HAL_NVIC_SetPriority(AT_USART_IRQ, 0, 1);
			//浣胯兘USART1涓�鏂��?氶亾
			HAL_NVIC_EnableIRQ(AT_USART_IRQ);
		}
	}
}
/*
void HAL_UART_MspInit(UART_HandleTypeDef *huart2)
{
	GPIO_InitTypeDef  GPIO_InitStruct;


	AT_USART_CLK_ENABLE();

	AT_USART_RX_GPIO_CLK_ENABLE();
	AT_USART_TX_GPIO_CLK_ENABLE();


	// 閰嶇疆Tx�?曡剼涓哄�嶇敤鍔熻兘
	GPIO_InitStruct.Pin = AT_USART_TX_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed =  GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(AT_USART_TX_GPIO_PORT, &GPIO_InitStruct);

	// 閰嶇疆Rx�?曡剼涓哄�嶇敤鍔熻兘
	GPIO_InitStruct.Pin = AT_USART_RX_PIN;
	GPIO_InitStruct.Mode=GPIO_MODE_AF_INPUT;	//�?″紡瑕佽�剧疆涓哄�嶇敤杈撳叆妯″紡锛�
	HAL_GPIO_Init(AT_USART_RX_GPIO_PORT, &GPIO_InitStruct);
	//鎶㈠崰浼樺厛绾�0锛屽瓙浼樺厛绾�1
	HAL_NVIC_SetPriority(AT_USART_IRQ ,0,1);
		//浣胯兘USART1涓�鏂��?氶亾
	HAL_NVIC_EnableIRQ(AT_USART_IRQ );

}*/

/*****************  鍙戦�?佸瓧绗︿�? **********************/
void Usart1_SendString(uint8_t *str)
{
	unsigned int k = 0;
	do
	{
		HAL_UART_Transmit(&huart1, (uint8_t *)(str + k), 1, 1000);
		k++;
	} while (*(str + k) != '\0');
}
void Usart2_SendString(uint8_t *str)
{
	unsigned int k = 0;
	do
	{
		HAL_UART_Transmit(&huart2, (uint8_t *)(str + k), 1, 1000);
		k++;
	} while (*(str + k) != '\0');
}
//閲嶅畾鍚慶搴撳嚱鏁皃rintf鍒�?��?�鍙�DEBUG_USART锛岄噸瀹氬悜鍚庡彲浣跨�?printf鍑芥�?
int fputc(int ch, FILE *f)
{
	/* 鍙戦�?佷�?�涓�瀛�?�妭鏁版嵁鍒颁�?�鍙�DEBUG_USART */
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 1000);

	return (ch);
}

//閲嶅畾鍚慶搴撳嚱鏁皊canf鍒�?��?�鍙�DEBUG_USART锛岄噸鍐欏悜鍚庡彲浣跨敤scanf銆乬etchar绛�?�嚱鏁�
int fgetc(FILE *f)
{
	int ch;
	HAL_UART_Receive(&huart1, (uint8_t *)&ch, 1, 1000);
	return (ch);
}
