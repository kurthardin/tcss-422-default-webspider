//
//  Process.c
//  SchedSim
//
//  Created by Kurt Hardin on 5/17/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include "Process.h"

Process * Process_init(int stepCount, int requestCount, RequestType *requests) {
    Process *proc = malloc(sizeof(Process));
    proc->noSteps = stepCount;
    proc->noRequests = requestCount;
    proc->requests = requests;
    return proc;
}