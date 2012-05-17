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

void *run(void *);

pthread_t * io_device_init(char * type) {
    pthread_t *thread_id = malloc(sizeof(pthread_t));
    pthread_create(thread_id, NULL, run, (void *)type);
    return thread_id;
}

void *run(void * type) {
    printf("%s device running", (char *)type);
    
    srand(time(NULL));
    sleep(rand() % 5000);
    
    // TODO Generate interrupt
    
    return EXIT_SUCCESS;
}