//
//  IODevice.c
//  SchedSim
//
//  Created by Kurt Hardin on 5/17/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "IODevice.h"

#define BLOCKED_QUEUE_SIZE 10

void* run(void*);

IODevice* io_device_init(char* type) {
    IODevice* device = malloc(sizeof(IODevice));
    device->type = type;
    device->blocked_queue = pcb_queue_init();
    return device;
}

void io_device_service(IODevice* device) {
    pthread_t* tid = malloc(sizeof(pthread_t));
    pthread_create(tid, NULL, run, (void*)device->type);
}

void* run(void* type) {
    printf("%s device running", (char*)type);
    
    srand(time(NULL));
    sleep(rand() % 5000);
    
    // TODO Generate interrupt
    
    return EXIT_SUCCESS;
}