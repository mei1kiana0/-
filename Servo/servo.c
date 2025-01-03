#include "servo.h"

extern volatile int center_x;
extern volatile int center_y;
extern   int numbers[7];
void Gnd_to_Tray(int i)
{
	if(i==0){
	PCA9685_SetServoAngle(UPPER,10); //�����ʱ��צ��������ͬһ�߶�
	HAL_Delay(500);
	PCA9685_SetServoAngle(JAW,0); //��צץס����
	HAL_Delay(200);
	PCA9685_SetServoAngle(UPPER,105); //�����ʱ��צ����������ͬһ�߶�
	HAL_Delay(600);
	PCA9685_SetServoAngle(ROTATE,89); //�Ƕȿ���ʹ��צ�����������Ϸ�
	HAL_Delay(500);
	PCA9685_SetServoAngle(UPPER,80); //�ʶȽ��ͼ�צ�߶�
	HAL_Delay(300);
	PCA9685_SetServoAngle(JAW,40); //��צ�ͷ�����
	HAL_Delay(500);
	
	
	/*����*/
	PCA9685_SetServoAngle(UPPER,110); //�����ʱ��צ����������ͬһ�߶�
	HAL_Delay(400);
	PCA9685_SetServoAngle(ROTATE,-3);
	HAL_Delay(500);
	PCA9685_SetServoAngle(UPPER,50);
	HAL_Delay(400);}
	
	else
	{
	PCA9685_SetServoAngle(UPPER,-48); //�����ʱ��צ��������ͬһ�߶�
	HAL_Delay(1000);
	PCA9685_SetServoAngle(JAW,0); //��צץס����
	HAL_Delay(200);
	PCA9685_SetServoAngle(UPPER,105); //�����ʱ��צ����������ͬһ�߶�
	HAL_Delay(600);
	PCA9685_SetServoAngle(ROTATE,89); //�Ƕȿ���ʹ��צ�����������Ϸ�
	HAL_Delay(800);
	PCA9685_SetServoAngle(UPPER,80); //�ʶȽ��ͼ�צ�߶�
	HAL_Delay(500);
	PCA9685_SetServoAngle(JAW,40); //��צ�ͷ�����
	HAL_Delay(500);
	
	
	/*����*/
	PCA9685_SetServoAngle(UPPER,105); //�����ʱ��צ����������ͬһ�߶�
	HAL_Delay(500);
	PCA9685_SetServoAngle(ROTATE,-3);
	HAL_Delay(500);
	PCA9685_SetServoAngle(UPPER,50);
	HAL_Delay(400);
	}
}

void Tray_test(void)
{
	PCA9685_SetServoAngle(TRAY,-34);
	HAL_Delay(1500);
	PCA9685_SetServoAngle(TRAY,38);
	HAL_Delay(1500);
	PCA9685_SetServoAngle(TRAY,110);
	HAL_Delay(1500);
}

void zhuaqu_1(void)
{
	int count = 0;
while(count<3){
	
if(center_x==numbers[0]&&count==0) //���ʶ�𵽵�������ɫΪ�������һλ���֣���count==0��ץ����
{	
	PCA9685_SetServoAngle(TRAY,-34);
	HAL_Delay(800);
	Gnd_to_Tray(0);
	count++;
}
else if(center_x==numbers[1]&&count==1) //���ʶ�𵽵�������ɫΪ������ڶ�λ���֣���count==1��ץ����
{	
	HAL_Delay(1500);
	PCA9685_SetServoAngle(TRAY,36);
	HAL_Delay(200);
	Gnd_to_Tray(0);
	count++;}	
else if(center_x==numbers[2]&&count==2)	//���ʶ�𵽵�������ɫΪ���������λ���֣���count==2��ץ����
{	HAL_Delay(1500);
	PCA9685_SetServoAngle(TRAY,110);
	HAL_Delay(200);
	Gnd_to_Tray(0);
	count++;}

}
}




void fangzhi(void)
{
	//�ݶ�Ϊ���������������ʼλ��
	PCA9685_SetServoAngle(UPPER,100); //�����ʱ��צ��������ͬһ�߶�
	HAL_Delay(1000);
	PCA9685_SetServoAngle(ROTATE,89); //�Ƕȿ���ʹ��צ�����������Ϸ�
	HAL_Delay(1500);
	PCA9685_SetServoAngle(UPPER,70); //�����ʱ��צ��������ͬһ�߶�
	HAL_Delay(1000);
	PCA9685_SetServoAngle(JAW,0); //��צץס����
	HAL_Delay(500);
	PCA9685_SetServoAngle(UPPER,100); //�����ʱ��צ��������ͬһ�߶�
	HAL_Delay(1000);
	PCA9685_SetServoAngle(ROTATE,-3);
	HAL_Delay(500);
	PCA9685_SetServoAngle(UPPER,-49); //�����ʱ��צ��������ͬһ�߶�
	HAL_Delay(1000);
	PCA9685_SetServoAngle(JAW,30); //��צ�ͷ�����	
	HAL_Delay(1000);
	PCA9685_SetServoAngle(UPPER,120);
}