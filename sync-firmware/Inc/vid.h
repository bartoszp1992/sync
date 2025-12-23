/*
 * vid.h
 *
 *  Created on: Dec 23, 2025
 *      Author: bartosz
 */

#ifndef VID_H_
#define VID_H_

#include <stdint.h>

typedef enum{

	PAL,
	NTSC,
	SECAM

}vid_system_t;

typedef struct{

	vid_system_t system;
	uint32_t columns;
	uint32_t lines; //with blanking
	uint32_t vsyncPeriod; //ns
	uint32_t vsyncPulseWidth; //ns
	uint32_t actualPulse;
	uint32_t pulses;

}vid_flow_t;

void vid_init(vid_flow_t *vid, vid_system_t system, uint32_t columns,
		uint32_t lines, uint32_t vsyncPeriod, uint32_t vsyncPulseWidth);

#endif /* VID_H_ */
