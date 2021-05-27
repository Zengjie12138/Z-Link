/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "debug.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define block_num 8
#define block_size 512
#define TESTADDR 0x00000020
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SD_HandleTypeDef hsd;
DMA_HandleTypeDef hdma_sdio;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
__align(4) uint8_t BufferRx[block_num * block_size];
__align(4) uint8_t BufferTx[block_num * block_size];

FATFS fs;                 // Work area (file system object) for logical drive
FIL fil;                  // file objects
uint32_t byteswritten;                /* File write counts */
uint32_t bytesread;                   /* File read counts */
uint8_t wtext[] = "This is STM32 working with FatFs"; /* File write buffer */
uint8_t rtext[100];                     /* File read buffers */
char filename[] = "STM32cube.txt";

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SDIO_SD_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int eBuffercmp(uint8_t* pBuffer, uint32_t BufferLength)
{
    while(BufferLength--)
    {
        if((*pBuffer != 0xFF) && (*pBuffer != 0))
        {
            return -1;
        }
        pBuffer++;
    }
    return 0;
}

int Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength)
{
    while(BufferLength--)
    {
        if(*pBuffer1 != *pBuffer2)
        {
            return -1;
        }
        pBuffer1++;
        pBuffer2++;
    }
    return 0;
}

void Fill_Buffer(uint8_t* pBuffer, uint32_t BufferLength, uint32_t offset)
{
    uint32_t index = 0;

    for(index = 0; index < BufferLength; index++)
    {
        pBuffer[index] = index + offset;
    }
}

void set_dma_read(SD_HandleTypeDef* hsd)
{
    /* SDIO DMA DeInit */
    HAL_DMA_DeInit(hsd->hdmatx);

    hdma_sdio.Instance = DMA2_Channel4;
    hdma_sdio.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_sdio.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_sdio.Init.MemInc = DMA_MINC_ENABLE;
    hdma_sdio.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_sdio.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_sdio.Init.Mode = DMA_NORMAL;
    hdma_sdio.Init.Priority = DMA_PRIORITY_LOW;
    if(HAL_DMA_Init(&hdma_sdio) != HAL_OK)
    {
        Error_Handler();
    }

    __HAL_LINKDMA(hsd, hdmarx, hdma_sdio);
}

void set_dma_write(SD_HandleTypeDef* hsd)
{

    /* SDIO DMA DeInit */
    HAL_DMA_DeInit(hsd->hdmarx);

    hdma_sdio.Instance = DMA2_Channel4;
    hdma_sdio.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_sdio.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_sdio.Init.MemInc = DMA_MINC_ENABLE;
    hdma_sdio.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_sdio.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_sdio.Init.Mode = DMA_NORMAL;
    hdma_sdio.Init.Priority = DMA_PRIORITY_LOW;
    if(HAL_DMA_Init(&hdma_sdio) != HAL_OK)
    {
        Error_Handler();
    }

    __HAL_LINKDMA(hsd, hdmatx, hdma_sdio);
}

void SD_EraseTest(void)
{
    int state = -1;

    if(HAL_OK == HAL_SD_Erase(&hsd, TESTADDR, TESTADDR + block_num * block_size))
    {
        printf("SD Erase over\r\n");
    }
    else
    {
        printf("SD Erase fail\r\n");
        while(1);
    }

    HAL_Delay(500);              //bug1

    state = HAL_SD_ReadBlocks(&hsd, BufferRx, TESTADDR, block_num, 0xffff);
    if(HAL_OK == state)
    {
        printf("SD read\r\n");
        if(0 == eBuffercmp(BufferRx, block_num * block_size))
        {
            printf("erase successful\r\n");
        }
    }
    else
    {
        printf("error:%d\r\n", state);
        printf("SD read error\r\n");
        printf("errcode:%d\r\n", HAL_SD_GetError(&hsd));
        while(1);
    }

}

int state = -1;
void SD_Write_Read_Test(void)
{

    Fill_Buffer(BufferTx, block_num * block_size, 0x0001);

    set_dma_write(&hsd);
    if(HAL_OK == HAL_SD_WriteBlocks_DMA(&hsd, BufferTx, TESTADDR, block_num))
    {
        printf("write ok\r\n");
    }

    HAL_Delay(1000);

    set_dma_read(&hsd);
    if(HAL_SD_ReadBlocks_DMA(&hsd, BufferRx, TESTADDR, block_num) == HAL_OK)
    {
        printf("read ok\r\n");
    }

    HAL_Delay(1000);

    if(0 == Buffercmp(BufferTx, BufferRx, block_num * block_size))
        printf("¶ÁÐ´²âÊÔ³É¹¦£¡\r\n");
    else
        printf("¶ÁÐ´²âÊÔÊ§°Ü£¡\r\n ");
}

void Fatfs_RW_test(void)
{
    printf("\r\n ****** FatFs Example ******\r\n\r\n");

    /*##-1- Register the file system object to the FatFs module ##############*/
    retSD = f_mount(&fs, "", 1);
    if(retSD)
    {
        printf(" mount error : %d \r\n", retSD);
        Error_Handler();
    }
    else
        printf(" mount sucess!!! \r\n");

    /*##-2- Create and Open new text file objects with write access ######*/
    retSD = f_open(&fil, "note.txt", FA_CREATE_ALWAYS | FA_WRITE);
    if(retSD)
        printf(" open file error : %d\r\n", retSD);
    else
        printf(" open file sucess!!! \r\n");

    /*##-3- Write data to the text files ###############################*/
    retSD = f_write(&fil, "welcome my home!", sizeof("welcome my home!"), (void*)&byteswritten);
    if(retSD)
        printf(" write file error : %d\r\n", retSD);
    else
    {
        printf(" write file sucess!!! \r\n");
        printf(" write Data : %s\r\n", wtext);
    }


    /*##-4- Close the open text files ################################*/
    retSD = f_close(&fil);
    if(retSD)
        printf(" close error : %d\r\n", retSD);
    else
        printf(" close sucess!!! \r\n");

    /*##-5- Open the text files object with read access ##############*/
    retSD = f_open(&fil, "note.txt", FA_READ);
    if(retSD)
        printf(" open file error : %d\r\n", retSD);
    else
        printf(" open file sucess!!! \r\n");

    /*##-6- Read data from the text files ##########################*/
    retSD = f_read(&fil, rtext, sizeof(rtext), (UINT*)&bytesread);
    if(retSD)
        printf(" read error!!! %d\r\n", retSD);
    else
    {
        printf(" read sucess!!! \r\n");
        printf(" read Data : %s\r\n", rtext);
    }

    /*##-7- Close the open text files ############################*/
    retSD = f_close(&fil);
    if(retSD)
        printf(" close error!!! %d\r\n", retSD);
    else
        printf(" close sucess!!! \r\n");

    /*##-8- Compare read data with the expected data ############*/
    if(bytesread == byteswritten)
    {
        printf(" FatFs is working well!!!\r\n");
    }
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_SDIO_SD_Init();
    MX_USART1_UART_Init();
    MX_FATFS_Init();
    /* USER CODE BEGIN 2 */

//  SD_EraseTest();
//  SD_Write_Read_Test();
    Fatfs_RW_test();

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while(1)
    {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
  * @brief SDIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_SDIO_SD_Init(void)
{

    /* USER CODE BEGIN SDIO_Init 0 */

    /* USER CODE END SDIO_Init 0 */

    /* USER CODE BEGIN SDIO_Init 1 */

    /* USER CODE END SDIO_Init 1 */
    hsd.Instance = SDIO;
    hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
    hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
    hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
    hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
    hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
    hsd.Init.ClockDiv = 4;
    /* USER CODE BEGIN SDIO_Init 2 */

    /* USER CODE END SDIO_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

    /* USER CODE BEGIN USART1_Init 0 */

    /* USER CODE END USART1_Init 0 */

    /* USER CODE BEGIN USART1_Init 1 */

    /* USER CODE END USART1_Init 1 */
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if(HAL_UART_Init(&huart1) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN USART1_Init 2 */

    /* USER CODE END USART1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

    /* DMA controller clock enable */
    __HAL_RCC_DMA2_CLK_ENABLE();

    /* DMA interrupt init */
    /* DMA2_Channel4_5_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA2_Channel4_5_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(DMA2_Channel4_5_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

}

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
    while(1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
