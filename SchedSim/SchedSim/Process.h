//  Team: default
//  Names: Bartholomew, Dan
//         Choe, Ju
//         Hardin, Kurt
//
//  Process.h
//  SchedSim
//
//  Created by Kurt Hardin on 5/17/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#ifndef SchedSim_Process_h
#define SchedSim_Process_h

#include "RequestType.h"

// List of processes:
//  user interface process (e.g. one waiting for keyboard input)
//  a calculating process (possible background job)
//  two processes that require inter-process communications, such as a producer-consumer pair

typedef struct {
	int	noSteps;               // number of time steps before resetting to 0 - number of instructions
	int noRequests;            // number of requests that will be generated during the process run
	RequestType *requests;		// An array of requests, each request (e.g. io service) is issued at a specific
                                // time step. These are the synchronous events that result in traps being issued.
                                // You can get fancy and make this an array of RequestTypeStr which contains the
                                // step number when issued, and a request type (e.g. 0=io, 1=sync, etc.)
} Process;

Process * Process_init(int, int, RequestType *);

#endif
