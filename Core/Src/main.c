#include "main.h"
#include "fdcan.h"
#include "gpio.h"

void SystemClock_Config(void);

#define NODE_ID 2
#define SDO_TX (0x600 + NODE_ID)
#define SDO_RX (0x580 + NODE_ID)
int main(void)
{
	// ВНИМАНИЕ КОСТЫЛЬ!!!!
	// ST, вероятно в файле system_stm32g4xx.c, портит значение регистра SCB->VTOR.
	// Из-за этого отваливается таблица прерываний
	// g_pfnVectors указатель на начало таблицы прерываний из файла startup_stm32g474retx.s
	extern void *g_pfnVectors;
	SCB->VTOR = (uint32_t)&g_pfnVectors;
	__enable_irq();
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();
	/* Configure the system clock */
	SystemClock_Config();
	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_FDCAN1_Init();
	FDCAN1_FilterSet(SDO_TX, 0);
	HAL_FDCAN_Start(&hfdcan1);

	HAL_Delay(1000);
	uint8_t TxData[8] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};
	uint8_t RxData[8] = {0};
	uint32_t RxDataLen = 0;
	FDCAN1_TransmitMessage(SDO_RX, TxData, 8);
	while (1)
	{
		FDCAN1_ReceiveMessage(RxData, &RxDataLen);
		if(RxDataLen)
		{
			Error_Handler();
		}
		HAL_GPIO_TogglePin(user_led_GPIO_Port, user_led_Pin);
		HAL_Delay(1000);
	}
}
/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/** Configure the main internal regulator output voltage
	 */
	HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV6;
	RCC_OscInitStruct.PLL.PLLN = 85;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
	{
		Error_Handler();
	}
}
/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
}
#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	   ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
}
#endif /* USE_FULL_ASSERT */
