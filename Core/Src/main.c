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
#include <ctype.h>
#include "tools.h"
#include "pca9685.h"
#include "servo.h"
#include "string.h"
#include "oled.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define Test 1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
  int numbers[7] = {0};                 // 用于存储每个数字，长度与输入字符串相同
  int num_index = 0;                    // 数组索引
 
uint8_t data2[13] = {0x04,0xFD,0x00,0x00,0x3C,0x00,0x00,0x00,0x0c,0x80,0x00,0x00,0x6B};//位置控制  命令格式：地址 + 0xFD + 方向 + 速度+ 加速度 + 脉冲数（4字节） + 相对/绝对模式标志 + 多机同步标志 + 校验字节 0x7D =32000
uint8_t data3[] = {0x02,0xFD,0x01,0x00,0x3C,0x00,0x00,0x00,0x19,0x00,0x00,0x00,0x6B};//位置控制  命令格式：地址 + 0xFD + 方向 + 速度+ 加速度 + 脉冲数（4字节） + 相对/绝对模式标志 + 多机同步标志 + 校验字节
uint8_t data4[] = {0x03,0xFD,0x01,0x00,0x3C,0x00,0x00,0x00,0x19,0x00,0x00,0x00,0x6B};//位置控制  命令格式：地址 + 0xFD + 方向 + 速度+ 加速度 + 脉冲数（4字节） + 相对/绝对模式标志 + 多机同步标志 + 校验字节
uint8_t data5[] = {0x04,0xFD,0x00,0x00,0x3C,0x00,0x00,0x00,0x19,0x00,0x00,0x00,0x6B};//位置控制  命令格式：地址 + 0xFD + 方向 + 速度+ 加速度 + 脉冲数（4字节） + 相对/绝对模式标志 + 多机同步标志 + 校验字节
//位置控制 初始设置为速度60rpm,脉冲6400，即1r/s，总共转2r（3200脉冲为1r）

//速度控制 初始设置为速度60rpm
uint8_t data10[5] = {0x01,0xFE,0x98,0x01,0x6B}; //立即停止
uint8_t data11[5] = {0x02,0xFE,0x98,0x01,0x6B}; //立即停止
uint8_t data12[5] = {0x03,0xFE,0x98,0x01,0x6B}; //立即停止
uint8_t data13[5] = {0x04,0xFE,0x98,0x01,0x6B}; //立即停止	同步控制
//速度控制 电机立即停止
uint8_t data14[] = {0x01,0xFD,0x00,0x00,0x6B,0x00,0x00,0x00,0x0c,0x80,0x00,0x01,0x6B};
uint8_t data15[] = {0x02,0xFD,0x00,0x00,0x6B,0x00,0x00,0x00,0x0c,0x80,0x00,0x01,0x6B};
uint8_t data16[] = {0x03,0xFD,0x00,0x00,0x6B,0x00,0x00,0x00,0x0c,0x80,0x00,0x01,0x6B};
uint8_t data17[] = {0x04,0xFD,0x00,0x00,0x6B,0x00,0x00,0x00,0x0c,0x80,0x00,0x01,0x6B};	//右转

uint8_t data18[] = {0x01,0xFD,0x01,0x00,0x6B,0x00,0x00,0x00,0x0c,0x80,0x00,0x01,0x6B};
uint8_t data19[] = {0x02,0xFD,0x01,0x00,0x6B,0x00,0x00,0x00,0x0c,0x80,0x00,0x01,0x6B};
uint8_t data20[] = {0x03,0xFD,0x01,0x00,0x6B,0x00,0x00,0x00,0x0c,0x80,0x00,0x01,0x6B};
uint8_t data21[] = {0x04,0xFD,0x01,0x00,0x6B,0x00,0x00,0x00,0x0c,0x80,0x00,0x01,0x6B};	//左转
const double pi =3.1416;
const double  rx =23;	//两个麦轮横向距离
const double  ry =17; //两个麦轮纵向距离
const double  r =5; //半径
const double standard_speed = 31.416; // 60rpm=1r/s  =0.31416m/s  =31.416cm/s


//usart通讯相关
//extern定义见usart.c
extern uint8_t bytedata[55];
extern uint8_t camdata[8];
extern uint8_t camByte;
extern uint8_t camChar[7];
extern uint8_t tdata[4];	//多机同步
//OpenMV接收指令

uint8_t qr='Q';					//识别二维码

uint8_t locate='L';				//地面定位

uint8_t locateArm='A';			//机械臂定位
uint8_t locate_stuff='S';		//码垛前车体定位

int a = 0; //用于循环控制定位
uint8_t my_data[8] = "123+321\0";
uint8_t color_data1[7] = "123+321";
uint8_t color_receive[7];
uint8_t colorOrder[2][3]={{1,2,3},{3,2,1}};	//抓取顺序，由openMV获得，第一维为轮数

//openmv测试
#define BUFFER_SIZE 64  // 定义缓冲区大小
uint8_t rx_byte;        // 单字节接收变量
char rx_buffer[BUFFER_SIZE];  // 数据缓冲区
uint8_t buffer_index = 0;     // 当前缓冲区索引
uint8_t data_ready = 0;       // 数据接收完成标志
volatile int center_x = 0;  // 全局变量表示中心 x 坐标
volatile int center_y = 0;  // 全局变量表示中心 y 坐标


/**/


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void Motor_stop(void);
void Car_turn(int i);// i=0为右转
void ScanQR(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//先设定沿x轴运动 data[2]至data[5]
//先设定沿y轴运动
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	
	
	//目前设定流程为从启动区出发到物料盘前并识别物料//
	
	
	
	
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
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */
	//PCA9685_Init();		
	HAL_Delay(200);
	//PCA9685_SetServoAngle(Chaos,35); //设定35为居中
	
	//正在测试正方向是哪个数值
	
	//必须上电接上pca9685模块，否则会导致后续代码无法运行
	/*
	SetCarSpeed(-20,0,0);
	HAL_Delay(600);
	Motor_stop();
	HAL_Delay(100);
	SetCarSpeed(0,40,0);
	HAL_Delay(3400);
	Motor_stop();
	HAL_Delay(500);
	Car_turn(0);
	
	*/
	
	
	//ScanQR();
	numbers[0]=1;
	numbers[1]=2;
	numbers[2]=3;
	//接收到了颜色顺序的数据
	HAL_Delay(1000);
	PCA9685_SetServoAngle(UPPER,55); //假设此时夹爪与物料在同一高度
	HAL_Delay(500);
	HAL_UART_Transmit_IT(&huart1,&locate_stuff,sizeof(locate_stuff));
	HAL_Delay(5);																//发送命令让openmv扫描二维码并接收二维码信息
	HAL_UART_Receive_IT(&huart1, &rx_byte, 1);  // 开启中断接收
	
	
	
	
	/*
	HAL_UART_Transmit_IT(&huart1,&qr,sizeof(qr));
	HAL_Delay(5);																//发送命令让openmv扫描二维码并接收二维码信息
	HAL_UART_Receive_IT(&huart1, &rx_byte, 1);  // 开启中断接收
	*/

//	while(memcmp(my_data,camChar,7)!=0);
// if(memcmp(my_data,camChar,7)==0)
	//Tray_test();
	//SetCarSpeed(0,42,0);
	//HAL_Delay(3500);//设定995ms为1s
	//Motor_stop();
		



while(center_x == 0);

for(a=0;a<3;a++)
{
	if(center_x<400){
		if(center_x!=0)
	{
			if(150<=center_x&&center_x<=170&&110<=center_y&&center_y<=130)
			{break;}
			else if(130<=center_x&&center_x<=190&&90<=center_y&&center_y<=150)
		{
			SetCarSpeed((160-center_x)/8.0,(center_y-120)/7.0,0);
			HAL_Delay(260);
			Motor_stop();
			HAL_Delay(500);
		}
			else if(115<=center_x&&center_x<=205&&75<=center_y&&center_y<=165)
		{
			SetCarSpeed((160-center_x)/8.0,(center_y-120)/7.0,0);
			HAL_Delay(380);
			Motor_stop();
			HAL_Delay(500);
		}
			else
		{
			SetCarSpeed((160-center_x)/8.0,(center_y-120)/7.0,0);
			HAL_Delay(420);
			Motor_stop();
			HAL_Delay(500);
		}
	}
}
}
a=0;





HAL_Delay(50);
HAL_UART_Transmit_IT(&huart1,&qr,sizeof(qr));
HAL_Delay(500);

zhuaqu();



#if nTest

//可能的方向

//先经过一系列位移，到达暂存区。
	HAL_UART_Transmit_IT(&huart1,&locate_stuff,sizeof(locate_stuff));
	HAL_Delay(5);																//发送命令让openmv扫描二维码并接收二维码信息
	HAL_UART_Receive_IT(&huart1, &rx_byte, 1);  // 开启中断接收
	while(center_x == 0);

for(a=0;a<3;a++)
{
	if(center_x<400){
		if(center_x!=0)
	{
			if(145<=center_x&&center_x<=175&&105<=center_y&&center_y<=135)
			{break;}
			else if(130<=center_x&&center_x<=190&&90<=center_y&&center_y<=150)
		{
			SetCarSpeed((160-center_x)/8.0,(center_y-120)/7.0,0);
			HAL_Delay(250);
			Motor_stop();
			HAL_Delay(500);
		}
			else if(115<=center_x&&center_x<=205&&75<=center_y&&center_y<=165)
		{
			SetCarSpeed((160-center_x)/8.0,(center_y-120)/7.0,0);
			HAL_Delay(295);
			Motor_stop();
			HAL_Delay(500);
		}
			else
		{
			SetCarSpeed((160-center_x)/8.0,(center_y-120)/7.0,0);
			HAL_Delay(355);
			Motor_stop();
			HAL_Delay(500);
		}
	}
}
}
a=0;

//进行类似的定位操作，但精度要求更高
if(numbers[0]==1&&numbers[1]==2&&numbers[2]==3)
{
	//运动到指定位置
	//转物料盘
	fangzhi();
	//运动到指定位置
	//转物料盘
	fangzhi();
	//运动到指定位置
	//转物料盘
	fangzhi();
	
}
//进行3*2*1=6次判断语句，完成放置
//再进行相反的运动，完成抓取
#endif


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		
							
			//if (HAL_UART_Transmit_IT(&huart1,&locate_stuff,sizeof(locate_stuff))== HAL_OK)
			{HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_5);
			HAL_Delay(500);}
			HAL_Delay(2);
			
	}
		//HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_SET);
		//HAL_Delay(1);
  
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
	HAL_UART_Transmit_IT(&huart3,data10,sizeof(data10));
	HAL_Delay(1);
	HAL_UART_Transmit_IT(&huart3,data11,sizeof(data11));
	HAL_Delay(1);
	HAL_UART_Transmit_IT(&huart3,data12,sizeof(data12));
	HAL_Delay(1);
	HAL_UART_Transmit_IT(&huart3,data13,sizeof(data13));
	HAL_Delay(1);
	HAL_UART_Transmit_IT(&huart3,tdata,sizeof(tdata));
	HAL_Delay(2);
}

void Car_turn(int i)
{
	if(i==0)
	{HAL_UART_Transmit_IT(&huart3,data14,sizeof(data14));
	HAL_Delay(1);
	HAL_UART_Transmit_IT(&huart3,data15,sizeof(data15));
	HAL_Delay(1);
	HAL_UART_Transmit_IT(&huart3,data16,sizeof(data16));
	HAL_Delay(1);
	HAL_UART_Transmit_IT(&huart3,data17,sizeof(data17));
	HAL_Delay(1);
	HAL_UART_Transmit_IT(&huart3,tdata,sizeof(tdata));
	HAL_Delay(2);}
	
	else
	{
		
	HAL_UART_Transmit_IT(&huart3,data18,sizeof(data18));
	HAL_Delay(1);
	HAL_UART_Transmit_IT(&huart3,data19,sizeof(data19));
	HAL_Delay(1);
	HAL_UART_Transmit_IT(&huart3,data20,sizeof(data20));
	HAL_Delay(1);
	HAL_UART_Transmit_IT(&huart3,data21,sizeof(data21));
	HAL_Delay(1);
	HAL_UART_Transmit_IT(&huart3,tdata,sizeof(tdata));
	HAL_Delay(2);
	
	}

	//SetCarSpeed(0,0,31.416/40.0); 也许能用，预期效果原地右转
}

void Locate_1(void)
{
	for(a=0;a<3;a++)
{
		if(center_x!=0)
	{
			if(150<=center_x&&center_x<=170&&100<=center_y&&center_y<=130)
			{break;}
			else if(130<=center_x&&center_x<=190&&90<=center_y&&center_y<=150)
		{
			SetCarSpeed((160-center_x)/8.0,(center_y-120)/8.0,0);
			HAL_Delay(245);
			Motor_stop();
			HAL_Delay(500);
		}
			else if(115<=center_x&&center_x<=205&&75<=center_y&&center_y<=165)
		{
			SetCarSpeed((160-center_x)/8.0,(center_y-120)/8.0,0);
			HAL_Delay(295);
			Motor_stop();
			HAL_Delay(500);
		}
			else
		{
			SetCarSpeed((160-center_x)/8.0,(center_y-120)/8.0,0);
			HAL_Delay(385);
			Motor_stop();
			HAL_Delay(500);
		}
	}
}
a=0;
}

/************************************
* ScanQR：从openMV读取原料位置
* 储存为colorOrder
************************************/
void ScanQR(void)
{
	HAL_UART_Transmit_IT(&huart5,&qr,sizeof(qr));
	HAL_Delay(5);
	HAL_UART_Receive_IT(&huart5,color_receive,sizeof(color_receive));
	 // 遍历 color_receive 提取数字
    for (int i = 0; i < 7; i++) {
        if (isdigit(color_receive[i])) {      // 检测是否是数字字符
            numbers[num_index++] = color_receive[i] - '0'; // 转换为整数并存入数组
        }
    }
}



void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {  // 确保是 USART3 的中断
       // static uint8_t rx_byte;       // 单字节接收变量

        // 接收数据并放入缓冲区
        HAL_UART_Receive_IT(&huart1, &rx_byte, 1);  // 再次启动中断接收
        if (rx_byte == '\n') {        // 检测到结束符
            rx_buffer[buffer_index] = '\0';  // 添加字符串结束符
            buffer_index = 0;        // 重置缓冲区索引

            // 解析接收到的数据
						
            char *token = strtok(rx_buffer, ",");  // 按逗号分隔
            if (token != NULL) {
                center_x = atoi(token);           // 更新中心 x 坐标
                token = strtok(NULL, ",");
                if (token != NULL) {
                    center_y = atoi(token);       // 更新中心 y 坐标
                }
            }
        } else {
            // 防止缓冲区溢出
            if (buffer_index < BUFFER_SIZE - 1) {
                rx_buffer[buffer_index++] = rx_byte;
            }
        }
    }
		
		if (huart->Instance == UART5) {  // 确保是 UART5 的中断
			HAL_UART_Receive_IT(&huart5,color_receive,sizeof(color_receive));}
		
		
}
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
