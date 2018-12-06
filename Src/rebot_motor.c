/*
 * rebot_motor.c
 *
 *  Created on: 2018. 11. 30.
 *      Author: kdj6724
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "rebot_motor.h"
#include "dev/display/ssd1306.h"
#include "dev/font/fonts.h"

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

	val = val * 3;
	if (val > REBOT_MOTOR_MAX)
			val = REBOT_MOTOR_MAX;

	user_pwm_setvalue(motorInfo, motorInfo->channel, val);

	return 0;
}

int rebot_init(RebotMotor* motor) {
	return 0;
}

int rebot_display_value(int left, int right, int dir) {
	char tmpStr[64];

	ssd1306_Fill(White);
	memset(tmpStr, 0, sizeof(tmpStr));
	sprintf(tmpStr, "LEFT :%03d", left);
	ssd1306_SetCursor(20,15);
	ssd1306_WriteString(tmpStr, Font_7x10, Black);

	memset(tmpStr, 0, sizeof(tmpStr));
	sprintf(tmpStr, "RIGHT:%03d", right);
	ssd1306_SetCursor(20,30);
	ssd1306_WriteString(tmpStr, Font_7x10, Black);

	memset(tmpStr, 0, sizeof(tmpStr));
	sprintf(tmpStr, "DIR  :%03d", dir);
	ssd1306_SetCursor(20,45);
	ssd1306_WriteString(tmpStr, Font_7x10, Black);
	ssd1306_UpdateScreen();
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

	rebot_set_motor(&motor->left, REBOT_FORWARD, val);
	rebot_set_motor(&motor->right, REBOT_FORWARD, val);
	return 0;
}

int rebot_backword(RebotMotor* motor, uint16_t val) {
	if (val == 0)
		rebot_stop(motor);

	rebot_set_motor(&motor->left, REBOT_BACKWARD, val);
	rebot_set_motor(&motor->right, REBOT_BACKWARD, val);
	return 0;
}

int rebot_turn_left(RebotMotor* motor, uint16_t val) {
	rebot_set_motor(&motor->left, REBOT_BACKWARD, val);
	rebot_set_motor(&motor->right, REBOT_BACKWARD, 0);
	return 0;
}

int rebot_turn_right(RebotMotor* motor, uint16_t val) {
	rebot_set_motor(&motor->left, REBOT_BACKWARD, 0);
	rebot_set_motor(&motor->right, REBOT_BACKWARD, val);
	return 0;
}

double mysqrt(unsigned int number) {
   unsigned int NUM_REPEAT = 16;
   unsigned int k;
   double res;
   double tmp = (double)number;
   for(k=0,res=tmp;k<NUM_REPEAT;k++) {
     if(res<1.0) break;
     res = (res*res+tmp)/(2.0*res);
   }
   return res;
}

int rebot_data_parser(RebotMotor* motor, char* data) {
	char* tmp;
	int xVal = 0;
	int yVal = 0;
	int dirLR = 0;	// 좌우
	int dirFB = 0;	// 앞뒤
	int dirPower = 0;
	GPIO_PinState gpioVal = GPIO_PIN_RESET;

	tmp = strtok(data, ",");
	xVal = atoi(tmp);
	tmp = strtok(NULL, ",");
	yVal = atoi(tmp);

	//rebot_display_value(xVal, yVal, yVal);
	if (yVal < 0) {
		dirLR = REBOT_LEFT;
		yVal = yVal * (-1);
	} else {
		dirLR = REBOT_RIGHT;
	}

	if (xVal < 0) {
		dirFB = REBOT_BACKWARD;
		xVal = xVal * (-1);
		gpioVal = GPIO_PIN_SET;
	} else {
		dirFB = REBOT_FORWARD;
	}
	if (yVal > 200 || xVal > 200)
		return -1;

	dirPower = (int)mysqrt(yVal*yVal + xVal*xVal) - xVal;

	//HAL_GPIO_WritePin(&motor->left.directionPin.group, &motor->left.directionPin.pin, gpioVal);
	//HAL_GPIO_WritePin(&motor->right.directionPin.group, &motor->right.directionPin.pin, gpioVal);
	if (dirLR == REBOT_LEFT) {
		rebot_set_motor(&motor->left, dirFB, xVal);
		rebot_set_motor(&motor->right, dirFB, xVal + dirPower);
		rebot_display_value(xVal, xVal + dirPower, dirPower);
	} else {
		rebot_set_motor(&motor->left, dirFB, xVal + dirPower);
		rebot_set_motor(&motor->right, dirFB, xVal);
		rebot_display_value(xVal + dirPower, xVal, dirPower);
	}

	return 0;
}

