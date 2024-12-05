#ifndef __MOTOR_H
#define __MOTOR_H

#include "math.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stdint.h"
//#include "toolfuncs.h"
//#include "tim.h"
#include "usart.h"
#include "gpio.h"

//����ID	F:front R:rear
#define LF_MOTOR_ID 1
#define LR_MOTOR_ID 2
#define RF_MOTOR_ID 3
#define RR_MOTOR_ID 4

//OPS���ݰ�
#define A 0			//�����
#define ANGLE_X 1	//����
#define ANGLE_Y 2	//����
#define X 3			//X����
#define Y 4			//Y����
#define V_FRONT 5	//������ٶȣ�����
//OPS��δʹ��

void Motor_SetSpeed(uint8_t address, uint8_t direction, uint16_t speed, uint8_t acceleration);
void SetCarSpeed(float vx, float vy, float vSpin);
void Motor_SetPosition(uint8_t address,uint8_t direction,uint16_t speed,uint32_t pluse);
void Car_move(float vx, float vy, float vSpin,uint32_t pluse);
void Car_turn_right(void);


#endif
