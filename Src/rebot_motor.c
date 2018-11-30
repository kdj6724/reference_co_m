/*
 * rebot_motor.c
 *
 *  Created on: 2018. 11. 30.
 *      Author: kdj6724
 */

#include <stdio.h>
#include "rebot_motor.h"

void user_pwm_setvalue(MotorInfo* motorInfo, uint32_t channel, uint16_t value) {

	TIM_OC_InitTypeDef sConfigOC;

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = value;              // set the pulse duration
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	HAL_TIM_PWM_ConfigChannel(motorInfo->pwm, &sConfigOC, channel);
	HAL_TIM_PWM_Start(motorInfo->pwm, channel);   // start pwm generation
}

int rebot_set_motor(MotorInfo* motorInfo, int dir, uint16_t val) {
	if (motorInfo == NULL)
		return -1;

	if (val == 0) {
		HAL_TIM_PWM_Stop(motorInfo->pwm, motorInfo->channel);
		return 0;
	}
	user_pwm_setvalue(motorInfo, motorInfo->channel, val);

	return 0;
}

int rebot_stop(RebotMotor* motor) {
	rebot_set_motor(&motor->left, REBOT_FORWARD, 0);
	rebot_set_motor(&motor->right, REBOT_FORWARD, 0);
	return 0;
}

int rebot_forward(RebotMotor* motor, uint16_t val) {
	if (val == 0)
		rebot_stop(motor);

	if (val > REBOT_MOTOR_MAX)
		val = REBOT_MOTOR_MAX;

	rebot_set_motor(&motor->left, REBOT_FORWARD, val);
	rebot_set_motor(&motor->right, REBOT_FORWARD, val);
	return 0;
}

int rebot_backword(RebotMotor* motor, uint16_t val) {
	if (val == 0)
		rebot_stop(motor);

	if (val > REBOT_MOTOR_MAX)
			val = REBOT_MOTOR_MAX;

	rebot_set_motor(&motor->left, REBOT_BACKWARD, val);
	rebot_set_motor(&motor->right, REBOT_BACKWARD, val);
	return 0;
}

int rebot_turn_left(RebotMotor* motor) {
	rebot_set_motor(&motor->left, REBOT_BACKWARD, REBOT_MOTOR_MAX);
	rebot_set_motor(&motor->right, REBOT_BACKWARD, 0);
	return 0;
}

int rebot_turn_right(RebotMotor* motor) {
	rebot_set_motor(&motor->left, REBOT_BACKWARD, 0);
	rebot_set_motor(&motor->right, REBOT_BACKWARD, REBOT_MOTOR_MAX);
	return 0;
}


