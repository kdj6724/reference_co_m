/*
 * rebot_motor.h
 *
 *  Created on: 2018. 11. 30.
 *      Author: kdj6724
 */

#ifndef REBOT_MOTOR_H_
#define REBOT_MOTOR_H_
#include "stm32f1xx_hal.h"

#define REBOT_MOTOR_MAX	500

enum {
	REBOT_FORWARD,
	REBOT_BACKWARD,
	REBOT_LEFT,
	REBOT_RIGHT
};

typedef struct MotorGpio {
	GPIO_TypeDef * group;
	uint16_t pin;
}MotorGpio;

typedef struct MotorControl {
	uint32_t pwm;
	uint32_t dir;
}MotorControl;

typedef struct MotorInfo {
	TIM_HandleTypeDef* pwm;
	MotorGpio directionPin;
	uint32_t channel;
}MotorInfo;

typedef struct RebotMotor {
	MotorInfo left;
	MotorInfo right;
}RebotMotor;

int rebot_init(RebotMotor* motor);
int rebot_stop(RebotMotor* motor);
int rebot_forward(RebotMotor* motor, uint16_t val);
int rebot_backword(RebotMotor* motor, uint16_t val);
int rebot_turn_left(RebotMotor* motor, uint16_t val);
int rebot_turn_right(RebotMotor* motor, uint16_t val);
int rebot_data_parser(RebotMotor* motor, char* data);

#endif /* REBOT_MOTOR_H_ */
