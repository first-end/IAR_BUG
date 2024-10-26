/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <rtthread.h>
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "driver_oled.h"
#include "circle_buffer.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
struct soft_timer {
	uint32_t timeout;
	void * args;
	void (*func)(void *);
};
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern UART_HandleTypeDef huart1;

int g_key_cnt = 0;

void key_timeout_func(void *args);

struct soft_timer key_timer = {~0, NULL, key_timeout_func};

static uint8_t g_data_buf[100];
static circle_buf g_key_bufs;

void key_timeout_func(void *args)
{
	uint8_t key_val; /* 按下显示0x1, 松开 0x81 */
	g_key_cnt++;
	key_timer.timeout = ~0;
	
	/* read gpio */
	if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) == GPIO_PIN_RESET)
		key_val = 0x1;
	else
		key_val = 0x81;
	
	/* put key val into circle buf */
	circle_buf_write(&g_key_bufs, key_val);
}

void mod_timer(struct soft_timer *pTimer, uint32_t timeout)
{
	pTimer->timeout = HAL_GetTick() + timeout;
}

void check_timer(void)
{
	if (key_timer.timeout <= HAL_GetTick())
	{
		key_timer.func(key_timer.args);
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == GPIO_PIN_3)
	{		
		mod_timer(&key_timer, 10);
	}
}
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void UsageFaultInit(void)
{
    SCB->SHCSR |= (SCB_SHCSR_USGFAULTENA_Msk);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
    void EnableDiv0Bug(void);
    void A(int n, int m);
    void AA(void);
    void led_blink(void *parameter);
    int add(int a, int b);
    void buf_overflow(int n, char val);

    int (*f)(int a, int b);
    
    //f = add;
    
    EnableDiv0Bug();
    
    //A(100, 0);

    int len;
    char *str = "www.100ask.net\r\n";
    char *str2 = "Please enter a char: \r\n";
    char c;
    
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  

  /* USER CODE BEGIN Init */
  rt_thread_t led_thread;
  led_thread = rt_thread_create("ledthread", led_blink, RT_NULL, 256, 29, 20);
  if(led_thread !=RT_NULL)
  {
    rt_thread_startup(led_thread);
  }
  /* USER CODE END Init */

  

  /* USER CODE BEGIN SysInit */
	circle_buf_init(&g_key_bufs, 100, g_data_buf);
  /* USER CODE END SysInit */

 
  /* USER CODE BEGIN 2 */
	OLED_Init();
	 
	// 清屏
	OLED_Clear();
	
	OLED_PrintString(0, 0, "Cnt     : ");
  len = OLED_PrintString(0, 2, "Key val : ");
         rt_kprintf(str); 
        //f(1, 2);
    buf_overflow(8, 'A'); // bit0=1,THUMB; bit0=0,ARM
    //A(100, 1);


	EnableDiv0Bug();

	//A(100, 0);
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		rt_thread_mdelay(100);
		// while (HAL_OK != HAL_UART_Receive(&huart1, &c, 1, 100));
		
		// c = c+1;
		
		// HAL_UART_Transmit(&huart1, &c, 1, 1000);
		// HAL_UART_Transmit(&huart1, "\r\n", 2, 1000);
		
//		OLED_PrintSignedVal(len, 0, g_key_cnt);
//	  uint8_t key_val = 0;
//	  if (0 == circle_buf_read(&g_key_bufs, &key_val))
//	  {
//		  OLED_ClearLine(len, 2);
//		  OLED_PrintHex(len, 2, key_val, 1);
//	  }
		/* set LED1\LED2 output high */
		//HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_SET);
		//HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_RESET);		
		
		//HAL_Delay(500);
			
		/* set LED1\LED2 output low */
		//HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_RESET);
		//HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_SET);	
		
		//HAL_Delay(500);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */


/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

void rt_hw_console_output(const char *str)
{
    rt_size_t i = 0, size = 0;
    char a = '\r';

    rt_enter_critical();

    size = rt_strlen(str);
    for (i = 0; i < size; i++)
    {
        if (*(str + i) == '\n')
        {
            HAL_UART_Transmit(&huart1, (uint8_t *)&a, 1, 1);
        }
        HAL_UART_Transmit(&huart1, (uint8_t *)(str + i), 1, 1);
    }
    rt_exit_critical();
}
//通过HAL_UART_Receive 获取输入值
// char rt_hw_console_getchar(void)
// {
//     uint8_t order =-1;
//     if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE) != RESET)
//     {
//         if(!HAL_OK == HAL_UART_Receive(&huart1, &order, 1, 100))
//           order = -1;
        
//     }
//     else
//     {
//         if(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_ORE) != RESET)
//         {
//             __HAL_UART_CLEAR_OREFLAG(&huart1);
//         }
//         rt_thread_mdelay(10);
//     }
//     return order;
// }
//直接从寄存器获取输入值
char rt_hw_console_getchar(void)
{
    int ch = -1;
    if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE) != RESET)
    {
        ch = huart1.Instance->DR & 0xff;
        
    }
    else
    {
        if(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_ORE) != RESET)
        {
            __HAL_UART_CLEAR_OREFLAG(&huart1);
        }
        rt_thread_mdelay(10);
    }
    return ch;
}
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}
#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}


#endif /* USE_FULL_ASSERT */
