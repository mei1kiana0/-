#include "servo.h"

extern volatile int center_x;
extern volatile int center_y;
extern   int numbers[7];
void Gnd_to_Tray(int i)
{
	if(i==0){
	PCA9685_SetServoAngle(UPPER,10); //假设此时夹爪与物料在同一高度
	HAL_Delay(500);
	PCA9685_SetServoAngle(JAW,0); //夹爪抓住物料
	HAL_Delay(200);
	PCA9685_SetServoAngle(UPPER,105); //假设此时夹爪与物料盘在同一高度
	HAL_Delay(600);
	PCA9685_SetServoAngle(ROTATE,89); //角度控制使夹爪处于物料盘上方
	HAL_Delay(500);
	PCA9685_SetServoAngle(UPPER,80); //适度降低夹爪高度
	HAL_Delay(300);
	PCA9685_SetServoAngle(JAW,40); //夹爪释放物料
	HAL_Delay(500);
	
	
	/*回正*/
	PCA9685_SetServoAngle(UPPER,110); //假设此时夹爪与物料盘在同一高度
	HAL_Delay(400);
	PCA9685_SetServoAngle(ROTATE,-3);
	HAL_Delay(500);
	PCA9685_SetServoAngle(UPPER,50);
	HAL_Delay(400);}
	
	else
	{
	PCA9685_SetServoAngle(UPPER,-48); //假设此时夹爪与物料在同一高度
	HAL_Delay(1000);
	PCA9685_SetServoAngle(JAW,0); //夹爪抓住物料
	HAL_Delay(200);
	PCA9685_SetServoAngle(UPPER,105); //假设此时夹爪与物料盘在同一高度
	HAL_Delay(600);
	PCA9685_SetServoAngle(ROTATE,89); //角度控制使夹爪处于物料盘上方
	HAL_Delay(800);
	PCA9685_SetServoAngle(UPPER,80); //适度降低夹爪高度
	HAL_Delay(500);
	PCA9685_SetServoAngle(JAW,40); //夹爪释放物料
	HAL_Delay(500);
	
	
	/*回正*/
	PCA9685_SetServoAngle(UPPER,105); //假设此时夹爪与物料盘在同一高度
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
	
if(center_x==numbers[0]&&count==0) //如果识别到的物料颜色为任务码第一位数字，且count==0，抓物料
{	
	PCA9685_SetServoAngle(TRAY,-34);
	HAL_Delay(800);
	Gnd_to_Tray(0);
	count++;
}
else if(center_x==numbers[1]&&count==1) //如果识别到的物料颜色为任务码第二位数字，且count==1，抓物料
{	
	HAL_Delay(1500);
	PCA9685_SetServoAngle(TRAY,36);
	HAL_Delay(200);
	Gnd_to_Tray(0);
	count++;}	
else if(center_x==numbers[2]&&count==2)	//如果识别到的物料颜色为任务码第三位数字，且count==2，抓物料
{	HAL_Delay(1500);
	PCA9685_SetServoAngle(TRAY,110);
	HAL_Delay(200);
	Gnd_to_Tray(0);
	count++;}

}
}




void fangzhi(void)
{
	//暂定为依靠任务码调整初始位置
	PCA9685_SetServoAngle(UPPER,100); //假设此时夹爪与物料在同一高度
	HAL_Delay(1000);
	PCA9685_SetServoAngle(ROTATE,89); //角度控制使夹爪处于物料盘上方
	HAL_Delay(1500);
	PCA9685_SetServoAngle(UPPER,70); //假设此时夹爪与物料在同一高度
	HAL_Delay(1000);
	PCA9685_SetServoAngle(JAW,0); //夹爪抓住物料
	HAL_Delay(500);
	PCA9685_SetServoAngle(UPPER,100); //假设此时夹爪与物料在同一高度
	HAL_Delay(1000);
	PCA9685_SetServoAngle(ROTATE,-3);
	HAL_Delay(500);
	PCA9685_SetServoAngle(UPPER,-49); //假设此时夹爪与物料在同一高度
	HAL_Delay(1000);
	PCA9685_SetServoAngle(JAW,30); //夹爪释放物料	
	HAL_Delay(1000);
	PCA9685_SetServoAngle(UPPER,120);
}