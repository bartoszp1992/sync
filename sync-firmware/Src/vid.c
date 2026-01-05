/*
 * vid.c
 *
 *  Created on: Dec 23, 2025
 *      Author: bartosz
 *
 *	notes:
 *	tim3 startuje z początkiem każdej linii i wywołuje hsync, który kończy się na CCR1
 *	tim3 liczy dalej i na CCR2(2gi kanał) generuje przerwanie do vsync
 *	przerwanie do vsync odpala tim4 w one pulse mode i odpala drugi impuls, zgodnie z patternem
 *
 *	problem:
 *	nie potrafię wywołać one pulse mode->nie działa.
 */

#include "vid.h"
#include <stdlib.h>

volatile uint8_t parity;

vid_state_t vid_init(vid_flow_t *vid, uint32_t columns, uint32_t lines,
		volatile uint32_t *regHsyncCCR) {

	vid_state_t status = VID_STAT_OK;
	vid->stepsOnPeriod = columns;
	vid->periods = lines;

	vid->actualPeriod = 0;

	vid->regHsyncCCR = regHsyncCCR;

	//start hsync PWM
	HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_1);
	*vid->regHsyncCCR = 5; //set duty

	//start OC for vsync start
	HAL_TIM_OC_Start_IT(&htim3, TIM_CHANNEL_2);

	//set start time of tim4
	TIM3->CCR2 = 16;

	//set duty for tim4 ch1
	TIM4->CCR1 = 20;

	return status;

}

/*
 * run in callback from OC channel in main timer
 */
void vid_timerHsyncOCCallback(vid_flow_t *vid, TIM_HandleTypeDef *htim) {

	if (htim->Instance == TIM3) {

		if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2) {

			parity++;

			if ((parity & 1) == 0)
				HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
			else
				HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);
		}

	}

}

/*
 * run in callback from PE in main timer
 */
void vid_timerHsyncPECallback(vid_flow_t *vid, TIM_HandleTypeDef *htim) {

	if (htim->Instance == TIM3) {

		vid->actualPeriod++;
		if (vid->actualPeriod >= vid->periods)
			vid->actualPeriod = 0;

	}

}

/*
 * fot future use: reset line due to sync with input signal
 */
void vid_lineReset(vid_flow_t *vid) {

}

void vid_screenReset(vid_flow_t *vid) {
	vid->actualPeriod = 0;
}
