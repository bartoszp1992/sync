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

vid_state_t vid_init(vid_flow_t *vid, uint32_t columns, uint32_t lines,
		volatile uint32_t *regHsyncCCR) {

	vid_state_t status = VID_STAT_OK;
	vid->stepsOnPeriod = columns;
	vid->periods = lines;

	vid->actualPeriod = 0;

	vid->regHsyncCCR = regHsyncCCR;

	*vid->regHsyncCCR = 5;

	vid->patterns = malloc(vid->periods * sizeof(uint64_t));

	if (vid->patterns == NULL) {
		status = VID_STAT_ERR_NOT_ENOUGH_MEMORY;
	}


	//temp
	// 5x: jeden impuls LOW 1us na początku
	for(int i=0;i<5;i++){
	    vid->patterns[i] = 0x0000000000000003ULL;
	}

	// 5x: dwa impulsy LOW 1us w okresie
	for(int i=5;i<10;i++){
	    vid->patterns[i] = (1ULL<<5)|(1ULL<<6)|(1ULL<<20)|(1ULL<<21);
	}

	HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_1);


	HAL_TIM_OC_Start_IT(&htim3, TIM_CHANNEL_2);
	//set start time of tim4
	TIM3->CCR2 = 16;


	//set duty for tim4 ch1
	TIM4->CCR1 = 20;



	//tempp end


	return status;

}


void vid_timerOCCallback(vid_flow_t *vid) {

}

void vid_timerPECallback(vid_flow_t *vid) {

	vid->actualPeriod++;
	if (vid->actualPeriod >= vid->periods)
		vid->actualPeriod = 0;

}

/*
 * fot future use: reset line due to sync with input signal
 */
void vid_lineReset(vid_flow_t *vid) {


}

void vid_screenReset(vid_flow_t *vid) {
	vid->actualPeriod = 0;
}
