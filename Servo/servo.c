#include "servo.h"
extern volatile int center_x;
extern volatile int center_y;
void Gnd_to_Tray(void)
{
	PCA9685_SetServoAngle(UPPER,30); //假设此时夹爪与物料在同一高度
	HAL_Delay(1000);
	PCA9685_SetServoAngle(JAW,0); //夹爪抓住物料
	HAL_Delay(1000);
	PCA9685_SetServoAngle(UPPER,110); //假设此时夹爪与物料在同一高度
	HAL_Delay(500);
	PCA9685_SetServoAngle(ROTATE,122); //角度控制使夹爪处于物料盘上方
	HAL_Delay(1500);
	PCA9685_SetServoAngle(UPPER,90); //适度降低夹爪高度
	HAL_Delay(500);
	PCA9685_SetServoAngle(JAW,30); //夹爪释放物料
	HAL_Delay(500);
	/*回正*/
	PCA9685_SetServoAngle(ROTATE,30);
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
if(center_x==1)
{	center_x=0;
	PCA9685_SetServoAngle(TRAY,-34);
	HAL_Delay(200);
	Gnd_to_Tray();
	count++;
}
else if(center_x==2)
{	center_x=0;
	PCA9685_SetServoAngle(TRAY,36);
	HAL_Delay(200);
	Gnd_to_Tray();
	count++;}	
else if(center_x==3)
{	center_x=0;
	PCA9685_SetServoAngle(TRAY,-34);
	HAL_Delay(200);
	Gnd_to_Tray();
	count++;}

}
}