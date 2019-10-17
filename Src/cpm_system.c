/*
 * cpm_system.c
 *
 *  Created on: Oct 14, 2019
 *      Author: Artur
 */

#include "stdlib.h"
#include "cpm_system.h"

void CPMsys_Run() {
	cpmdisp_Init();
	mem_Init(MEMTYPE_CPM);
}
