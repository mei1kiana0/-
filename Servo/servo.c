#include "servo.h"

void Gnd_to_Tray(void)
{
	PCA9685_SetServoAngle(UPPER,30); //�����ʱ��צ��������ͬһ�߶�
	HAL_Delay(1000);
	PCA9685_SetServoAngle(JAW,0); //��צץס����
	HAL_Delay(1000);
	PCA9685_SetServoAngle(UPPER,110); //�����ʱ��צ��������ͬһ�߶�
	HAL_Delay(500);
	PCA9685_SetServoAngle(ROTATE,122); //�Ƕȿ���ʹ��צ�����������Ϸ�
	HAL_Delay(1500);
	//PCA9685_SetServoAngle(UPPER,90); //�ʶȽ��ͼ�צ�߶�
	//HAL_Delay(500);
	PCA9685_SetServoAngle(JAW,30); //��צ�ͷ�����
	HAL_Delay(500);
	/*����*/
	PCA9685_SetServoAngle(ROTATE,30);
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