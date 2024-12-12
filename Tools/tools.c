#include "tools.h"

extern const double pi;
extern const double  rx; //�������ֺ������
extern const double  ry; //���������������
extern const double  r;  //�뾶
extern const double standard_speed;
uint8_t tdata[4]={0x00,0xFF,0x66,0x6B};	//���ͬ��
void Motor_SetSpeed(uint8_t address, uint8_t direction, uint16_t speed, uint8_t acceleration)
{
	uint8_t command[8];  // ������������Ϊ8�ֽ�
	
	// ���������ֽ�����
    command[0] = address;               // �����ַ
    command[1] = 0xF6;                  // �����ʶ��
    command[2] = direction; // ����0ΪCW��1ΪCCW
	
	// �ٶ�ת��Ϊ�����ֽڣ���λ�͵�λ��
    command[3] = (speed >> 8) & 0xFF;   // �ٶȸ��ֽ�
    command[4] = speed & 0xFF;          // �ٶȵ��ֽ�
	
	// ���ٶ�
    command[5] = acceleration;          // ���ٶȵ�λ
	
	 // ���ͬ����־���ٶ�������ͬ����0x00��
    //command[6] = 0x00;
		command[6] = 0x01;	//ʹ��ͬ��
	 //  �����ֽ�
	  command[7] = 0x6B;
	HAL_UART_Transmit_IT(&huart3,command,sizeof(command));
	HAL_Delay(2);
	
	/*�������*/
	/*	����01 F6 01 05 DC 0A 00 6B
			01��ʾ��ת����ΪCCW��00��ʾCW����05 DC��ʾ�ٶ�Ϊ0x05DC = 1500(RPM)�� �ְ뾶Ϊ0.05m��1r��Ӧ����0.1pi=0.31416m��60rpmΪ1r/s����0.31416m/s
			0A��ʾ���ٶȵ�λΪ0x0A = 10��00��ʾ�����ö��ͬ����01��ʾ���ã���
			�����Ҫ������ͬ����ʼ�˶�����ο������ͨѶ��ͬ�����ơ��½ڣ�*/
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
	
	
	//����˳�������Ų�����ת����Ҳδ������Ҫ���²ⶨ
}

void Motor_SetPosition(uint8_t address,uint8_t direction,uint16_t speed,uint32_t pluse)
{
	uint8_t command[13];	// ������������Ϊ13�ֽ�
	//uint8_t data2[13] = {0x01,0xFD,0x00,0x00,0x3C,0x00,0x00,0x00,0x0c,0x80,0x00,0x00,0x6B};//λ�ÿ���  �����ʽ����ַ + 0xFD + ���� + �ٶ�+ ���ٶ� + ��������4�ֽڣ� + ���/����ģʽ��־ + ���ͬ����־ + У���ֽ� 0x7D =32000

	command[0] = address;
	command[1] = 0xFD;
	command[2] = direction; // ����0ΪCW��1ΪCCW
	
	// �ٶ�ת��Ϊ�����ֽڣ���λ�͵�λ��
  command[3] = (speed >> 8) & 0xFF;   // �ٶȸ��ֽ�
  command[4] = speed & 0xFF;          // �ٶȵ��ֽ�
	
	// ���ٶ�
    command[5] = 0x00;          // ���ٶȵ�λ
	//������
	command[6] = (pluse >> 24) & 0xFF;  // ��7���ֽڣ�����ֽڣ�
  command[7] = (pluse >> 16) & 0xFF;  // ��8���ֽ�
  command[8] = (pluse >> 8) & 0xFF;   // ��9���ֽ�
  command[9] = pluse & 0xFF;          // ��10���ֽڣ�����ֽڣ�
	
	//���/����ģʽ��־
	command[10] = 0x00;
	//���ͬ����־
	command[11] = 0x00;
	//У���ֽ�
	command [12] = 0x6B;
	HAL_UART_Transmit_IT(&huart3,command,sizeof(command));
	HAL_Delay(2);
	
	/*�������*/
	/*������3200Ϊ1r*/
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
	
	
	//��������3200��������Ӧ1rǡ�ö�Ӧ������ת90��
	//Ŀǰ��Ϊ��һ��������ת
}
