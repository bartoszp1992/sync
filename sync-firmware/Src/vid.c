/*
 * vid.c
 *
 *  Created on: Dec 23, 2025
 *      Author: bartosz
 */

#include "vid.h"

void vid_init(vid_flow_t *vid, vid_system_t system, uint32_t columns,
		uint32_t lines, uint32_t vsyncPeriod, uint32_t vsyncPulseWidth) {

	vid->columns = columns;
	vid->lines = lines;
	vid->system = system;
	vid->vsyncPeriod = vsyncPeriod;
	vid->vsyncPulseWidth = vsyncPulseWidth;

}
