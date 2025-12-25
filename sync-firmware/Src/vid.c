/*
 * vid.c
 *
 *  Created on: Dec 23, 2025
 *      Author: bartosz
 *
 *      manual:
 *      1. set timer tick for 1us
 *      2. set timer period for 64 ticks
 *
 *
 */

#include "vid.h"
#include <stdlib.h>

vid_state_t vid_init(vid_flow_t *vid, vid_system_t system, uint32_t columns,
		uint32_t lines, volatile uint32_t *regCCR, volatile uint32_t *regCCMR1) {

	vid_state_t status = VID_STAT_OK;

	vid->system = system;
	vid->columns = columns;
	vid->lines = lines;

	vid->actualLine = 0;

	vid->regCCR = regCCR;
	vid->regCNT = 0;
	vid->regCCMR1 = regCCMR1;

	vid->patterns = malloc(vid->lines * sizeof(uint64_t));

	if(vid->patterns == NULL){
		free(vid->patterns);
		status = VID_STAT_ERR_NOT_ENOUGH_MEMORY;
	}

	return status;

}

/*
 * set target level on next compare event
 */
void vid_levelOnCompare(vid_flow_t *vid, vid_level_t level){

	*vid->regCCMR1 &= ~(1<<16);//OC1M[3] - reset bit 16
	*vid->regCCMR1 &= ~(7<<4);// reset OC1M[2:0] - reset bits 4,5,6,7

	if(level == HIGH){
		//switch register
		*vid->regCCMR1 |= 1<<4;//active level on match

	}
	else{
		//switch register
		*vid->regCCMR1 |= 1<<5;//inactive level on match
	}

}

void vid_timerOCCallback(vid_flow_t *vid){

	//pull actual line pattern
	uint64_t actualLinePattern = vid->patterns[vid->actualLine];

	//read bit corresponding to actual OC
	uint8_t actualState = (uint8_t)(actualLinePattern >> *vid->regCNT);

	//if actual state are high, set next OC switch to high
	if(actualState){
		vid_levelOnCompare(vid, LOW);
	}else{
		vid_levelOnCompare(vid, HIGH);
	}

}

void vid_timerPECallback(vid_flow_t *vid){

	vid->actualLine ++;
	if(vid->actualLine > vid->lines) vid->actualLine = 0;

}


/*
 * fot future use: reset line due to sync with input signal
 */
void vid_lineReset(vid_flow_t *vid){

	vid->regCNT = 0;

}
