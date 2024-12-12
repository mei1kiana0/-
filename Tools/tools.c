#include "tools.h"

extern const double pi;
extern const double  rx; //两个麦轮横向距离
extern const double  ry; //两个麦轮纵向距离
extern const double  r;  //半径
extern const double standard_speed;
uint8_t tdata[4]={0x00,0xFF,0x66,0x6B};	//多机同步
void Motor_SetSpeed(uint8_t address, uint8_t direction, uint16_t speed, uint8_t acceleration)
{
	uint8_t command[8];  // 假设最大命令长度为8字节
	
	// 构建命令字节序列
    command[0] = address;               // 电机地址
    command[1] = 0xF6;                  // 命令标识符
    command[2] = direction; // 方向，0为CW，1为CCW
	
	// 速度转换为两个字节（高位和低位）
    command[3] = (speed >> 8) & 0xFF;   // 速度高字节
    command[4] = speed & 0xFF;          // 速度低字节
	
	// 加速度
    command[5] = acceleration;          // 加速度档位
	
	 // 多机同步标志：假定不启用同步（0x00）
    //command[6] = 0x00;
		command[6] = 0x01;	//使用同步
	 //  检验字节
	  command[7] = 0x6B;
	HAL_UART_Transmit_IT(&huart3,command,sizeof(command));
	HAL_Delay(2);
	
	/*命令解析*/
	/*	发送01 F6 01 05 DC 0A 00 6B
			01表示旋转方向为CCW（00表示CW），05 DC表示速度为0x05DC = 1500(RPM)， 轮半径为0.05m，1r对应距离0.1pi=0.31416m，60rpm为1r/s，即0.31416m/s
			0A表示加速度档位为0x0A = 10，00表示不启用多机同步（01表示启用），
			如果需要多个电机同步开始运动，请参考“多机通讯及同步控制”章节；*/
}

void SetCarSpeed(float vx, float vy, float vSpin)
{
	float vLF = vx+vy+(rx+ry)*vSpin;
	float vLR = -vx+vy+(rx+ry)*vSpin;
	float vRF = -vx+vy-(rx+ry)*vSpin;
	float vRR = vx+vy-(rx+ry)*vSpin;
	int V[4];
	
	int flag[4]={0,0,1,1};
	if(vLF<1e-3f)
		flag[0]=1;
	if(vLR<1e-3f)
		flag[1]=1;
	if(vRF<1e-3f)
		flag[2]=0;
	if(vRR<1e-3f)
		flag[3]=0;
	V[0] = abs((int)(vLF/standard_speed*60-2));
	V[1] = abs((int)(vLR/standard_speed*60));
	V[2] = abs((int)(vRF/standard_speed*60-2));
	V[3] = abs((int)(vRR/standard_speed*60));
	Motor_SetSpeed(LF_MOTOR_ID,flag[0],V[0],0x00);
	HAL_Delay(2);
	Motor_SetSpeed(LR_MOTOR_ID,flag[1],V[1],0x00);
	HAL_Delay(2);
	Motor_SetSpeed(RF_MOTOR_ID,flag[2],V[2],0x00);
	HAL_Delay(2);
	Motor_SetSpeed(RR_MOTOR_ID,flag[3],V[3],0x00);
	HAL_Delay(2);
	HAL_UART_Transmit_IT(&huart3,tdata,sizeof(tdata));
	HAL_Delay(2);
	
	
	//轮子顺序重新排布，旋转方向也未定，需要重新测定
}

void Motor_SetPosition(uint8_t address,uint8_t direction,uint16_t speed,uint32_t pluse)
{
	uint8_t command[13];	// 假设最大命令长度为13字节
	//uint8_t data2[13] = {0x01,0xFD,0x00,0x00,0x3C,0x00,0x00,0x00,0x0c,0x80,0x00,0x00,0x6B};//位置控制  命令格式：地址 + 0xFD + 方向 + 速度+ 加速度 + 脉冲数（4字节） + 相对/绝对模式标志 + 多机同步标志 + 校验字节 0x7D =32000

	command[0] = address;
	command[1] = 0xFD;
	command[2] = direction; // 方向，0为CW，1为CCW
	
	// 速度转换为两个字节（高位和低位）
  command[3] = (speed >> 8) & 0xFF;   // 速度高字节
  command[4] = speed & 0xFF;          // 速度低字节
	
	// 加速度
    command[5] = 0x00;          // 加速度档位
	//脉冲数
	command[6] = (pluse >> 24) & 0xFF;  // 第7个字节（最高字节）
  command[7] = (pluse >> 16) & 0xFF;  // 第8个字节
  command[8] = (pluse >> 8) & 0xFF;   // 第9个字节
  command[9] = pluse & 0xFF;          // 第10个字节（最低字节）
	
	//相对/绝对模式标志
	command[10] = 0x00;
	//多机同步标志
	command[11] = 0x00;
	//校验字节
	command [12] = 0x6B;
	HAL_UART_Transmit_IT(&huart3,command,sizeof(command));
	HAL_Delay(2);
	
	/*命令解释*/
	/*脉冲数3200为1r*/
}

void Car_move(float vx, float vy, float vSpin,uint32_t pluse)
{
	float vLF = vx+vy+(rx+ry)*vSpin;
	float vLR = -vx+vy+(rx+ry)*vSpin;
	float vRF = -vx+vy-(rx+ry)*vSpin;
	float vRR = vx+vy-(rx+ry)*vSpin;
	int V[4];
	
	uint8_t flag[4]={0};
	if(vLF<1e-3f)
		flag[0]=1;
	if(vLR<1e-3f)
		flag[1]=1;
	if(vRF<1e-3f)
		flag[2]=1;
	if(vRR<1e-3f)
		flag[3]=1;
	V[0] = abs((int)(vLF/standard_speed*60+1));
	V[1] = abs((int)(vLR/standard_speed*60)+1);
	V[2] = abs((int)(vRF/standard_speed*60)+1);
	V[3] = abs((int)(vRR/standard_speed*60)+1);
	Motor_SetPosition(LF_MOTOR_ID,flag[0],V[0],pluse);
	HAL_Delay(2);
	Motor_SetPosition(LR_MOTOR_ID,flag[1],V[1],pluse);
	HAL_Delay(2);
	Motor_SetPosition(RF_MOTOR_ID,flag[2],V[2],pluse);
	HAL_Delay(2);
	Motor_SetPosition(LR_MOTOR_ID,flag[3],V[3],pluse);
	HAL_Delay(2);
}

void Car_turn_right(void)
{
	Motor_SetPosition(LF_MOTOR_ID,0,60,3200);
	HAL_Delay(2);
	Motor_SetPosition(LR_MOTOR_ID,0,60,3200);
	HAL_Delay(2);
	Motor_SetPosition(RF_MOTOR_ID,0,60,3200);
	HAL_Delay(2);
	Motor_SetPosition(RF_MOTOR_ID,0,60,3200);
	HAL_Delay(2);
	
	
	//以往测试3200脉冲数对应1r恰好对应底盘旋转90°
	//目前仅为往一个方向旋转
}
