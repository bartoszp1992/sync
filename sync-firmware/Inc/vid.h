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

typedef enum{
	VID_STAT_OK,
	VID_STAT_ERR_NOT_ENOUGH_MEMORY
}vid_state_t;


typedef struct {

	vid_system_t system;
	uint32_t columns;
	uint32_t lines; //with blanking

	uint32_t actualLine;

	uint32_t *regCCR;
	uint32_t *regCNT;
	uint32_t *regCCMR1;

	uint64_t *patterns;


} vid_flow_t;

void vid_timerOCCallback(vid_flow_t *vid);
void vid_timerPECallback(vid_flow_t *vid);

//void vid_init(vid_flow_t *vid, vid_system_t system, uint32_t columns,
//		uint32_t lines, uint32_t vsyncPeriod, uint32_t vsyncPulseWidth);

#endif /* VID_H_ */
