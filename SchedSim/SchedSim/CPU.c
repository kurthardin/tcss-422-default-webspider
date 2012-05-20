//
//  CPU.c
//  SchedSim
//
//  Created by Kurt Hardin on 5/17/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#include <stdio.h>
#include "CPU.h"

Interrupt Interrupt_make(int type) {
    Interrupt interrupt = {type};
    return interrupt;
}


void CPU_signalInterrupt(CPU *cpu, Interrupt interrupt) {
    // STUB
}