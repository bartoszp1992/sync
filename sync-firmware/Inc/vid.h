/*
 * vid.h
 *
 *  Created on: Dec 23, 2025
 *      Author: bartosz
 */

#ifndef VID_H_
#define VID_H_

#include <stdint.h>
#include "gpio.h"
#include "tim.h"


typedef enum {
	LOW, HIGH
} vid_level_t;

typedef enum {
	VID_STAT_OK, VID_STAT_ERR_NOT_ENOUGH_MEMORY
} vid_state_t;

typedef struct {
	uint32_t stepsOnPeriod;
	uint32_t periods; //with blanking

	volatile uint32_t actualPeriod;

	TIM_HandleTypeDef *timerPrimary, *timerSecondary;

	//shortcuts to timer registers
	volatile uint32_t *regHsyncCCR;


} vid_flow_t;

vid_state_t vid_init(vid_flow_t *vid, uint32_t columns, uint32_t lines,
		TIM_HandleTypeDef *timerPrimary, TIM_HandleTypeDef *timerSecodnary);

//run this in output compare INT
void vid_timerHsyncOCCallback(vid_flow_t *vid, TIM_HandleTypeDef *htim);

//run this in period elapsed INT
void vid_timerHsyncPECallback(vid_flow_t *vid, TIM_HandleTypeDef *htim);

#endif /* VID_H_ */
