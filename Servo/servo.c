#include "servo.h"

extern volatile int center_x;
extern volatile int center_y;
extern   int numbers[7];
void Gnd_to_Tray(void)
{
	PCA9685_SetServoAngle(UPPER,35); //�����ʱ��צ��������ͬһ�߶�
	HAL_Delay(500);
	PCA9685_SetServoAngle(JAW,0); //��צץס����
	HAL_Delay(500);
	PCA9685_SetServoAngle(UPPER,120); //�����ʱ��צ����������ͬһ�߶�
	HAL_Delay(500);
	PCA9685_SetServoAngle(ROTATE,86); //�Ƕȿ���ʹ��צ�����������Ϸ�
	HAL_Delay(500);
	PCA9685_SetServoAngle(UPPER,95); //�ʶȽ��ͼ�צ�߶�
	HAL_Delay(700);
	PCA9685_SetServoAngle(JAW,30); //��צ�ͷ�����
	HAL_Delay(500);
	PCA9685_SetServoAngle(UPPER,120);
	HAL_Delay(400);
	
	/*����*/
	PCA9685_SetServoAngle(ROTATE,-5);
	HAL_Delay(500);
}

void Tray_test(void)
{
	PCA9685_SetServoAngle(TRAY,-34);
	HAL_Delay(1500);
	PCA9685_SetServoAngle(TRAY,36);
	HAL_Delay(1500);
	PCA9685_SetServoAngle(TRAY,110);
	HAL_Delay(1500);
}

void zhuaqu(void)
{
	int count = 0;
while(count<3){
	
if(center_x==numbers[0]&&count==0) //���ʶ�𵽵�������ɫΪ�������һλ���֣���count==0��ץ����
{	
	PCA9685_SetServoAngle(TRAY,-34);
	HAL_Delay(200);
	Gnd_to_Tray();
	count++;
}
else if(center_x==numbers[1]&&count==1) //���ʶ�𵽵�������ɫΪ������ڶ�λ���֣���count==1��ץ����
{	
	PCA9685_SetServoAngle(TRAY,36);
	HAL_Delay(200);
	Gnd_to_Tray();
	count++;}	
else if(center_x==numbers[2]&&count==2)	//���ʶ�𵽵�������ɫΪ���������λ���֣���count==2��ץ����
{	
	PCA9685_SetServoAngle(TRAY,-34);
	HAL_Delay(200);
	Gnd_to_Tray();
	count++;}

}
}

void fangzhi(void)
{
	//�ݶ�Ϊ���������������ʼλ��
	PCA9685_SetServoAngle(UPPER,120); //�����ʱ��צ��������ͬһ�߶�
	HAL_Delay(1000);
	PCA9685_SetServoAngle(ROTATE,87); //�Ƕȿ���ʹ��צ�����������Ϸ�
	HAL_Delay(1500);
	PCA9685_SetServoAngle(UPPER,35); //�����ʱ��צ��������ͬһ�߶�
	HAL_Delay(1000);
	PCA9685_SetServoAngle(JAW,0); //��צץס����
	HAL_Delay(500);
	PCA9685_SetServoAngle(UPPER,35); //�����ʱ��צ��������ͬһ�߶�
	HAL_Delay(1000);
	PCA9685_SetServoAngle(ROTATE,-5);
	HAL_Delay(500);
	PCA9685_SetServoAngle(JAW,30); //��צץס����	
}