/*
 * vid.h
 *
 *  Created on: Dec 23, 2025
 *      Author: bartosz
 */

#ifndef VID_H_
#define VID_H_

#include <stdint.h>

typedef enum {

	PAL, NTSC, SECAM

} vid_system_t;

typedef enum {
	LOW, HIGH
} vid_level_t;

typedef enum {
	VID_STAT_OK, VID_STAT_ERR_NOT_ENOUGH_MEMORY
} vid_state_t;

typedef struct {

	vid_system_t system;
	uint32_t columns;
	uint32_t lines; //with blanking

	uint32_t actualLine;

	//shortcuts to timer registers
	volatile uint32_t *regCCR;
	volatile uint32_t *regCNT;
	volatile uint32_t *regCCMR1;

	//pointer for sync array(64bits per line * target number of lines)
	uint64_t *patterns;

} vid_flow_t;

vid_state_t vid_init(vid_flow_t *vid, vid_system_t system, uint32_t columns,
		uint32_t lines, volatile uint32_t *regCCR, volatile uint32_t *regCCMR1);

//run this in output compare INT
void vid_timerOCCallback(vid_flow_t *vid);

//run this in period elapsed INT
void vid_timerPECallback(vid_flow_t *vid);

#endif /* VID_H_ */
