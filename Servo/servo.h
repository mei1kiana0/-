#ifndef __SERVO_H
#define __SERVO_H

#include "stdint.h"
#include "stdbool.h"
//#include "toolfuncs.h"
#include "pca9685.h"
//#include "tim.h"
#include "usart.h"
#include "math.h"


//���ID
#define JAW 0				//��צ
#define ROTATE 4		//��е����ת
#define UPPER 8			//��צ����λ��
#define TRAY 12 		//��������ת

/*#define HAND_X 0
#define HAND_Y 1
#define HAND_Z 2*/

//��צ����
#define OPEN 1
#define CLOSE 0
//λ�ô���
#define LEFT 1
#define MID 2
#define RIGHT 3

void Gnd_to_Tray(void);
void Tray_test(void);
#endif
