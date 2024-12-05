#include "pca9685.h"


	void PCA9685_Init(void)
{
    uint8_t data[2];
    // ��������ģʽ�Ա�����Ԥ��Ƶ��
    data[0] = PCA9685_MODE1;
    data[1] = 0x10;
    HAL_I2C_Master_Transmit(&hi2c1, PCA9685_ADDRESS, data, 2, HAL_MAX_DELAY);
		HAL_Delay(2);
    // ����PWMƵ��Ϊ50Hz�����ͨ����Ҫ50Hz��
    uint8_t prescale = 121; // Ƶ�ʼ��㣺prescale = round(25000000 / (4096 * 50)) - 1
    data[0] = PCA9685_PRESCALE;
    data[1] = prescale;
    HAL_I2C_Master_Transmit(&hi2c1, PCA9685_ADDRESS, data, 2, HAL_MAX_DELAY);
		HAL_Delay(2);
    // �˳�����ģʽ�������Զ�����ģʽ
    data[0] = PCA9685_MODE1;
    data[1] = 0xA1;
		HAL_I2C_Master_Transmit(&hi2c1, PCA9685_ADDRESS, data, 2, HAL_MAX_DELAY);
		HAL_Delay(2);
}

void PCA9685_SetServoAngle(uint8_t channel, float angle)
{
    if (channel > 15)
        return;

    // ���Ƕ�ת��Ϊ��Ӧ��PWMռ�ձȣ�������������Χ��0.5ms��2.5ms��
    uint16_t on_time = 0;
    uint16_t off_time = (uint16_t)(204.8f + (angle / 180.0f) * 409.6f); // 0.5ms��ӦԼ205��2.5ms��ӦԼ615

    uint8_t data[5];
    data[0] = LED0_ON_L + 4 * channel;
    data[1] = on_time & 0xFF;
    data[2] = (on_time >> 8) & 0xFF;
    data[3] = off_time & 0xFF;
    data[4] = (off_time >> 8) & 0xFF;
    HAL_I2C_Master_Transmit(&hi2c1, PCA9685_ADDRESS, data, 5, HAL_MAX_DELAY);
}
