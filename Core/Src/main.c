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
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "tools.h"
#include "pca9685.h"
#include "servo.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint8_t data2[13] = {0x04,0xFD,0x00,0x00,0x3C,0x00,0x00,0x00,0x0c,0x80,0x00,0x00,0x6B};//λ�ÿ���  �����ʽ����ַ + 0xFD + ���� + �ٶ�+ ���ٶ� + ��������4�ֽڣ� + ���/����ģʽ��־ + ���ͬ����־ + У���ֽ� 0x7D =32000
uint8_t data3[] = {0x02,0xFD,0x01,0x00,0x3C,0x00,0x00,0x00,0x19,0x00,0x00,0x00,0x6B};//λ�ÿ���  �����ʽ����ַ + 0xFD + ���� + �ٶ�+ ���ٶ� + ��������4�ֽڣ� + ���/����ģʽ��־ + ���ͬ����־ + У���ֽ�
uint8_t data4[] = {0x03,0xFD,0x01,0x00,0x3C,0x00,0x00,0x00,0x19,0x00,0x00,0x00,0x6B};//λ�ÿ���  �����ʽ����ַ + 0xFD + ���� + �ٶ�+ ���ٶ� + ��������4�ֽڣ� + ���/����ģʽ��־ + ���ͬ����־ + У���ֽ�
uint8_t data5[] = {0x04,0xFD,0x00,0x00,0x3C,0x00,0x00,0x00,0x19,0x00,0x00,0x00,0x6B};//λ�ÿ���  �����ʽ����ַ + 0xFD + ���� + �ٶ�+ ���ٶ� + ��������4�ֽڣ� + ���/����ģʽ��־ + ���ͬ����־ + У���ֽ�
//λ�ÿ��� ��ʼ����Ϊ�ٶ�60rpm,����6400����1r/s���ܹ�ת2r��3200����Ϊ1r��

//�ٶȿ��� ��ʼ����Ϊ�ٶ�60rpm
uint8_t data10[5] = {0x01,0xFE,0x98,0x01,0x6B}; //����ֹͣ
uint8_t data11[5] = {0x02,0xFE,0x98,0x01,0x6B}; //����ֹͣ
uint8_t data12[5] = {0x03,0xFE,0x98,0x01,0x6B}; //����ֹͣ
uint8_t data13[5] = {0x04,0xFE,0x98,0x01,0x6B}; //����ֹͣ	ͬ������
//�ٶȿ��� �������ֹͣ
uint8_t data14[] = {0x01,0xFE,0x98,0x00,0x6B};
uint8_t data15[] = {0x02,0xFE,0x98,0x00,0x6B};
uint8_t data16[] = {0x03,0xFE,0x98,0x00,0x6B};
uint8_t data17[] = {0x04,0xFE,0x98,0x00,0x6B};

const double pi =3.1416;
const double  rx =23;	//�������ֺ������
const double  ry =17; //���������������
const double  r =5; //�뾶
const double standard_speed = 31.416; // 60rpm=1r/s  =0.31416m/s  =31.416cm/s


//usartͨѶ���
//extern�����usart.c
extern uint8_t bytedata[55];
extern uint8_t camdata[8];
extern uint8_t camByte;
extern uint8_t camChar[7];
extern uint8_t tdata[4];	//���ͬ��
//OpenMV����ָ��
uint8_t OPSreset[4]={'A','C','T','0'};	//OPS����
uint8_t qr='Q';					//ʶ���ά��
uint8_t get='G';				//δʹ��
uint8_t locate='L';				//���涨λ
uint8_t locateGet='l';			//ת�̴���λ
uint8_t locateArm='A';			//��е�۶�λ
uint8_t locate_stuff='S';		//���ǰ���嶨λ
uint8_t locateArm_stuff='s';	//���ǰ��е�۶�λ
uint8_t catch_char='c';			//ʶ��ת������
int a = 1;
uint8_t my_data[7] = "123+321";
uint8_t colorOrder[2][3]={{1,2,3},{3,2,1}};	//ץȡ˳����openMV��ã���һάΪ����

//openmv����
#define BUFFER_SIZE 64  // ���建������С
uint8_t rx_byte;        // ���ֽڽ��ձ���
char rx_buffer[BUFFER_SIZE];  // ���ݻ�����
uint8_t buffer_index = 0;     // ��ǰ����������
uint8_t data_ready = 0;       // ���ݽ�����ɱ�־
volatile int center_x = 0;  // ȫ�ֱ�����ʾ���� x ����
volatile int center_y = 0;  // ȫ�ֱ�����ʾ���� y ����
/**/


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void Motor_stop(void);
void ScanQR(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//���趨��x���˶� data[2]��data[5]
//���趨��y���˶�
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
  MX_USART1_UART_Init();
  MX_I2C1_Init();
  MX_USART3_UART_Init();
  MX_UART5_Init();
  /* USER CODE BEGIN 2 */
	//PCA9685_Init();		//�����ϵ����pca9685ģ�飬����ᵼ�º��������޷�����
	//HAL_Delay(200);
	SetCarSpeed(-40,40,0);
	HAL_Delay(195);
	Motor_stop();
	SetCarSpeed(0,42,0);
	HAL_Delay(995);
	Motor_stop();
	SetCarSpeed(0,40,0);
	HAL_Delay(1995);
	Motor_stop();
	HAL_UART_Transmit_IT(&huart3,&qr,sizeof(qr));
	HAL_Delay(5);																//����������openmvɨ���ά�벢���ն�ά����Ϣ
	HAL_UART_Receive_IT(&huart3, &rx_byte, 1);  // �����жϽ���
	
	//1250mm
	//HAL_Delay(1000);
//	while(memcmp(my_data,camChar,7)!=0);
	//if(memcmp(my_data,camChar,7)==0)
	//Tray_test();
	/*SetCarSpeed(0,42,0);
	HAL_Delay(3500);//�趨995msΪ1s
	Motor_stop();*/
		
	//SetCarSpeed(-40,0,0);
	//HAL_Delay(200);
	//Motor_stop();
	//HAL_Delay(1000);
	
	//Motor_stop();
	//HAL_Delay(2);
	//HAL_UART_Transmit_IT(&huart1,&qr,sizeof(qr));
	//HAL_Delay(2000);
	//HAL_UART_Receive_IT(&huart1, camChar, 7);
	//SetCarSpeed(0,42,0);
	//HAL_Delay(2000);
	//Motor_stop();


//HAL_UART_Transmit_IT(&huart1,data2,sizeof(data2));
//SetCarSpeed(0,31.416,0);
//HAL_Delay(985); //΢���趨980msΪ1s
//Motor_stop();
//HAL_Delay(1000);







  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		
			
			
			{HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_5);
				HAL_Delay(500);}
			}
	
		//HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_SET);
		//HAL_Delay(1);
  }
  /* USER CODE END 3 */


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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
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

/* USER CODE BEGIN 4 */
void Motor_stop(void)
{
	HAL_UART_Transmit_IT(&huart1,data10,sizeof(data10));
	HAL_Delay(1);
	HAL_UART_Transmit_IT(&huart1,data11,sizeof(data11));
	HAL_Delay(1);
	HAL_UART_Transmit_IT(&huart1,data12,sizeof(data12));
	HAL_Delay(1);
	HAL_UART_Transmit_IT(&huart1,data13,sizeof(data13));
	HAL_Delay(1);
	HAL_UART_Transmit_IT(&huart1,tdata,sizeof(tdata));
	HAL_Delay(2);
}

/************************************
* ScanQR����openMV��ȡԭ��λ��
* ����ΪcolorOrder
************************************/
void ScanQR(void)
{
	//��OpenMV����ɨ��ά��ָ��
	HAL_UART_Transmit(&huart5,(uint8_t *)&qr,sizeof(qr),0xffff);
	//HAL_UART_Receive(&huart5,camChar,7*sizeof(uint8_t),0xffff);
	HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_5);
	HAL_Delay(500);
	//HAL_UART_Receive_IT(&huart3,(uint8_t *)&aRxBuffer,1);
	for (int i=0;i<3;i++)
	{
		colorOrder[0][i]=camChar[i]-'0';
		colorOrder[1][i]=camChar[i+4]-'0';				
	}
	/*OLED_Clear();
	for (int i=0;i<3;i++)
	{
		OLED_ShowChar(i*40,0,colorOrder[0][i]+'0',32,1);
		OLED_ShowChar(i*40+20,32,colorOrder[1][i]+'0',32,1);
	}
	OLED_ShowChar(90,0,'+',32,1);
	OLED_Refresh_Gram();
	//isCamOver=false;
	//HAL_UART_Receive_IT(&huart4,(uint8_t *)&aRxBuffer, 1);*/
}

/************************************
* Locate������ջ���λУ׼
* OpenMVʶ��Բ�Ĳ�����Բ������
* ���հ���ʽ��{FF,AA,x,x,y,a,AA,EE}
************************************/
/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM7 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM7) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART3) {  // ȷ���� USART3 ���ж�
        static uint8_t rx_byte;       // ���ֽڽ��ձ���

        // �������ݲ����뻺����
        HAL_UART_Receive_IT(&huart3, &rx_byte, 1);  // �ٴ������жϽ���
        if (rx_byte == '\n') {        // ��⵽������
            rx_buffer[buffer_index] = '\0';  // �����ַ���������
            buffer_index = 0;        // ���û���������

            // �������յ�������
            char *token = strtok(rx_buffer, ",");  // �����ŷָ�
            if (token != NULL) {
                center_x = atoi(token);           // �������� x ����
                token = strtok(NULL, ",");
                if (token != NULL) {
                    center_y = atoi(token);       // �������� y ����
                }
            }
        } else {
            // ��ֹ���������
            if (buffer_index < BUFFER_SIZE - 1) {
                rx_buffer[buffer_index++] = rx_byte;
            }
        }
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