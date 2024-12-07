#ifndef __SERVO_H
#define __SERVO_H

#include "stdint.h"
#include "stdbool.h"
//#include "toolfuncs.h"
#include "pca9685.h"
//#include "tim.h"
#include "usart.h"
#include "math.h"


//舵机ID
#define JAW 0				//夹爪
#define ROTATE 4		//机械臂旋转
#define UPPER 8			//夹爪上下位置
#define TRAY 12 		//物料盘旋转

/*#define HAND_X 0
#define HAND_Y 1
#define HAND_Z 2*/

//夹爪开合
#define OPEN 1
#define CLOSE 0
//位置代号
#define LEFT 1
#define MID 2
#define RIGHT 3

void Gnd_to_Tray(void);
void Tray_test(void);
#endif
